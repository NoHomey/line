#include "./revert.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <utility>
#include "../../core/Hasher/Hasher.h"
#include "../../core/PathBuilder/PathBuilder.h"
#include "../common/Navigator/Navigator.h"
#include "../common/FilePathMatcher/FilePathMatcher.h"
#include "../common/FileLineReader/FileLineReader.h"
#include "../common/funcs/funcs.h"

static void revertFile(line::core::PathBuilder& pathBuilder, const line::cli::common::FileInfoFromCommitLine& fileInfo) {
    line::cli::common::Navigator& navigator = line::cli::common::Navigator::navigator();
    pathBuilder.empty();
    pathBuilder.addToPath(navigator.navigateToDirectory());
    pathBuilder.addToPath(fileInfo.first);
    if(!line::cli::common::funcs::fileExists(pathBuilder.path().beginning)) {
        if(!line::cli::common::funcs::ensurePathExists(pathBuilder, fileInfo.first)) {
            std::cout << "failed to revert file " << fileInfo.first << std::endl;
            return;
        }
    }
    line::cli::common::funcs::copyFile(
        pathBuilder.path().beginning,
        navigator.navigateToObject(fileInfo.second)
    );
    std::cout << "reverted " << fileInfo.first << std::endl;
}

static void revertFilesFromCommit(const char* commitIdStr, const char* filePattern) {
    std::size_t commitId;
    std::size_t commitsCounter = line::cli::common::funcs::readCommitsCounter();
    if(!commitsCounter) {
        std::cout << "No commit have been made. Nothing to revert from." << std::endl;
        return;
    }
    if(!line::cli::common::funcs::parseCommitId(commitId, commitIdStr, commitsCounter) || (!commitId)) {
        std::cout << "Fatal: " << commitId << " is not a valid commit id." << std::endl;
        return;
    }
    if(commitId > commitsCounter) {
        std::cout << "Failed to revert files!" << std::endl;
        std::cout << "Last commit id is " << commitsCounter
            << ". Trying to revert files matching file pattern "
            << filePattern << " from commit with id " << commitId;
        return;
    }
    line::cli::common::FilePathMatcher fileMatcher = filePattern[0]
        ? line::cli::common::FilePathMatcher{line::core::String::StringSlice{filePattern, std::strlen(filePattern)}}
        : line::cli::common::FilePathMatcher{};
    line::cli::common::Navigator& navigator = line::cli::common::Navigator::navigator();
    line::cli::common::FileLineReader commitFile{navigator.navigateToCommit(commitId)};
    line::core::PathBuilder pathBuilder{256};
    bool matchedFile = false;
    ++commitFile;
    ++commitFile;
    ++commitFile;
    while(commitFile) {
        line::cli::common::FileInfoFromCommitLine fileInfo = line::cli::common::funcs::parseFileInfoFromCommitLine(*commitFile);
        if(fileMatcher.match(fileInfo.first)) {
            matchedFile = true;
            revertFile(pathBuilder, fileInfo);
        }
        ++commitFile;
    }
    if(!matchedFile) {
        std::cout << "No files were reverted. No file matches pattern " << filePattern << std::endl;
    }
}

void line::cli::revert(int argc, char** argv) {
    if(argc < 3) {
        std::cout << "'line revert' expects 3 arguments" << std::endl;
        return;
    }
    line::cli::common::Navigator::init(argv[0]);
    if(line::cli::common::funcs::isRepository()) {
        if(!line::cli::common::funcs::isCheckouted()) {
            revertFilesFromCommit(argv[1], argv[2]);
        }
    }
}