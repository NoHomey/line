#pragma once

#include <iostream>

namespace line {

namespace cli {

namespace common {

class Timestamp {
public:
    friend std::istream& operator>>(std::istream& input, Timestamp& timestamp);
public:
    Timestamp() noexcept;

    Timestamp(const Timestamp& other) noexcept = default;

    Timestamp(Timestamp&& other) noexcept = default;

    ~Timestamp() noexcept = default;

    Timestamp& operator=(const Timestamp& other) noexcept = default;

    Timestamp& operator=(Timestamp&& other) noexcept = default;

public:
    unsigned int getHours() const noexcept;

    unsigned int getMinutes() const noexcept;

    unsigned int getDay() const noexcept;

    unsigned int getMonth() const noexcept;

    unsigned int getYear() const noexcept;

private:
    unsigned int hours : 5;
    unsigned int minutes : 5;
    unsigned int day : 5;
    unsigned int month : 4;
    unsigned int year : 13;
};

std::istream& operator>>(std::istream& input, Timestamp& timestamp);

std::ostream& operator<<(std::ostream& output, const Timestamp& timestamp);

}

}

}