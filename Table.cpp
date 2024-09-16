#include "Table.h"

Table::Table()
    :rowsCount(0), rowsCapacity(0), hasHeaders(false)
{
    this->rows = new(std::nothrow) Row[8];
    if (this->rows != nullptr)
    {
        rowsCapacity = 8;
    }
}

Table::Table(const Table& other) 
    : rows(nullptr),rowsCount(0),rowsCapacity(0),hasHeaders(false)
{
    Row* temp = nullptr;
    try
    {
        temp = new Row[other.rowsCapacity];
        for (size_t i = 0; i < other.rowsCount; i++)
        {
            temp[i] = other.rows[i];
        }
    }
    catch (const std::bad_alloc&)
    {
        delete[] temp;
        throw;
    }
    this->rows = temp;
    this->rowsCapacity = other.rowsCapacity;
    this->rowsCount = other.rowsCount;
    this->hasHeaders = other.hasHeaders;
    this->headers = other.headers;
}

Table& Table::operator=(const Table& other)
{
    if (this == &other)
    {
        return *this;
    }
    Row* temp = nullptr;
    try
    {
        temp = new Row[other.rowsCapacity];
        for (size_t i = 0; i < other.rowsCount; i++)
        {
            temp[i] = other.rows[i];
        }
    }
    catch (const std::bad_alloc&)
    {
        delete[] temp;
        throw;
    }
    free();
    this->rows = temp;
    this->rowsCapacity = other.rowsCapacity;
    this->rowsCount = other.rowsCount;
    this->hasHeaders = other.hasHeaders;
    this->headers = other.headers;

    return *this;
}

Table::~Table()
{
    free();
}

void Table::addRow(const Row& toAdd)
{
    if (rowsCount == rowsCapacity)
    {
        resize(rowsCapacity * 2);
    }
    rows[rowsCount] = toAdd;
    rowsCount++;
}

void Table::removeRow(size_t index)
{
    if (index >= rowsCount)
    {
        throw std::length_error("No such row exists in the table!");
    }
    for (size_t i = index; i < rowsCount-1; i++)
    {
        rows[i] = rows[i + 1];
    }
    rowsCount--;
    if (rowsCount == rowsCapacity / 4)
    {
        resize(rowsCapacity / 2);
    }
}

bool isLetter(char c)
{
    return (tolower(c) >= 'a' && tolower(c) <= 'z');
}

bool Table::checkForHeaders(const std::string& line) const
{
    if (line.size() == 0)
    {
        return false;
    }
    std::string currentTitle;

    size_t index = 0;
    while (index<line.size())
    {
        if (line[index] == ',')
        {
            if (!isLetter(currentTitle[0]))
            {
                return false;
            }
            index++;

            currentTitle.clear();
        }
        currentTitle += line[index++];
    }
    if (!isLetter(currentTitle[0]))
    {
        return false;
    }

    return true;
}

void Table::fillArray(size_t* delimitersPosition, size_t size) const
{
    if (hasHeaders == 1)
    {
        for (size_t i = 0; i < size; i++)
        {
            if (delimitersPosition[i] < headers[i].size())
            {
                delimitersPosition[i] = headers[i].size();
            }
        }
    }

    for (size_t i = 0; i < rowsCount; i++)
    {
        for (size_t j = 0; j < size; j++)
        {
            if (rows[i][j] != nullptr)
            {
                size_t curr = rows[i][j]->getSizeOfCell();
                if (curr > delimitersPosition[j])
                {
                    delimitersPosition[j] = curr;
                }
            }
        }
    }
}

int Table::getWayOfFiltering(const std::string& str) const
{
    if (str == "<")
    {
        return 0;
    }
    else if (str == "<=")
    {
        return 1;
    }
    else if (str == ">")
    {
        return 2;
    }
    else if (str == ">=")
    {
        return 3;
    }
    else if (str == "==")
    {
        return 4;
    }
    else if (str == "!=")
    {
        return 5;
    }
    else
    {
        return -1;
    }
}

const Cell* Table::getTypeOfCondition(const std::string& str) const
{
    Cell* result = CellFactory::getInstance().createCell(str);
    return result;
}

void Table::permutateHeaders(const std::vector<unsigned>& permutation)
{
    std::vector<std::string> temp;
    unsigned currentIndex = 0;
    for (size_t i = 0; i < headers.size(); i++)
    {
        currentIndex = permutation[i] - 1;

        temp.push_back(headers[currentIndex]);
    }
    for (size_t i = 0; i < headers.size(); i++)
    {
        std::swap(headers[i], temp[i]);
    }
}

