#include "cell.h"

#define MOVE 1
#define QUIET 2

enum CharacterFlags
{
    CHARACTER_FLAG_PACMAN = 0x01,
    CHARACTER_FLAG_GHOST = 0x02,
    CHARACTER_FLAG_AUTO_GHOST = 0x04,
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

        void setCell(Cell* cell){
            visited = cell;
        }

        void setGridXY(int x, int y){
            grid_x = x;
            grid_y = y;
        }
};


