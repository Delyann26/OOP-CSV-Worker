#include "TextCell.h"



TextCell::TextCell():Cell(CellType::TextCell)
{
	this->text = "";
}

TextCell::TextCell(std::string text):Cell(CellType::TextCell)
{
	setText(text);
}

void TextCell::setText(std::string text)
{
	if (!text.empty())
	{
		this->text = text;
		return;
	}
	throw std::invalid_argument("String is empty!");
}

std::string TextCell::getText() const
{
	return text;
}

void TextCell::printCell() const
{
	std::cout << this->text;
}

Cell* TextCell::clone() const
{
	return new TextCell(*this);
}

void TextCell::writeToCSV(std::ofstream& ofs) const
{
	if (this->text.size() == 1)
	{
		ofs << "'" << this->getText() << "'";
		return;
	}
	ofs << '"' << this->getText() << '"';
}

bool TextCell::compareGreaterThan(const Cell& other) const
{
	if (typeid(*this) == typeid(other)) {
		const TextCell& otherTextCell = static_cast<const TextCell&>(other);
		return *this > otherTextCell;
	}
}

bool TextCell::compareLessThan(const Cell& other) const
{
	if (typeid(*this) == typeid(other)) {
		const TextCell& otherTextCell = static_cast<const TextCell&>(other);
		return *this < otherTextCell;
	}
}

bool TextCell::compareEqual(const Cell& other) const
{
	return !compareGreaterThan(other) && !compareLessThan(other);
}

size_t TextCell::getSizeOfCell() const
{
	return text.size() + 4;
}

size_t TextCell::getLengthOfValueInCell() const
{
	return text.size();
}

bool operator==(const TextCell& lhs, const TextCell& rhs)
{
	return lhs.getText() == rhs.getText();
}

bool operator<(const TextCell& lhs, const TextCell& rhs)
{
	return lhs.getText() < rhs.getText();
}

bool operator!=(const TextCell& lhs, const TextCell& rhs)
{
	return !(lhs == rhs);
}

bool operator<=(const TextCell& lhs, const TextCell& rhs)
{
	return (lhs < rhs) || (lhs == rhs);
}

bool operator>(const TextCell& lhs, const TextCell& rhs)
{
	return !(lhs <= rhs);
}

bool operator>=(const TextCell& lhs, const TextCell& rhs)
{
	return !(lhs < rhs);
}

TextCellCreator::TextCellCreator()
	: CellCreator(0)
{
}

Cell* TextCellCreator::createCell(const std::string& str) const
{
	std::string copy = str;
	if ((copy[0] == '\"' && copy[copy.size() - 1] == '\"') || (copy[0] == '\'' && copy[copy.size() - 1] == '\''))
	{
		copy.erase(0, 1);
		size_t size = str.size();
		copy.erase(size - 2, 1);
	}
	return new TextCell(copy);
}
static TextCellCreator __;