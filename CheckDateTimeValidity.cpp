struct Number {
    int value = 0;
    std::string name;
};

void CheckNumber(const Number& number, int min_value, int max_value) {
    if (number.value < min_value) {
        throw std::domain_error(number.name + " is too small"s);
    }
    if (number.value > max_value) {
        throw std::domain_error(number.name + " is too big"s);
    }
}

int GetMonthLength(int month, bool is_leap_year) {
    const std::array month_lengths = {31, 28 + is_leap_year, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    return month_lengths[month - 1];
}

bool IsLeapYear(int year) {
    return (year % 4 == 0) && !(year % 100 == 0 && year % 400 != 0);
}

void CheckDateTimeValidity(const DateTime& dt) {
    CheckNumber({dt.year, "year"s}, 1, 9999);
    CheckNumber({dt.month, "month"s}, 1, 12);
    CheckNumber({dt.day, "day"s}, 1, GetMonthLength(dt.month, IsLeapYear(dt.year)));
    CheckNumber({dt.hour, "hour"s}, 0, 23);
    CheckNumber({dt.minute, "minute"s}, 0, 59);
    CheckNumber({dt.second, "second"s}, 0, 59);
}