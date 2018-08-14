#pragma once

#include "../../../core/String/String.h"

namespace line {

namespace cli {

namespace common {

class PathCutter {
public:
    static void init(const char* directoryPath) noexcept;

    static line::core::String::StringSlice cutPath(const line::core::String::StringSlice& path) noexcept;

public:
    PathCutter() noexcept = delete;

    PathCutter(const PathCutter& other) noexcept = delete;

    PathCutter(PathCutter&& other) noexcept = delete;

private:
    static std::size_t count;
};

}

}

}