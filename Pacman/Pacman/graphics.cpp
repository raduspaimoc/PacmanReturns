#include <GL/glut.h>

#include "graphics.h"
#include "ShareDefines.h"

void Graphics::display()
{
    glClearColor(0, 0, 255, 255);
    glClear(GL_COLOR_BUFFER_BIT);

    std::vector<std::vector<Cell> > grid = s_map.grid;

    s_map.showInfo();

    float cell_width = (float)WIDTH / (float)s_columns;
    float cell_height = (float)HEIGHT / (float)s_rows;


    for (int i = 0; i < s_rows; i++)
    {
        for (int j = 0; j < s_columns; j++)
        {
            int real_i = s_rows - i - 1;

            Cell* cell = &grid[real_i][j];
            if (cell->isWall() && !cell->isAdded())
                continue;

                glColor3f(0.0, 0.0, 0.0);


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
