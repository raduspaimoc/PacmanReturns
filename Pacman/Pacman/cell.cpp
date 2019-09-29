#include "cell.h"
#include <iostream>
#include <string>
#include <cstdarg>


Cell::Cell(int x, int y) : x(x), y(y), flags(CellFlags::CELL_FLAG_WALL) { }
Cell::Cell(int x, int y, bool wall) : x(x), y(y), flags( wall ? CellFlags::CELL_FLAG_WALL : 0) { }

std::ostream& operator<<(std::ostream& os, const Cell& cell)
{
    os << ((cell.flags & Cell::CellFlags::CELL_FLAG_WALL) != 0 ? '0' : ' ');
    return os;
}
