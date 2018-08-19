#include "./checkout.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <utility>
#include <unistd.h>
#include <errno.h>
#include "../../core/FileRecursiveIterator/FileRecursiveIterator.h"
#include "../../core/DirectoryStructure.thd"
#include "../../core/Hasher/Hasher.h"
#include "../../core/PathBuilder/PathBuilder.h"
#include "../../core/FilePathIterator/FilePathIterator.h"
#include "../common/Navigator/Navigator.h"
#include "../common/FilePathMatcher/FilePathMatcher.h"
#include "../common/FileLineReader/FileLineReader.h"
#include "../common/PathCutter/PathCutter.h"
#include "../common/Timestamp/Timestamp.h"
#include "../common/funcs/funcs.h"

static void removeFile(line::core::PathBuilder& pathBuilder, std::size_t dirsToRemove) {
    std::cout << "removing " << pathBuilder.path() << std::endl;
    errno = 0;
    unlink(pathBuilder.path().beginning);
    for(std::size_t counter = 0; counter < dirsToRemove; ++counter) {
        pathBuilder.removeLast();
        std::cout << "removing directory " << pathBuilder.path() << std::endl;
        errno = 0;
        rmdir(pathBuilder.path().beginning);
    }
}

static void removeFiles(line::core::DirectoryStructure<line::cli::common::FileStatus>& diffTree, line::core::PathBuilder& pathBuilder) {
    line::core::DirectoryStructure<line::cli::common::FileStatus>::ConstFileIterator fileIterator = diffTree.constFileIterator();
    while(fileIterator) {
        line::core::DirectoryStructure<line::cli::common::FileStatus>::ConstFileIterator::ConstData data = *fileIterator;
        if(data.second.action == line::cli::common::FileAction::Remove) {
            pathBuilder.empty();
            pathBuilder.addToPath(data.first);
            ++fileIterator;
            line::core::String::StringSlice relativeFilePath = line::cli::common::PathCutter::cutPath(pathBuilder.path());
            removeFile(pathBuilder, diffTree.remove(line::core::FilePathIterator{relativeFilePath}));
        } else {
            ++fileIterator;
        }
    }
}

static void setFileContent(const line::core::String::StringSlice& filePath, const line::core::Hasher::Hash& fileHash) {
    line::cli::common::funcs::copyFile(
        filePath.beginning,
        line::cli::common::Navigator::navigator().navigateToObject(
            line::core::Hasher::Hash::toHexHashCode(fileHash)
        )
    );
}

static void updateFile(const line::core::String::StringSlice& filePath, const line::core::Hasher::Hash& fileHash) {
    std::cout << "updating " << filePath << std::endl;
    setFileContent(filePath, fileHash);
}

static void addFile(const line::core::String::StringSlice& filePath, const line::core::Hasher::Hash& fileHash, line::core::PathBuilder& pathBuilder) {
    std::cout << "adding " << filePath << std::endl;
    line::cli::common::funcs::ensurePathExists(
        pathBuilder,
        line::cli::common::PathCutter::cutPath(filePath)
    );
    setFileContent(filePath, fileHash);
}

static void updateAndAddFiles(const line::core::DirectoryStructure<line::cli::common::FileStatus>& diffTree, line::core::PathBuilder& pathBuilder) {
    line::core::DirectoryStructure<line::cli::common::FileStatus>::ConstFileIterator fileIterator = diffTree.constFileIterator();
    while(fileIterator) {
        line::core::DirectoryStructure<line::cli::common::FileStatus>::ConstFileIterator::ConstData data = *fileIterator;
        switch(data.second.action) {
            case line::cli::common::FileAction::Update:
                updateFile(data.first, data.second.fileHash);
                break;
            case line::cli::common::FileAction::Add:
                addFile(data.first, data.second.fileHash, pathBuilder);
                break;
            default: break;
        }
        ++fileIterator;
    }
}

static void reflectToWorkingDir(line::core::DirectoryStructure<line::cli::common::FileStatus>& diffTree) {
    line::core::PathBuilder pathBuilder{diffTree.maxAbsoluteFilePathLength()};
    removeFiles(diffTree, pathBuilder);
    updateAndAddFiles(diffTree, pathBuilder);
}

