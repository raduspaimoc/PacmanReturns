#include <GL/glut.h>

#include "graphics.h"
#include "ShareDefines.h"

void Graphics::display()
{
    glClearColor(0.0, 0.0, 0.8, 0.8);
    glClear(GL_COLOR_BUFFER_BIT);

    std::vector<std::vector<Cell> > grid = s_map.grid;

    s_map.showInfo();

    int cell_width = WIDTH / s_columns;
    int cell_height = HEIGHT / s_rows;

    for (int i = 0; i < s_rows; i++)
    {
        for (int j = 0; j < s_columns; j++)
        {
            int real_i = s_rows - i - 1;

            /*if (i == 0 || i == s_rows - 1 || j == 0 || j == s_columns - 1)
                continue;*/

            if (grid[real_i][j].isWall() && !grid[real_i][j].isAdded())
                continue;

            if(grid[real_i][j].isDeleted())
                glColor3f(0.5, 0.5, 0.5);
            else if (grid[real_i][j].isAdded())
                glColor3f(0.0, 1.0, 0.5);
            else
                glColor3f(0.8, 0.8, 0.8);

            glBegin(GL_QUADS);

            glVertex2i(j * cell_width + MARGIN, i * cell_height + MARGIN);
            glVertex2i((j + 1) * cell_width + MARGIN, i * cell_height + MARGIN);

            glVertex2i((j + 1) * cell_width + MARGIN, (i + 1) * cell_height + MARGIN);
            glVertex2i(j * cell_width + MARGIN, (i + 1) * cell_height + MARGIN);

            glEnd();
        }
    }

    glutSwapBuffers();
}

void Graphics::keyboard(unsigned char c, int x, int y)
{
    s_map.reset();
    s_map.setWalls();

    glutPostRedisplay();
}
