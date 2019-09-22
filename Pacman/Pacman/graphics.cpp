#include <GL/glut.h>

#include "graphics.h"
#include "ShareDefines.h"

void Graphics::display()
{
    glClearColor(0.0, 0.0, 0.8, 0.8);
    glClear(GL_COLOR_BUFFER_BIT);

    static std::vector<std::vector<Cell> > grid = static_cast<std::vector<std::vector<Cell> >>(s_map.grid);

    s_map.showInfo();

    for (int i = 0; i < s_columns; i++)
        for (int j = 0; j < s_rows; j++)
        {
          if (i == 0 || i == s_columns - 1 || j == 0 || j == s_rows - 1)
            continue;
          if (grid[i][j].isWall())
            continue;

          glColor3f(0.8, 0.8, 0.8);
          glBegin(GL_QUADS);
          glVertex2i(i * WIDTH / grid.size(), j * HEIGHT / grid.size());
          glVertex2i((i + 1) * WIDTH / grid[0].size(), j * HEIGHT / grid.size());
          glVertex2i((i + 1) * WIDTH / grid[0].size(), (j + 1) * HEIGHT / grid.size());
          glVertex2i(i * WIDTH / grid[0].size(), (j + 1) * HEIGHT / grid.size());
          glEnd();
        }

    glutSwapBuffers();

}
