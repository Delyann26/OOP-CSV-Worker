#include "FacultyNumber.h"



FacultyNumber::FacultyNumber(const std::string& fn):
    fn(""),
    isNewStyle(-1)
{
    try
    {
        setFn(fn);
    }
    catch (std::invalid_argument& error)
    {
        throw;
    }
}

void FacultyNumber::setFn(const std::string& fn)
{
    if (!isValid(fn))
    {
        throw std::invalid_argument("Invalid faculty number!");
    }
    this->isNewStyle = checkIfNewStyle(fn);
    if (isNewStyle == -1)
    {
        throw std::invalid_argument("Invalid faculty number!");
    }
    this->fn = fn;

    setSpecialty();
}

std::string FacultyNumber::getFn() const
{
    if (!fn.empty())
    {
        return fn;
    }
    throw std::length_error("Faculty number is nullptr");
}

bool FacultyNumber::getStyle() const
{
    return this->isNewStyle;
}

int FacultyNumber::getSpecialty() const
{
    return (int)spec;
}

int FacultyNumber::checkIfNewStyle(const std::string& fn) const
{
    if (fn.size() == 5)
    {
        return 0;
    }
    else if (fn.size() == 10)
    {
        if (fn[1] == 'M' && fn[2] == 'I')
        {
            return 1;
        }
        return -1;
    }
    else
    {
        return -1;
    }
}

bool FacultyNumber::isValid(const std::string& fn) const
{
    if (fn.empty())
    {
        return false;
    }
    if (containsSpace(fn))
    {
        return false;
    }
    if (fn.size() != 5 && fn.size() != 10)
    {
        return false;
    }

    return true;
}

void FacultyNumber::print() const
{
    std::cout << this->fn << " ";

    switch (this->spec)
    {
    case Specialty::INFORMATICS: std::cout << "INFORMATICS"; break;
    case Specialty::COMPUTER_SCIENCE: std::cout << "COMPUTER_SCIENCE"; break;
    case Specialty::MATH: std::cout << "MATH"; break;
    case Specialty::APPLIED_MATH: std::cout << "APPLIED_MATH"; break;
    case Specialty::STATISTICS: std::cout << "STATISTICS"; break;
    default: std::cout << "UNKNOWN"; break;
    }
    std::cout << " " << this->isNewStyle << std::endl;
}

bool FacultyNumber::containsSpace(const std::string& fn) const
{
    for (size_t i = 0; i < fn.size(); i++)
    {
        if (isspace(fn[i]))
        {
            return true;
        }
    }
    return false;
}

void FacultyNumber::setSpecialty()
{
    if (isNewStyle == 0)
    {
        switch (this->fn[0])
        {
        case '4': this->spec = Specialty::INFORMATICS; break;
        case '8': this->spec = Specialty::COMPUTER_SCIENCE; break;
        case '1': this->spec = Specialty::MATH; break;
        case '3':this->spec = Specialty::APPLIED_MATH; break;
        case '0':this->spec = Specialty::STATISTICS; break;
        default: this->spec = Specialty::UNKNOWN; throw std::invalid_argument("Invalid faculty number");
        }
        return;
    }
    else if (isNewStyle == 1)
    {
        switch (this->fn[4])
        {
        case '4': this->spec = Specialty::INFORMATICS; break;
        case '8': this->spec = Specialty::COMPUTER_SCIENCE; break;
        case '1': this->spec = Specialty::MATH; break;
        case '3':this->spec = Specialty::APPLIED_MATH; break;
        case '0':this->spec = Specialty::STATISTICS; break;
        default: this->spec = Specialty::UNKNOWN; throw std::invalid_argument("Invalid faculty number");
        }
        return;
    }
    throw std::invalid_argument("Invalid faculty number");
}

bool operator==(const FacultyNumber& lhs, const FacultyNumber& rhs)
{
    return lhs.getFn() == rhs.getFn() && lhs.getSpecialty() == rhs.getSpecialty() && lhs.getStyle() == rhs.getStyle();
}

bool operator!=(const FacultyNumber& lhs, const FacultyNumber& rhs)
{
    return !(lhs == rhs);
}

bool operator<(const FacultyNumber& lhs, const FacultyNumber& rhs)
{
    if (lhs.getStyle() == 0 && rhs.getStyle() == 1)
    {
        return true;
    }
    else if(lhs.getStyle() == 1 && rhs.getStyle() == 1)
    {
        if (lhs.getSpecialty() < rhs.getSpecialty())
        {
            return true;
        }
        else if (lhs.getSpecialty() == rhs.getSpecialty())
        {
            if (lhs.getFn()[0] - '0' < rhs.getFn()[0] - '0')
            {
                return true;
            }
            else if (lhs.getFn()[0] - '0' == rhs.getFn()[0] - '0')
            {
                int a = 0;
                int b = 0;
                for (size_t i = 7; i < 10; i++)
                {
                    a = a * 10 + (lhs.getFn()[i] - '0');
                    b = b * 10 + (rhs.getFn()[i] - '0');
                }
                if (a < b)
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    else if (lhs.getStyle() == 0 && rhs.getStyle() == 0)
    {
        if (lhs.getSpecialty() < rhs.getSpecialty())
        {
            return true;
        }
        else if (lhs.getSpecialty() == rhs.getSpecialty())
        {
            int a = 0;
            int b = 0;
            for (size_t i = 2; i < 5; i++)
            {
                a = a * 10 + (lhs.getFn()[i] - '0');
                b = b * 10 + (rhs.getFn()[i] - '0');
            }
            if (a < b)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

bool operator>=(const FacultyNumber& lhs, const FacultyNumber& rhs)
{
    return !(lhs < rhs);
}

bool operator>(const FacultyNumber& lhs, const FacultyNumber& rhs)
{
    return !(lhs <= rhs);
}

bool operator<=(const FacultyNumber& lhs, const FacultyNumber& rhs)
{
    return lhs < rhs || lhs == rhs;
}
