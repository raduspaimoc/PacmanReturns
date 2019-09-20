#include "cell.h"
#include <iostream>
#include <string>
#include <cstdarg>


Cell::Cell(int x, int y) : x(x), y(y), wall(false) { }

std::ostream& operator<<(std::ostream& os, const Cell& cell)
{
    os << (cell.wall ? '0' : 'X');
    return os;
}
