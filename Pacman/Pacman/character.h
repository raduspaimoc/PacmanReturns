#include "cell.h"

#define MOVE 1
#define QUIET 2

enum CharacterFlags
{
    CHARACTER_FLAG_PACMAN = 0x01,
    CHARACTER_FLAG_GHOST = 0x02,
    /*CELL_FLAG_DELETED = 0x04,
    CELL_FLAG_ADDED = 0x08,
    CELL_FLAG_VISITED = 0x10,
    CELL_FLAG_PACMAN = 0x20,
    CELL_FLAG_GHOST = 0x40,
    CELL_FLAG_EMPTY = 0x80*/
};

struct Character {
    float x, y;
    int grid_x, grid_y;
    int r, c;
    float vx,vy; //-- Velocity vector
    int state = QUIET;
    long time_remaining;
    long last_t  = 0;
    bool set = false;
    Cell* visited;
    unsigned int flags;

    Character();
    Character(float x, float y, int r, int c);

    public:
        void set_position(float x, float y);
        void init_movement(float destination_x, float destination_y, float duration);
        void integrate(long t);
        void draw();

        void setFlag(unsigned int p_flags) { flags = p_flags; };
        void addFlag(unsigned int p_flags) { flags |= p_flags; };
        void removeFlag(unsigned int p_flags) { flags &= ~p_flags; };
        bool hasFlag(unsigned int p_Flag) { return (flags & p_Flag) != 0; };


        //void emptyCell();

        void setCell(Cell* cell){
            visited = cell;
        }

        void setGridXY(int x, int y){
            grid_x = x;
            grid_y = y;
        }
};


