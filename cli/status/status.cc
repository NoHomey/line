#include "./status.h"
#include <iostream>
#include "../../core/Hasher/Hasher.h"
#include "../../core/FileRecursiveIterator/FileRecursiveIterator.h"
#include "../../core/DirectoryStructure.thd"
#include "../common/Navigator/Navigator.h"
#include "../common/PathCutter/PathCutter.h"
#include "../common/FileLineReader/FileLineReader.h"
#include "../common/funcs/funcs.h"

static void readCommitDirectorySnapshot(std::size_t commitId, line::core::DirectoryStructure<line::cli::common::FileStatus>& filesStatus) {
    line::cli::common::Navigator& navigator = line::cli::common::Navigator::navigator();
    line::cli::common::FileLineReader commitFile{navigator.navigateToCommit(commitId)};
    ++commitFile;
    ++commitFile;
    ++commitFile;
    while(commitFile) {
        line::cli::common::FileInfoFromCommitLine fileInfo = line::cli::common::funcs::parseFileInfoFromCommitLine(*commitFile);
        filesStatus.insert(
            line::core::FilePathIterator{fileInfo.first},
            line::cli::common::FileStatus{
                line::core::Hasher::Hash::fromHexHashCode(fileInfo.second),
                line::cli::common::FileAction::Remove
            }
        );
        ++commitFile;
    }
}

static line::core::DirectoryStructure<line::cli::common::FileStatus> getFilesStatus(std::size_t commitsCounter) {
    line::cli::common::Navigator& navigator = line::cli::common::Navigator::navigator();
    line::core::DirectoryStructure<line::cli::common::FileStatus> filesStatus{navigator.navigateToDirectory()};
    if(commitsCounter) {
        readCommitDirectorySnapshot(commitsCounter, filesStatus);
    }
    line::core::FileRecursiveIterator directoryIterator{navigator.navigateToDirectory()};
    while(directoryIterator) {
        line::core::String::StringSlice absoluteFilePath = *directoryIterator;
        line::core::String::StringSlice relativeFilePath = line::cli::common::PathCutter::cutPath(*directoryIterator);
        line::core::Hasher::Hash fileHash = line::core::Hasher::hashFile(absoluteFilePath.beginning);
        line::utils::types::Optional<line::cli::common::FileStatus&> findResult = filesStatus.find(relativeFilePath);
        if(findResult) {
            line::cli::common::FileStatus& fileStatus = *findResult;
            fileStatus.action = fileStatus.fileHash == fileHash
                ? line::cli::common::FileAction::Keep : line::cli::common::FileAction::Update;
        } else {
            filesStatus.insert(
                line::core::FilePathIterator{relativeFilePath},
                line::cli::common::FileStatus{fileHash, line::cli::common::FileAction::Add}
            );
        }
        ++directoryIterator;
    }
    return filesStatus;
}

static void printFileStatus(const line::core::String::StringSlice& relativeFileName, line::cli::common::FileAction fileAction) {
    switch(fileAction) {
        case line::cli::common::FileAction::Add:
            std::cout << "add ";
            break;
        case line::cli::common::FileAction::Remove:
            std::cout << "remove ";
            break;
        case line::cli::common::FileAction::Update:
            std::cout << "update ";
            break;
        default: assert(false);
    }
    std::cout << relativeFileName << std::endl;
}

static void reportStatus() {
    std::size_t commitsCounter = line::cli::common::funcs::readCommitsCounter();
    line::core::DirectoryStructure<line::cli::common::FileStatus> filesStatus = getFilesStatus(commitsCounter);
    bool changes = false;
    line::core::DirectoryStructure<line::cli::common::FileStatus>::ConstFileIterator iter = filesStatus.constFileIterator();
    while(iter) {
        line::core::DirectoryStructure<line::cli::common::FileStatus>::ConstFileIterator::ConstData fileInfo = *iter;
        if(fileInfo.second.action != line::cli::common::FileAction::Keep) {
            if(!changes) {
                changes = true;
                std::cout << "Current commit: " << (commitsCounter + 1) << std::endl;
                std::cout << "changes:" << std::endl;
            }
            line::core::String::StringSlice relativeFilePath = line::cli::common::PathCutter::cutPath(fileInfo.first);
            printFileStatus(relativeFilePath, fileInfo.second.action);
        }
        ++iter;
    }
    if(!changes) {
        std::cout << "No changes found." << std::endl;
    }
}

void line::cli::status(int argc, char** argv) {
    const char* argumentDescription = "Directory wich is line repository wich status should be repoprted.";
    if(!argc) {
        std::cout << "'line status' expects one argument. " << argumentDescription << std::endl;
        return;
    }
    if(argc > 1) {
        std::cout << "'line status' expects only one argument. " << argumentDescription << std::endl;
        return;
    }
    const char* directoryPath = argv[0];
    line::cli::common::Navigator::init(directoryPath);
    line::cli::common::PathCutter::init(directoryPath);
    if(line::cli::common::funcs::isRepository()) {
        if(!line::cli::common::funcs::isCheckouted()) {
            reportStatus();
        }
    }
}