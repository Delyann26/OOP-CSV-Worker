#pragma once
#pragma warning (disable:4996)
#include <iostream>
#include <string>
#include "Cell.h"
#include <fstream>
class TextCell : public Cell
{
public:
	TextCell();
	TextCell(std::string text);

	void setText(std::string text);
	
	std::string getText() const;

	virtual void printCell() const override;
	virtual Cell* clone() const override;
	virtual void writeToCSV(std::ofstream& ofs) const override;



	virtual bool compareGreaterThan(const Cell& other) const override;
	virtual bool compareLessThan(const Cell& other) const override;
	virtual bool compareEqual(const Cell& other) const override;

	virtual size_t getSizeOfCell() const override;
	size_t getLengthOfValueInCell() const override;

private:
	std::string text;
};
bool operator==(const TextCell& lhs, const TextCell& rhs);
bool operator<(const TextCell& lhs, const TextCell& rhs);
bool operator!=(const TextCell& lhs, const TextCell& rhs);
bool operator<=(const TextCell& lhs, const TextCell& rhs);
bool operator>(const TextCell& lhs, const TextCell& rhs);
bool operator>=(const TextCell& lhs, const TextCell& rhs);


class TextCellCreator : public CellCreator
{
public:
	TextCellCreator();

	virtual Cell* createCell(const std::string& str) const override;
};