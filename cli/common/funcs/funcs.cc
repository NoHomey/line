#include "./funcs.h"
#include <sys/stat.h>
#include <errno.h>
#include <iostream>
#include <fstream>
#include <cassert>
#include "../Navigator/Navigator.h"

enum class DirectoryCheckResult {NotExist, Error, NotADirectory, Directory};

static DirectoryCheckResult checkIsDirectory(const char* directoryPath) {
    struct stat statStruct;
    errno = 0;
    if(lstat(directoryPath, &statStruct)) {
        if(errno == ENOENT) {
            return DirectoryCheckResult::NotExist;
        } else {
            return DirectoryCheckResult::Error;
        }
    }
    if(!S_ISDIR(statStruct.st_mode)) {
        return DirectoryCheckResult::NotADirectory;
    }
    return DirectoryCheckResult::Directory;
}

bool line::cli::common::funcs::isDirectory(const char* directoryPath) {
    switch(checkIsDirectory(directoryPath)) {
        case DirectoryCheckResult::NotExist:
            std::cout << directoryPath << " is not an existing directory." << std::endl;
            return false;
        case DirectoryCheckResult::Error:
            std::cout << "Error occured while trying to check if "
                << directoryPath << " is directory." << std::endl;
            return false;
        case DirectoryCheckResult::NotADirectory:
            std::cout << directoryPath << " is not a directory." << std::endl;
            return false;
        case DirectoryCheckResult::Directory: return true;
        default: assert(false);
    }
}

bool line::cli::common::funcs::isRepository() {
    line::cli::common::Navigator& navigator = line::cli::common::Navigator::navigator();
    if(!isDirectory(navigator.navigateToDirectory())) {
        return false;
    }
    switch(checkIsDirectory(navigator.navigateToRepoInfoDir())) {
        case DirectoryCheckResult::NotExist:
            std::cout << navigator.navigateToDirectory()
                << " is not a line reposiory. Run 'line init "
                << navigator.path() << "' to initialize it as an empty line repository." << std::endl;
            return false;
        case DirectoryCheckResult::Error:
            std::cout << "Error occured while trying to check if "
                << navigator.navigateToDirectory() << " is line repository." << std::endl;
            return false;
        case DirectoryCheckResult::NotADirectory:
            std::cout << std::cout << navigator.navigateToDirectory()
                << " dose not seems to be a valid line repository. "
                << navigator.navigateToRepoInfoDir() << " is not a directory." << std::endl;
            return false;
        case DirectoryCheckResult::Directory: return true;
        default: assert(false);
    }
}

std::size_t line::cli::common::funcs::readCommitsCounter() {
    line::cli::common::Navigator& navigator = line::cli::common::Navigator::navigator();
    std::ifstream commitsCounterFile;
    std::size_t commitsCounter;
    commitsCounterFile.exceptions(std::ifstream::badbit);
    commitsCounterFile.open(navigator.navigateToCommitsCounter());
    commitsCounterFile >> commitsCounter;
    return commitsCounter;
}