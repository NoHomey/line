#include "./Timestamp.h"
#include  <ctime>

line::cli::common::Timestamp::Timestamp() noexcept {
    std::time_t timePoint = std::time(nullptr);
    std::tm timeInfo = *std::localtime(&timePoint);
    hours = timeInfo.tm_hour;
    minutes = timeInfo.tm_min;
    day = timeInfo.tm_mday;
    month = timeInfo.tm_mon;
    year = 1900 + timeInfo.tm_year;
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
    return year;
}

static unsigned int readTwoDigitsNumber(std::istream& input) {
    char d1;
    char d2;
    input >> d1 >> d2;
    return (d1 - '0') * 10 + (d2 - '0');
}

std::istream& line::cli::common::operator>>(std::istream& input, Timestamp& timestamp) {
    unsigned int year;
    timestamp.hours = readTwoDigitsNumber(input);
    input.ignore();
    timestamp.minutes = readTwoDigitsNumber(input);
    input.ignore();
    timestamp.day = readTwoDigitsNumber(input);
    input.ignore();
    timestamp.month = readTwoDigitsNumber(input);
    input.ignore();
    input >> year;
    timestamp.year = year;
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