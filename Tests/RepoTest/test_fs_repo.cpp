#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include "Repository/FSRep/realisation/fs_repository.h"
#include "Repository/FSRep/realisation/Path/path.h"
#include "Entity/User/user.h"
#include "Entity/Directory/realisation/directory_descriptor.h"
#include "Entity/File/realisation/file_descriptor.h"

TEST_CASE("FileSystemRepository") {
    FileSystemRepository repo;
    User admin(1, "admin");

    SECTION("setRootDirectory") {
        auto rootDir = std::make_unique<DirectoryDescriptor>("/", 0, admin, 0);
        auto* rootPtr = dynamic_cast<IDirectory*>(rootDir.get());
        REQUIRE(rootPtr != nullptr);
        repo.setRootDirectory(rootPtr);
        REQUIRE(repo.getRootDirectory() == rootPtr);
        repo.setRootDirectory(nullptr);
        REQUIRE(repo.getRootDirectory() != nullptr);

        auto otherDir = std::make_unique<DirectoryDescriptor>("other", 1, admin, 1);
        auto* otherPtr = dynamic_cast<IDirectory*>(otherDir.get());
        REQUIRE(otherPtr != nullptr);

        repo.setRootDirectory(otherPtr);
        REQUIRE(repo.getRootDirectory() != otherPtr);
        auto file = std::make_unique<FileDescriptor>("file_txt", 0, admin, 100);
        auto* filePtr = dynamic_cast<IDirectory*>(file.get());
        REQUIRE(filePtr == nullptr);

        repo.setRootDirectory(filePtr);
        REQUIRE(repo.getRootDirectory() != nullptr);
    }

    SECTION("getObjectByPath - полное покрытие") {
        auto* root = repo.getRootDirectory();
        REQUIRE(root != nullptr);

        auto* rootObj = repo.getObjectByPath("/");
        REQUIRE(rootObj != nullptr);
        REQUIRE(rootObj->getName() == "/");

        REQUIRE(repo.getObjectByPath("/nonexistent") == nullptr);
        REQUIRE(repo.getObjectByPath("") == repo.getObjectByPath("/"));
        REQUIRE(repo.getObjectByPath("relative/path") == nullptr);

        auto dir1 = std::make_unique<DirectoryDescriptor>("dir1", 0, admin, repo.getAddress());
        unsigned int dir1Address = dir1->getAddress();
        REQUIRE(repo.saveObject(std::move(dir1)));

        auto* dir1Obj = dynamic_cast<IFileSystemObject*>(repo.getObjectByAddress(dir1Address));
        REQUIRE(dir1Obj != nullptr);
        root->addChild(dir1Obj);

        auto file1 = std::make_unique<FileDescriptor>("file1_txt", dir1Address, admin, repo.getAddress());
        unsigned int file1Address = file1->getAddress();
        REQUIRE(repo.saveObject(std::move(file1)));

        auto* dir1Ptr = dynamic_cast<IDirectory*>(dir1Obj);
        REQUIRE(dir1Ptr != nullptr);
        auto* file1Obj = repo.getObjectByAddress(file1Address);
        REQUIRE(file1Obj != nullptr);
        dir1Ptr->addChild(file1Obj);

        auto subdir = std::make_unique<DirectoryDescriptor>("subdir", dir1Address, admin, repo.getAddress());
        unsigned int subdirAddress = subdir->getAddress();
        REQUIRE(repo.saveObject(std::move(subdir)));

        auto* subdirObj = dynamic_cast<IFileSystemObject*>(repo.getObjectByAddress(subdirAddress));
        REQUIRE(subdirObj != nullptr);
        dir1Ptr->addChild(subdirObj);

        auto file2 = std::make_unique<FileDescriptor>("file2_txt", subdirAddress, admin, repo.getAddress());
        unsigned int file2Address = file2->getAddress();
        REQUIRE(repo.saveObject(std::move(file2)));

        auto* subdirPtr = dynamic_cast<IDirectory*>(subdirObj);
        REQUIRE(subdirPtr != nullptr);
        auto* file2Obj = repo.getObjectByAddress(file2Address);
        REQUIRE(file2Obj != nullptr);
        subdirPtr->addChild(file2Obj);

        REQUIRE(repo.getObjectByPath("/dir1") != nullptr);
        REQUIRE(repo.getObjectByPath("/dir1/file1_txt") != nullptr);
        REQUIRE(repo.getObjectByPath("/dir1/subdir") != nullptr);
        REQUIRE(repo.getObjectByPath("/dir1/subdir/file2_txt") != nullptr);

        REQUIRE(repo.getObjectByPath("/dir1/./file1_txt") != nullptr);
        REQUIRE(repo.getObjectByPath("/dir1/../dir1/file1_txt") != nullptr);
        REQUIRE(repo.getObjectByPath("//dir1///file1_txt") != nullptr);

        REQUIRE(repo.getObjectByPath("/dir1/.") != nullptr);
        REQUIRE(repo.getObjectByPath("/dir1/..") != nullptr);

        REQUIRE(repo.getObjectByPath("/dir1/nonexistent_txt") == nullptr);

        REQUIRE(repo.getObjectByPath("/dir1/file1_txt/subdir") == nullptr);

        REQUIRE(repo.getObjectByPath("/dir1//file1_txt") != nullptr);

        auto* parentOfRoot = repo.getObjectByPath("/..");
        REQUIRE(parentOfRoot != nullptr);
        REQUIRE(parentOfRoot->getName() == "/");
        auto* dir1PathObj = repo.getObjectByPath("/dir1");
        REQUIRE(dir1PathObj != nullptr);
        auto* fromSubdir = repo.getObjectByPath("/dir1/subdir/..");
        REQUIRE(fromSubdir != nullptr);
        REQUIRE(fromSubdir->getName() == "dir1");
        REQUIRE(repo.getObjectByPath("/dir1/subdir/../nonexistent") == nullptr);
        auto* multipleUp = repo.getObjectByPath("/dir1/subdir/../../..");
        REQUIRE(multipleUp != nullptr);
        REQUIRE(multipleUp->getName() == "/");
    }

    SECTION("clear") {
        auto file1 = std::make_unique<FileDescriptor>("file1_txt", 0, admin, repo.getAddress());
        unsigned int address1 = file1->getAddress();
        REQUIRE(repo.saveObject(std::move(file1)));

        REQUIRE(repo.objectExists(address1));
        REQUIRE(repo.getAllObjects().size() > 1);

        repo.clear();

        REQUIRE_FALSE(repo.objectExists(address1));
        auto allObjects = repo.getAllObjects();
        REQUIRE(allObjects.size() == 1);
        REQUIRE(allObjects[0]->getName() == "/");
        REQUIRE(repo.getRootDirectory() != nullptr);

        auto* root = repo.getRootDirectory();
        REQUIRE(root != nullptr);
        auto rootObj = dynamic_cast<IFileSystemObject*>(root);
        REQUIRE(rootObj != nullptr);

        FileSystemRepository repo2;

        auto file2 = std::make_unique<FileDescriptor>("file2_txt", 0, admin, repo2.getAddress());
        unsigned int address2 = file2->getAddress();
        REQUIRE(repo2.saveObject(std::move(file2)));

        auto allObjs = repo2.getAllObjects();
        IFileSystemObject* rootToRemove = nullptr;

        for (auto* obj : allObjs) {
            if (obj->getName() == "/" && obj->getAddress() == 0) {
                rootToRemove = obj;
                break;
            }
        }
        REQUIRE(rootToRemove != nullptr);
        repo2.clear();
        repo2.clear();
        REQUIRE(repo2.getRootDirectory() != nullptr);
        REQUIRE(repo2.getAllObjects().size() == 1);
    }

    SECTION("getDirectoryByPath и getFileByPath") {
        auto dir = std::make_unique<DirectoryDescriptor>("testdir", 0, admin, repo.getAddress());
        unsigned int dirAddress = dir->getAddress();
        REQUIRE(repo.saveObject(std::move(dir)));
        auto* root = repo.getRootDirectory();
        auto* dirObj = dynamic_cast<IFileSystemObject*>(repo.getObjectByAddress(dirAddress));
        REQUIRE(dirObj != nullptr);
        root->addChild(dirObj);
        auto file = std::make_unique<FileDescriptor>("testfile_txt", dirAddress, admin, repo.getAddress());
        unsigned int fileAddress = file->getAddress();
        REQUIRE(repo.saveObject(std::move(file)));
        auto* dirPtr = dynamic_cast<IDirectory*>(dirObj);
        REQUIRE(dirPtr != nullptr);
        auto* fileObj = repo.getObjectByAddress(fileAddress);
        REQUIRE(fileObj != nullptr);
        dirPtr->addChild(fileObj);

        auto* dirByPath = repo.getDirectoryByPath("/testdir");
        REQUIRE(dirByPath != nullptr);
        REQUIRE(dynamic_cast<IFileSystemObject*>(dirByPath)->getName() == "testdir");

        auto* fileByPath = repo.getFileByPath("/testdir/testfile_txt");
        REQUIRE(fileByPath != nullptr);
        REQUIRE(dynamic_cast<IFileSystemObject*>(fileByPath)->getName() == "testfile_txt");
        REQUIRE(repo.getDirectoryByPath("/testdir/testfile_txt") == nullptr);
        REQUIRE(repo.getFileByPath("/testdir") == nullptr);
        REQUIRE(repo.getDirectoryByPath("/nonexistent") == nullptr);
        REQUIRE(repo.getFileByPath("/nonexistent_txt") == nullptr);
        REQUIRE(repo.getDirectoryByPath("") == repo.getDirectoryByPath("/"));
        REQUIRE(repo.getFileByPath("") == nullptr);
    }

    SECTION("saveObject и deleteObject") {
        auto file = std::make_unique<FileDescriptor>("test_txt", 0, admin, repo.getAddress());
        unsigned int address = file->getAddress();

        REQUIRE(repo.saveObject(std::move(file)));
        REQUIRE(repo.objectExists(address));
        REQUIRE_FALSE(repo.saveObject(nullptr));
        REQUIRE(repo.deleteObject(address));
        REQUIRE_FALSE(repo.objectExists(address));
        REQUIRE_FALSE(repo.deleteObject(9999));
        REQUIRE_FALSE(repo.deleteObject(0));

        auto dir = std::make_unique<DirectoryDescriptor>("parent", 0, admin, repo.getAddress());
        unsigned int dirAddress = dir->getAddress();
        REQUIRE(repo.saveObject(std::move(dir)));

        auto* root = repo.getRootDirectory();
        auto* dirObj = dynamic_cast<IFileSystemObject*>(repo.getObjectByAddress(dirAddress));
        REQUIRE(dirObj != nullptr);
        root->addChild(dirObj);

        auto childFile = std::make_unique<FileDescriptor>("child_txt", dirAddress, admin, repo.getAddress());
        unsigned int childAddress = childFile->getAddress();
        REQUIRE(repo.saveObject(std::move(childFile)));

        auto* dirPtr = dynamic_cast<IDirectory*>(dirObj);
        REQUIRE(dirPtr != nullptr);
        auto* childObj = repo.getObjectByAddress(childAddress);
        REQUIRE(childObj != nullptr);
        dirPtr->addChild(childObj);

        REQUIRE(repo.deleteObject(childAddress));
        REQUIRE_FALSE(dirPtr->containChild("child_txt"));
    }

    SECTION("objectExists и pathExists") {
        auto dir = std::make_unique<DirectoryDescriptor>("mydir", 0, admin, repo.getAddress());
        unsigned int dirAddress = dir->getAddress();
        REQUIRE(repo.saveObject(std::move(dir)));
        auto* root = repo.getRootDirectory();
        auto* dirObj = dynamic_cast<IFileSystemObject*>(repo.getObjectByAddress(dirAddress));
        REQUIRE(dirObj != nullptr);
        root->addChild(dirObj);
        REQUIRE(repo.objectExists(dirAddress));
        REQUIRE_FALSE(repo.objectExists(9999));
        REQUIRE(repo.pathExists("/mydir"));
        REQUIRE_FALSE(repo.pathExists("/nonexistent"));
        REQUIRE(repo.pathExists(""));
    }

    SECTION("getAllObjects") {
        size_t initialCount = repo.getAllObjects().size();

        auto file1 = std::make_unique<FileDescriptor>("file1_txt", 0, admin, repo.getAddress());
        auto file2 = std::make_unique<FileDescriptor>("file2_txt", 0, admin, repo.getAddress());
        REQUIRE(repo.saveObject(std::move(file1)));
        REQUIRE(repo.saveObject(std::move(file2)));

        auto allObjects = repo.getAllObjects();
        REQUIRE(allObjects.size() == initialCount + 2);

        for (auto* obj : allObjects) {
            REQUIRE(obj != nullptr);
        }
    }

    SECTION("getObjectByAddress") {
        auto file = std::make_unique<FileDescriptor>("test_txt", 0, admin, repo.getAddress());
        unsigned int address = file->getAddress();
        REQUIRE(repo.saveObject(std::move(file)));

        auto* obj = repo.getObjectByAddress(address);
        REQUIRE(obj != nullptr);
        REQUIRE(obj->getName() == "test_txt");

        REQUIRE(repo.getObjectByAddress(9999) == nullptr);

        auto* root = repo.getObjectByAddress(0);
        REQUIRE(root != nullptr);
        REQUIRE(root->getName() == "/");
    }

    SECTION("getAddress") {
        unsigned int addr1 = repo.getAddress();
        unsigned int addr2 = repo.getAddress();
        REQUIRE(addr2 > addr1);
    }

    SECTION("getPath") {
        auto* root = dynamic_cast<IFileSystemObject*>(repo.getRootDirectory());
        REQUIRE(root != nullptr);
        REQUIRE(repo.getPath(root) == "/");
        REQUIRE(repo.getPath(nullptr).empty());

        auto dir1 = std::make_unique<DirectoryDescriptor>("dir1", 0, admin, repo.getAddress());
        unsigned int dir1Address = dir1->getAddress();
        REQUIRE(repo.saveObject(std::move(dir1)));
        auto* rootDir = repo.getRootDirectory();
        auto* dir1Obj = dynamic_cast<IFileSystemObject*>(repo.getObjectByAddress(dir1Address));
        REQUIRE(dir1Obj != nullptr);
        rootDir->addChild(dir1Obj);

        REQUIRE(repo.getPath(dir1Obj) == "/dir1");

        auto subdir = std::make_unique<DirectoryDescriptor>("subdir", dir1Address, admin, repo.getAddress());
        unsigned int subdirAddress = subdir->getAddress();
        REQUIRE(repo.saveObject(std::move(subdir)));

        auto* dir1Ptr = dynamic_cast<IDirectory*>(dir1Obj);
        REQUIRE(dir1Ptr != nullptr);
        auto* subdirObj = dynamic_cast<IFileSystemObject*>(repo.getObjectByAddress(subdirAddress));
        REQUIRE(subdirObj != nullptr);
        dir1Ptr->addChild(subdirObj);

        REQUIRE(repo.getPath(subdirObj) == "/dir1/subdir");
    }

    SECTION("findObjects") {
        auto dir = std::make_unique<DirectoryDescriptor>("searchdir", 0, admin, repo.getAddress());
        unsigned int dirAddress = dir->getAddress();
        REQUIRE(repo.saveObject(std::move(dir)));

        auto* root = repo.getRootDirectory();
        auto* dirObj = dynamic_cast<IFileSystemObject*>(repo.getObjectByAddress(dirAddress));
        REQUIRE(dirObj != nullptr);
        root->addChild(dirObj);
        auto file1 = std::make_unique<FileDescriptor>("test1_txt", dirAddress, admin, repo.getAddress());
        unsigned int file1Address = file1->getAddress();
        REQUIRE(repo.saveObject(std::move(file1)));

        auto file2 = std::make_unique<FileDescriptor>("test2_doc", dirAddress, admin, repo.getAddress());
        unsigned int file2Address = file2->getAddress();
        REQUIRE(repo.saveObject(std::move(file2)));

        auto file3 = std::make_unique<FileDescriptor>("data_txt", dirAddress, admin, repo.getAddress());
        unsigned int file3Address = file3->getAddress();
        REQUIRE(repo.saveObject(std::move(file3)));

        auto* dirPtr = dynamic_cast<IDirectory*>(dirObj);
        REQUIRE(dirPtr != nullptr);

        auto* file1Obj = repo.getObjectByAddress(file1Address);
        auto* file2Obj = repo.getObjectByAddress(file2Address);
        auto* file3Obj = repo.getObjectByAddress(file3Address);

        if (file1Obj) dirPtr->addChild(file1Obj);
        if (file2Obj) dirPtr->addChild(file2Obj);
        if (file3Obj) dirPtr->addChild(file3Obj);
        auto txtFiles = repo.findObjects("*_txt", "/searchdir");
        REQUIRE(txtFiles.size() == 2);
        auto allFiles = repo.findObjects("*", "/searchdir");
        REQUIRE(allFiles.size() == 3);
        auto rootSearch = repo.findObjects("*");
        REQUIRE(rootSearch.size() >= 3);
        auto badStart = repo.findObjects("*", "/nonexistent");
        REQUIRE(badStart.empty());
        auto emptyPattern = repo.findObjects("", "/searchdir");
        REQUIRE(emptyPattern.empty());
    }
}


