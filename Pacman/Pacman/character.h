#include "cell.h"

#define MOVE 1
#define QUIET 2

struct Character {
    float x, y;
    int r, c;
    float vx,vy; //-- Velocity vector
    int state = QUIET;
    long time_remaining;
    long last_t  = 0;
    bool set = false;
    Cell* visited;

    Character();
    Character(float x, float y, int r, int c);

    public:
        void set_position(float x, float y);
        void init_movement(float destination_x, float destination_y, float duration);
        void integrate(long t);
        void draw();
        void emptyCell();

        void setCell(Cell* cell){
            visited = cell;
        }
};


