#include "DateCell.h"

DateCell::DateCell(const Date& date):Cell(CellType::DateCell)
{
    this->date = date;
}

void DateCell::printCell() const
{
    this->date.print();
}

Cell* DateCell::clone() const
{
    return new DateCell(*this);
}

void DateCell::writeToCSV(std::ofstream& ofs) const
{
    ofs << date.getYear() << "-";
    if (date.getMonth() / 10 == 0)
    {
        ofs << '0' << date.getMonth() << "-";
    }
    else
    {
        ofs << date.getMonth() << "-";
    }
    if (date.getDay() / 10 == 0)
    {
        ofs << '0' << date.getDay();
    }
    else
    {
        ofs << date.getDay();
    }
}

Date DateCell::getDate() const
{
    return date;
}

size_t DateCell::getSizeOfCell() const
{
    return 10;
}

size_t DateCell::getLengthOfValueInCell() const
{
    return 10;
}

bool DateCell::compareGreaterThan(const Cell& other) const
{
    if (typeid(*this) == typeid(other)) {
        const DateCell& otherDateCell = static_cast<const DateCell&>(other);
        return *this > otherDateCell;
    }
}

bool DateCell::compareLessThan(const Cell& other) const
{
    if (typeid(*this) == typeid(other)) {
        const DateCell& otherDateCell = static_cast<const DateCell&>(other);
        return *this < otherDateCell;
    }
}

bool DateCell::compareEqual(const Cell& other) const
{
    return !compareGreaterThan(other) && !compareLessThan(other);
}

bool operator==(const DateCell& lhs, const DateCell& rhs)
{
    return (lhs.getDate() == rhs.getDate());
}

bool operator!=(const DateCell& lhs, const DateCell& rhs)
{
    return !(lhs == rhs);
}

bool operator<(const DateCell& lhs, const DateCell& rhs)
{
    return lhs.getDate() < rhs.getDate();
}

bool operator<=(const DateCell& lhs, const DateCell& rhs)
{
    return (lhs < rhs) || (lhs == rhs);
}

bool operator>(const DateCell& lhs, const DateCell& rhs)
{
    return !(lhs <= rhs);
}

bool operator>=(const DateCell& lhs, const DateCell& rhs)
{
    return !(lhs < rhs);
}

DateCellCreator::DateCellCreator()
    : CellCreator(1)
{
}

Cell* DateCellCreator::createCell(const std::string& str) const
{
    int day = 0, month = 0, year = 0;
    getDateDataFromString(str, day, month, year);
    Date date(day, month, year);
    return new DateCell(date);
}

void DateCellCreator::getDateDataFromString(const std::string& str, int& day, int& month, int& year) const
{
    size_t i = 0;
    while (i < 4)
    {
        year = year * 10 + str[i] - '0';
        i++;
    }
    i++;
    while (i < 7)
    {
        month = month * 10 + str[i] - '0';
        i++;
    }
    i++;
    while (i < 10)
    {
        day = day * 10 + str[i] - '0';
        i++;
    }

}
static DateCellCreator __;
