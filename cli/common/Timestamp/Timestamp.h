// WARNING Timestamp can hold years from 1900 to 5995 !!!!!!!!!

#pragma once

#include <iostream>

namespace line {

namespace cli {

namespace common {

class Timestamp {
public:
    friend std::istream& operator>>(std::istream& input, Timestamp& timestamp);

public:
    static Timestamp fromString(const char* timestampString) noexcept;

    static Timestamp now() noexcept;

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
    void setHours(unsigned int newHours) noexcept;

    void setMinutes(unsigned int newMinutes) noexcept;

    void setDay(unsigned int newDay) noexcept;

    void setMonth(unsigned int newMonth) noexcept;

    void setYear(unsigned int newYear) noexcept;

private:
    unsigned int hours : 5;
    unsigned int minutes : 6;
    unsigned int day : 5;
    unsigned int month : 4;
    // WARNING Timestamp can hold years from 1900 to 5995 !!!!!!!!!
    unsigned int year : 12;
};

std::istream& operator>>(std::istream& input, Timestamp& timestamp);

std::ostream& operator<<(std::ostream& output, const Timestamp& timestamp);

}

}

}