#include "map.h"

struct Graphics
{
    public:
        Graphics() = default;
        static void display();
        static void keyboard(unsigned char c, int x, int y);
        static void idle();
        static void movePacman(int x);
        static void moveAutoGhosts();
        static void moveCharacters(int x);
        static void PositionObserver(float alpha,float beta,int radi);
        static void GhostMovement(unsigned char c);
        static void readJPEG(char *filename,unsigned char **image,int *width, int *height);
        static void LoadTexture(char *filename,int dim);
};
