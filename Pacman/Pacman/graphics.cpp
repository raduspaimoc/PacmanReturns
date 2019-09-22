#include <GL/glut.h>

#include "graphics.h"
#include "ShareDefines.h"

void Graphics::display()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    //static std::vector<std::vector<Cell> > grid = static_cast<std::vector<std::vector<Cell> >>(s_map.grid);

    s_map.showInfo();

    for (int i = 0; i < s_columns; i++)
        for (int j = 0; j < s_rows; j++)
        {
           // if (grid[i][j].isWall())
             //   continue;

            glColor3f(0.8, 0.8, 0.8);
            glBegin(GL_QUADS);
            /*
            glVertex2i(i * WIDTH / grid.size(), j * HEIGHT / grid[0].size());
            glVertex2i((i + 1) * WIDTH / grid.size(), j * HEIGHT / grid[0].size());
            glVertex2i((i + 1) * WIDTH / grid.size(), (j + 1) * HEIGHT / grid[0].size());
            glVertex2i(i * WIDTH / grid.size(), (j + 1) * HEIGHT / grid[0].size());
            */
            glEnd();
        }

    glutSwapBuffers();

}