#include "FacultyNumberCell.h"

FacultyNumberCell::FacultyNumberCell(const FacultyNumber& fn):Cell(CellType::FacultyNumberCell)
{
	this->fn = fn;
}

void FacultyNumberCell::printCell() const
{
	std::cout << this->fn.getFn();
}

Cell* FacultyNumberCell::clone() const
{
	return new FacultyNumberCell(*this);
}

void FacultyNumberCell::writeToCSV(std::ofstream& ofs) const
{
	ofs << fn.getFn();
}

FacultyNumber FacultyNumberCell::getFn() const
{
	return fn;
}

bool FacultyNumberCell::compareGreaterThan(const Cell& other) const
{
	if (typeid(*this) == typeid(other)) {
		const FacultyNumberCell& otherFnCell = static_cast<const FacultyNumberCell&>(other);
		return *this > otherFnCell;
	}
}

bool FacultyNumberCell::compareLessThan(const Cell& other) const
{
	if (typeid(*this) == typeid(other)) {
		const FacultyNumberCell& otherFnCell = static_cast<const FacultyNumberCell&>(other);
		return *this < otherFnCell;
	}
}

bool FacultyNumberCell::compareEqual(const Cell& other) const
{
	return !compareGreaterThan(other) && !compareLessThan(other);
}

size_t FacultyNumberCell::getSizeOfCell() const
{
	return 10;
}

size_t FacultyNumberCell::getLengthOfValueInCell() const
{
	return fn.getStyle() == 1 ? 10 : 5;
}

bool operator==(const FacultyNumberCell& lhs, const FacultyNumberCell& rhs)
{
	return (lhs.getFn() == rhs.getFn());
}

bool operator<(const FacultyNumberCell& lhs, const FacultyNumberCell& rhs)
{
	return (lhs.getFn() < rhs.getFn());
}

bool operator!=(const FacultyNumberCell& lhs, const FacultyNumberCell& rhs)
{
	return !(lhs == rhs);
}

bool operator<=(const FacultyNumberCell& lhs, const FacultyNumberCell& rhs)
{
	return (lhs < rhs) || (lhs == rhs);
}

bool operator>(const FacultyNumberCell& lhs, const FacultyNumberCell& rhs)
{
	return !(lhs <= rhs);
}

bool operator>=(const FacultyNumberCell& lhs, const FacultyNumberCell& rhs)
{
	return !(lhs < rhs);
}

FacultyNumberCreator::FacultyNumberCreator()
	: CellCreator(2)
{
}

Cell* FacultyNumberCreator::createCell(const std::string& str) const
{
	FacultyNumber fn(str);
	return new FacultyNumberCell(fn);
}
static FacultyNumberCreator __;