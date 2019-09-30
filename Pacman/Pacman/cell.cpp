#include "cell.h"
#include <iostream>
#include <string>
#include <cstdarg>


Cell::Cell(float x, float y) : x(x), y(y), flags(CellFlags::CELL_FLAG_WALL) { }
Cell::Cell(float x, float y, bool wall) : x(x), y(y), flags( wall ? CellFlags::CELL_FLAG_WALL : 0) { }

std::ostream& operator<<(std::ostream& os, const Cell& cell)
{
    //os << ((cell.flags & CellFlags::CELL_FLAG_WALL) != 0 ? '0' : ' ');
    if((cell.flags & CellFlags::CELL_FLAG_WALL) != 0){
      os << '0';
    } else if(cell.flags & CellFlags::CELL_FLAG_PACMAN){
      os << 'P';
    } else {
      os << ' ';
    }  
    return os;
}
