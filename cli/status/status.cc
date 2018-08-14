#include "./status.h"
#include <iostream>
#include "../../core/Hasher/Hasher.h"
#include "../../core/FileRecursiveIterator/FileRecursiveIterator.h"
#include "../../core/DirectoryStructure.thd"
#include "../common/Navigator/Navigator.h"
#include "../common/funcs/funcs.h"

enum class FileAction {Add, Update, Remove, Keep};

struct FileStatus {
    line::core::Hasher::Hash fileHash;
    FileAction action;

    FileStatus(const line::core::Hasher::Hash& fileHash, FileAction action)
    : fileHash{fileHash}, action{action} { }
};

using FilesStatus = line::core::DirectoryStructure<FileStatus>;

static void readCommitDirectorySnapshot(std::size_t commitId, FilesStatus& filesStatus) {
    // IMPELMENT !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}

static FilesStatus getFilesStatus(std::size_t commitsCounter) {
    line::cli::common::Navigator& navigator = line::cli::common::Navigator::navigator();
    FilesStatus filesStatus{navigator.navigateToDirectory()};
    if(commitsCounter) {
        readCommitDirectorySnapshot(commitsCounter, filesStatus);
    }
    line::core::FileRecursiveIterator directoryIterator(navigator.path());
    while(directoryIterator) {
        std::size_t directoryLength = std::strlen(navigator.path());
        line::core::String::StringSlice absolutePath = *directoryIterator;
        line::core::String::StringSlice relativeFilePath{
            absolutePath.beginning + directoryLength + 1,
            absolutePath.count - (directoryLength + 1)
        };
        line::core::Hasher::Hash fileHash = line::core::Hasher::hashFile(relativeFilePath.beginning);
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

static void printFileStatus(FilesStatus::ConstFileIterator::ConstData& fileInfo) {
    switch(fileInfo.second.action) {
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
    std::cout << fileInfo.first.beginning << std::endl;
}

static void reportStatus() {
    std::size_t commitsCounter = line::cli::common::funcs::readCommitsCounter();
    FilesStatus filesStatus = getFilesStatus(commitsCounter);
    bool changes = false;
    FilesStatus::ConstFileIterator iter = filesStatus.constFileIterator();
    while(iter) {
        FilesStatus::ConstFileIterator::ConstData fileInfo = *iter;
        if(fileInfo.second.action != FileAction::Keep) {
            if(!changes) {
                changes = true;
                std::cout << "Current commit: " << (commitsCounter + 1) << std::endl;
                std::cout << "changes:" << std::endl;
            }
            printFileStatus(fileInfo);
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
    line::cli::common::Navigator::init(argv[0]);
    if(line::cli::common::funcs::isRepository()) {
        reportStatus();
    }
}