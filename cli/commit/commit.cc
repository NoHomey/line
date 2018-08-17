#include "./commit.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <utility>
#include "../../core/Hasher/Hasher.h"
#include "../../core/FileRecursiveIterator/FileRecursiveIterator.h"
#include "../../core/DirectoryStructure.thd"
#include "../common/Navigator/Navigator.h"
#include "../common/FilePathMatcher/FilePathMatcher.h"
#include "../common/PathCutter/PathCutter.h"
#include "../common/Timestamp/Timestamp.h"
#include "../common/funcs/funcs.h"

struct Commit {
    const char* author;
    const char* description;
    line::cli::common::Timestamp timestamp;

    Commit() noexcept = delete;

    Commit(const char* author, const char* description) noexcept
    : author{author}, description{description}, timestamp{line::cli::common::Timestamp::now()} { }

    Commit(const Commit& other) noexcept = default;

    Commit(Commit&& other) noexcept = default;

    ~Commit() noexcept = default;

    Commit& operator=(const Commit& other) noexcept = default;

    Commit& operator=(Commit&& other) noexcept = default;
};

struct CommitCommandArguments {
    Commit commit;
    line::cli::common::FilePathMatcher filePathMatcher;

    CommitCommandArguments(Commit&& commit, line::cli::common::FilePathMatcher&& filePathMatcher) noexcept
    : commit{std::move(commit)}, filePathMatcher{std::move(filePathMatcher)} { }

    CommitCommandArguments(const CommitCommandArguments& other) noexcept = default;

    CommitCommandArguments(CommitCommandArguments&& other) noexcept = default;

    ~CommitCommandArguments() noexcept = default;

    CommitCommandArguments& operator=(const CommitCommandArguments& other) noexcept = default;

    CommitCommandArguments& operator=(CommitCommandArguments&& other) noexcept = default;
};

static line::core::DirectoryStructure<line::core::Hasher::Hash> readDirectoryStructure(const line::cli::common::FilePathMatcher& filePathMatcher) {
    line::cli::common::Navigator& navigator = line::cli::common::Navigator::navigator();
    line::core::DirectoryStructure<line::core::Hasher::Hash> directoryStructure{navigator.navigateToDirectory()};
    line::core::FileRecursiveIterator directoryIterator{navigator.path()};
    while(directoryIterator) {
        line::core::String::StringSlice absoluteFilePath = *directoryIterator;
        line::core::String::StringSlice relativeFilePath = line::cli::common::PathCutter::cutPath(*directoryIterator);
        if(filePathMatcher.match(relativeFilePath)) {
            directoryStructure.insert(relativeFilePath, line::core::Hasher::hashFile(absoluteFilePath.beginning));
        }
        ++directoryIterator;
    }
    return directoryStructure;
}

static void storeFileData(const line::core::DirectoryStructure<line::core::Hasher::Hash>::ConstFileIterator::ConstData& fileData) {
    line::cli::common::Navigator& navigator = line::cli::common::Navigator::navigator();
    if(!line::cli::common::funcs::fileExists(
        navigator.navigateToObject(line::core::Hasher::Hash::toHexHashCode(fileData.second))
    )) {
        line::cli::common::funcs::copyFile(navigator.path(), fileData.first.beginning);
    }
}

static void writeCommitInfo(std::ofstream& file, const Commit& commit) {
    file << commit.timestamp << std::endl;
    file << commit.description << std::endl;
    file << commit.author << std::endl;
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

static void createNewCommit(const CommitCommandArguments& commitCommandArguments) {
    const Commit& commit = commitCommandArguments.commit;
    std::size_t commitsCounter = line::cli::common::funcs::readCommitsCounter();
    line::core::DirectoryStructure<line::core::Hasher::Hash> directoryStructure = readDirectoryStructure(commitCommandArguments.filePathMatcher);
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
        line::core::String::StringSlice relativeFilePath = line::cli::common::PathCutter::cutPath(fileData.first);
        commitFile << relativeFilePath << '/' << hexHashCode << std::endl;
        storeFileData(fileData);
        ++fileIterator;
    }
    commitFile.close();
    addCommitToLog(commit);
    updateCommitsCounter(commitsCounter + 1);
    std::cout << "Changes commited with id: " << (commitsCounter + 1) << std::endl;
    std::cout  << '"' << commit.description << "\" at " << commit.timestamp
     << " by " << commit.author  << std::endl;
    std::cout << "files included in the commit:" << std::endl;
    fileIterator = directoryStructure.constFileIterator();
    while(fileIterator) {
        std::cout << line::cli::common::PathCutter::cutPath((*fileIterator).first) << std::endl;
        ++fileIterator;
    }
}

void line::cli::commit(int argc, char** argv) {
    if(argc < 3) {
        std::cout << "'line commit' expects at least 3 arguments" << std::endl;
        return;
    }
    const char* directoryPath = argv[0];
    line::cli::common::Navigator::init(directoryPath);
    line::cli::common::PathCutter::init(directoryPath);
    if(line::cli::common::funcs::isRepository()) {
        createNewCommit(argc == 3
            ? CommitCommandArguments{
                Commit{argv[1], argv[2]},
                line::cli::common::FilePathMatcher{}
            }
            : CommitCommandArguments{
                Commit{argv[2], argv[3]},
                line::cli::common::FilePathMatcher{line::core::String::StringSlice{argv[1], std::strlen(argv[1])}}
            }
        );
    }
}