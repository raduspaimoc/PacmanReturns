#pragma once
#include "ShareDefines.h"
#include "cell.h"
#include <GL/glut.h>

#define MOVE 1
#define QUIET 2

enum CharacterFlags
{
    CHARACTER_FLAG_PACMAN = 0x01,
    CHARACTER_FLAG_GHOST = 0x02,
    CHARACTER_FLAG_AUTO_GHOST = 0x04,
};


struct Character
{
    float x, y;
    int grid_x, grid_y;
    int rows, columns;
    float vx,vy; //-- Velocity vector
    int state = QUIET;
    long time_remaining;
    Cell* visited;
    Cell current_cell;
    unsigned int flags;
    GLfloat dir[3];

    Character() : Character(0.0f, 0.0f, 0, 0, 0) { };
    Character(float x, float y, int r, int c, unsigned int flags);

    public:
        void initMovement(float destination_x, float destination_y, float duration);
        void integrate(long t);
        void draw();
        void getAction(Map map, std::vector<std::vector<int>> actions, int depth);
        void getResult();
        void getUtility();
        bool isTerminalState(int depth, Map map);
        int maxValue();
        int minValue();
        double evaluationFunction(Map map);
        std::vector<std::vector<int>> getLegalActions(Map map);
        void setFlag(unsigned int p_flags) { flags = p_flags; };
        void addFlag(unsigned int p_flags) { flags |= p_flags; };
        void removeFlag(unsigned int p_flags) { flags &= ~p_flags; };
        bool hasFlag(unsigned int p_Flag) { return (flags & p_Flag) != 0; };
        void setCell(Cell* cell) {
            visited = cell;
        };
        void setXY(int x, int y){
            this->grid_x = x;
            this->grid_y = y;
        }
};


