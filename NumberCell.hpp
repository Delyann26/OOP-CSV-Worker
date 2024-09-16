#pragma once
#include <iostream>
#include "Cell.h"
#include <string>
#include <cmath>
template <typename T>
class NumberCell : public Cell
{
public:
    NumberCell(const T& value);

	const T& getValue() const;

	virtual Cell* clone() const override;
	virtual void printCell() const override;
	virtual void writeToCSV(std::ofstream& ofs) const override;

	virtual size_t getSizeOfCell() const override;
	size_t getLengthOfValueInCell() const override;


	virtual bool compareGreaterThan(const Cell& other) const override;
	virtual bool compareLessThan(const Cell& other) const override;
	virtual bool compareEqual(const Cell& other) const override;

private:
	T value;
};
template<typename T>
bool operator==(const NumberCell<T>& lhs, const NumberCell<T>& rhs);
template<typename T>
bool operator<(const NumberCell<T>& lhs, const NumberCell<T>& rhs);
template<typename T>
bool operator!=(const NumberCell<T>& lhs, const NumberCell<T>& rhs);
template<typename T>
bool operator<=(const NumberCell<T>& lhs, const NumberCell<T>& rhs);
template<typename T>
bool operator>(const NumberCell<T>& lhs, const NumberCell<T>& rhs);
template<typename T>
bool operator>=(const NumberCell<T>& lhs, const NumberCell<T>& rhs);

template<typename T>
NumberCell<T>::NumberCell(const T& value) :Cell(CellType::NumberCell), value(value)
{

}

template<typename T>
const T& NumberCell<T>::getValue() const
{
	return value;
}

template<typename T>
Cell* NumberCell<T>::clone() const
{
	return new NumberCell(*this);
}

template<typename T>
void NumberCell<T>::printCell() const
{
	std::cout << this->value;
}

template<typename T>
void NumberCell<T>::writeToCSV(std::ofstream& ofs) const
{
	ofs << getValue();
}

template<typename T>
size_t NumberCell<T>::getSizeOfCell() const
{
	std::string str = std::to_string(value);
	return str.length() + 4;
}

template<typename T>
double roundToFiveDecimalPlaces(const T& number) {
	double rounded = number * 100000.0;

	rounded = round(rounded);

	rounded /= 100000.0;

	return rounded;
}
template<typename T>
size_t NumberCell<T>::getLengthOfValueInCell() const
{
	std::string str = std::to_string(value);
	size_t decimalPnt = str.find('.');
	if (decimalPnt == std::string::npos)
	{
		return str.length();
	}
	else
	{
		str = std::to_string(roundToFiveDecimalPlaces(value));
		size_t i = str.size() - 1;
		while (str[i] == '0')
		{
			str.erase(i, 1);
			i--;
		}
		if (str[i] == '.')
		{
			str.erase(i, 1);
		}
	}

	return str.length();
}

template<typename T>
bool NumberCell<T>::compareGreaterThan(const Cell& other) const
{
    if (typeid(*this) == typeid(other)) {
        const NumberCell<T>& otherNumberCell = static_cast<const NumberCell<T>&>(other);
        return *this > otherNumberCell;
    }
    else
    {
        if (typeid(NumberCell<int>) == typeid(*this) && typeid(NumberCell<double>) == typeid(other))
        {
            const NumberCell<double>& otherDoubleCell = static_cast<const NumberCell<double>&>(other);
            return static_cast<double>(this->getValue()) > otherDoubleCell.getValue();
        }
        else if (typeid(NumberCell<double>) == typeid(*this) && typeid(NumberCell<int>) == typeid(other))
        {
            const NumberCell<int>& otherIntCell = static_cast<const NumberCell<int>&>(other);
            return this->getValue() > static_cast<double>(otherIntCell.getValue());
        }
    }
}

