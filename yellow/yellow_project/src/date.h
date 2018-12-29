#pragma once

#include <iostream>

class Date {
public:
    Date(int year, int month, int day) : year_(year), month_(month), day_(day) {}

    const int year_;
    const int month_;
    const int day_;
};

std::ostream& operator<< (std::ostream& os, const Date& date);
bool operator< (const Date& lhs, const Date& rhs);

Date ParseDate(std::istream& is);