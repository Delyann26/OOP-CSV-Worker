#pragma once
#include <iostream>
class Cell
{
public:
	enum class CellType
	{
		DateCell = 0,
		FacultyNumberCell,
		NumberCell,
		TextCell,
		Unknown
	};
	Cell(CellType type);

	virtual void printCell() const = 0;
	virtual Cell* clone() const = 0;
	virtual void writeToCSV(std::ofstream& ofs) const = 0;

	int getCellType() const;
	
	virtual size_t getSizeOfCell() const = 0;
	virtual size_t getLengthOfValueInCell() const = 0;

	virtual bool compareGreaterThan(const Cell& other) const = 0;
	virtual bool compareLessThan(const Cell& other) const = 0;
	virtual bool compareEqual(const Cell& other) const = 0;

	virtual ~Cell() = default;
private:
	CellType type;
};

class CellCreator
{
public:
	CellCreator(int type);
	virtual ~CellCreator() = default;
	virtual Cell* createCell(const std::string& str) const = 0;
	int getCellType() const;

private:
	int type;
};
