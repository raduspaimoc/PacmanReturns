#include "map.h"

struct Graphics
{
    public:
        Graphics() = default;
        static void display();
        static void keyboard(unsigned char c, int x, int y);
        static void idle();
};
