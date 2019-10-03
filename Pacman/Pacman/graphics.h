#include "map.h"

struct Graphics
{
    public:
        Graphics() = default;
        //long last_t = 0;
        static void display();
        static void keyboard(unsigned char c, int x, int y);
        static void idle();
        static void movePacman(int x);
        static void moveAutoGhosts();
        static void moveCharacters(int x);
        //static void redrawMap(std::vector<Cell*> cells);
        //static void reboot(int x);
        //static void redrawMap(Cell* pacman, Cell* cell);

};