const Cell* Table::getMinInColumn(size_t index) const
{
    Cell* min = nullptr;
    for (size_t i = 0; i < rowsCount; i++)
    {
        Cell* currentCell = rows[i][index];
        if (currentCell == nullptr)
        {
            continue;
        }
        if (min == nullptr || currentCell->compareLessThan(*min))
        {
            min = currentCell;
        }
    }

    return min;
}

const Cell* Table::getMaxInColumn(size_t index) const
{
    Cell* max = nullptr;
    for (size_t i = 0; i < rowsCount; i++)
    {
        Cell* currentCell = rows[i][index];
        if (currentCell == nullptr)
        {
            continue;
        }
        if (max == nullptr || currentCell->compareGreaterThan(*max))
        {
            max = currentCell;
        }
    }
    return max;
}

Cell* Table::getMostFrequent(size_t index) const
{
    std::vector<Pair> pairs;
    for (size_t i = 0; i < rowsCount; i++)
    {
        if (pairs.empty())
        {
            pairs.push_back({ rows[i][index], 1 });
        }
        else
        {
            int indexInPair = -1;
            if (isInPairs(pairs, rows[i][index], indexInPair) != -1)
            {
                pairs[indexInPair].amount++;
            }
            else
            {
                pairs.push_back({ rows[i][index], 1 });
            }
        }
    }
    unsigned mostFrequentAmount = getMostFrequentAmount(pairs);
    sortPairsByAmount(pairs);
    size_t j = pairs.size() - 1;
    while (pairs[j].amount != mostFrequentAmount)
    {
        pairs.pop_back();
        j--;
    }
    if (pairs.size() == 1)
    {
        return pairs[0].value;
    }
    
    Cell *mostFrequent = nullptr;
    for (size_t i = 0; i < pairs.size(); i++)
    {
        Cell* currentCell = pairs[i].value;
        if (currentCell == nullptr)
        {
            continue;
        }
        if (mostFrequent == nullptr || pairs[i].value->compareLessThan(*mostFrequent))
        {
            mostFrequent = pairs[i].value;
        }
    }
    return mostFrequent;
}

int Table::isInPairs(const std::vector<Pair>& pairs, const Cell* cell, int& index) const
{
    for (size_t i = 0; i < pairs.size(); i++)
    {
        if (cell == nullptr && pairs[i].value == nullptr)
        {
            index = i;
            return index;
        }
        if (cell != nullptr && pairs[i].value != nullptr && pairs[i].value->compareEqual(*cell))
        {
            index = i;
            return index;
        }
    }

    return -1;
}

unsigned Table::getMostFrequentAmount(const std::vector<Pair>& pairs) const
{
    size_t max = 0;
    for (size_t i = 0; i < pairs.size(); i++)
    {
        if (pairs[i].amount > max)
        {
            max = pairs[i].amount;
        }
    }
    return max;
}

void Table::sortPairsByAmount(std::vector<Pair>& pairs) const
{
    for (size_t i = 0; i < pairs.size()-1; i++)
    {
        size_t maxIndex = i;
        for (size_t j = i+1; j < pairs.size(); j++)
        {
            if (pairs[maxIndex].amount < pairs[j].amount)
            {
                maxIndex = j;
            }
        }
        if (maxIndex != i)
        {
            std::swap(pairs[maxIndex], pairs[i]);
        }
    }
}

size_t Table::getFirstDifferentAmount(const std::vector<Pair>& pairs, unsigned max)const
{
    for (size_t i = 0; i < pairs.size(); i++)
    {
        if (max != pairs[i].amount)
        {
            return i;
        }
    }
    return -1;
}

void Table::copyRow(size_t index)
{
    if (index >= rowsCount)
    {
        throw std::invalid_argument("Invalid index!");
    }
    Row copy = rows[index];
    addRow(copy);
}

void Table::addRowWithMinValues()
{
    if (rowsCount == 0)
        throw std::runtime_error("Table is empty!");

    Row result;
    size_t columns = getColumnsSize();
    for (size_t i = 0; i < columns; i++)
    {
        const Cell* minValue = nullptr;
        minValue = getMinInColumn(i);
        result.addCell(minValue ? minValue->clone() : nullptr);
    }
    addRow(result);
}

