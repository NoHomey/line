#include "./init.h"
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <cstring>

static int mkdirWithPermisions(const char* directory) {
    errno = 0;
    return mkdir(directory, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}

static void warnFailedToInitializeRepostiory(const char* directory) {
    std::cout << "Failed to initialize line repository in " << directory << std::endl;
}

static void initRepository(const char* directory) {
    const std::size_t additionalBytesNeeded = 15;
    const std::size_t directoryLength = std::strlen(directory);
    char* lineDirectory = new char[directoryLength + additionalBytesNeeded];
    std::memcpy(lineDirectory, directory, directoryLength);
    std::memcpy(lineDirectory + directoryLength, "/.line", 7);
    if(mkdirWithPermisions(lineDirectory)) {
        if(errno == EEXIST) {
            std::cout << directory << " is a line repository." << std::endl;
        } else {
            warnFailedToInitializeRepostiory(directory);
        }
    } else {
        std::memcpy(lineDirectory + directoryLength + 6, "/objects", 9);
        if(mkdirWithPermisions(lineDirectory)) {
            warnFailedToInitializeRepostiory(directory);
        } else {
            std::memcpy(lineDirectory + directoryLength + 6, "/commits", 9);
            if(mkdirWithPermisions(lineDirectory)) {
                warnFailedToInitializeRepostiory(directory);
            } else {
                std::cout << "Initialized empty line repository in " << directory << std::endl;
            }
        }
    }
    delete[] lineDirectory;
}

void line::cli::init(int argc, char** argv) {
    if(!argc) {
        std::cout << "'line init' expects one argument. The directory to be initialized as line repository." << std::endl;
        return;
    }
    if(argc > 1) {
        std::cout << "'line init' expects only one argument. The directory to be initialized as line repository." << std::endl;
        return;
    }
    const char* directory = argv[0];
    struct stat statStruct;
    errno = 0;
    if(lstat(directory, &statStruct)) {
        if(errno == ENOENT) {
            std::cout << directory << " is not pointing to existing directory" << std::endl;
        } else {
            std::cout << "Error occured while trying to check if "
                << directory << " is directory." << std::endl;
        }
    } else {
        if(S_ISDIR(statStruct.st_mode)) {
            initRepository(directory);
        } else {
            std::cout << directory << " is not a directory." << std::endl;
        }
    }
}