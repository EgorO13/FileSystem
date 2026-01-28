#include <catch2/catch_test_macros.hpp>

#include "Command/Commands/realisation/Base/base_command.h"
#include "FileSystem/interface/i_file_system.h"
#include <stdexcept>

class TestCommand : public BaseCommand {
public:
    TestCommand(const std::string &name, const std::string &description, 
                const std::string &usage, bool adminOnly = false)
        : BaseCommand(name, description, usage, adminOnly) {}

    bool validateArgs(const std::vector<std::string> &args) const override {
        return args.size() >= 1;
    }

    CommandResult execute(const std::vector<std::string> &args, IFileSystem& fs) override {
        if (!validateArgs(args)) {
            return CommandResult{false, {}, "Invalid arguments"};
        }
        return CommandResult{true, {}, "Test command executed with " + std::to_string(args.size()) + " args"};
    }
};


TEST_CASE("BaseCommand") {
    SECTION("constructor valid arguments") {
        REQUIRE_NOTHROW(TestCommand("test", "Test command", "test [arg]", false));
        REQUIRE_NOTHROW(TestCommand("cmd", "Another command", "cmd", true));
    }

    SECTION("constructor empty name throws") {
        REQUIRE_THROWS_AS(TestCommand("", "Desc", "usage"), std::invalid_argument);
    }

    SECTION("constructor name with slash throws") {
        REQUIRE_THROWS_AS(TestCommand("test/name", "Desc", "usage"), std::invalid_argument);
    }

    SECTION("constructor name with dot throws") {
        REQUIRE_THROWS_AS(TestCommand("test.name", "Desc", "usage"), std::invalid_argument);
    }

    SECTION("getName returns correct name") {
        TestCommand cmd("testcmd", "Description", "testcmd arg", false);
        REQUIRE(cmd.getName() == "testcmd");
    }

    SECTION("getDescription returns correct description") {
        TestCommand cmd("test", "Test description", "usage", false);
        REQUIRE(cmd.getDescription() == "Test description");
    }

    SECTION("getUsage returns correct usage") {
        TestCommand cmd("test", "Desc", "test [options]", false);
        REQUIRE(cmd.getUsage() == "test [options]");
    }

    SECTION("isOnlyForAdmin returns correct value") {
        TestCommand cmd1("userCmd", "Desc", "usage", false);
        TestCommand cmd2("adminCmd", "Desc", "usage", true);
        
        REQUIRE(cmd1.isOnlyForAdmin() == false);
        REQUIRE(cmd2.isOnlyForAdmin() == true);
    }

    SECTION("validateArgs works correctly") {
        TestCommand cmd("test", "Desc", "usage", false);
        REQUIRE(cmd.validateArgs({"arg1"}) == true);
        REQUIRE(cmd.validateArgs({"arg1", "arg2"}) == true);
        REQUIRE(cmd.validateArgs({}) == false);
    }
}