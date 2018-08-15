#pragma once

#include <cstddef>
#include <fstream>
#include "../../../core/String/String.h"

namespace line {

namespace cli {

namespace common {

class FileLineReader {
public:
    FileLineReader(const char* filePath);

    FileLineReader(const FileLineReader& other) = delete;

    FileLineReader(FileLineReader&& other);

    ~FileLineReader() noexcept;

    FileLineReader& operator=(const FileLineReader& other) = delete;

    FileLineReader& operator=(FileLineReader&& other) = delete;

public:
    operator bool() const noexcept;

    line::core::String::StringSlice operator*() const noexcept;

public:
    FileLineReader& operator++();

private:
    void readLine();

    void null() noexcept;

    void clean() noexcept;

private:
    static const std::size_t initialBufferCapacity = 8;

private:
    char* buffer;
    std::size_t length;
    std::size_t capacity;
    std::ifstream file;
};

}

}

}