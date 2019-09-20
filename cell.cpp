#include "cell.h"
#include <iostream>
#include <string>
#include <cstdarg>


Cell::Cell(int x, int y) : x(x), y(y) { }

std::ostream& operator<<(std::ostream& os, const Cell& cell)
{
    os << cell.wall ? '0' : ' ';
    return os;
}
