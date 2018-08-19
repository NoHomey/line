#pragma once

#include <cstddef>
#include "../../../core/String/String.h"
#include "../../../core/Hasher/Hasher.h"

namespace line {

namespace cli {

namespace common {

class Navigator {
public:
    static Navigator& navigator() noexcept;

    static void init(const char* directoryPath);

public:
    const char* path() const noexcept;

public:
    const char* navigateToDirectory() noexcept;

    const char* navigateToRepoInfoDir() noexcept;

    const char* navigateToCheckoutInfo() noexcept;

    const char* navigateToCommits() noexcept;

    const char* navigateToCommitsCounter() noexcept;

    const char* navigateToCommitsLog() noexcept;

    const char* navigateToCommit(std::size_t commitId) noexcept;

    const char* navigateToObjects() noexcept;

    const char* navigateToObject(const line::core::String::StringSlice& hexHashString) noexcept;

private:
    Navigator() noexcept;

    Navigator(const Navigator& other) = delete;

    Navigator(Navigator&& other) noexcept = delete;

    ~Navigator() noexcept;

    Navigator& operator=(const Navigator& other) = delete;

    Navigator& operator=(Navigator&& other) noexcept = delete;

private:
    bool isInitialized() const noexcept;

private:
    void initialize(const char* directoryPath);

private:
    static const std::size_t maximumAdditionalChars = 15 + line::core::Hasher::Hash::hexHashCodeLength + 1;

    static Navigator instance;

private:
    char* currentPath;
    std::size_t directoryPathLength;
};

}

}

}