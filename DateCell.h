#pragma once
#include <iostream>
#include "Cell.h"
#include "Date.h"
#include <fstream>
class DateCell:public Cell
{
public:
	DateCell(const Date& date);
	virtual void printCell() const override;
	virtual Cell* clone() const override;

	virtual void writeToCSV(std::ofstream& ofs) const override;

	Date getDate() const;

	virtual bool compareGreaterThan(const Cell& other) const override;
	virtual bool compareLessThan(const Cell& other) const override;
	virtual bool compareEqual(const Cell& other) const override;

	virtual size_t getSizeOfCell() const override;
	virtual size_t getLengthOfValueInCell() const override;
private:
	Date date;
};

bool operator==(const DateCell& lhs, const DateCell& rhs);
bool operator<(const DateCell& lhs, const DateCell& rhs);
bool operator!=(const DateCell& lhs, const DateCell& rhs);
bool operator<=(const DateCell& lhs, const DateCell& rhs);
bool operator>(const DateCell& lhs, const DateCell& rhs);
bool operator>=(const DateCell& lhs, const DateCell& rhs);

class DateCellCreator : public CellCreator
{
public:
	DateCellCreator();
	virtual Cell* createCell(const std::string& str) const override;

private:
	void getDateDataFromString(const std::string& str, int& day, int& month, int& year) const;

};

