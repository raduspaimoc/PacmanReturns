#include "cell.h"
#include <iostream>
#include <string>
#include <cstdarg>


Cell::Cell(int x, int y) : x(x), y(y), wall(true), deleted(false), added(false), visited(false) { }
Cell::Cell(int x, int y, bool wall) : x(x), y(y), wall(wall), deleted(false), added(false) { }

bool Cell::isWall() { return wall; };

std::ostream& operator<<(std::ostream& os, const Cell& cell)
{
    os << (cell.wall ? '0' : ' ');
    return os;
}
