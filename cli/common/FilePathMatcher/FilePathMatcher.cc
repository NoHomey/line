#include "./FilePathMatcher.h"
#include <cassert>
#include "../../../core/PatternMatcher/PatternMatcher.h"

line::cli::common::FilePathMatcher::FilePathMatcher() noexcept
: patternString{nullptr}, patternLength{0} { }

line::cli::common::FilePathMatcher::FilePathMatcher(const line::core::String::StringSlice& pattern) noexcept
: patternString{pattern.beginning}, patternLength{pattern.count} { }

bool line::cli::common::FilePathMatcher::hasPattern() const noexcept {
    return patternLength;
}

line::core::String::StringSlice line::cli::common::FilePathMatcher::pattern() const noexcept {
    assert(hasPattern());
    return {patternString, patternLength};
}

bool line::cli::common::FilePathMatcher::match(const line::core::String::StringSlice& filePath) const noexcept {
    if(hasPattern()) {
        SegmentIterator patternSegmentIterator{pattern()};
        SegmentIterator filePathSegmentIterator{filePath};
        while(patternSegmentIterator) {
            if(!filePathSegmentIterator) {
                return false;
            }
            if(!line::core::PatternMatcher::test(*filePathSegmentIterator, *patternSegmentIterator)) {
                return false;
            }
            ++patternSegmentIterator;
            ++filePathSegmentIterator;
        }
    }
    return true;
}

line::cli::common::FilePathMatcher::SegmentIterator::SegmentIterator(const line::core::String::StringSlice& iterated) noexcept
: iterated{iterated}, segmentBeginning{0}, segmentEnd{findSegmentEnd(iterated, 0)} { }

line::cli::common::FilePathMatcher::SegmentIterator::operator bool() const noexcept {
    return segmentBeginning < iterated.count;
}

line::core::String::StringSlice line::cli::common::FilePathMatcher::SegmentIterator::operator*() const noexcept {
    assert(*this);
    return {iterated.beginning + segmentBeginning, segmentEnd - segmentBeginning};
}

line::cli::common::FilePathMatcher::SegmentIterator& line::cli::common::FilePathMatcher::SegmentIterator::operator++() noexcept {
    segmentBeginning = segmentEnd + 1;
    segmentEnd = findSegmentEnd(iterated, segmentBeginning);
    return *this;
}

std::size_t line::cli::common::FilePathMatcher::SegmentIterator::findSegmentEnd(
        const line::core::String::StringSlice& iterated, std::size_t segmentBeginning
) noexcept {
    for(std::size_t i = segmentBeginning; i < iterated.count; ++i) {
        if(iterated.beginning[i] == '/') {
            return i;
        }
    }
    return iterated.count;
}