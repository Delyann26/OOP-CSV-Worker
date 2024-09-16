#pragma once
#include <iostream>

class Date
{
public:
	Date();
	Date(unsigned day, unsigned month, unsigned year);

	void setDate(unsigned day, unsigned month, unsigned year);

	unsigned getDay() const;
	unsigned getMonth() const;
	unsigned getYear() const;

	unsigned daysInMonth(unsigned month) const;
	bool isLeapYear(unsigned year) const;

	friend std::istream& operator>>(std::istream& is, Date& date);

	friend bool operator==(const Date& lhs, const Date& rhs);
	friend bool operator!=(const Date& lhs, const Date& rhs);

	friend bool operator<(const Date& lhs, const Date& rhs);
	friend bool operator<=(const Date& lhs, const Date& rhs);
	friend bool operator>(const Date& lhs, const Date& rhs);
	friend bool operator>=(const Date& lhs, const Date& rhs);

	void print() const;
private:
	unsigned day;
	unsigned month;
	unsigned year;

	void setDay(unsigned day);
	void setMonth(unsigned month);
	void setYear(unsigned year);
};


