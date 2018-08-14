#include "./Navigator.h"
#include <cassert>
#include <cstring>
#include <cstdio>

line::cli::common::Navigator line::cli::common::Navigator::instance;

line::cli::common::Navigator& line::cli::common::Navigator::navigator() noexcept {
    return instance;
}

void line::cli::common::Navigator::init(const char* directoryPath) {
    instance.initialize(directoryPath);
}

const char* line::cli::common::Navigator::path() const noexcept {
    assert(isInitialized());
    return currentPath;
}

const char* line::cli::common::Navigator::navigateToDirectory() noexcept {
    assert(isInitialized());
    currentPath[directoryPathLength] = '\0';
    return currentPath;
}

const char* line::cli::common::Navigator::navigateToRepoInfoDir() noexcept {
    assert(isInitialized());
    std::memcpy(currentPath + directoryPathLength, "/.line", 7);
    return currentPath;
}

const char* line::cli::common::Navigator::navigateToCommits() noexcept {
    assert(isInitialized());
    std::memcpy(currentPath + directoryPathLength, "/.line/commits", 15);
    return currentPath;
}

const char* line::cli::common::Navigator::navigateToCommitsCounter() noexcept {
    assert(isInitialized());
    std::memcpy(currentPath + directoryPathLength, "/.line/commits/counter", 23);
    return currentPath;
}

const char* line::cli::common::Navigator::navigateToCommitsLog() noexcept {
    assert(isInitialized());
    std::memcpy(currentPath + directoryPathLength, "/.line/commits/log", 19);
    return currentPath;
}

const char* line::cli::common::Navigator::navigateToCommit(std::size_t commitId) noexcept {
    assert(isInitialized());
    assert(commitId);
    std::sprintf(currentPath + directoryPathLength, "/.line/commits/%lu", commitId);
    return currentPath;
}

const char* line::cli::common::Navigator::navigateToObjects() noexcept {
    assert(isInitialized());
    std::memcpy(currentPath + directoryPathLength, "/.line/objects", 15);
    return currentPath;
}

const char* line::cli::common::Navigator::navigateToObject(const line::core::Hasher::Hash& hash) noexcept {
    assert(isInitialized());
    std::memcpy(currentPath + directoryPathLength, "/.line/objects/", 15);
    line::core::String::StringSlice hexHashCode = line::core::Hasher::Hash::toHexHashCode(hash);
    std::memcpy(currentPath + directoryPathLength + 15, hexHashCode.beginning, hexHashCode.count);
    currentPath[maximumAdditionalChars - 1] = '\0';
    return currentPath;
}

line::cli::common::Navigator::Navigator() noexcept
: currentPath{nullptr}, directoryPathLength{0} { }

line::cli::common::Navigator::~Navigator() noexcept {
    delete[] currentPath;
}

bool line::cli::common::Navigator::isInitialized() const noexcept {
    return currentPath;
}

void line::cli::common::Navigator::initialize(const char* directoryPath) {
    assert(!isInitialized());
    assert(directoryPath);
    assert(directoryPath[0]);
    directoryPathLength = std::strlen(directoryPath);
    currentPath = new char[directoryPathLength + maximumAdditionalChars];
    std::memcpy(currentPath, directoryPath, directoryPathLength + 1);
}