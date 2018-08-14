#include "./commit.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include "../../core/Hasher/Hasher.h"
#include "../../core/FileRecursiveIterator/FileRecursiveIterator.h"
#include "../../core/DirectoryStructure.thd"
#include "../common/Navigator/Navigator.h"
#include "../common/Timestamp/Timestamp.h"
#include "../common/funcs/funcs.h"

struct Commit {
    const char* author;
    const char* description;
    line::cli::common::Timestamp timestamp;

    Commit(const char* author, const char* description) noexcept
    : author{author}, description{description}, timestamp{} { }
};

static line::core::DirectoryStructure<line::core::Hasher::Hash> readDirectoryStructure() {
    line::cli::common::Navigator& navigator = line::cli::common::Navigator::navigator();
    line::core::DirectoryStructure<line::core::Hasher::Hash> directoryStructure{navigator.navigateToDirectory()};
    line::core::FileRecursiveIterator directoryIterator{navigator.path()};
    const std::size_t directoryLength = std::strlen(navigator.path());
    while(directoryIterator) {
        line::core::String::StringSlice absoluteFilePath = *directoryIterator;
        line::core::String::StringSlice relativeFilePath{
            absoluteFilePath.beginning + directoryLength + 1,
            absoluteFilePath.count - (directoryLength + 1)
        };
        directoryStructure.insert(relativeFilePath, line::core::Hasher::hashFile(absoluteFilePath.beginning));
        ++directoryIterator;
    }
    return directoryStructure;
}

static void storeFileData(const line::core::DirectoryStructure<line::core::Hasher::Hash>::ConstFileIterator::ConstData& fileData) {
    line::cli::common::Navigator& navigator = line::cli::common::Navigator::navigator();
    {
        std::ifstream fileObject{navigator.navigateToObject(fileData.second)};
        if(fileObject.good()) {
            return;
        }
        fileObject.close();
    }
    std::ofstream fileObject;
    std::ifstream file;
    static char buffer[4096];
    fileObject.exceptions(std::ofstream::badbit);
    fileObject.open(navigator.path());
    file.exceptions(std::ifstream::badbit);
    const std::size_t direcotryLength = std::strlen(navigator.navigateToDirectory());
    char* filePath = new char[direcotryLength + fileData.first.count + 1];
    std::memcpy(filePath, navigator.path(), direcotryLength);
    filePath[direcotryLength] = '/';
    std::memcpy(filePath + direcotryLength + 1, fileData.first.beginning, fileData.first.count + 1);
    file.open(filePath);
    delete[] filePath;
    while(file) {
        file.read(buffer, 4096);
        if(file.gcount()) {
            fileObject.write(buffer, file.gcount());
        }
    }
    file.close();
    fileObject.close();
}

static void writeCommitInfo(std::ofstream& file, const Commit& commit) {
    file << commit.timestamp << std::endl;
    file << commit.author << std::endl;
    file << commit.description << std::endl;
}

static void addCommitToLog(const Commit& commit) {
    line::cli::common::Navigator& navigator = line::cli::common::Navigator::navigator();
    std::ofstream commitsLog;
    commitsLog.exceptions(std::ofstream::badbit);
    commitsLog.open(navigator.navigateToCommitsLog(), std::ios_base::out | std::ios_base::app);
    writeCommitInfo(commitsLog, commit);
    commitsLog.close();
}

static void updateCommitsCounter(std::size_t newCounter) {
    line::cli::common::Navigator& navigator = line::cli::common::Navigator::navigator();
    std::ofstream commitsCounter;
    commitsCounter.exceptions(std::ofstream::badbit);
    commitsCounter.open(navigator.navigateToCommitsCounter());
    commitsCounter << newCounter;
    commitsCounter.close();
}

static void createNewCommit(const Commit& commit) {
    std::size_t commitsCounter = line::cli::common::funcs::readCommitsCounter();
    line::core::DirectoryStructure<line::core::Hasher::Hash> directoryStructure = readDirectoryStructure();
    line::cli::common::Navigator& navigator = line::cli::common::Navigator::navigator();
    std::ofstream commitFile;
    if(directoryStructure.isEmpty()) {
        std::cout << "Empty line repository. Nothing to commit" << std::endl;
        return;
    }
    commitFile.exceptions(std::ofstream::badbit);
    commitFile.open(navigator.navigateToCommit(commitsCounter + 1));
    writeCommitInfo(commitFile, commit);
    line::core::DirectoryStructure<line::core::Hasher::Hash>::ConstFileIterator fileIterator = directoryStructure.constFileIterator();
    while(fileIterator) {
        line::core::DirectoryStructure<line::core::Hasher::Hash>::ConstFileIterator::ConstData fileData = *fileIterator;
        line::core::String::StringSlice hexHashCode = line::core::Hasher::Hash::toHexHashCode(fileData.second);
        commitFile.write(fileData.first.beginning, fileData.first.count);
        commitFile << '/' ;
        commitFile.write(hexHashCode.beginning, hexHashCode.count);
        commitFile << std::endl;
        storeFileData(fileData);
        ++fileIterator;
    }
    commitFile.close();
    addCommitToLog(commit);
    updateCommitsCounter(commitsCounter + 1);
    std::cout << "Changes commited with id: " << (commitsCounter + 1) << std::endl;
}

void line::cli::commit(int argc, char** argv) {
    if(argc < 3) {
        std::cout << "'line commit' expects at least 3 arguments" << std::endl;
        return;
    }
    line::cli::common::Navigator::init(argv[0]);
    if(line::cli::common::funcs::isRepository()) {
        createNewCommit(Commit{argv[1], argv[2]});
    }
}