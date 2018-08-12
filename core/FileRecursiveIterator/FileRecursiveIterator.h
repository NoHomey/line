#pragma once

#include "../../dataStructures/Stack.thd"
#include "../../dataStructures/SinglyLinkedList.thd"
#include "../String/String.h"

namespace line {

namespace core {

class FileRecursiveIterator {
public:
    FileRecursiveIterator() = delete;

    FileRecursiveIterator(const char* directory);

    FileRecursiveIterator(const FileRecursiveIterator& other) = default;

    FileRecursiveIterator(FileRecursiveIterator&& other) noexcept = default;

    ~FileRecursiveIterator() noexcept = default;

    FileRecursiveIterator& operator=(const FileRecursiveIterator& other) = default;

    FileRecursiveIterator& operator=(FileRecursiveIterator&& other) noexcept = default;

public:
    operator bool() const noexcept;

    line::core::String::StringSlice operator*() const noexcept;

public:
    FileRecursiveIterator& operator++();

private:
    static bool isSpecialEntry(const char* entryName) noexcept;

    static bool ignoreSpecialEntriesAndLineInfoDirectory(const char* entryName) noexcept;

private:
    void addToFilePath(const char* directory);

    int readCurrentDirectory(bool (*ignoreEntry) (const char*));

    void walkUntilFileIsReached();

    void removeFromFilePath() noexcept;

    void removeFromPaths() noexcept;

private:
    static const std::size_t initialFilePathCapacity = 4096;

    static const std::size_t initialDirectoriesPathCapacity = 256;

    static const char* const lineInfoDirectoryName;

private:

class ConstCharWrapper {
public:
    ConstCharWrapper() noexcept;

    ConstCharWrapper(const char* string);

    ConstCharWrapper(const ConstCharWrapper& other);

    ConstCharWrapper(ConstCharWrapper&& other) noexcept;

    ~ConstCharWrapper() noexcept;

    ConstCharWrapper& operator=(const ConstCharWrapper& other);

    ConstCharWrapper& operator=(ConstCharWrapper&& other) noexcept;

public:
    bool isEmpty() const noexcept;

    const char* cString() const noexcept;

public:
    void clean() noexcept;

private:
    static const char* copy(const char* str);

private:
    const char* data;
};

private:
    using Entries = line::dataStructures::SinglyLinkedList<ConstCharWrapper>;

private:
    line::dataStructures::Stack<char> filePath;

    line::dataStructures::Stack<Entries> directoriesPath;
};

}

}