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
        void setWallsRec(int i, int j);
        void addMiddle();
        void removeTrees();
        void reset();
        void addWalls();
        bool isMiddle(int i, int j);
        void createVerticalSymetry();
        int countVisitedNeighbor(int i, int j);

        friend std::ostream& operator<<(std::ostream& os, const Map& map);

        int rows, columns;
        std::vector<std::vector<Cell> > grid;
};