void Table::addRowWithMaxValues()
{
    if (rowsCount == 0)
        throw std::runtime_error("Table is empty!");

    Row result;
    size_t columns = getColumnsSize();
    for (size_t i = 0; i < columns; i++)
    {
        const Cell* maxValue = nullptr;
        maxValue = getMaxInColumn(i);
        result.addCell(maxValue ? maxValue->clone() : nullptr);
    }
    addRow(result);
}

void Table::addRowFromMostFrequentValues()
{
    if (rowsCount == 0)
        throw std::runtime_error("Table is empty!");

    Row result;
    size_t columns = getColumnsSize();
    for (size_t i = 0; i < columns; i++)
    {
        const Cell* mostFrequent = nullptr;
        mostFrequent = getMostFrequent(i);
        result.addCell(mostFrequent ? mostFrequent->clone() : nullptr);
    }
    addRow(result);
}

size_t Table::getColumnsSize() const
{
    if (hasHeaders == true)
    {
        return headers.size();
    }
    else
    {
        return rows[0].getSize();
    }
}
bool isEmpty(const std::string& str)
{
    for (size_t i = 0; i < str.size(); i++)
    {
        if (str[i] != ' ')
        {
            return false;
        }
    }
    return true;
}
const std::vector<unsigned> Table::getPermutation(const std::string& str) const
{
    if (isEmpty(str))
    {
        throw std::invalid_argument("Invalid permutation!");
    }
    std::vector<unsigned> result;
    std::string copy = str;
    unsigned number = 0;
    for (size_t i = 0; i < copy.size(); i++)
    {
        if (!isdigit(copy[i]) && copy[i] != ' ')
        {
            throw std::invalid_argument("Invalid permutation!");
        }

        if (copy[i] == ' ')
        {
            if (i == 0 || copy[i - 1] == ' ')
            {
                continue;
            }
            result.push_back(number);
            number = 0;
            continue;
        }
        number = number * 10 + copy[i] - '0';
    }
    if (number != 0)
    {
        result.push_back(number);
    }
    return isValidPermutation(result) ? result : throw std::invalid_argument("Invalid permutation!");
}

bool Table::isValidPermutation(const std::vector<unsigned>& permutation) const
{
    size_t columnsCount = permutation.size();
    if (columnsCount != getColumnsSize())
    {
        return false;
    }
    std::vector<bool> encountered(columnsCount, 0);
    for (size_t i = 0; i < columnsCount; i++)
    {
        if (permutation[i] < 1 || permutation[i] > columnsCount)
            return false;


        if (encountered[permutation[i] - 1])
            return false;

        
        encountered[permutation[i] - 1] = true;
    }
    for (size_t i = 0; i < encountered.size(); i++)
    {
        if (encountered[i] == false)
        {
            return false;
        }
    }
    return true;
}

void printHelper(char c, size_t count)
{
    for (size_t i = 0; i < count; i++)
    {
        std::cout << c;
    }
}

void Table::print() const
{
    size_t cols;
    if (hasHeaders == 1)
        cols = headers.size();
    else
        cols = rows[0].getSize();

    if (cols == 0)
        return;

    size_t* delimitersPosition = new size_t[cols];
    for (size_t i = 0; i < cols; i++)
        delimitersPosition[i] = 4;

    fillArray(delimitersPosition, cols);

    size_t width = cols + 1;
    for (size_t i = 0; i < cols; i++)
        width += delimitersPosition[i];

    printHelper('_', width);
    std::cout << std::endl;
    if (hasHeaders == 1)
    {
        std::cout << '|';
        for (size_t i = 0; i < cols; i++)
        {
            std::cout << headers[i];
            size_t fillersCnt = delimitersPosition[i] - headers[i].size();
            printHelper(' ', fillersCnt);
            std::cout << '|';
        }
        std::cout << std::endl;
    }

    for (size_t i = 0; i < rowsCount; i++)
    {
        std::cout << '|';
        for (size_t j = 0; j < cols; j++)
        {
            if (rows[i][j] != nullptr)
            {
                rows[i][j]->printCell();
                size_t fillerCnt = delimitersPosition[j] - rows[i][j]->getLengthOfValueInCell();
                printHelper(' ', fillerCnt);
            }
            else
            {
                size_t fillerCnt = delimitersPosition[j];
                printHelper(' ', fillerCnt);
            }
            std::cout << '|';
        }
        std::cout << std::endl;
    }
    printHelper('_', width);
    std::cout << std::endl;

    delete[] delimitersPosition;
}

