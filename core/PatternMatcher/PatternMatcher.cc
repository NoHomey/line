#include "./PatternMatcher.h"

line::dataStructures::FixedCapacityStack<
    line::core::PatternMatcher::Split,
    line::core::PatternMatcher::maximumSupportedSplits
> line::core::PatternMatcher::splits;

bool line::core::PatternMatcher::test(
    const line::core::String::StringSlice& string,
    const line::core::String::StringSlice& pattern
) noexcept {
    splits.empty();
    if(run(string, pattern, 0, 0)) {
        return true;
    }
    while(!splits.isEmpty()) {
        Split topSplit = splits.top();
        splits.pop();
        if(run(string, pattern, topSplit.stringPosition, topSplit.patternPosition)) {
            return true;
        }
    }
    return false;
}

bool line::core::PatternMatcher::run(
    const line::core::String::StringSlice& string,
    const line::core::String::StringSlice& pattern,
    std::size_t stringPosition,
    std::size_t patternPosition
) noexcept {
    std::size_t patternCurrentPosition = patternPosition;
    std::size_t stringCurrentPosition = stringPosition;
    while(patternCurrentPosition < pattern.count) {
        if(stringCurrentPosition == string.count) {
            while(patternCurrentPosition < pattern.count) {
                if(pattern.beginning[patternCurrentPosition] != '*') {
                    return false;
                }
                ++patternCurrentPosition;
            }
            return true;
        }
        const char patternSymbol = pattern.beginning[patternCurrentPosition];
        const char stringSymbol = string.beginning[stringCurrentPosition];
        if(patternSymbol == '?') {
            ++stringCurrentPosition;
            ++patternCurrentPosition;
            continue;
        }
        if(patternSymbol != '*') {
            if(patternSymbol != stringSymbol) {
                return false;
            }
            ++stringCurrentPosition;
            ++patternCurrentPosition;
            continue;
        }
        while((patternCurrentPosition < pattern.count) && (pattern.beginning[patternCurrentPosition] == '*')) {
            ++patternCurrentPosition;
        }
        if(patternCurrentPosition == pattern.count) {
            return true;
        }
        if((patternCurrentPosition < pattern.count) && (stringCurrentPosition < string.count)) {
            splits.push(Split{stringCurrentPosition + 1, patternCurrentPosition - 1});
        }
    }
    return stringCurrentPosition == string.count;
}

line::core::PatternMatcher::Split::Split() noexcept
: stringPosition{0}, patternPosition{0} { }

line::core::PatternMatcher::Split::Split(std::size_t stringPosition, std::size_t patternPosition) noexcept
: stringPosition{stringPosition}, patternPosition{patternPosition} { }