#include "./status.h"
#include <iostream>
#include "../../core/Hasher/Hasher.h"
#include "../../core/FileRecursiveIterator/FileRecursiveIterator.h"
#include "../../core/DirectoryStructure.thd"
#include "../common/Navigator/Navigator.h"
#include "../common/PathCutter/PathCutter.h"
#include "../common/FileLineReader/FileLineReader.h"
#include "../common/funcs/funcs.h"

enum class FileAction {Add, Update, Remove, Keep};

struct FileStatus {
    line::core::Hasher::Hash fileHash;
    FileAction action;

    FileStatus(const line::core::Hasher::Hash& fileHash, FileAction action)
    : fileHash{fileHash}, action{action} { }
};

static void readCommitDirectorySnapshot(std::size_t commitId, line::core::DirectoryStructure<FileStatus>& filesStatus) {
    line::cli::common::Navigator& navigator = line::cli::common::Navigator::navigator();
    line::cli::common::FileLineReader commitFile{navigator.navigateToCommit(commitId)};
    ++commitFile;
    ++commitFile;
    ++commitFile;
    while(commitFile) {
        line::core::String::StringSlice fileInfo = *commitFile;
        line::core::String::StringSlice relativeFilePath{
            fileInfo.beginning,
            fileInfo.count - (line::core::Hasher::Hash::hexHashCodeLength + 1)
        };
        line::core::String::StringSlice hexHashCode{
            fileInfo.beginning + (relativeFilePath.count + 1),
            line::core::Hasher::Hash::hexHashCodeLength
        };
        filesStatus.insert(
            line::core::FilePathIterator{relativeFilePath},
            FileStatus{line::core::Hasher::Hash::fromHexHashCode(hexHashCode), FileAction::Remove}
        );
        ++commitFile;
    }
}

static line::core::DirectoryStructure<FileStatus> getFilesStatus(std::size_t commitsCounter) {
    line::cli::common::Navigator& navigator = line::cli::common::Navigator::navigator();
    line::core::DirectoryStructure<FileStatus> filesStatus{navigator.navigateToDirectory()};
    if(commitsCounter) {
        readCommitDirectorySnapshot(commitsCounter, filesStatus);
    }
    line::core::FileRecursiveIterator directoryIterator(navigator.navigateToDirectory());
    while(directoryIterator) {
        line::core::String::StringSlice absoluteFilePath = *directoryIterator;
        line::core::String::StringSlice relativeFilePath = line::cli::common::PathCutter::cutPath(*directoryIterator);
        line::core::Hasher::Hash fileHash = line::core::Hasher::hashFile(absoluteFilePath.beginning);
        line::utils::types::Optional<FileStatus&> findResult = filesStatus.find(relativeFilePath);
        if(findResult) {
            FileStatus& fileStatus = *findResult;
            fileStatus.action = fileStatus.fileHash == fileHash ? FileAction::Keep : FileAction::Update;
        } else {
            filesStatus.insert(
                line::core::FilePathIterator{relativeFilePath},
                FileStatus{fileHash, FileAction::Add}
            );
        }
        ++directoryIterator;
    }
    return filesStatus;
}

static void printFileStatus(const line::core::String::StringSlice& relativeFileName, FileAction fileAction) {
    switch(fileAction) {
        case FileAction::Add:
            std::cout << "add ";
            break;
        case FileAction::Remove:
            std::cout << "remove ";
            break;
        case FileAction::Update:
            std::cout << "update ";
            break;
        default: assert(false);
    }
    std::cout << relativeFileName << std::endl;
}

static void reportStatus() {
    std::size_t commitsCounter = line::cli::common::funcs::readCommitsCounter();
    line::core::DirectoryStructure<FileStatus> filesStatus = getFilesStatus(commitsCounter);
    bool changes = false;
    line::core::DirectoryStructure<FileStatus>::ConstFileIterator iter = filesStatus.constFileIterator();
    while(iter) {
        line::core::DirectoryStructure<FileStatus>::ConstFileIterator::ConstData fileInfo = *iter;
        if(fileInfo.second.action != FileAction::Keep) {
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
        reportStatus();
    }
}