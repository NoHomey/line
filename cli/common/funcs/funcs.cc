#include "./funcs.h"
#include <sys/stat.h>
#include <errno.h>
#include <iostream>
#include <fstream>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include "../Navigator/Navigator.h"
#include "../FileLineReader/FileLineReader.h"
#include "../Timestamp/Timestamp.h"
#include "../../core/FilePathIterator/FilePathIterator.h"

line::cli::common::FileStatus::FileStatus(const line::core::Hasher::Hash& fileHash, FileAction action) noexcept
: fileHash{fileHash}, action{action} { }

line::cli::common::DirectoryCheckResult line::cli::common::funcs::checkIsDirectory(const char* directoryPath) {
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

int line::cli::common::funcs::mkdirWithPermisions(const char* directory) {
    errno = 0;
    return mkdir(directory, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
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
            std::cout << navigator.navigateToDirectory()
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
    commitsCounterFile.close();
    return commitsCounter;
}

bool line::cli::common::funcs::fileExists(const char* filePath) {
    std::ifstream file{filePath};
    bool isGood = file.good();
    file.close();
    return isGood;
}

void line::cli::common::funcs::copyFile(const char* destFilePath, const char* srcFilePath) {
    std::ofstream destFile;
    std::ifstream srcFile;
    static char buffer[4096];
    destFile.exceptions(std::ofstream::badbit);
    srcFile.exceptions(std::ifstream::badbit);
    destFile.open(destFilePath);
    srcFile.open(srcFilePath);
    while(srcFile) {
        srcFile.read(buffer, 4096);
        if(srcFile.gcount()) {
            destFile.write(buffer, srcFile.gcount());
        }
    }
    srcFile.close();
    destFile.close();
}

line::cli::common::FileInfoFromCommitLine
line::cli::common::funcs::parseFileInfoFromCommitLine(const line::core::String::StringSlice& commitLine) noexcept {
    line::core::String::StringSlice relativeFilePath{
        commitLine.beginning,
        commitLine.count - (line::core::Hasher::Hash::hexHashCodeLength + 1)
    };
    line::core::String::StringSlice hexHashCode{
        commitLine.beginning + (relativeFilePath.count + 1),
        line::core::Hasher::Hash::hexHashCodeLength
    };
    return {relativeFilePath, hexHashCode};
}

bool line::cli::common::funcs::parseCommitId(std::size_t& commitId, const char* str, std::size_t headId) {
    assert(str);
    assert(str[0]);
    if(!std::strcmp(str, "HEAD")) {
        commitId = headId;
        return true;
    }
    char* end;
    commitId = std::strtoul(str, &end, 10);
    if(end == (str + std::strlen(str))) {
        return true;
    }
    return false;
}

bool line::cli::common::funcs::ensurePathExists(line::core::PathBuilder& pathBuilder, const line::core::String::StringSlice& filePath) {
    line::cli::common::Navigator& navigator = line::cli::common::Navigator::navigator();
    line::core::FilePathIterator filePathIterator{filePath};
    pathBuilder.empty();
    pathBuilder.addToPath(navigator.navigateToDirectory());
    while(filePathIterator) {
        pathBuilder.addToPath(*filePathIterator);
        line::cli::common::DirectoryCheckResult directoryCheckResult
            = line::cli::common::funcs::checkIsDirectory(pathBuilder.path().beginning);
        if(directoryCheckResult == line::cli::common::DirectoryCheckResult::NotADirectory) {
            return false;
        }
        if(directoryCheckResult == line::cli::common::DirectoryCheckResult::NotExist) {
            break;
        }
        ++filePathIterator;
    }
    while(filePathIterator) {
        if(line::cli::common::funcs::mkdirWithPermisions(pathBuilder.path().beginning)) {
            return false;
        }
        ++filePathIterator;
        if(filePathIterator) {
            pathBuilder.addToPath(*filePathIterator);
        }
    }
    pathBuilder.addToPath(filePathIterator.getFileName());
    return true;
}

static void failFromCheckout(std::size_t commitId) {
    line::cli::common::FileLineReader commitFile{line::cli::common::Navigator::navigator().navigateToCommit(commitId)};
    line::cli::common::Timestamp commitTimestamp = line::cli::common::Timestamp::fromString((*commitFile).beginning);
    ++commitFile;
    std::cout << "Fatal: this operation can't be performed while beeing checkouted to commit preceding HEAD." << std::endl;
    std::cout << "Currently checkouted at commit " << commitId << ": \""
        << (*commitFile) << "\" from " << commitTimestamp << " by ";
    ++commitFile;
    std::cout << (*commitFile) << std::endl;
} 

bool line::cli::common::funcs::isCheckouted() {
    line::cli::common::Navigator& navigator = line::cli::common::Navigator::navigator();
    if(fileExists(navigator.navigateToCheckoutInfo())) {
        std::ifstream checkout;
        std::size_t commitId;
        checkout.exceptions(std::ifstream::badbit);
        checkout.open(navigator.path());
        checkout >> commitId;
        checkout.close();
        failFromCheckout(commitId);
        return true;
    }
    return false; 
}