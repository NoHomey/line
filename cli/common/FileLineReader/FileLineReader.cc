#include "./FileLineReader.h"
#include <cassert>
#include <utility>
#include <cstring>

line::cli::common::FileLineReader::FileLineReader(const char* filePath)
: buffer{nullptr},
position{0},
capacity{initialBufferCapacity},
file{} {
    file.exceptions(std::ifstream::badbit);
    file.open(filePath);
    buffer = new char[initialBufferCapacity];
    readLine();
}

line::cli::common::FileLineReader::FileLineReader(FileLineReader&& other)
: buffer{other.buffer},
position{other.position},
capacity{other.capacity}, 
file{std::move(other.file)} {
    other.buffer = nullptr;
    other.position = 0;
    other.capacity = 0;
}

line::cli::common::FileLineReader::~FileLineReader() noexcept {
    delete[] buffer;
}

line::cli::common::FileLineReader::operator bool() const noexcept {
    return position;
}

line::core::String::StringSlice line::cli::common::FileLineReader::operator*() const noexcept {
    assert(*this);
    return {buffer, position};
}

line::cli::common::FileLineReader& line::cli::common::FileLineReader::operator++() {
    readLine();
    return *this;
}

void line::cli::common::FileLineReader::readLine() {
    position = 0;
    char* inputBuffer = buffer;
    std::size_t count = capacity;
    while(file) {
        file.getline(inputBuffer, count);
        position += file.gcount();
        if(file.eof()) {
            file.close();
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