TEST_CASE("Path - базовые операции") {
    SECTION("splitPath - разбиение путей") {
        auto parts1 = Path::splitPath("/");
        REQUIRE(parts1.empty());

        auto parts2 = Path::splitPath("/home");
        REQUIRE(parts2.size() == 1);
        REQUIRE(parts2[0] == "home");

        auto parts3 = Path::splitPath("/home/user/documents");
        REQUIRE(parts3.size() == 3);
        REQUIRE(parts3[0] == "home");
        REQUIRE(parts3[1] == "user");
        REQUIRE(parts3[2] == "documents");

        auto parts4 = Path::splitPath("home/user");
        REQUIRE(parts4.size() == 2);
        REQUIRE(parts4[0] == "home");
        REQUIRE(parts4[1] == "user");

        auto parts5 = Path::splitPath("///home///user///");
        REQUIRE(parts5.size() == 2);
        REQUIRE(parts5[0] == "home");
        REQUIRE(parts5[1] == "user");
    }

    SECTION("normalizePath - нормализация путей") {
        REQUIRE(Path::normalizePath("") == "/");
        REQUIRE(Path::normalizePath("/") == "/");
        REQUIRE(Path::normalizePath("/home") == "/home");
        REQUIRE(Path::normalizePath("/home/") == "/home");
        REQUIRE(Path::normalizePath("/home//user") == "/home/user");
        REQUIRE(Path::normalizePath("/home/./user") == "/home/user");
        REQUIRE(Path::normalizePath("/home/../user") == "/user");
        REQUIRE(Path::normalizePath("/home/user/..") == "/home");
        REQUIRE(Path::normalizePath("/home/user/../..") == "/");
        REQUIRE(Path::normalizePath("/home/user/../../..") == "/..");
    }

    SECTION("resolvePath - разрешение относительных путей") {
        REQUIRE(Path::resolvePath("/home", "user") == "/home/user");
        REQUIRE(Path::resolvePath("/home/", "user") == "/home/user");
        REQUIRE(Path::resolvePath("/home", "/user") == "/user");
        REQUIRE(Path::resolvePath("/home", ".") == "/home");
        REQUIRE(Path::resolvePath("/home", "..") == "/");
        REQUIRE(Path::resolvePath("/home/user", "../documents") == "/home/documents");
        REQUIRE(Path::resolvePath("/home/user", "../../documents") == "/documents");
        REQUIRE(Path::resolvePath("/", "home/user") == "/home/user");
        REQUIRE(Path::resolvePath("/home/user", "") == "/home/user");
        REQUIRE(Path::resolvePath("", "home/user") == "/home/user");
        REQUIRE(Path::resolvePath("/home", "user/../documents") == "/home/documents");
        REQUIRE(Path::resolvePath("/home", "./user/./docs") == "/home/user/docs");
    }

    SECTION("getParentPath - получение родительской директории") {
        REQUIRE(Path::getParentPath("/") == "/");
        REQUIRE(Path::getParentPath("/home") == "/");
        REQUIRE(Path::getParentPath("/home/") == "/");
        REQUIRE(Path::getParentPath("/home/user") == "/home");
        REQUIRE(Path::getParentPath("/home/user/") == "/home");
        REQUIRE(Path::getParentPath("/home/user/documents") == "/home/user");
        REQUIRE(Path::getParentPath("home/user") == "/home");
        REQUIRE(Path::getParentPath("home") == "/");
    }

    SECTION("getFileName - извлечение имени файла") {
        REQUIRE(Path::getFileName("/") == "/");
        REQUIRE(Path::getFileName("/home") == "home");
        REQUIRE(Path::getFileName("/home/") == "home");
        REQUIRE(Path::getFileName("/home/user.txt") == "user.txt");
        REQUIRE(Path::getFileName("/home/user/documents/report.pdf") == "report.pdf");
        REQUIRE(Path::getFileName("home/user.txt") == "user.txt");
        REQUIRE(Path::getFileName("file.txt") == "file.txt");
        REQUIRE(Path::getFileName("") == "/");
    }

    SECTION("isValidPath - проверка валидности путей") {
        REQUIRE(Path::isValidPath("/") == true);
        REQUIRE(Path::isValidPath("/home") == true);
        REQUIRE(Path::isValidPath("/home/user") == true);
        REQUIRE(Path::isValidPath("") == false);
        REQUIRE(Path::isValidPath("home") == false);
        REQUIRE(Path::isValidPath("home/user") == false);
        std::string pathWithNull = "/home/";
        pathWithNull.push_back('\0');
        pathWithNull += "user";
        REQUIRE(Path::isValidPath(pathWithNull) == false);
        REQUIRE(Path::isValidPath("/home/../user") == true);
        REQUIRE(Path::isValidPath("/home/./user") == true);
    }

    SECTION("matchesPattern - сопоставление с шаблоном") {
        REQUIRE(Path::matchesPattern("file.txt", "*") == true);
        REQUIRE(Path::matchesPattern("file.txt", "*.txt") == true);
        REQUIRE(Path::matchesPattern("file.txt", "*.pdf") == false);
        REQUIRE(Path::matchesPattern("document.pdf", "*.pdf") == true);
        REQUIRE(Path::matchesPattern("file.txt", "file.*") == true);
        REQUIRE(Path::matchesPattern("file.txt", "f?le.txt") == true);
        REQUIRE(Path::matchesPattern("file.txt", "f??e.txt") == true);
        REQUIRE(Path::matchesPattern("file.txt", "file.txt") == true);
        REQUIRE(Path::matchesPattern("file.txt", "FILE.TXT") == false);
        REQUIRE(Path::matchesPattern("report_2024.pdf", "report_*.pdf") == true);
        REQUIRE(Path::matchesPattern("image.jpg", "image*.jpg") == true);
        REQUIRE(Path::matchesPattern("image.jpg", "image?jpg") == true);
    }
}