void Table::writeToCSVFile(std::ofstream& ofs) const
{
    if (hasHeaders == 1)
    {
        for (size_t i = 0; i < headers.size(); i++)
        {
            ofs << headers[i];
            if (i != headers.size() - 1)
            {
                ofs << ',';
            }
        }
        ofs << std::endl;
    }

    for (size_t i = 0; i < rowsCount; i++)
    {
        rows[i].writeToCSVFile(ofs);
        ofs << std::endl;
    }
}

void Table::readFromCSVFile(std::ifstream& ifs)
{
    if (!isValidFile(ifs))
    {
        ifs.close();
        throw std::runtime_error("Invalid file syntax!");
    }
    ifs.clear();
    ifs.seekg(0, std::ios::beg);

    std::string firstRow;
    std::getline(ifs, firstRow, '\n');

    hasHeaders = checkForHeaders(firstRow);
    if (hasHeaders == true)
    {
        hasHeaders = true;
        setColumns(firstRow);
    }
    else
    {
        ifs.clear();
        ifs.seekg(0, std::ios::beg);
    }
    try
    {
        while (!ifs.eof())
        {
            try
            {
                Row toAdd;
                toAdd.readFromCSVFile(ifs);
                if (!isSync(toAdd))
                {
                    throw std::runtime_error("The new row is not correct compared to the previous rows!");
                }
                addRow(toAdd);
            }
            catch (const std::bad_alloc&)
            {
                throw;
            }
            catch (...)
            {
            }
        }
    }
    catch (const std::bad_alloc&)
    {
        throw;
    }

}

void Table::sortByHeaderOfColInAscendingOrder(const std::string& header)
{
    size_t index = getHeaderIndex(header);
    if (index == -1)
    {
        throw std::invalid_argument("Sorting failed: No such header exists!");
    }
    sortByIndexOfColInAscendingOrder(index);
}

void Table::sortByHeaderOfColInDescendingOrder(const std::string& header)
{
    size_t index = getHeaderIndex(header);
    if (index == -1)
    {
        throw std::invalid_argument("Sorting failed: No such header exists!");
    }
    sortByIndexOfColInDescendingOrder(index);
}

void Table::sortByIndexOfColInAscendingOrder(size_t index)
{
    if (rowsCount == 0)
    {
        throw std::runtime_error("Sorting failed: Empty table!");
    }
    if (rowsCount == 1)
    {
        return;
    }
    if (index >= rows[0].getSize() || index < 0)
    {
        throw std::length_error("Sorting failed: Invalid index!");
    }
    std::vector<Row> nullRows = getNullRows(index);
    if (nullRows.size() == rowsCount)
    {
        return;
    }
    removeNullRows(index, nullRows);

    Row* temp = nullptr;
    try
    {
        temp = new Row[rowsCapacity];
        for (size_t i = 0; i < rowsCount; i++)
        {
            temp[i] = rows[i];
        }
        for (size_t i = 1; i < rowsCount; i++)
        {
            Row curr = temp[i];
            size_t j = i - 1;
            while (j != -1 && (temp[j][index]->compareGreaterThan(*curr[index])))
            {
                temp[j + 1] = temp[j];
                j--;
            }
            std::swap(temp[j + 1], curr);
        }
    }
    catch (const std::bad_alloc&)
    {
        delete[] temp;
        addNullRows(nullRows);
        throw;
    }
    delete[] rows;
    rows = temp;

    addNullRows(nullRows);
}

void Table::sortByIndexOfColInDescendingOrder(size_t index)
{
    if (rowsCount == 0)
    {
        throw std::runtime_error("Sorting failed: Empty table!");
    }
    if (rowsCount == 1)
    {
        return;
    }
    if (index >= rows[0].getSize())
    {
        throw std::length_error("Sorting failed: Invalid index!");
    }
    std::vector<Row> nullRows = getNullRows(index);
    removeNullRows(index, nullRows);

    Row* temp = nullptr;
    try
    {
        temp = new Row[rowsCapacity];
        for (size_t i = 0; i < rowsCount; i++)
        {
            temp[i] = rows[i];
        }
        for (size_t i = 1; i < rowsCount; i++)
        {
            Row curr = temp[i];
            size_t j = i - 1;
            while (j != -1 && (temp[j][index]->compareLessThan(*curr[index])))
            {
                temp[j + 1] = temp[j];
                j--;
            }
            std::swap(temp[j + 1], curr);
        }
    }
    catch (const std::bad_alloc&)
    {
        delete[] temp;
        addNullRows(nullRows);
        throw;
    }
    delete[] rows;
    rows = temp;

    addNullRows(nullRows);
}

