#include <catch2/catch_test_macros.hpp>
#include <memory>

#include "Command/CommandRep/realisation/command_repository.h"
#include "Command/Commands/realisation/Base/base_command.h"
#include "FileSystem/interface/i_file_system.h"
#include "Service/SecurityService/interface/i_security_service.h"

class TestCommand : public BaseCommand {
public:
    TestCommand(const std::string &name, bool adminOnly = false)
        : BaseCommand(name, "Test command", name + " [args]", adminOnly) {}

    bool validateArgs(const std::vector<std::string> &args) const override {
        return !args.empty();
    }

    CommandResult execute(const std::vector<std::string> &args, IFileSystem& fs) override {
        return CommandResult{true, {}, "Executed: " + name + " with " + std::to_string(args.size()) + " args"};
    }
};


TEST_CASE("CommandRepository") {
    CommandRepository repo;

    SECTION("constructor initializes default commands") {
        REQUIRE(repo.commandExists("cd"));
        REQUIRE(repo.commandExists("ls"));
        REQUIRE(repo.commandExists("mkdir"));
        REQUIRE(repo.commandExists("touch"));
        REQUIRE(repo.commandExists("cat"));
        REQUIRE(repo.commandExists("rm"));
        REQUIRE(repo.commandExists("cp"));
        REQUIRE(repo.commandExists("mv"));
        REQUIRE(repo.commandExists("chmod"));
        REQUIRE(repo.commandExists("chown"));
        REQUIRE(repo.commandExists("useradd"));
        REQUIRE(repo.commandExists("groupadd"));
        REQUIRE(repo.commandExists("usermod"));
        REQUIRE(repo.commandExists("userdel"));
        REQUIRE(repo.commandExists("groupdel"));
        REQUIRE(repo.commandExists("edit"));
        REQUIRE(repo.commandExists("save"));
        REQUIRE(repo.commandExists("load"));
        REQUIRE(repo.commandExists("stat"));
        REQUIRE(repo.commandExists("find"));
    }

    SECTION("saveCommand adds new command") {
        auto cmd = std::make_unique<TestCommand>("testcmd");
        REQUIRE(repo.saveCommand("testcmd", std::move(cmd)));
        REQUIRE(repo.commandExists("testcmd"));
    }

    SECTION("saveCommand with null returns false") {
        REQUIRE_FALSE(repo.saveCommand("nullcmd", nullptr));
        REQUIRE_FALSE(repo.commandExists("nullcmd"));
    }

    SECTION("saveCommand overwrites existing command") {
        auto cmd1 = std::make_unique<TestCommand>("overwrite", false);
        REQUIRE(repo.saveCommand("overwrite", std::move(cmd1)));

        auto cmd2 = std::make_unique<TestCommand>("overwrite", true);
        REQUIRE(repo.saveCommand("overwrite", std::move(cmd2)));

        auto* retrieved = repo.getCommand("overwrite");
        REQUIRE(retrieved->isOnlyForAdmin());
    }

    SECTION("deleteCommand removes command") {
        auto cmd = std::make_unique<TestCommand>("todelete");
        repo.saveCommand("todelete", std::move(cmd));

        REQUIRE(repo.commandExists("todelete"));
        REQUIRE(repo.deleteCommand("todelete"));
        REQUIRE_FALSE(repo.commandExists("todelete"));
    }

    SECTION("deleteCommand nonexistent returns false") {
        REQUIRE_FALSE(repo.deleteCommand("nonexistent"));
    }

    SECTION("getCommand returns correct command") {
        auto* cmd = repo.getCommand("cd");
        REQUIRE(cmd);
        REQUIRE(cmd->getName() == "cd");
    }

    SECTION("getCommand returns nullptr for nonexistent") {
        REQUIRE(repo.getCommand("nonexistent") == nullptr);
    }

    SECTION("commandExists works correctly") {
        REQUIRE(repo.commandExists("ls"));
        REQUIRE_FALSE(repo.commandExists("nonexistent"));
    }

    SECTION("getCommandNames returns all names") {
        auto names = repo.getCommandNames();
        REQUIRE(names.size() >= 20);

        bool hasCd = false, hasLs = false;
        for (const auto& name : names) {
            if (name == "cd") hasCd = true;
            if (name == "ls") hasLs = true;
        }
        REQUIRE(hasCd);
        REQUIRE(hasLs);
    }

    SECTION("createCompositeCommand creates composite") {
        REQUIRE(repo.createCompositeCommand("composite1", "Test"));
        REQUIRE(repo.commandExists("composite1"));

        auto* cmd = repo.getCommand("composite1");
        REQUIRE(cmd);
        REQUIRE(cmd->getName() == "composite1");
    }

    SECTION("getAllCommands returns all commands") {
        auto all = repo.getAllCommands();
        REQUIRE(all.size() >= 20);
        REQUIRE(all["cd"]->getName() == "cd");
        REQUIRE(all["ls"]->getName() == "ls");
    }
}