#include "./FileRecursiveIterator.h"
#include <cstddef>
#include <cstring>
#include <cassert>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

const char* const line::core::FileRecursiveIterator::lineInfoDirectoryName = ".line";

line::core::FileRecursiveIterator::FileRecursiveIterator(const char* directory)
: filePath{initialFilePathCapacity}, directoriesPath{initialDirectoriesPathCapacity} {
    addToFilePath(directory);
    int error = readCurrentDirectory(ignoreSpecialEntriesAndLineInfoDirectory);
    if(error) {
        // handle
        clean();
    } else {
        walkUntilFileIsReached();
    }
}

line::core::FileRecursiveIterator::operator bool() const noexcept {
    return !directoriesPath.isEmpty();
}

line::core::String::StringSlice line::core::FileRecursiveIterator::operator*() const noexcept {
    assert(*this);
    return {filePath.data(), filePath.size() - 1};
}

line::core::FileRecursiveIterator& line::core::FileRecursiveIterator::operator++() {
    removeFromPaths();
    walkUntilFileIsReached();
    if(!(*this)) {
        clean();
    }
    return *this;
}

bool line::core::FileRecursiveIterator::isSpecialEntry(const char* entryName) noexcept {
    assert(entryName);
    assert(entryName[0]);
    if(entryName[0] == '.') {
        if(entryName[1] == '\0') {
            return true;
        }
        if((entryName[1] == '.') && (entryName[2] == '\0')) {
            return true;
        }
    }
    return false;
}

bool line::core::FileRecursiveIterator::ignoreSpecialEntriesAndLineInfoDirectory(const char* entryName) noexcept {
    if(isSpecialEntry(entryName)) {
        return true;
    }
    if(!std::strcmp(entryName, lineInfoDirectoryName)) {
        return true;
    }
    return false;
}

void line::core::FileRecursiveIterator::addToFilePath(const char* directory) {
    for(std::size_t i = 0; directory[i]; ++i) {
        filePath.push(directory[i]);
    }
    filePath.push('\0');
}

int line::core::FileRecursiveIterator::readCurrentDirectory(bool (*ignoreEntry) (const char*)) {
    errno = 0;
    DIR* directoryEntryStream = opendir(filePath.data());
    if(directoryEntryStream) {
        directoriesPath.push({});
        Entries& entries = directoriesPath.top();
        while(true) {
            struct dirent* directoryEntry = readdir(directoryEntryStream);
            if(!directoryEntry) {
                break;
            }
            if(!ignoreEntry(directoryEntry->d_name)) {
                entries.append(ConstCharWrapper{directoryEntry->d_name});
            }
        }
        closedir(directoryEntryStream);
        if(entries.isEmpty()) {
            directoriesPath.pop();
            removeFromFilePath();
        }
    }
    return errno;
}

void line::core::FileRecursiveIterator::walkUntilFileIsReached() {
    if(directoriesPath.isEmpty()) {
        clean();
        return;
    }
    while(!directoriesPath.isEmpty()) {
        Entries& entries = directoriesPath.top();
        filePath.pop();
        filePath.push('/');
        addToFilePath(entries.first().cString());
        entries.first().clean();
        entries.popFirst();
        int error = readCurrentDirectory(isSpecialEntry);
        if(error == ENOTDIR) {
            break;
        } else {
            // handle
        }
    }
    if(directoriesPath.isEmpty()) {
        clean();
    }
}

void line::core::FileRecursiveIterator::removeFromFilePath() noexcept {
    while(filePath.top() != '/') {
        filePath.pop();
    }
    filePath.pop();
    filePath.push('\0');
}

void line::core::FileRecursiveIterator::clean() noexcept {
    filePath.clean();
    directoriesPath.clean();
}

void line::core::FileRecursiveIterator::removeFromPaths() noexcept {
    removeFromFilePath();
    while(!directoriesPath.isEmpty()) {
        if(directoriesPath.top().isEmpty()) {
            directoriesPath.pop();
            if(!directoriesPath.isEmpty()) {
                removeFromFilePath();
            }
        } else {
            break;
        }
    }
}

line::core::FileRecursiveIterator::ConstCharWrapper::ConstCharWrapper() noexcept
: data{nullptr} { }

line::core::FileRecursiveIterator::ConstCharWrapper::ConstCharWrapper(const char* string)
: data{copy(string)} { }

line::core::FileRecursiveIterator::ConstCharWrapper::ConstCharWrapper(const ConstCharWrapper& other)
: data{other.data} { }

line::core::FileRecursiveIterator::ConstCharWrapper::ConstCharWrapper(ConstCharWrapper&& other) noexcept
: data{other.data} {
    other.data = nullptr;
}

line::core::FileRecursiveIterator::ConstCharWrapper::~ConstCharWrapper() noexcept {
    clean();
}

line::core::FileRecursiveIterator::ConstCharWrapper& line::core::FileRecursiveIterator::ConstCharWrapper::operator=(const ConstCharWrapper& other) {
    if(this != &other) {
        if(other.isEmpty()) {
            clean();
        } else {
            const char* tmp = copy(other.data);
            clean();
            data = tmp;
        }
    }
    return *this;
}

line::core::FileRecursiveIterator::ConstCharWrapper& line::core::FileRecursiveIterator::ConstCharWrapper::operator=(ConstCharWrapper&& other) noexcept {
    if(this != &other) {
        clean();
        data = other.data;
        other.data = nullptr;
    }
    return *this;
}

bool line::core::FileRecursiveIterator::ConstCharWrapper::isEmpty() const noexcept {
    return !data;
}

const char* line::core::FileRecursiveIterator::ConstCharWrapper::cString() const noexcept {
    return data;
}

void line::core::FileRecursiveIterator::ConstCharWrapper::clean() noexcept {
    delete[] data;
    data = nullptr;
}

const char* line::core::FileRecursiveIterator::ConstCharWrapper::copy(const char* str) {
    assert(str);
    if(!str[0]) {
        return nullptr;
    }
    const std::size_t count = std::strlen(str) + 1;
    char* tmp = new char[count];
    std::memcpy(tmp, str, count);
    return tmp;
}