TEST_CASE("Path - комплексные сценарии") {
    SECTION("Нормализация сложных путей") {
        REQUIRE(Path::normalizePath("//home///user//.././documents//file.txt") == "/home/documents/file.txt");
        REQUIRE(Path::normalizePath("/.././../home/./user/../documents") == "/home/documents");
    }

    SECTION("Разрешение сложных относительных путей") {
        REQUIRE(Path::resolvePath("/home/user/docs", "../../var/log/./../cache") == "/home/var/cache");
        REQUIRE(Path::resolvePath("/a/b/c", "./d/../e/./f/../../g") == "/a/b/c/g");
        REQUIRE(Path::resolvePath("/", "a/./b/../c/d/../../e") == "/a/e");
    }

    SECTION("Вложенные шаблоны поиска") {
        REQUIRE(Path::matchesPattern("data_2024_01_15.log", "data_*_*.log") == true);
        REQUIRE(Path::matchesPattern("img_001.jpg", "img_???.jpg") == true);
        REQUIRE(Path::matchesPattern("img_01.jpg", "img_???.jpg") == false);
        REQUIRE(Path::matchesPattern("archive.tar.gz", "*.tar.gz") == true);
        REQUIRE(Path::matchesPattern("archive.tar.gz", "*.gz") == true);
        REQUIRE(Path::matchesPattern("archive.tar.gz", "*.tar.*") == true);
    }
}

TEST_CASE("Path - граничные случаи и обработка ошибок") {
    SECTION("Обработка путей с точкой и двумя точками") {
        REQUIRE(Path::normalizePath("/.") == "/");
        REQUIRE(Path::normalizePath("/home/./././") == "/home");
        REQUIRE(Path::normalizePath("/../../..") == "/..");
    }

    SECTION("Разрешение путей из корня") {
        REQUIRE(Path::resolvePath("/", ".") == "/");
        REQUIRE(Path::resolvePath("/", "../..") == "/");
        REQUIRE(Path::resolvePath("/", "../../home") == "/home");
    }

    SECTION("Шаблоны с особыми символами") {
        REQUIRE(Path::matchesPattern("file.name.txt", "file*.txt") == true);
        REQUIRE(Path::matchesPattern("file-name.txt", "file*.txt") == true);
        REQUIRE(Path::matchesPattern("file_name.txt", "file*.txt") == true);
        REQUIRE(Path::matchesPattern("file.txt", "file\\*.txt") == false);
        REQUIRE(Path::matchesPattern("file*star.txt", "file\\*star.txt") == false);
    }
}