void Table::filterByHeader(const std::string& header, const std::string& wayOfFiltering, const std::string& condition)
{
    size_t index = getHeaderIndex(header);
    if (index == -1)
    {
        throw std::invalid_argument("Filtering failed: No such header exists!");
    }
    filterByColumnIndex(index, wayOfFiltering, condition);
}

void Table::filterByColumnIndex(size_t index, const std::string& wayOfFiltering, const std::string& condition)
{
    if (rowsCount == 0)
    {
        throw std::runtime_error("Filtering failed: Empty table!");
    }
    if (index >= rows[0].getSize())
    {
        throw std::length_error("Filtering failed: Invalid index!");
    }
    std::vector<size_t> removeIndexes;

    int way = getWayOfFiltering(wayOfFiltering);
    if (way == -1)
    {
        throw std::invalid_argument("Filtering failed: Invalid way of filtering!");
    }
    const Cell* typeCondition = getTypeOfCondition(condition);
    if (typeCondition == nullptr)
    {
        throw std::invalid_argument("Filtering failed: Invalid condition!");
    }

    bool toRemove = false;

    for (size_t i = 0; i < rowsCount; i++)
    {
        if (rows[i][index] == nullptr)
        {
            removeIndexes.push_back(i);
            continue;
        }
        if (typeCondition->getCellType() != rows[i][index]->getCellType())
        {
            throw std::runtime_error("Filtering failed: Inconsistent cell types!");
        }
        switch (way)
        {   
        case 0:
            toRemove = !(typeCondition->compareGreaterThan(*rows[i][index])); break;
        case 1:
            toRemove = !(typeCondition->compareGreaterThan(*rows[i][index]) || typeCondition->compareEqual(*rows[i][index])); break;
        case 2:
            toRemove = !(typeCondition->compareLessThan(*rows[i][index])); break;
        case 3:
            toRemove = !(typeCondition->compareLessThan(*rows[i][index]) || typeCondition->compareEqual(*rows[i][index])); break;
        case 4:
            toRemove = !(typeCondition->compareEqual(*rows[i][index])); break;
        case 5:
            toRemove = !(!(typeCondition->compareEqual(*rows[i][index]))); break;
        default:
            return;
        }
        if (toRemove == true)
        {
            removeIndexes.push_back(i);
            toRemove = false;
        }
    }

    for (size_t i = 0; i < removeIndexes.size(); i++)
    {
        removeRow(removeIndexes[i]);
        for (unsigned j = i + 1; j < removeIndexes.size(); j++)
            removeIndexes[j]--;
    }
}

void Table::changeColumnsSequence(const std::string& str)
{
    std::vector<unsigned> permutation;
    Row* temp = nullptr;
    try
    {
        permutation = getPermutation(str);
        temp = new Row[rowsCapacity];
        for (size_t i = 0; i < rowsCount; i++)
        {
            temp[i] = rows[i];
        }
        for (size_t i = 0; i < rowsCount; i++)
        {
            temp[i].permutateRow(permutation);
        }
    }
    catch (const std::bad_alloc&)
    {
        delete[] temp;
        throw;
    }
    delete[] rows;
    rows = temp;
    if (hasHeaders == 1)
    {
        permutateHeaders(permutation);
    }
}

bool contains(const Row* arr, size_t size, const Row& value)
{
    for (size_t i = 0; i < size; i++)
    {
        if (arr[i] == value)
        {
            return true;
        }
    }
    return false;
}

void Table::removeDuplicates()
{
    if (rowsCount == 1)
    {
        return;
    }
    Row* uniqueRows = nullptr;
    size_t sizeOfUniqueRows = 0;
    try
    {
        uniqueRows = new Row[rowsCount];
        for (size_t i = 0; i < rowsCount; i++)
        {
            if (!contains(uniqueRows, sizeOfUniqueRows, rows[i]))
            {
                uniqueRows[sizeOfUniqueRows] = rows[i];
                sizeOfUniqueRows++;
            }
        }
    }
    catch (const std::bad_alloc&)
    {
        delete[] uniqueRows;
        throw;
    }
    delete[] rows;
    rows = uniqueRows;
    rowsCount = sizeOfUniqueRows;
}

void Table::removeCol(const std::string& header)
{
    size_t index = getHeaderIndex(header);
    if (index == -1)
    {
        throw std::invalid_argument("Removing failed: No such header exists!");
    }
    removeCol(index);
}

