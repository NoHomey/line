#include "./PathCutter.h"
#include <cassert>
#include <cstring>

std::size_t line::cli::common::PathCutter::count = 0;

void line::cli::common::PathCutter::init(const char* directoryPath) noexcept {
    assert(directoryPath);
    assert(directoryPath[0]);
    assert(!count);
    count = std::strlen(directoryPath) + 1;
}

line::core::String::StringSlice line::cli::common::PathCutter::cutPath(const line::core::String::StringSlice& path) noexcept {
    assert(count);
    return {path.beginning + count, path.count - count};
}