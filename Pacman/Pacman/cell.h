#pragma once

#include <iostream>

struct Cell
{
    Cell(int x, int y);
    Cell(int x, int y, bool wall);
    Cell() : Cell(0, 0) {};

    int x, y;
    unsigned int flags;
    bool wall, deleted, added, visited; ///< Fucking awful, TODO: implement flags

public:

    enum CellFlags
    {
        CELL_FLAG_WALL = 0x01,
        CELL_FLAG_FOOD = 0x02,
        CELL_FLAG_DELETED = 0x04,
        CELL_FLAG_ADDED = 0x08,
        CELL_FLAG_VISITED = 0x10
    };

    bool isWall();
    bool isDeleted() { return deleted; };
    bool isAdded() { return added; };
    bool isVisited() { return visited; };

    void setFlag(unsigned int p_flags) { flags = p_flags; };
    void addFlag(unsigned int p_flags) { flags |= p_flags; };
    void removeFlag(unsigned int p_flags) { flags &= ~p_flags; };
    bool hasFlag(unsigned int p_Flag) { return flags & p_Flag; };

    void setVisited(bool apply) { visited = apply; };
    void setWall(bool apply) { wall = apply; };
    void SetDeleted(bool apply) { deleted = apply; };
    void SetAdded(bool apply) { added = apply; };

    bool isHorizontalBorder(int size) { return size - 1 == x || x == 0; };
    bool isVerticalBorder(int size) { return size - 1 == y || y == 0; };
    bool isBorder(int h_size, int v_size) { return isHorizontalBorder(h_size) || isVerticalBorder(v_size); };
    friend std::ostream& operator<<(std::ostream& os, const Cell& cell);

};
