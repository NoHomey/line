#pragma once

#include "../String/String.h"

namespace line {

namespace core {

class FilePathIterator {
public:
    FilePathIterator(const String::StringSlice& filePath) noexcept;

    FilePathIterator(const FilePathIterator& other) noexcept = default;

    FilePathIterator(FilePathIterator&& other) noexcept = default;

    ~FilePathIterator() noexcept = default;

    FilePathIterator& operator=(const FilePathIterator& other) noexcept = default;

    FilePathIterator& operator=(FilePathIterator&& other) noexcept = default;

public:
    const String::StringSlice& getFileName() const noexcept;

    String::StringSlice operator*() const noexcept;

    operator bool() const noexcept;

    std::size_t depth() const noexcept;

    std::size_t length() const noexcept;

public:
    FilePathIterator& operator++() noexcept;

private:
    static String::StringSlice getFileNameAsStringSlice(const String::StringSlice& filePath) noexcept;

    static std::size_t findFirstDirectoryDelimiter(const String::StringSlice& filePath) noexcept;

    static std::size_t depthOfFilePath(const String::StringSlice& filePath) noexcept;

private:
    std::size_t filePathDepth;
    std::size_t filePathLength;
    String::StringSlice currentFilePath;
    std::size_t currentDirectoryLength;
    String::StringSlice fileName;
};

}

}