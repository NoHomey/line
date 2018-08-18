#pragma once

#include <cstddef>
#include "../String/String.h"

namespace line {

namespace core {

class PathBuilder {
public:
    PathBuilder() noexcept = delete;

    PathBuilder(std::size_t capacity);

    PathBuilder(const PathBuilder& other);

    PathBuilder(PathBuilder&& other) noexcept;

    ~PathBuilder() noexcept;

    PathBuilder& operator=(const PathBuilder& other);

    PathBuilder& operator=(PathBuilder&& other) noexcept;

public:
    bool isEmpty() const noexcept;

    line::core::String::StringSlice path() const noexcept;

public:
    void addToPath(const char* name);

    void addToPath(const line::core::String::StringSlice& name);

    void removeLast() noexcept;

    void empty() noexcept;

    void clean() noexcept;

private:
    void null() noexcept;

    void addToPath(const char* name, std::size_t length);

private:
    char* buffer;
    std::size_t bufferSize;
    std::size_t bufferCapacity;
};

}

}