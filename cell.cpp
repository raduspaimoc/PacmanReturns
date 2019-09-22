#include "cell.h"
#include <iostream>
#include <string>
#include <cstdarg>


Cell::Cell(int x, int y) : x(x), y(y), wall(true) { }

void Cell::setWall(bool apply) { wall = apply; }

bool Cell::isWall() { return wall; };

std::ostream& operator<<(std::ostream& os, const Cell& cell)
{
    os << (cell.wall ? '0' : ' ');
    return os;
}
