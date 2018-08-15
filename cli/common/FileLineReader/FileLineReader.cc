#include "./FileLineReader.h"
#include <cassert>
#include <utility>
#include <cstring>

line::cli::common::FileLineReader::FileLineReader(const char* filePath)
: buffer{nullptr},
length{0},
capacity{initialBufferCapacity},
file{} {
    file.exceptions(std::ifstream::badbit);
    file.open(filePath);
    buffer = new char[initialBufferCapacity];
    readLine();
}

line::cli::common::FileLineReader::FileLineReader(FileLineReader&& other)
: buffer{other.buffer},
length{other.length},
capacity{other.capacity}, 
file{std::move(other.file)} {
    other.null();
}

line::cli::common::FileLineReader::~FileLineReader() noexcept {
    clean();
}

line::cli::common::FileLineReader::operator bool() const noexcept {
    return length;
}

line::core::String::StringSlice line::cli::common::FileLineReader::operator*() const noexcept {
    assert(*this);
    return {buffer, length};
}

line::cli::common::FileLineReader& line::cli::common::FileLineReader::operator++() {
    readLine();
    return *this;
}

void line::cli::common::FileLineReader::readLine() {
    char* inputBuffer = buffer;
    std::size_t count = capacity;
    length = 0;
    while(file) {
        file.getline(inputBuffer, count);
        length += file.gcount();
        if(file.eof()) {
            file.close();
            clean();
            break;
        }
        if(static_cast<std::size_t>(file.gcount()) == (count - 1)) {
            char* tmp = new char[2 * capacity];
            std::memcpy(tmp, buffer, capacity - 1);
            delete[] buffer;
            buffer = tmp;
            inputBuffer = buffer + (capacity - 1);
            count = capacity + 1;
            capacity *= 2;
            file.clear();
            continue;
        }
        break;
    }
}

void line::cli::common::FileLineReader::null() noexcept {
    buffer = nullptr;
    length = 0;
    capacity = 0;
}

void line::cli::common::FileLineReader::clean() noexcept {
    delete[] buffer;
    null();
}