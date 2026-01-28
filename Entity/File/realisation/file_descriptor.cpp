#include "base.h"
#include "file_descriptor.h"
#include <string_view>
#include <stdexcept>
#include <string>


FileDescriptor::FileDescriptor(const std::string &name, unsigned int parentAddress, const User &owner, unsigned int adr)
    : FileSystemObject(name, parentAddress, owner, adr), size(0), mode(Lock::NotLock) {}

bool FileDescriptor::writeContent(std::string_view cont) {
    if (!isWritable()) throw std::runtime_error("File is not writable");
    this->content = cont;
    updateFileSize();
    updateModificationTime();
    return true;
}

bool FileDescriptor::writeContentAlways(std::string_view cont) {
    this->content = cont;
    updateFileSize();
    updateModificationTime();
    return true;
}

std::string FileDescriptor::readContent() const {
    if (!isReadable()) throw std::runtime_error("File is not readable");
    return content;
}

std::string FileDescriptor::readContentAlways() const {
    return content;
}

bool FileDescriptor::truncateContent(int index) {
    if (!isWritable()) throw std::runtime_error("File is not writable");
    if (index < 0 || static_cast<size_t>(index) > content.size()) return false;
    content.resize(index);
    updateFileSize();
    updateModificationTime();
    return true;
}

void FileDescriptor::clearContent() {
    if (!isWritable()) throw std::runtime_error("File is not writable");
    content.clear();
    updateFileSize();
    updateModificationTime();
}

int FileDescriptor::getSize() const {
    return static_cast<int>(size);
}

void FileDescriptor::updateFileSize() {
    size = content.size();
}
