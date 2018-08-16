#pragma once

#include "../../../core/String/String.h"

namespace line {

namespace cli {

namespace common {

class FilePathMatcher {
public:
    FilePathMatcher() noexcept;

    FilePathMatcher(const line::core::String::StringSlice& pattern) noexcept;

    FilePathMatcher(const FilePathMatcher& other) noexcept = default;

    FilePathMatcher(FilePathMatcher&& other) noexcept = default;

    ~FilePathMatcher() noexcept = default;

    FilePathMatcher& operator=(const FilePathMatcher& other) noexcept = default;

    FilePathMatcher& operator=(FilePathMatcher&& other) noexcept = default;

public:
    bool match(const line::core::String::StringSlice& filePath) noexcept;

private:
class SegmentIterator {
public:
    SegmentIterator() noexcept = delete;

    SegmentIterator(const line::core::String::StringSlice& iterated) noexcept;

    SegmentIterator(const SegmentIterator& other) noexcept = default;

    SegmentIterator(SegmentIterator&& other) noexcept = default;

    ~SegmentIterator() noexcept = default;

    SegmentIterator& operator=(const SegmentIterator& other) noexcept = default;

    SegmentIterator& operator=(SegmentIterator&& other) noexcept = default;

public:
    operator bool() const noexcept;

    line::core::String::StringSlice operator*() const noexcept;

public:
    SegmentIterator& operator++() noexcept;

private:
    static std::size_t findSegmentEnd(
        const line::core::String::StringSlice& iterated, std::size_t segmentBeginning
    ) noexcept;

private:
    line::core::String::StringSlice iterated;
    std::size_t segmentBeginning;
    std::size_t segmentEnd;
};

private:
    const char* patternString;
    std::size_t patternLength;
};

}

}

}