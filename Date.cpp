#include "Date.h"

Date::Date() :day(1), month(1), year(1)
{
}

Date::Date(unsigned day, unsigned month, unsigned year) :day(1), month(1), year(1)
{
    try
    {
        setDate(day, month, year);
    }
    catch (std::invalid_argument& error)
    {
        throw;
    }
}

void Date::setDay(unsigned day)
{
    unsigned dayCounterInMonth = daysInMonth(this->month);
    if (1 <= day && day <= dayCounterInMonth)
    {
        this->day = day;
        return;
    }
    throw std::invalid_argument("Invalid day!");
}

void Date::setMonth(unsigned month)
{
    if (1 <= month && month <= 12)
    {
        this->month = month;
        return;
    }
    throw std::invalid_argument("Invalid month!");
}

void Date::setYear(unsigned year)
{
    if (1980 <= year && year <= 2024 && year != 1)
    {
        this->year = year;
        return;
    }
    throw std::invalid_argument("Invalid year");
}

void Date::setDate(unsigned day, unsigned month, unsigned year)
{
    try
    {
        setYear(year);
        setMonth(month);
        setDay(day);
    }
    catch (std::invalid_argument& error)
    {
        throw;
    }
}

unsigned Date::getDay() const
{
    return day;
}

unsigned Date::getMonth() const
{
    return month;
}

unsigned Date::getYear() const
{
    return year;
}

unsigned Date::daysInMonth(unsigned month) const
{
    if (month == 2)
    {
        return isLeapYear(this->year) ? 29 : 28;
    }
    else if (month == 4 || month == 6 || month == 9 || month == 11)
    {
        return 30;
    }
    else
    {
        return 31;
    }
}

bool Date::isLeapYear(unsigned year) const
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

void Date::print() const
{
    std::cout << year << "-";
    if (month / 10 == 0)
    {
        std::cout << '0' << month << "-";
    }
    else
    {
        std::cout << month << "-";
    }
    if (day / 10 == 0)
    {
        std::cout << '0' << day;
    }
    else
    {
        std::cout << day;
    }
}

std::istream& operator>>(std::istream& is, Date& date)
{
    return is >> date.day >> date.month >> date.year;
}

bool operator==(const Date& lhs, const Date& rhs)
{
    return lhs.day == rhs.day && lhs.month == rhs.month && lhs.year == rhs.year;
}

bool operator!=(const Date& lhs, const Date& rhs)
{
    return !(lhs == rhs);
}

bool operator<(const Date& lhs, const Date& rhs)
{
    if (lhs.year < rhs.year)
    {
        return true;
    }
    else if (lhs.year == rhs.year)
    {
        if (lhs.month < rhs.month)
        {
            return true;
        }
        else if (lhs.month == rhs.month)
        {
            if (lhs.day < rhs.day)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }

}
bool operator<=(const Date& lhs, const Date& rhs)
{
    return (lhs < rhs) || (lhs == rhs);
}

bool operator>(const Date& lhs, const Date& rhs)
{
    return !(lhs <= rhs);
}

bool operator>=(const Date& lhs, const Date& rhs)
{
    return !(lhs < rhs);
}
