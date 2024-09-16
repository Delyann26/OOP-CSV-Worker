#pragma once
#include <iostream>
#include "Cell.h"
#include "FacultyNumber.h"
#include <fstream>
class FacultyNumberCell : public Cell
{
public:
	FacultyNumberCell(const FacultyNumber& fn);

	virtual void printCell() const override;
	virtual Cell* clone() const override;
	virtual void writeToCSV(std::ofstream& ofs) const override;
	
	FacultyNumber getFn() const;

	virtual bool compareGreaterThan(const Cell& other) const override;
	virtual bool compareLessThan(const Cell& other) const override;
	virtual bool compareEqual(const Cell& other) const override;

	virtual size_t getSizeOfCell() const override;
	virtual size_t getLengthOfValueInCell() const override;
private:
	FacultyNumber fn;
};

bool operator==(const FacultyNumberCell& lhs, const FacultyNumberCell& rhs);
bool operator<(const FacultyNumberCell& lhs, const FacultyNumberCell& rhs);
bool operator!=(const FacultyNumberCell& lhs, const FacultyNumberCell& rhs);
bool operator<=(const FacultyNumberCell& lhs, const FacultyNumberCell& rhs);
bool operator>(const FacultyNumberCell& lhs, const FacultyNumberCell& rhs);
bool operator>=(const FacultyNumberCell& lhs, const FacultyNumberCell& rhs);

class FacultyNumberCreator : public CellCreator
{
public:
	FacultyNumberCreator();
	virtual Cell* createCell(const std::string& str) const override;
};