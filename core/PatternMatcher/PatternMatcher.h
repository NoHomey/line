#pragma once

#include "../String/String.h"
#include "../../dataStructures/FixedCapacityStack.thd"

namespace line {

namespace core {

class PatternMatcher {
public:
    static bool test(
        const line::core::String::StringSlice& string,
        const line::core::String::StringSlice& pattern
    ) noexcept;

public:
    PatternMatcher() noexcept = delete;

    PatternMatcher(const PatternMatcher& other) noexcept = delete;

    PatternMatcher(PatternMatcher&& other) noexcept = delete;

private:
    static bool run(
        const line::core::String::StringSlice& string,
        const line::core::String::StringSlice& pattern,
        std::size_t stringPosition,
        std::size_t patternPosition
    ) noexcept;

private:
    struct Split {
        std::size_t stringPosition;

        std::size_t patternPosition;

        Split() noexcept;

        Split(std::size_t stringPosition, std::size_t patternPosition) noexcept;

        Split(const Split& other) noexcept = default;

        Split(Split&& other) noexcept = default;

        Split& operator=(const Split& other) noexcept = default;

        Split& operator=(Split&& other) noexcept = default;
    };

private:
    static const std::size_t maximumSupportedSplits = 64;

private:
    static line::dataStructures::FixedCapacityStack<Split, maximumSupportedSplits> splits;
};

}

}