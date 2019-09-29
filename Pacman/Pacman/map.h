#pragma once

#include <vector>
#include "cell.h"

struct Map
{
    public:

        Map(int r, int c);
        Map() : Map(0, 0) { };

        void showInfo();
        void setWalls();
        void addMiddle();
        void removeTrees();
        void reset();
        void addAloneWalls();
        bool isMiddle(int i, int j);
        void createVerticalSymetry();

        Cell* getPairCell(Cell cell);
        Cell* getPairCell(Cell* cell);
        int getWallCount(Cell* cell);
        void CheckTreesRec(Cell* cell);

        void DFS(Cell* cell);
        void cleanMiddle();

        friend std::ostream& operator<<(std::ostream& os, const Map& map);

        int rows, columns;
        std::vector<std::vector<Cell> > grid;
};
