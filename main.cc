
#include <iostream>
#include <cassert>
#include "./cli/common/FilePathMatcher/FilePathMatcher.h"

int main() {
    {
        line::cli::common::FilePathMatcher matcher{};
        std::cout << matcher.match(line::core::String::StringSlice{"file.txt", 8}) << std::endl;
        std::cout << matcher.match(line::core::String::StringSlice{"folder/file.txt", 15}) << std::endl;
        std::cout << matcher.match(line::core::String::StringSlice{"folder/a/b/c/file.txt", 21}) << std::endl;
    }
    {
        line::cli::common::FilePathMatcher matcher{line::core::String::StringSlice{"*", 1}};
        std::cout << matcher.match(line::core::String::StringSlice{"file.txt", 8}) << std::endl;
        std::cout << matcher.match(line::core::String::StringSlice{"folder/file.txt", 15}) << std::endl;
        std::cout << matcher.match(line::core::String::StringSlice{"folder/a/b/c/file.txt", 21}) << std::endl;
    }
    {
        line::cli::common::FilePathMatcher matcher{line::core::String::StringSlice{"*/file*.txt", 11}};
        std::cout << !matcher.match(line::core::String::StringSlice{"file.txt", 8}) << std::endl;
        std::cout << matcher.match(line::core::String::StringSlice{"folder/file.txt", 15}) << std::endl;
        std::cout << !matcher.match(line::core::String::StringSlice{"folder/a/b/c/file.txt", 21}) << std::endl;
    }
    {
        line::cli::common::FilePathMatcher matcher{line::core::String::StringSlice{"folder/*/b", 10}};
        std::cout << !matcher.match(line::core::String::StringSlice{"file.txt", 8}) << std::endl;
        std::cout << !matcher.match(line::core::String::StringSlice{"folder/file.txt", 15}) << std::endl;
        std::cout << matcher.match(line::core::String::StringSlice{"folder/a/b/c/file.txt", 21}) << std::endl;
        std::cout << matcher.match(line::core::String::StringSlice{"folder/a/b/file.txt", 19}) << std::endl;
        std::cout << matcher.match(line::core::String::StringSlice{"folder/a/b/c/d/file.txt", 23}) << std::endl;
        std::cout << matcher.match(line::core::String::StringSlice{"folder/abc/b/c/file.txt", 23}) << std::endl;
        std::cout << matcher.match(line::core::String::StringSlice{"folder/abc/b/file.txt", 21}) << std::endl;
        std::cout << matcher.match(line::core::String::StringSlice{"folder/abc/b/c/d/file.txt", 25}) << std::endl;
    }
    {
        line::cli::common::FilePathMatcher matcher{line::core::String::StringSlice{"folder/*/?/*/d", 14}};
        std::cout << !matcher.match(line::core::String::StringSlice{"file.txt", 8}) << std::endl;
        std::cout << !matcher.match(line::core::String::StringSlice{"folder/file.txt", 15}) << std::endl;
        std::cout << !matcher.match(line::core::String::StringSlice{"folder/a/b/c/file.txt", 21}) << std::endl;
        std::cout << !matcher.match(line::core::String::StringSlice{"folder/a/b/file.txt", 19}) << std::endl;
        std::cout << matcher.match(line::core::String::StringSlice{"folder/a/b/c/d/file.txt", 23}) << std::endl;
        std::cout << matcher.match(line::core::String::StringSlice{"folder/aaa/b/cd/d/file.txt", 26}) << std::endl;
        std::cout << !matcher.match(line::core::String::StringSlice{"folder/aaa/b/file.txt", 21}) << std::endl;
        std::cout << matcher.match(line::core::String::StringSlice{"folder/aaa/b/cd/d/file.txt", 26}) << std::endl;
    }
    {
        line::cli::common::FilePathMatcher matcher{line::core::String::StringSlice{"folder/man*/page?*/*/dir/*", 26}};

        std::cout << matcher.match(line::core::String::StringSlice{"folder/man/page1/some/dir/file.txt", 34}) << std::endl;
        std::cout << matcher.match(line::core::String::StringSlice{"folder/man/page12/a/dir/folder/file.txt", 39}) << std::endl;
        std::cout << matcher.match(line::core::String::StringSlice{"folder/man/page12/a/dir/folder/some/verry/long/path/file.txt", 60}) << std::endl;
        std::cout << matcher.match(line::core::String::StringSlice{"folder/man_node/page123/chapter1/dir/some_ver_long_file_name.txt", 64}) << std::endl;

        std::cout << !matcher.match(line::core::String::StringSlice{"file.txt", 8}) << std::endl;
        std::cout << !matcher.match(line::core::String::StringSlice{"folder/file.txt", 15}) << std::endl;
        std::cout << !matcher.match(line::core::String::StringSlice{"folder/a/b/c/file.txt", 21}) << std::endl;
        std::cout << !matcher.match(line::core::String::StringSlice{"folder/man/page1/some/dir123/file.txt", 37}) << std::endl;
        std::cout << !matcher.match(line::core::String::StringSlice{"folder/man/page/a/dir/folder/file.txt", 37}) << std::endl;
        std::cout << !matcher.match(line::core::String::StringSlice{"folder/man/page12/dir/folder/some/verry/long/path/file.txt", 58}) << std::endl;
        std::cout << !matcher.match(line::core::String::StringSlice{"folder/some_book/page123/chapter1/dir/some_ver_long_file_name.txt", 65}) << std::endl;
    }

    return 0;
}

/*#include <cstring>
#include "./cli/init/init.h"
#include "./cli/log/log.h"
#include "./cli/status/status.h"
#include "./cli/commit/commit.h"

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
    const unsigned int commandsCount = 4;
    Command commands[commandsCount] = {
        {"init", line::cli::init},
        {"log", line::cli::log},
        {"status", line::cli::status},
        {"commit", line::cli::commit}
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