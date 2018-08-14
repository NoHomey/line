#include "./funcs.h"
#include <sys/stat.h>
#include <errno.h>
#include <iostream>

bool line::cli::common::funcs::isDirectory(const char* directoryPath) {
    struct stat statStruct;
    errno = 0;
    if(lstat(directoryPath, &statStruct)) {
        if(errno == ENOENT) {
            std::cout << directoryPath << " is not pointing to existing directory" << std::endl;
        } else {
            std::cout << "Error occured while trying to check if "
                << directoryPath << " is directory." << std::endl;
        }
        return false;
    }
    if(!S_ISDIR(statStruct.st_mode)) {
        std::cout << directoryPath << " is not a directory." << std::endl;
        return false;
    }
    return true;
}