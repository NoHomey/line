
#include <iostream>
#include <cassert>
#include "./core/PatternMatcher/PatternMatcher.h"

int main() {
    std::cout << line::core::PatternMatcher::test(
        line::core::String::StringSlice{"acb", 3},
        line::core::String::StringSlice{"a*b", 3}
    ) << std::endl;
    std::cout << line::core::PatternMatcher::test(
        line::core::String::StringSlice{"ab", 2},
        line::core::String::StringSlice{"a*b", 3}
    ) << std::endl;
    std::cout << line::core::PatternMatcher::test(
        line::core::String::StringSlice{"ab", 2},
        line::core::String::StringSlice{"a**b", 4}
    ) << std::endl;
    std::cout << line::core::PatternMatcher::test(
        line::core::String::StringSlice{"ab", 2},
        line::core::String::StringSlice{"a***b", 5}
    ) << std::endl;
    std::cout << line::core::PatternMatcher::test(
        line::core::String::StringSlice{"acdb", 4},
        line::core::String::StringSlice{"a**b", 4}
    ) << std::endl;
    std::cout << line::core::PatternMatcher::test(
        line::core::String::StringSlice{"acdb", 4},
        line::core::String::StringSlice{"a*", 2}
    ) << std::endl;
    std::cout << line::core::PatternMatcher::test(
        line::core::String::StringSlice{"abc", 3},
        line::core::String::StringSlice{"a*b*c", 5}
    ) << std::endl;
    std::cout << line::core::PatternMatcher::test(
        line::core::String::StringSlice{"abbbdccbc", 9},
        line::core::String::StringSlice{"a*b*c", 5}
    ) << std::endl;
    std::cout << !line::core::PatternMatcher::test(
        line::core::String::StringSlice{"abbbdccbd", 9},
        line::core::String::StringSlice{"a*b*c", 5}
    ) << std::endl;
    std::cout << !line::core::PatternMatcher::test(
        line::core::String::StringSlice{"abbbdccbd", 9},
        line::core::String::StringSlice{"a*b*c?", 6}
    ) << std::endl;
    std::cout << line::core::PatternMatcher::test(
        line::core::String::StringSlice{"abbbdccbcd", 10},
        line::core::String::StringSlice{"a*b*c?", 6}
    ) << std::endl;
    std::cout << line::core::PatternMatcher::test(
        line::core::String::StringSlice{"abbbdccbcd", 10},
        line::core::String::StringSlice{"a*b*c?", 6}
    ) << std::endl;
    std::cout << line::core::PatternMatcher::test(
        line::core::String::StringSlice{"abbbdccbcd", 10},
        line::core::String::StringSlice{"?*?*?", 5}
    ) << std::endl;
    std::cout << line::core::PatternMatcher::test(
        line::core::String::StringSlice{"abc", 3},
        line::core::String::StringSlice{"?*?*?", 5}
    ) << std::endl;
    std::cout << !line::core::PatternMatcher::test(
        line::core::String::StringSlice{"ab", 2},
        line::core::String::StringSlice{"?*?*?", 5}
    ) << std::endl;
    std::cout << line::core::PatternMatcher::test(
        line::core::String::StringSlice{"abcd", 4},
        line::core::String::StringSlice{"?*?*?", 5}
    ) << std::endl;
    std::cout << line::core::PatternMatcher::test(
        line::core::String::StringSlice{"abcd", 4},
        line::core::String::StringSlice{"*", 1}
    ) << std::endl;
    std::cout << line::core::PatternMatcher::test(
        line::core::String::StringSlice{"abcd", 4},
        line::core::String::StringSlice{"a**?*?", 6}
    ) << std::endl;
    std::cout << line::core::PatternMatcher::test(
        line::core::String::StringSlice{"abcd", 4},
        line::core::String::StringSlice{"a**?*?*?", 7}
    ) << std::endl;
    std::cout << !line::core::PatternMatcher::test(
        line::core::String::StringSlice{"abcd", 4},
        line::core::String::StringSlice{"a**?*?**?*?", 11}
    ) << std::endl;
    std::cout << line::core::PatternMatcher::test(
        line::core::String::StringSlice{"abcddd", 6},
        line::core::String::StringSlice{"a**?*?**?*?d", 12}
    ) << std::endl;
    std::cout << line::core::PatternMatcher::test(
        line::core::String::StringSlice{"abcddd", 6},
        line::core::String::StringSlice{"a**?*?**?*?d*", 13}
    ) << std::endl;
    std::cout << line::core::PatternMatcher::test(
        line::core::String::StringSlice{"abcddd", 6},
        line::core::String::StringSlice{"?*****", 6}
    ) << std::endl;
    std::cout << line::core::PatternMatcher::test(
        line::core::String::StringSlice{"a", 1},
        line::core::String::StringSlice{"*?*", 3}
    ) << std::endl;
    std::cout << line::core::PatternMatcher::test(
        line::core::String::StringSlice{"aaa", 3},
        line::core::String::StringSlice{"*?*?*?*", 7}
    ) << std::endl;
    std::cout << !line::core::PatternMatcher::test(
        line::core::String::StringSlice{"aa", 2},
        line::core::String::StringSlice{"*?*?*?*", 7}
    ) << std::endl;
    std::cout << line::core::PatternMatcher::test(
        line::core::String::StringSlice{"aa", 2},
        line::core::String::StringSlice{"*??*", 4}
    ) << std::endl;
    std::cout << line::core::PatternMatcher::test(
        line::core::String::StringSlice{"aa", 2},
        line::core::String::StringSlice{"*?*?*", 5}
    ) << std::endl;
    std::cout << line::core::PatternMatcher::test(
        line::core::String::StringSlice{"aaacbca", 7},
        line::core::String::StringSlice{"*bc*", 4}
    ) << std::endl;
    std::cout << line::core::PatternMatcher::test(
        line::core::String::StringSlice{"aaabbcbca", 9},
        line::core::String::StringSlice{"*b?c*", 5}
    ) << std::endl;
    std::cout << line::core::PatternMatcher::test(
        line::core::String::StringSlice{"cccbcc", 6},
        line::core::String::StringSlice{"*cbc*", 5}
    ) << std::endl;
    std::cout << line::core::PatternMatcher::test(
        line::core::String::StringSlice{"cccbcc", 6},
        line::core::String::StringSlice{"*cbc*c", 6}
    ) << std::endl;
    std::cout << line::core::PatternMatcher::test(
        line::core::String::StringSlice{"ccbccbcc", 8},
        line::core::String::StringSlice{"*cb?c*c", 7}
    ) << std::endl;
    std::cout << !line::core::PatternMatcher::test(
        line::core::String::StringSlice{"ccbcbcc", 7},
        line::core::String::StringSlice{"*cb?c*c", 7}
    ) << std::endl;
    std::cout << !line::core::PatternMatcher::test(
        line::core::String::StringSlice{"cbcaccc", 7},
        line::core::String::StringSlice{"*cb?c*c", 7}
    ) << std::endl;
    std::cout << !line::core::PatternMatcher::test(
        line::core::String::StringSlice{"c", 1},
        line::core::String::StringSlice{"*c*c", 4}
    ) << std::endl;
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