#pragma once

#include <cstddef>

namespace line {

namespace core {

class String {
public:
    struct StringSlice {
        const char* beginning;

        std::size_t count;

        StringSlice() = delete;

        StringSlice(const char* beginning, std::size_t count) noexcept;

        StringSlice(const StringSlice& other) noexcept = default;

        StringSlice(StringSlice&& other) noexcept = default;

        ~StringSlice() noexcept = default;

        StringSlice& operator=(const StringSlice& other) noexcept = default;

        StringSlice& operator=(StringSlice&& other) noexcept = default;

        bool operator==(const StringSlice& other) const noexcept;

         bool operator!=(const StringSlice& other) const noexcept;
    };

public:
    static StringSlice toStringSlice(const String& string) noexcept;

    static StringSlice toStringSlice(const StringSlice& stringSlice) noexcept;

public:
    String() noexcept;

    String(const char* string);

    explicit String(const StringSlice& stringSlice);

    String(const String& other);

    String(String&& other) noexcept;

    ~String() noexcept;

public:
    String& operator=(const String& other);

    String& operator=(String&& other) noexcept;

public:
    bool isEmpty() const noexcept;

    const char* cString() const noexcept;

    std::size_t length() const noexcept;

public:
    void destroy() noexcept;

private:
    static char* copy(const char* string, std::size_t& length);

private:
    const char* string;
    std::size_t stringLength;
};

}

}