void Table::removeCol(size_t index)
{
    if (rowsCount == 0)
    {
        if (hasHeaders == 0)
        {
            throw std::runtime_error("Empty table");
        }
    }
    if (index >= rows[0].getSize())
    {
        if (index >= headers.size())
        {
            throw std::length_error("Removing failed: No such column exists in the table!");
        }
    }

    if (hasHeaders == 1)
    {
        headers.erase(headers.begin() + index);
    }

    for (size_t i = 0; i < rowsCount; i++)
    {
        rows[i].removeCell(index);
    }
}

bool Table::isValidFile(std::ifstream& ifs) const
{
    unsigned countOfCommasInRow = 0;
    std::string row;
    
    std::getline(ifs, row, '\n');
    if (!ifs || !hasValidChars(row))
        return false;

    countOfCommasInRow = countCommas(row);
    while (std::getline(ifs, row, '\n'))
    {
        if (!ifs || !hasValidChars(row))
            return false;

        unsigned currentRowCountOfCommas = countCommas(row);

        if (countOfCommasInRow != currentRowCountOfCommas) 
            return false;
    }

    return true;
}

bool Table::hasValidChars(const std::string& str) const
{
    for (size_t i = 0; i < str.size(); i++)
    {
        if (str[i] < 0 || str[i]>127)
        {
            return false;
        }
    }
    return true;
}

unsigned Table::countCommas(const std::string& str) const
{
    unsigned count = 0;
    bool isInsideQuotes = false;
    for (size_t i = 0; i < str.size(); i++)
    {
        if (str[i] == '"' || str[i] == '\'')
        {
            isInsideQuotes = !isInsideQuotes;
        }
        else if (str[i] == ',' && isInsideQuotes == false)
        {
            count++;
        }
        else if (str[i] == ' ' && isInsideQuotes == false)
        {
            throw std::runtime_error("Invalid file syntax!");
        }
    }
    return count;
}

bool Table::isSync(const Row& row) const
{
    if (rowsCount == 0)
    {
        return true;
    }
    for (size_t i = 0; i < rowsCount; i++)
    {
        if (row.getSize() != rows[i].getSize())
        {
            return false;
        }
    }
    size_t size = rows[0].getSize();
    for (size_t i = 0; i < rowsCount; i++)
    {
        for (size_t j = 0; j < size; j++)
        {   
            if (rows[i][j] == nullptr || row[j] == nullptr)
            {
                continue;
            }
            if (row[j]->getCellType() != rows[i][j]->getCellType())
            {
                return false;
            }
        }
    }
    return true;
}

void Table::setColumns(const std::string& line)
{
    std::string currentTitle;
    size_t index = 0;
    while (index < line.size())
    {
        if (line[index] == ',')
        {
            headers.push_back(currentTitle);
            index++;
            currentTitle.clear();
        }
        currentTitle += line[index++];
    }
    headers.push_back(currentTitle);
}

void Table::free()
{
    delete[] rows;
    rowsCount = 0;
    rowsCapacity = 0;
    hasHeaders = false;
}


size_t Table::getHeaderIndex(const std::string& str) const
{
    for (size_t i = 0; i < headers.size(); i++)
    {
        if (str == headers[i])
        {
            return i;
        }
    }
    return -1;
}

void Table::resize(size_t newCapacity)
{
    Row* temp = nullptr;
    try
    {
        temp = new Row[newCapacity];
        for (size_t i = 0; i < this->rowsCount; i++)
        {
            temp[i] = this->rows[i];
        }
    }
    catch (const std::bad_alloc&)
    {
        delete[] temp;
        throw;
    }
    delete[] this->rows;
    this->rows = temp;
    rowsCapacity = newCapacity;
}

const std::vector<Row> Table::getNullRows(size_t index) const
{
    std::vector<Row> result;
    for (size_t i = 0; i < rowsCount; i++)
    {
        if (rows[i][index] == nullptr)
        {
            result.push_back(rows[i]);
        }
    }
    return result;
}

void Table::removeNullRows(size_t index,const std::vector<Row>& nullRows)
{
    if (!nullRows.empty())
    {
        for (size_t i = rowsCount; i > 0; --i)
        {
            if (rows[i - 1][index] == nullptr)
            {
                removeRow(i - 1);
            }
        }
    }
}

void Table::addNullRows(const std::vector<Row>& nullRows)
{
    for (size_t i = 0; i < nullRows.size(); i++)
    {
        addRow(nullRows[i]);
    }
}


