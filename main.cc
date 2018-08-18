
#include <iostream>
#include "./core/DirectoryStructure.thd"

int main() {
    line::core::DirectoryStructure<int> tree{"~/tmp/play-with-git"};

    tree.insert(
        line::core::FilePathIterator{line::core::String::StringSlice{"a/b/c/f.txt", 11}},
        22
    );

    tree.insert(
        line::core::FilePathIterator{line::core::String::StringSlice{"a/folder/f.txt", 14}},
        18
    );

    std::cout << tree.remove(
        line::core::FilePathIterator{line::core::String::StringSlice{"a/b/c/f.txt", 11}}
    ) << std::endl;

    return 0;
}

/*#include <cassert>
#include <cstring>
#include "./cli/init/init.h"
#include "./cli/log/log.h"
#include "./cli/status/status.h"
#include "./cli/commit/commit.h"
#include "./cli/revert/revert.h"

struct Command {
    const char* name;
    void (*exec)(int, char**);
};

static bool areEqual(const char* str1, const char* str2) {
    assert(str1);
    assert(str2);
    return !std::strcmp(str1, str2);
}

static void seeHelp() {
    std::cout << " See 'line help' for list of avalible line commands. See 'line help <command>' for more info about a given line command." << std::endl;
}

int main(int argc, char** argv) {
    const unsigned int commandsCount = 5;
    Command commands[commandsCount] = {
        {"init", line::cli::init},
        {"log", line::cli::log},
        {"status", line::cli::status},
        {"commit", line::cli::commit},
        {"revert", line::cli::revert}
    };
    void (*command)(int, char**) = nullptr;
    if(argc == 1) {
        std::cout << "line expects at least one argument. The command to execute.";
        seeHelp();
        return 0;
    } else {
        for(unsigned int i = 0; i < commandsCount; ++i) {
            if(areEqual(argv[1], commands[i].name)) {
                command = commands[i].exec;
                break;
            }
        }
        if(command) {
            command(argc - 2, argv + 2);
        } else {
            std::cout << '\'' << "line " << argv[1] << '\'' << " is not a line command.";
            seeHelp();
        }
    }

    return 0;
}*/