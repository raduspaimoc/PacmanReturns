#pragma once

#include <vector>
#include "cell.h"
#include "character.h"

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
        void initCells();
        void addAloneWalls();
        bool isMiddle(int i, int j);
        void createVerticalSymetry();

        Cell* getPairCell(Cell cell);
        Cell* getPairCell(Cell* cell);
        int getWallCount(Cell* cell);
        void CheckTreesRec(Cell* cell);

        void DFS(Cell* cell);
        void cleanMiddle();
        void initCharacters(int pacman_x, int pacman_y);

        friend std::ostream& operator<<(std::ostream& os, const Map& map);

        int rows, columns;
        //Cell* pacman;
        Character pacman, ghost;
        std::vector<Character> auto_ghosts;
        float pacman_x, pacman_y;
        std::vector<std::vector<Cell>> grid;
};
