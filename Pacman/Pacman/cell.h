#pragma once

#include <iostream>

struct Cell
{
    Cell(int x, int y);
    Cell() : Cell(0, 0) {};

    int x, y;
    bool wall, deleted, added;

    void setWall(bool apply) { wall = apply; };
    bool isWall();
    bool isDeleted() { return deleted; };
    bool isAdded() { return added; };
    void SetDeleted(bool apply) { deleted = apply; };
    void SetAdded(bool apply) { added = apply; };
    bool isHorizontalBorder(int size) { return size - 1 == x || x == 0; };
    bool isVerticalBorder(int size) { return size - 1 == y || y == 0; };
    bool isBorder(int h_size, int v_size) { return isHorizontalBorder(h_size) || isVerticalBorder(v_size); };
    friend std::ostream& operator<<(std::ostream& os, const Cell& cell);

};
