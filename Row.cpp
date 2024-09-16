#include "Row.h"

Row::Row() 
    : rowData(nullptr), size(0), capacity(0)
{
    rowData = new(std::nothrow) Cell * [8] {nullptr};
    if (rowData != nullptr)
    {
        capacity = 8;
    }
}

Row::Row(const Row& other) 
    : rowData(nullptr), size(0), capacity(0)
{
    Cell** temp = nullptr;
    try
    {
        temp = new Cell * [other.capacity] {nullptr};
        for (size_t i = 0; i < other.size; i++)
        {
            if (other.rowData[i] != nullptr)
            {
                temp[i] = other.rowData[i]->clone();
            }
        }
    }
    catch (const std::bad_alloc& err)
    {
        if (temp != nullptr)
        {
            for (size_t i = 0; i < other.capacity; i++)
            {
                delete temp[i];
            }
            delete[] temp;
        }
        throw;
    }
    this->rowData = temp;
    this->size = other.size;
    this->capacity = other.capacity;
}

Row& Row::operator=(const Row& other)
{
    if (this == &other)
    {
        return *this;
    }
    Cell** temp = nullptr;
    try
    {
        temp = new Cell * [other.capacity] {nullptr};
        for (size_t i = 0; i < other.size; i++)
        {
            if (other.rowData[i] != nullptr)
            {
                temp[i] = other.rowData[i]->clone();
            }
        }
    }
    catch (const std::bad_alloc&)
    {
        if (temp != nullptr)
        {
            for (size_t i = 0; i < other.capacity; i++)
            {
                delete temp[i];
            }
            delete[] temp;
        }
        throw;
    }
    free();
    this->rowData = temp;
    this->size = other.size;
    this->capacity = other.capacity;

    return*this;
}

Row::~Row()
{
    free();
}

void Row::addCell(const std::string& str)
{
    Cell* toAdd = CellFactory::getInstance().createCell(str);
    addCell(toAdd);
}

void Row::addCell(Cell* cell)
{
    if (size == capacity)
    {
        resize(capacity * 2);
    }
    rowData[size] = cell;
    size++;
}

void Row::removeCell(size_t index)
{
    if (index >= size)
    {
        throw std::length_error("No such cell exists in the row!");
    }
    delete rowData[index];
    for (size_t i = index+1; i < size; i++)
    {
        rowData[i - 1] = rowData[i];
    }
    rowData[--size] = nullptr;
    if (size == capacity / 4)
    {
        resize(capacity / 2);
    }
}

size_t Row::getSize() const
{
    return size;
}

void Row::printRow() const
{
    for (size_t i = 0; i < getSize(); i++)
    {
        rowData[i]->printCell(); std::cout << " ";
    }
    std::cout << std::endl;
}

Cell* Row::operator[](size_t index)
{
    if (index >= size)
    {
        throw std::length_error("Invalid index!");
    }
    return rowData[index];
}

const Cell* Row::operator[](size_t index) const
{
    if (index >= size)
    {
        throw std::length_error("Invalid index!");
    }
    return rowData[index];
}

void Row::writeToCSVFile(std::ofstream& ofs) const
{
    for (size_t i = 0; i < size; i++)
    {
        if (rowData[i] != nullptr)
        {
            rowData[i]->writeToCSV(ofs);
        }
        if (i != size - 1)
        {
        ofs << ',';
        }
    }
}

void Row::readFromCSVFile(std::ifstream& ifs)
{
    std::string line;
    std::getline(ifs, line);
    if (!ifs || line.empty())
    {
        throw std::runtime_error("Row reading failed!");
    }
    
    std::vector<std::string> tokens = tokenize(line);

    for (size_t i = 0; i < tokens.size(); i++)
    {
        addCell(tokens[i]);
    }
}

void Row::permutateRow(const std::vector<unsigned>& permutation)
{
    std::vector<Cell*> permutated;

    unsigned currentIndex = 0;

    for (size_t i = 0; i < size; i++)
    {
        currentIndex = permutation[i] - 1;
        permutated.push_back(rowData[currentIndex]);
    }

    Cell** temp = nullptr;
    try
    {
        temp = new Cell * [capacity];
        for (size_t i = 0; i < size; i++)
        {
            temp[i] = permutated[i];
        }
    }
    catch (const std::bad_alloc&)
    {
        delete[] temp;
        throw;
    }
    delete[] rowData;
    rowData = temp;
}

void Row::free()
{
    for (size_t i = 0; i < size; i++)
    {
        delete rowData[i];
    }
    delete[] rowData;
    this->capacity = 0;
    this->size = 0;
}


void Row::resize(size_t newCapacity)
{
    Cell** temp = nullptr;
    try
    {
        temp = new Cell * [newCapacity] {nullptr};
        for (size_t i = 0; i < size; i++)
        {
            temp[i] = this->rowData[i];
        }
    }
    catch (const std::bad_alloc&)
    {
        delete[] temp;
        throw;
    }
    delete[] this->rowData;
    this->rowData = temp;
    capacity = newCapacity;
}

const std::vector<std::string> Row::tokenize(const std::string& line) const
{
    std::vector<std::string> tokens;
    std::string token;
    bool insideQuotes = false;
    for (char c : line)
    {
        if (c == '"' || c=='\'')
        {
            insideQuotes = !insideQuotes;
        }
        if (c == ',' && insideQuotes == false)
        {
            tokens.push_back(token);
            token.clear();
        }
        else
        {
            token += c;
        }
    }
    tokens.push_back(token);

    return tokens;
}

bool operator==(const Row& lhs, const Row& rhs)
{
    if (lhs.getSize() != rhs.getSize())
    {
        return false;
    }
    for (size_t i = 0; i < lhs.getSize(); i++)
    {
        if (lhs.rowData[i] == nullptr && rhs.rowData[i] == nullptr)
        {
            continue;
        }
        if (lhs.rowData[i] == nullptr || rhs.rowData[i] == nullptr)
        {
            return false;
        }
        if (lhs.rowData[i]->getCellType() != rhs.rowData[i]->getCellType())
        {
            return false;
        }
        if (!(lhs.rowData[i]->compareEqual(*rhs.rowData[i])))
        {
            return false;
        }
    }
    return true;
}
