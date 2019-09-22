#include "map.h"

struct Graphics
{
    public:
        Graphics() { };
        static void display();
        static void keyboard(unsigned char c, int x, int y);
};