template<typename T>
bool NumberCell<T>::compareLessThan(const Cell& other) const
{
    if (typeid(*this) == typeid(other)) {
        const NumberCell<T>& otherNumberCell = static_cast<const NumberCell<T>&>(other);
        return *this < otherNumberCell;
    }
    else 
    {
        if (typeid(NumberCell<int>) == typeid(*this) && typeid(NumberCell<double>) == typeid(other))
        {
            const NumberCell<double>& otherDoubleCell = static_cast<const NumberCell<double>&>(other);
            return static_cast<double>(this->getValue()) < otherDoubleCell.getValue();
        }
        else if (typeid(NumberCell<double>) == typeid(*this) && typeid(NumberCell<int>) == typeid(other))
        {
            const NumberCell<int>& otherIntCell = static_cast<const NumberCell<int>&>(other);
            return this->getValue() < static_cast<double>(otherIntCell.getValue());
        }
    }
}

template<typename T>
bool NumberCell<T>::compareEqual(const Cell& other) const
{
	return !compareGreaterThan(other) && !compareLessThan(other);
}

template<typename T>
bool operator==(const NumberCell<T>& lhs, const NumberCell<T>& rhs)
{
	return lhs.getValue() == rhs.getValue();
}

template<typename T>
bool operator<(const NumberCell<T>& lhs, const NumberCell<T>& rhs)
{
	return lhs.getValue() < rhs.getValue();
}

template<typename T>
bool operator!=(const NumberCell<T>& lhs, const NumberCell<T>& rhs)
{
	return !(lhs == rhs);
}

template<typename T>
bool operator<=(const NumberCell<T>& lhs, const NumberCell<T>& rhs)
{
	return (lhs < rhs) || (lhs == rhs);
}

template<typename T>
bool operator>(const NumberCell<T>& lhs, const NumberCell<T>& rhs)
{
	return !(lhs <= rhs);
}

template<typename T>
bool operator>=(const NumberCell<T>& lhs, const NumberCell<T>& rhs)
{
	return !(lhs < rhs);
}

class NumberCellCreator : public CellCreator
{
public:
	NumberCellCreator()
		:CellCreator(3)
	{
	}

	virtual Cell* createCell(const std::string& str) const override
	{
        if (isInt(str))
            return new NumberCell<int>(std::stoi(str));
        else
            return new NumberCell<double>(std::stod(str));
	}

private:

    bool isInt(const std::string& str) const
    {
        if (str.empty())
        {
            return false;
        }
        bool isNegativeNumber = false;
        for (size_t i = 0; i < str.size(); i++)
        {
            if (i == 0 && str[i] == '-')
            {
                isNegativeNumber = true;
                continue;
            }
            else if ((i == 0 && str[i] == '0') || (i == 1 && str[i] == '0' && isNegativeNumber == true))
            {
                return false;
            }
            else if (isdigit(str[i]))
            {
                continue;
            }
            else
            {
                return false;
            }
        }
        return true;
    }


    bool isDouble(const std::string& str) const
    {
        if (str.empty())
        {
            return false;
        }
        bool decimalPointFound = false;
        bool atleastOneNumberBeforeDecimalPoint = false;
        bool atleastOneNumberAfterDecimalPoint = false;
        bool isNegativeNumber = false;
        for (size_t i = 0; i < str.size(); i++)
        {
            if (i == 0 && str[i] == '-')
            {
                isNegativeNumber = true;
                continue;
            }
            else if ((i == 0 && str[i] == '0' && isNegativeNumber == false) || (i == 1 && str[i] == '0' && isNegativeNumber == true))
            {
                return false;
            }
            else if (str[i] == '.' && atleastOneNumberBeforeDecimalPoint == true)
            {
                if (decimalPointFound == true)
                {
                    return false;
                }
                decimalPointFound = true;
            }
            else if (isdigit(str[i]))
            {
                if (decimalPointFound == false)
                {
                    atleastOneNumberBeforeDecimalPoint = true;
                }
                else if (decimalPointFound == true)
                {
                    atleastOneNumberAfterDecimalPoint = true;
                }
            }
            else
            {
                return false;
            }
        }
        return true;
    }
};


static NumberCellCreator __;