static line::core::DirectoryStructure<line::cli::common::FileStatus> getCurrentWorkingTree() {
    line::cli::common::Navigator& navigator = line::cli::common::Navigator::navigator();
    line::core::DirectoryStructure<line::cli::common::FileStatus> filesStatus{navigator.navigateToDirectory()};
    line::core::FileRecursiveIterator directoryIterator{navigator.path()};
    line::core::String::StringSlice absoluteFilePath = *directoryIterator;
    while(directoryIterator) {
        line::core::String::StringSlice absoluteFilePath = *directoryIterator;
        line::core::String::StringSlice relativeFilePath = line::cli::common::PathCutter::cutPath(*directoryIterator);
        line::core::Hasher::Hash fileHash = line::core::Hasher::hashFile(absoluteFilePath.beginning);
        filesStatus.insert(
            line::core::FilePathIterator{relativeFilePath},
            line::cli::common::FileStatus{fileHash, line::cli::common::FileAction::Remove}
        );
        ++directoryIterator;
    }
    return filesStatus;
}

static void diff(line::core::DirectoryStructure<line::cli::common::FileStatus>& diffTree, line::cli::common::FileLineReader& commitFile) {
    while(commitFile) {
        line::cli::common::FileInfoFromCommitLine fileInfo = line::cli::common::funcs::parseFileInfoFromCommitLine(*commitFile);
        line::core::Hasher::Hash fileHash = line::core::Hasher::Hash::fromHexHashCode(fileInfo.second);
        line::utils::types::Optional<line::cli::common::FileStatus&> findResult = diffTree.find(fileInfo.first);
        if(findResult) {
            line::cli::common::FileStatus& fileStatus = *findResult;
            if(fileStatus.fileHash == fileHash) {
                fileStatus.action = line::cli::common::FileAction::Keep;
            } else {
                fileStatus.action = line::cli::common::FileAction::Update;
                fileStatus.fileHash = fileHash;
            }
        } else {
            diffTree.insert(
                line::core::FilePathIterator{fileInfo.first},
                line::cli::common::FileStatus{fileHash, line::cli::common::FileAction::Add}
            );
        }
        ++commitFile;
    }
}

static void storeCheckout(std::size_t commitId) {
    std::ofstream checkout;
    checkout.exceptions(std::ofstream::badbit);
    checkout.open(line::cli::common::Navigator::navigator().navigateToCheckoutInfo());
    checkout << commitId;
    checkout.close();
}

static void removeCheckout() {
    errno = 0;
    unlink(line::cli::common::Navigator::navigator().navigateToCheckoutInfo());
}

static void checkoutFromCommit(const char* commitIdStr) {
    std::size_t commitId;
    std::size_t commitsCounter = line::cli::common::funcs::readCommitsCounter();
    if(!commitsCounter) {
        std::cout << "No commit have been made. Nothing to checkout from." << std::endl;
        return;
    }
    if(!line::cli::common::funcs::parseCommitId(commitId, commitIdStr, commitsCounter) || (!commitId)) {
        std::cout << "Fatal: " << commitId << " is not a valid commit id." << std::endl;
        return;
    }
    if(commitId > commitsCounter) {
        std::cout << "Failed to do checkout." << std::endl;
        std::cout << "Last commit id is " << commitsCounter
            << ". Trying to do checkout to commit with id " << commitId;
        return;
    }
    line::core::DirectoryStructure<line::cli::common::FileStatus> filesStatus = getCurrentWorkingTree();
    line::cli::common::FileLineReader commitFile{line::cli::common::Navigator::navigator().navigateToCommit(commitId)};
    line::cli::common::Timestamp commitTimestamp = line::cli::common::Timestamp::fromString((*commitFile).beginning);
    ++commitFile;
    line::core::String commitDescription{*commitFile};
    ++commitFile;
    line::core::String commitAuthor{*commitFile};
    ++commitFile;
    diff(filesStatus, commitFile);
    reflectToWorkingDir(filesStatus);
    if(commitId < commitsCounter) {
        storeCheckout(commitId);
    } else {
        removeCheckout();
    }
    std::cout << "checkout to commit " << commitId << ": \""
        << line::core::String::toStringSlice(commitDescription)
        << "\" from " << commitTimestamp << " by "
        << line::core::String::toStringSlice(commitAuthor) << std::endl;
}

void line::cli::checkout(int argc, char** argv) {
    if(argc != 2) {
        std::cout << "'line checkout' expects 2 arguments" << std::endl;
        return;
    }
    const char* directoryPath = argv[0];
    line::cli::common::Navigator::init(directoryPath);
    line::cli::common::PathCutter::init(directoryPath);
    if(line::cli::common::funcs::isRepository()) {
        checkoutFromCommit(argv[1]);
    }
}