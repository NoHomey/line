#include "./String.h"
#include <cassert>
#include <cstddef>
#include <cstring>

line::core::String::StringSlice::StringSlice(const char* beginning, std::size_t count) noexcept
: beginning{beginning}, count{count} {
    assert(beginning);
    assert(count);
}

bool line::core::String::StringSlice::operator==(const StringSlice& other) const noexcept {
    return (count == other.count) && std::memcmp(beginning, other.beginning, count);
}

bool line::core::String::StringSlice::operator!=(const StringSlice& other) const noexcept {
    return !((*this) == other);
}

line::core::String::StringSlice line::core::String::toStringSlice(const String& string) noexcept {
    assert(!string.isEmpty());
    return {string.cString(), string.length()};
}

line::core::String::StringSlice line::core::String::toStringSlice(const StringSlice& stringSlice) noexcept {
    return stringSlice;
}

line::core::String::String() noexcept
: string{nullptr}, stringLength{0} { }

line::core::String::String(const char* string)
: String{} {
    this->string = copy(string, stringLength);
}

line::core::String::String(const StringSlice& stringSlice) {
    char* tmp = new char[stringSlice.count + 1];
    std::memcpy(tmp, stringSlice.beginning, stringSlice.count);
    tmp[stringSlice.count] = '\0';
    string = tmp;
    stringLength = stringSlice.count;
}

line::core::String::String(const String& other)
: String{} {
    if(!other.isEmpty()) {
        string = copy(other.string, stringLength);
    }
} 

line::core::String::String(String&& other) noexcept
: string{other.string}, stringLength{other.stringLength} {
    other.string = nullptr;
    other.stringLength = 0;
}

line::core::String::~String() noexcept {
    delete[] string;
}

line::core::String& line::core::String::operator=(const String& other) {
    if(this != &other) {
        if(other.isEmpty()) {
            destroy();
        } else {
            char* tmp = copy(other.string, stringLength);
            delete[] string;
            string = tmp;
        }
    }
    return *this;
}

line::core::String& line::core::String::operator=(String&& other) noexcept {
    if(this != &other) {
        delete[] string;
        string = other.string;
        other.string = nullptr;
        stringLength = other.stringLength;
        other.stringLength = 0;
    }
    return *this;
}

bool line::core::String::isEmpty() const noexcept {
    return !stringLength;
}

const char* line::core::String::cString() const noexcept {
    return string;
}

std::size_t line::core::String::length() const noexcept {
    return stringLength;
}

void line::core::String::destroy() noexcept {
    delete[] string;
    string = nullptr;
    stringLength = 0;
}

char* line::core::String::copy(const char* string, std::size_t& length) {
    assert(string);
    length = std::strlen(string);
    assert(length);
    char* data = new char[length + 1];
    std::memcpy(data, string, length + 1);
    return data;
}