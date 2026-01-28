#include <catch2/catch_test_macros.hpp>

#include "Command/Commands/realisation/Composite/composite_command.h"
#include "FileSystem/interface/i_file_system.h"
#include <stdexcept>

TEST_CASE("CompositeCommand") {

    SECTION("constructor creates command with correct properties") {
        CompositeCommand cmd("composite", "Composite command description");
        
        REQUIRE(cmd.getName() == "composite");
        REQUIRE(cmd.getDescription() == "Composite command description");
        REQUIRE(cmd.getUsage() == "composite [args...]\nComposite command: Composite command description");
        REQUIRE(cmd.isOnlyForAdmin() == false);
    }

    SECTION("validateArgs always returns true") {
        CompositeCommand cmd("test", "Desc");
        
        REQUIRE(cmd.validateArgs({}) == true);
        REQUIRE(cmd.validateArgs({"arg1"}) == true);
        REQUIRE(cmd.validateArgs({"arg1", "arg2", "arg3"}) == true);
    }

    SECTION("addCommand with valid parameters") {
        CompositeCommand cmd("test", "Desc");
        
        REQUIRE_NOTHROW(cmd.addCommand("cmd1"));
        REQUIRE(cmd.getCommandCount() == 1);
        
        REQUIRE_NOTHROW(cmd.addCommand("cmd2", {"fixed1", "fixed2"}));
        REQUIRE(cmd.getCommandCount() == 2);
        
        REQUIRE_NOTHROW(cmd.addCommand("cmd3", {}, {0, 2}));
        REQUIRE(cmd.getCommandCount() == 3);
        
        REQUIRE_NOTHROW(cmd.addCommand("cmd4", {"fixed"}, {1, 3}));
        REQUIRE(cmd.getCommandCount() == 4);
    }

    SECTION("addCommand with empty name throws") {
        CompositeCommand cmd("test", "Desc");
        REQUIRE_THROWS_AS(cmd.addCommand(""), std::invalid_argument);
    }

    SECTION("addCommand with negative dynamic index throws") {
        CompositeCommand cmd("test", "Desc");
        REQUIRE_THROWS_AS(cmd.addCommand("cmd", {}, {-1}), std::invalid_argument);
    }

    SECTION("removeCommand removes commands correctly") {
        CompositeCommand cmd("test", "Desc");
        cmd.addCommand("cmd1");
        cmd.addCommand("cmd2");
        cmd.addCommand("cmd3");
        
        REQUIRE(cmd.getCommandCount() == 3);
        
        REQUIRE(cmd.removeCommand(1) == true);
        REQUIRE(cmd.getCommandCount() == 2);
        
        REQUIRE(cmd.removeCommand(0) == true);
        REQUIRE(cmd.getCommandCount() == 1);
        
        REQUIRE(cmd.removeCommand(0) == true);
        REQUIRE(cmd.getCommandCount() == 0);
    }

    SECTION("removeCommand with out of bounds index returns false") {
        CompositeCommand cmd("test", "Desc");
        cmd.addCommand("cmd1");
        
        REQUIRE(cmd.removeCommand(1) == false);
        REQUIRE(cmd.removeCommand(100) == false);
    }

    SECTION("clearCommands removes all commands") {
        CompositeCommand cmd("test", "Desc");
        cmd.addCommand("cmd1");
        cmd.addCommand("cmd2");
        cmd.addCommand("cmd3");
        
        REQUIRE(cmd.getCommandCount() == 3);
        
        cmd.clearCommands();
        REQUIRE(cmd.getCommandCount() == 0);
        
        cmd.addCommand("cmd4");
        REQUIRE(cmd.getCommandCount() == 1);
        
        cmd.clearCommands();
        REQUIRE(cmd.getCommandCount() == 0);
    }

    SECTION("getSubCommands returns correct entries") {
        CompositeCommand cmd("test", "Desc");
        cmd.addCommand("cmd1", {"fixed1"}, {0});
        cmd.addCommand("cmd2", {}, {1, 2});
        
        const auto& subCommands = cmd.getSubCommands();
        REQUIRE(subCommands.size() == 2);
        
        REQUIRE(subCommands[0].commandName == "cmd1");
        REQUIRE(subCommands[0].fixedArgs.size() == 1);
        REQUIRE(subCommands[0].fixedArgs[0] == "fixed1");
        
        REQUIRE(subCommands[1].commandName == "cmd2");
        REQUIRE(subCommands[1].fixedArgs.empty());
    }

    SECTION("getCommandCount returns correct number") {
        CompositeCommand cmd("test", "Desc");
        
        REQUIRE(cmd.getCommandCount() == 0);
        
        cmd.addCommand("cmd1");
        REQUIRE(cmd.getCommandCount() == 1);
        
        cmd.addCommand("cmd2");
        cmd.addCommand("cmd3");
        REQUIRE(cmd.getCommandCount() == 3);
        
        cmd.removeCommand(1);
        REQUIRE(cmd.getCommandCount() == 2);
        
        cmd.clearCommands();
        REQUIRE(cmd.getCommandCount() == 0);
    }
}