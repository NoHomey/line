#include "./PathBuilder.h"
#include <cstring>
#include <cassert>
#include <algorithm>

line::core::PathBuilder::PathBuilder(std::size_t capacity)
: buffer{nullptr}, bufferSize{0}, bufferCapacity{capacity} {
    if(capacity) {
        buffer = new char[capacity];
    }
}

line::core::PathBuilder::PathBuilder(const PathBuilder& other)
: PathBuilder{other.bufferCapacity} {
    std::memcpy(buffer, other.buffer, other.bufferSize);
}

line::core::PathBuilder::PathBuilder(PathBuilder&& other) noexcept
: buffer{other.buffer}, bufferSize{other.bufferSize}, bufferCapacity{other.bufferCapacity} {
    other.buffer = nullptr;
    other.bufferSize = 0;
    other.bufferCapacity = 0;
}

line::core::PathBuilder::~PathBuilder() noexcept {
    clean();
}

line::core::PathBuilder& line::core::PathBuilder::operator=(const PathBuilder& other) {
    if(this != &other) {
        *this = PathBuilder{other};
    }
    return  *this;
}

line::core::PathBuilder& line::core::PathBuilder::operator=(PathBuilder&& other) noexcept {
    if(this != &other) {
        delete[] buffer;
        buffer = other.buffer;
        bufferSize = other.bufferSize;
        bufferCapacity = other.bufferCapacity;
        other.null();
    }
    return *this;
}

bool line::core::PathBuilder::isEmpty() const noexcept {
    return !bufferSize;
}

line::core::String::StringSlice line::core::PathBuilder::path() const noexcept {
    assert(!isEmpty());
    return {buffer, bufferSize - 1};
}

void line::core::PathBuilder::addToPath(const char* name) {
    addToPath(name, std::strlen(name));
}

void line::core::PathBuilder::addToPath(const line::core::String::StringSlice& name) {
    addToPath(name.beginning, name.count);
}

void line::core::PathBuilder::removeLast() noexcept {
    assert(!isEmpty());
    while(bufferSize && (buffer[bufferSize - 1] != '/')) {
        --bufferSize;
    }
    buffer[bufferSize - 1] = '\0';
}

void line::core::PathBuilder::clean() noexcept {
    delete[] buffer;
    null();
}

void line::core::PathBuilder::null() noexcept {
    buffer = nullptr;
    bufferSize = 0;
    bufferCapacity = 0;
}

void line::core::PathBuilder::addToPath(const char* name, std::size_t length) {
    if(isEmpty()) {
        if(bufferCapacity < (length + 1)) {
            char* tmp = new char[length + 1];
            delete[] buffer;
            buffer = tmp;
            bufferCapacity = length + 1;
        }
        std::memcpy(buffer, name, length);
        buffer[length] = '\0';
        bufferSize = length + 1;
        return;
    }
    if(bufferCapacity < (bufferSize + length + 1)) {
        std::size_t newCapacity = std::max(bufferCapacity * 2, bufferSize + length + 1);
        char* tmp = new char[newCapacity];
        std::memcpy(tmp, buffer, bufferSize);
        delete[] buffer;
        buffer = tmp;
        bufferCapacity = newCapacity;
    }
    buffer[bufferSize - 1] = '/';
    std::memcpy(buffer + bufferSize, name, length);
    buffer[bufferSize + length] = '\0';
    bufferSize += (length + 1);
}