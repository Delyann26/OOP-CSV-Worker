#include "CellFactory.h"
#include "Cell.h"
#include <string>


CellFactory& CellFactory::getInstance()
{
	static CellFactory factory;
	return factory;
}

void CellFactory::registerCell(const CellCreator* crt)
{
    if (size < MAX_CELL_CREATORS)
    {
	    creators[size] = crt;
	    size++;
    }
}

Cell* CellFactory::createCell(const std::string& str)
{
	int dataType = getDataType(str);
    const CellCreator* crt = getCreator(dataType);

    if (crt != nullptr)
    {
        return crt->createCell(str);
    }
    else
    {
        return nullptr;
    }
}

CellFactory::CellFactory()
    :size(0)
{
}


const CellCreator* CellFactory::getCreator(int type) const
{
    for (size_t i = 0; i < size; i++)
    {
        if (creators[i]->getCellType() == type)
        {
            return creators[i];
        }
    }
    return nullptr;
}

int CellFactory::getDataType(const std::string& str) const
{
    if (str.empty())
    {
        return -1;
    }
	if (isText(str))
	{
		return 0;
	}
	else if (isDate(str))
	{
		return 1;
	}
	else if (isFacultyNumber(str))
	{
		return 2;
	}
	else if (isNumber(str))
	{
		return 3;
	}
	else
	{
		return 0;
	}
}

bool CellFactory::isText(const std::string& str) const
{
    if (str.empty())
    {
        return false;
    }
    size_t size = str.size();
    if (str[0] == '\"' && str[size - 1] == '\"')
    {
        return true;
    }
    else if (str[0] == '\'' && str[size - 1] == '\'')
    {
        return true;
    }
    return false;
}

bool CellFactory::isFacultyNumber(const std::string& str) const
{
    if (str.empty())
    {
        return false;
    }
    if (str.size() != 5 && str.size() != 10)
    {
        return false;
    }
    size_t i = 0;
    if (str.size() == 10)
    {
        if (str[4] != '4' && str[4] != '8' && str[4] != '1' && str[4] != '3' && str[4] != '0')
        {
            return false;
        }
        while (i < str.size())
        {
            if (!isdigit(str[i]) && str[i] != 'M' && str[i] != 'I')
            {
                return false;
            }
            i++;
        }
    }
    else
    {
        if (str[0] != '4' && str[0] != '8' && str[0] != '1' && str[0] != '3' && str[0] != '0')
        {
            return false;
        }
        while (i < str.size())
        {
            if (!isdigit(str[i]))
            {
                return false;
            }
            i++;
        }
    }
    return true;
}

bool CellFactory::isDate(const std::string& str) const
{
    if (str.empty())
    {
        return false;
    }
    if (str.size() != 10)
    {
        return false;
    }
    unsigned year = 0;
    unsigned month = 0;
    unsigned day = 0;

    size_t i = 0;
    while (i < 4 && str[i] != '-')
    {
        if (!isdigit(str[i]))
        {
            return false;
        }
        year = year * 10 + str[i] - '0';
        i++;
    }
    if (year > 2024 || year == 0 || str[i] != '-')
    {
        return false;
    }
    i++;
    while (i < 7 && str[i] != '-')
    {
        if (!isdigit(str[i]))
        {
            return false;
        }
        month = month * 10 + str[i] - '0';
        i++;
    }
    if (month > 12 || month == 0 || str[i] != '-')
    {
        return false;
    }
    i++;
    while (i < 10)
    {
        if (!isdigit(str[i]))
        {
            return false;
        }
        day = day * 10 + str[i] - '0';
        i++;
    }
    if (day > 31 || day == 0)
    {
        return false;
    }

    return true;
}

bool CellFactory::isNumber(const std::string& str) const
{
    if (str.empty()) 
    {
        return false;
    }

    bool hasDecimalPoint = false;
    bool hasDigitsBeforeDecimal = false;
    bool hasDigitsAfterDecimal = false;
    size_t start = 0;

    if (str[0] == '-') 
    {
        if (str.size() == 1) 
        {
            return false;
        }
        start = 1;
    }

    for (size_t i = start; i < str.size(); ++i) 
    {
        if (isdigit(str[i])) 
        {
            if (hasDecimalPoint) 
            {
                hasDigitsAfterDecimal = true;
            }
            else 
            {
                hasDigitsBeforeDecimal = true;
            }
        }
        else if (str[i] == '.') 
        {
            if (hasDecimalPoint) 
            {
                return false;
            }
            hasDecimalPoint = true;
        }
        else 
        {
            return false;
        }
    }

    if (hasDecimalPoint) {
        return hasDigitsBeforeDecimal || hasDigitsAfterDecimal;
    }

    return hasDigitsBeforeDecimal;
}



