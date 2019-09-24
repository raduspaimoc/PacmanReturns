#include <GL/glut.h>

#include "graphics.h"
#include "ShareDefines.h"

void Graphics::display()
{
    glClearColor(0.0, 0.0, 0.8, 0.8);
    glClear(GL_COLOR_BUFFER_BIT);

    std::vector<std::vector<Cell> > grid = s_map.grid;

    s_map.showInfo();

    int realColumns = s_columns;
    int realRows = s_rows + 1;
    int cell_width = WIDTH / realColumns;
    int cell_height = HEIGHT / realRows;

    for (int i = 0; i < s_rows; i++)
    {
        for (int j = 0; j < s_columns; j++)
        {
            int real_i = s_rows - i - 1;

            /*if (i == 0 || i == s_rows - 1 || j == 0 || j == s_columns - 1)
                continue;*/

            if (grid[real_i][j].isWall())
                continue;

            if(grid[real_i][j].isDeleted())
                glColor3f(0.5, 0.5, 0.5);
            else if (grid[real_i][j].isAdded())
                glColor3f(0.0, 1.0, 0.5);
            else
                glColor3f(0.8, 0.8, 0.8);
            glBegin(GL_QUADS);

            int canvas_x = j + 1;
            int canvas_y = i + 1;

            glVertex2i(canvas_x * cell_width, canvas_y * cell_height);
            glVertex2i((canvas_x + 1) * cell_width, canvas_y * cell_height);

            glVertex2i((canvas_x + 1) * cell_width, (canvas_y + 1) * cell_height);
            glVertex2i(canvas_x * cell_width, (canvas_y + 1) * cell_height);

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
