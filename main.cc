
#include <iostream>
#include <cassert>
#include <cstring>
#include "./cli/init/init.h"

static bool areEqual(const char* str1, const char* str2) {
    assert(str1);
    assert(str2);
    return !std::strcmp(str1, str2);
}

static void seeHelp() {
    std::cout << " See 'line help' for list of avalible line commands. See 'line help <command>' for more info about a given line command." << std::endl;
}

int main(int argc, char** argv) {
    if(argc == 1) {
        std::cout << "line expects at least one argument. The command to execute.";
        seeHelp();
    } else {
        if(areEqual(argv[1], "init")) {
            line::cli::init(argc - 2, argv + 2);
        } else {
            std::cout << '\'' << "line " << argv[1] << '\''
                << " is not a line command. See 'line help' for list of avalible line commands.";
                seeHelp();
        }
    }

    return 0;
}