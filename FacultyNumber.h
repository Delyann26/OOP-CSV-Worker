#pragma once
#pragma warning(disable:4996)
#include <iostream>

enum class Specialty
{
	UNKNOWN = 0,
	INFORMATICS, //4
	COMPUTER_SCIENCE, //8
	MATH, // 1
	APPLIED_MATH, //3
	STATISTICS //0
};

class FacultyNumber
{
public:
	FacultyNumber() = default;
	FacultyNumber(const std::string& fn);

	void setFn(const std::string& fn);

	std::string getFn() const;
	bool getStyle() const;	
	int getSpecialty() const;

	void print() const;
private:
	std::string fn;
	int isNewStyle;
	Specialty spec;

private:
	bool isValid(const std::string& fn) const;
	void setSpecialty();
	int checkIfNewStyle(const std::string& fn) const;
	bool containsSpace(const std::string& fn) const;
};

bool operator==(const FacultyNumber& lhs, const FacultyNumber& rhs);
bool operator!=(const FacultyNumber& lhs, const FacultyNumber& rhs);

bool operator<(const FacultyNumber& lhs, const FacultyNumber& rhs);
bool operator>=(const FacultyNumber& lhs, const FacultyNumber& rhs);
bool operator>(const FacultyNumber& lhs, const FacultyNumber& rhs);
bool operator<=(const FacultyNumber& lhs, const FacultyNumber& rhs);
