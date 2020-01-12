#pragma once

#include <vector>
#include "cell.h"
#include "character.h"


struct Map
{
    public:

        Map(int r, int c);
        Map() : Map(0, 0) { };

        int rows, columns;
        int total_food;
        Character pacman, ghost;
        std::vector<Character> auto_ghosts;
        std::vector<std::vector<Cell>> grid;

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
        bool pacmanWins();
        bool pacmanLoses();
        void generateSuccesor(int agent, std::vector<int> action);
        Character getAgent(int agent);
        std::vector<Cell> getCellsWithFood();
        std::vector<Character> getGhosts();
        void checkGameState(long t, long last_t);
        std::vector<Cell> getWalls();
        int getTotalScore();
        friend std::ostream& operator<<(std::ostream& os, const Map& map);

        int rows, columns;
        int total_food;
        int score;
        Character pacman, ghost;
        std::vector<Character> auto_ghosts;
        std::vector<std::vector<Cell>> grid;
        std::vector<Cell> getCellsWithFood();



        bool pacmanWins(){
            return total_food == 0;
        }

        bool pacmanLoses(){

            int pacman_x = (int) s_map.pacman.x;
            int pacman_y = (int) s_map.pacman.y;
            int ghost_x = (int) s_map.ghost.x;
            int ghost_y = (int) s_map.ghost.y;

            if (pacman_x == ghost_x && pacman_y == ghost_y)
                return true;

            for (auto & auto_ghost : s_map.auto_ghosts)
            {
                ghost_x = (int) s_map.ghost.x;
                ghost_y = (int) s_map.ghost.y;

                if (pacman_x == ghost_x && pacman_y == ghost_y)
                    return true;
            }
            return false;
        }
};
