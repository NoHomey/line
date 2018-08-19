#pragma once

#include <cstddef>
#include <utility>
#include "../../../core/Hasher/Hasher.h"
#include "../../../core/String/String.h"
#include "../../core/PathBuilder/PathBuilder.h"

namespace line {

namespace cli {

namespace common {

enum class DirectoryCheckResult {NotExist, Error, NotADirectory, Directory};

enum class FileAction {Add, Update, Remove, Keep};

struct FileStatus {
    line::core::Hasher::Hash fileHash;
    FileAction action;

    FileStatus(const line::core::Hasher::Hash& fileHash, FileAction action) noexcept;
};

using FileInfoFromCommitLine = std::pair<line::core::String::StringSlice, line::core::String::StringSlice>;

namespace funcs {

DirectoryCheckResult checkIsDirectory(const char* directoryPath);

int mkdirWithPermisions(const char* directory);

bool isDirectory(const char* directoryPath);

bool isRepository();

std::size_t readCommitsCounter();

bool fileExists(const char* filePath);

void copyFile(const char* destFilePath, const char* srcFilePath);

FileInfoFromCommitLine parseFileInfoFromCommitLine(const line::core::String::StringSlice& commitLine) noexcept;

bool parseCommitId(std::size_t& commitId, const char* str, std::size_t headId);

bool ensurePathExists(line::core::PathBuilder& pathBuilder, const line::core::String::StringSlice& filePath);

bool isCheckouted();

}

}

}

}