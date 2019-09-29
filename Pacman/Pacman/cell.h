#pragma once

#include <iostream>

enum CellFlags
{
    CELL_FLAG_WALL = 0x01,
    CELL_FLAG_FOOD = 0x02,
    CELL_FLAG_DELETED = 0x04,
    CELL_FLAG_ADDED = 0x08,
    CELL_FLAG_VISITED = 0x10,
    CELL_FLAG_PACMAN = 0x20,
    CELL_FLAG_GHOST = 0x40
};

struct Cell
{
    Cell(int x, int y);
    Cell(int x, int y, bool wall);
    Cell() : Cell(0, 0) {};

    int x, y;
    unsigned int flags;
public:

    bool isWall() { return hasFlag(CellFlags::CELL_FLAG_WALL); };
    bool isDeleted() { return hasFlag(CellFlags::CELL_FLAG_DELETED); };
    bool isAdded() { return hasFlag(CellFlags::CELL_FLAG_ADDED); };
    bool isVisited() { return hasFlag(CellFlags::CELL_FLAG_VISITED); };

    void setFlag(unsigned int p_flags) { flags = p_flags; };
    void addFlag(unsigned int p_flags) { flags |= p_flags; };
    void removeFlag(unsigned int p_flags) { flags &= ~p_flags; };
    bool hasFlag(unsigned int p_Flag) { return (flags & p_Flag) != 0; };

    void setVisited(bool apply)
    {
        if (apply)
            addFlag(CellFlags::CELL_FLAG_VISITED);
        else
            removeFlag(CellFlags::CELL_FLAG_VISITED);
    };
    void setWall(bool apply)
    {
        if (apply)
            addFlag(CellFlags::CELL_FLAG_WALL);
        else
            removeFlag(CellFlags::CELL_FLAG_WALL);
    };
    void SetDeleted(bool apply)
    {
        if (apply)
            addFlag(CellFlags::CELL_FLAG_DELETED);
        else
            removeFlag(CellFlags::CELL_FLAG_DELETED);
    };
    void SetAdded(bool apply)
    {
        if (apply)
            addFlag(CellFlags::CELL_FLAG_DELETED);
        else
            removeFlag(CellFlags::CELL_FLAG_DELETED);
    };

    bool isHorizontalBorder(int size) { return size - 1 == x || x == 0; };
    bool isVerticalBorder(int size) { return size - 1 == y || y == 0; };
    bool isBorder(int h_size, int v_size) { return isHorizontalBorder(h_size) || isVerticalBorder(v_size); };
    friend std::ostream& operator<<(std::ostream& os, const Cell& cell);

};
