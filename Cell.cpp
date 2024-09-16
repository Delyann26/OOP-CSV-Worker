#include "Cell.h"
#include "CellFactory.h"
Cell::Cell(CellType type) : type(type)
{}

int Cell::getCellType() const
{
    if (this == nullptr)
    {
        return 4;
    }
    return (int)type;
}

CellCreator::CellCreator(int type) : type(type)
{
    CellFactory::getInstance().registerCell(this);
}


int CellCreator::getCellType() const
{
    return type;
}
