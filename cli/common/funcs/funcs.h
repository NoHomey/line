#pragma once

#include <cstddef>
#include <utility>
#include "../../../core/String/String.h"

namespace line {

namespace cli {

namespace common {

enum class DirectoryCheckResult {NotExist, Error, NotADirectory, Directory};

using FileInfoFromCommitLine = std::pair<line::core::String::StringSlice, line::core::String::StringSlice>;

namespace funcs {

DirectoryCheckResult checkIsDirectory(const char* directoryPath);

bool isDirectory(const char* directoryPath);

bool isRepository();

std::size_t readCommitsCounter();

bool fileExists(const char* filePath);

void copyFile(const char* destFilePath, const char* srcFilePath);

FileInfoFromCommitLine parseFileInfoFromCommitLine(const line::core::String::StringSlice& commitLine) noexcept;

}

}

}

}