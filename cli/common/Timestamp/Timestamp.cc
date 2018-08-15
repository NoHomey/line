#include "./Timestamp.h"
#include  <ctime>
#include <cassert>

static bool isDigit(char c) noexcept {
    return (c >= '0') && (c <= '9');
}

static unsigned int readTwoDigitsNumber(const char*& string) noexcept {
    assert(isDigit(string[0]));
    assert(isDigit(string[1]));
    unsigned int number = (string[0] - '0') * 10 + (string[1] - '0');
    string += 2;
    return number;
}

static unsigned int readYear(const char*& string) noexcept {
    return readTwoDigitsNumber(string) * 100 + readTwoDigitsNumber(string);
}

line::cli::common::Timestamp line::cli::common::Timestamp::fromString(const char* timestampString) noexcept {
    Timestamp timestamp;
    timestamp.setHours(readTwoDigitsNumber(timestampString));
    assert(*timestampString == ':');
    ++timestampString;
    timestamp.setMinutes(readTwoDigitsNumber(timestampString));
    assert(*timestampString == ' ');
    ++timestampString;
    timestamp.setDay(readTwoDigitsNumber(timestampString));
    assert(*timestampString == '.');
    ++timestampString;
    timestamp.setMonth(readTwoDigitsNumber(timestampString));
    assert(*timestampString == '.');
    ++timestampString;
    timestamp.setYear(readYear(timestampString));
    assert(!*timestampString);
    return timestamp;
}

line::cli::common::Timestamp line::cli::common::Timestamp::now() noexcept {
    return {};
}

line::cli::common::Timestamp::Timestamp() noexcept {
    std::time_t timePoint = std::time(nullptr);
    std::tm timeInfo = *std::localtime(&timePoint);
    setHours(timeInfo.tm_hour);
    setMinutes(timeInfo.tm_min);
    setDay(timeInfo.tm_mday);
    setMonth(timeInfo.tm_mon + 1);
    setYear(1900 + timeInfo.tm_year);
}

unsigned int line::cli::common::Timestamp::getHours() const noexcept {
    return hours;
}

unsigned int line::cli::common::Timestamp::getMinutes() const noexcept {
    return minutes;
}

unsigned int line::cli::common::Timestamp::getDay() const noexcept {
    return day;
}

unsigned int line::cli::common::Timestamp::getMonth() const noexcept {
    return month;
}

unsigned int line::cli::common::Timestamp::getYear() const noexcept {
    return 1900 + year;
}

void line::cli::common::Timestamp::setHours(unsigned int newHours) noexcept {
    assert(newHours < 24);
    hours = newHours;
}

void line::cli::common::Timestamp::setMinutes(unsigned int newMinutes) noexcept {
    assert(newMinutes <= 59);
    minutes = newMinutes;
}

void line::cli::common::Timestamp::setDay(unsigned int newDay) noexcept {
    assert((newDay >= 1) && (newDay <= 31));
    day = newDay;
}

void line::cli::common::Timestamp::setMonth(unsigned int newMonth) noexcept {
    assert((newMonth >= 1) && (newMonth <= 12));
    month = newMonth;
}

void line::cli::common::Timestamp::setYear(unsigned int newYear) noexcept {
    assert((newYear >= 1900) && (newYear <= 5995));
    year = (newYear - 1900);
}

static unsigned int readTwoDigitsNumber(std::istream& input) {
    char d1;
    char d2;
    input >> d1 >> d2;
    assert(isDigit(d1));
    assert(isDigit(d2));
    return (d1 - '0') * 10 + (d2 - '0');
}

std::istream& line::cli::common::operator>>(std::istream& input, Timestamp& timestamp) {
    unsigned int year;
    timestamp.setHours(readTwoDigitsNumber(input));
    assert(input.peek() == ':');
    input.ignore();
    timestamp.setMinutes(readTwoDigitsNumber(input));
    assert(input.peek() == ' ');
    input.ignore();
    timestamp.setDay(readTwoDigitsNumber(input));
    assert(input.peek() == '.');
    input.ignore();
    timestamp.setMonth(readTwoDigitsNumber(input));
    assert(input.peek() == '.');
    input.ignore();
    input >> year;
    timestamp.setYear(year);
    return input;
}

static void printTwoDigits(std::ostream& output, unsigned int num) {
    if(num < 10) {
        output << '0';
    }
    output << num;
}

std::ostream& line::cli::common::operator<<(std::ostream& output, const Timestamp& timestamp) {
    printTwoDigits(output, timestamp.getHours());
    output << ':';
    printTwoDigits(output, timestamp.getMinutes());
    output << ' ';
    printTwoDigits(output, timestamp.getDay());
    output << '.';
    printTwoDigits(output, timestamp.getMonth());
    output << '.' << timestamp.getYear();
    return output;
}