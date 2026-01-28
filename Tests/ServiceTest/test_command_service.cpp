#include <catch2/catch_test_macros.hpp>
#include <memory>

#include "Command/CommandService/realisation/command_service.h"
#include "Command/CommandRep/realisation/command_repository.h"
#include "Service/SecurityService/interface/i_security_service.h"
#include "FileSystem/interface/i_file_system.h"

class MockSecurityService : public ISecurityService {
public:
    bool isAdmin = false;
    bool owner = true;
    bool canReadValue = true;
    bool canWriteValue = true;
    bool canExecuteValue = true;

    bool checkPermission(const User&, const IFileSystemObject&, PermissionType) override { return true; }
    std::map<PermissionType, bool> getEffectivePermissions(const User&, const IFileSystemObject&) override { return {}; }
    bool canRead(const User&, const IFileSystemObject&) override { return canReadValue; }
    bool canWrite(const User&, const IFileSystemObject&) override { return canWriteValue; }
    bool canExecute(const User&, const IFileSystemObject&) override { return canExecuteValue; }
    bool canModify(const User&, const IFileSystemObject&) override { return true; }
    bool canModifyMetadata(const User&, const IFileSystemObject&) override { return true; }
    bool canChangePermissions(const User&, const IFileSystemObject&) override { return true; }
    User* authenticate(const std::string&) override { return nullptr; }
    bool isAdministrator(const User&) override { return isAdmin; }
    bool isOwner(const User&, const IFileSystemObject&) override { return owner; }
};

class TestCommand : public BaseCommand {
public:
    bool validateArgsCalled = false;
    bool executeCalled = false;
    std::vector<std::string> lastArgs;

    TestCommand(const std::string &name, bool adminOnly = false, bool valid = true)
        : BaseCommand(name, "Test command", name + " [args]", adminOnly), validArgs(valid) {}

    bool validateArgs(const std::vector<std::string> &args) const override {
        const_cast<TestCommand*>(this)->validateArgsCalled = true;
        const_cast<TestCommand*>(this)->lastArgs = args;
        return validArgs;
    }

    CommandResult execute(const std::vector<std::string> &args, IFileSystem&) override {
        executeCalled = true;
        lastArgs = args;
        return CommandResult{true, {}, "Test executed"};
    }

private:
    bool validArgs;
};

