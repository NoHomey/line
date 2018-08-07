#include "./FilePathIterator.h"
#include <cassert>

line::core::FilePathIterator::FilePathIterator(const line::core::String::StringSlice& filePath) noexcept
: filePath{filePath},
currentDirectoryLength{findFirstDirectoryDelimiter(filePath)},
fileName{getFileNameAsStringSlice(filePath)} { }

const line::core::String::StringSlice& line::core::FilePathIterator::getFileName() const noexcept {
    return fileName;
}

line::core::String::StringSlice line::core::FilePathIterator::operator*() const noexcept {
    assert(*this);
    return {filePath.beginning, currentDirectoryLength};
}

line::core::FilePathIterator::operator bool() const noexcept {
    return filePath.beginning != fileName.beginning;
}

line::core::FilePathIterator& line::core::FilePathIterator::operator++() noexcept {
    assert(*this);
    const std::size_t offset = currentDirectoryLength + 1;
    filePath = {filePath.beginning + offset, filePath.count - offset};
    if(*this) {
        currentDirectoryLength = findFirstDirectoryDelimiter(filePath);
    }
    return *this;
}

line::core::String::StringSlice
line::core::FilePathIterator::getFileNameAsStringSlice(const line::core::String::StringSlice& filePath) noexcept {
    std::size_t index = filePath.count;
    while(index) {
        if(filePath.beginning[index - 1] == '/') {
            assert(index > 1);
            return {filePath.beginning + index, filePath.count - index};
        }
        --index;
    }
    return filePath;
}

std::size_t line::core::FilePathIterator::findFirstDirectoryDelimiter(const line::core::String::StringSlice& filePath) noexcept {
    for(std::size_t i = 0; i < filePath.count; ++i) {
        if(filePath.beginning[i] == '/') {
            assert(i);
            return i;
        }
    }
    return filePath.count;
}