TEST_CASE("CommandService") {
    CommandRepository repo;
    MockSecurityService securityService;
    CommandService service(repo, securityService);


    SECTION("registerCommand new command") {
        auto cmd = std::make_unique<TestCommand>("newcmd");
        REQUIRE(service.registerCommand(std::move(cmd)));
        REQUIRE(service.commandExists("newcmd"));
    }

    SECTION("registerCommand null returns false") {
        REQUIRE_FALSE(service.registerCommand(nullptr));
    }

    SECTION("registerCommand existing returns false") {
        REQUIRE_FALSE(service.registerCommand(std::make_unique<TestCommand>("cd")));
    }

    SECTION("unregisterCommand removes command") {
        auto cmd = std::make_unique<TestCommand>("toremove");
        service.registerCommand(std::move(cmd));

        REQUIRE(service.unregisterCommand("toremove"));
        REQUIRE_FALSE(service.commandExists("toremove"));
    }

    SECTION("unregisterCommand nonexistent returns false") {
        REQUIRE_FALSE(service.unregisterCommand("nonexistent"));
    }

    SECTION("createCompositeCommand success") {
        REQUIRE(service.createCompositeCommand("composite1", "Test"));
        REQUIRE(service.commandExists("composite1"));

        auto* cmd = service.getCompositeCommand("composite1");
        REQUIRE(cmd);
        REQUIRE(cmd->getName() == "composite1");
    }

    SECTION("createCompositeCommand duplicate returns false") {
        REQUIRE_FALSE(service.createCompositeCommand("cd", "Duplicate"));
    }

    SECTION("deleteCompositeCommand success") {
        REQUIRE(service.createCompositeCommand("composite2", "To delete"));
        REQUIRE(service.deleteCompositeCommand("composite2"));
        REQUIRE_FALSE(service.commandExists("composite2"));
    }

    SECTION("deleteCompositeCommand non-composite returns false") {
        REQUIRE_FALSE(service.deleteCompositeCommand("cd"));
    }

    SECTION("deleteCompositeCommand nonexistent returns false") {
        REQUIRE_FALSE(service.deleteCompositeCommand("nonexistent"));
    }

    SECTION("addToComposite success") {
        REQUIRE(service.createCompositeCommand("composite3", "Test"));
        REQUIRE(service.addToComposite("composite3", "cd", {"dir1"}, {0}));

        auto* composite = service.getCompositeCommand("composite3");
        REQUIRE(composite);
    }

    SECTION("addToComposite nonexistent composite returns false") {
        REQUIRE_FALSE(service.addToComposite("nonexistent", "cd"));
    }

    SECTION("addToComposite nonexistent subcommand returns false") {
        REQUIRE(service.createCompositeCommand("composite4", "Test"));
        REQUIRE_FALSE(service.addToComposite("composite4", "nonexistent"));
    }

    SECTION("addToComposite self-reference returns false") {
        REQUIRE(service.createCompositeCommand("composite5", "Test"));
        REQUIRE_FALSE(service.addToComposite("composite5", "composite5"));
    }

    SECTION("removeFromComposite success") {
        REQUIRE(service.createCompositeCommand("composite6", "Test"));
        REQUIRE(service.addToComposite("composite6", "cd"));
        REQUIRE(service.addToComposite("composite6", "ls"));
        REQUIRE(service.removeFromComposite("composite6", 0));

        auto* composite = service.getCompositeCommand("composite6");
        REQUIRE(composite);
    }

    SECTION("removeFromComposite invalid index returns false") {
        REQUIRE(service.createCompositeCommand("composite7", "Test"));
        REQUIRE(service.addToComposite("composite7", "cd"));
        REQUIRE_FALSE(service.removeFromComposite("composite7", 1));
    }

    SECTION("getAvailableCommands returns all commands") {
        auto commands = service.getAvailableCommands();
        REQUIRE(commands.size() >= 20);

        bool hasCd = false, hasLs = false;
        for (const auto& cmd : commands) {
            if (cmd == "cd") hasCd = true;
            if (cmd == "ls") hasLs = true;
        }
        REQUIRE(hasCd);
        REQUIRE(hasLs);
    }

    SECTION("getCommand returns command") {
        auto* cmd = service.getCommand("cd");
        REQUIRE(cmd);
        REQUIRE(cmd->getName() == "cd");

        REQUIRE(service.getCommand("nonexistent") == nullptr);
    }

    SECTION("commandExists works correctly") {
        REQUIRE(service.commandExists("cd"));
        REQUIRE_FALSE(service.commandExists("nonexistent"));
    }

    SECTION("getCompositeCommands returns only composites") {
        REQUIRE(service.createCompositeCommand("comp1", "Composite 1"));
        REQUIRE(service.createCompositeCommand("comp2", "Composite 2"));

        auto composites = service.getCompositeCommands();
        REQUIRE(composites.size() >= 2);

        bool hasComp1 = false, hasComp2 = false;
        for (const auto& comp : composites) {
            if (comp == "comp1") hasComp1 = true;
            if (comp == "comp2") hasComp2 = true;
        }
        REQUIRE(hasComp1);
        REQUIRE(hasComp2);
    }

    SECTION("getCompositeInfo returns formatted string") {
        REQUIRE(service.createCompositeCommand("infoComp", "Information"));
        REQUIRE(service.addToComposite("infoComp", "cd", {"home"}, {0}));
        REQUIRE(service.addToComposite("infoComp", "ls", {}, {1}));

        std::string info = service.getCompositeInfo("infoComp");
        REQUIRE(info.find("Composite command: infoComp") != std::string::npos);
        REQUIRE(info.find("Description: Information") != std::string::npos);
    }

    SECTION("getCompositeInfo non-composite returns error") {
        std::string info = service.getCompositeInfo("cd");
        REQUIRE(info.find("Not a composite command") != std::string::npos);
    }

    SECTION("checkRecursion prevents circular references") {
        REQUIRE(service.createCompositeCommand("compA", "A"));
        REQUIRE(service.createCompositeCommand("compB", "B"));

        REQUIRE(service.addToComposite("compA", "compB"));
        REQUIRE_FALSE(service.addToComposite("compA", "compA"));
    }
}