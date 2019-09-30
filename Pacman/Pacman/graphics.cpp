#include <GL/glut.h>
#include <iostream>

#include "graphics.h"
#include "ShareDefines.h"
#include "Utils.h"

long last_t = 0;

void Graphics::display()
{
    glClearColor(0, 0, 255, 255);
    glClear(GL_COLOR_BUFFER_BIT);

    std::vector<std::vector<Cell> > grid = s_map.grid;

    s_map.showInfo();

    float cell_width = (float)WIDTH / (float)s_columns;
    float cell_height = (float)HEIGHT / (float)s_rows;
    float cell_width4 = cell_width / 4;
    float cell_height4 = cell_height / 4;


    for (int i = 0; i < s_rows; i++)
    {
        for (int j = 0; j < s_columns; j++)
        {
            int real_i = s_rows - i - 1;

            Cell* cell = &grid[real_i][j];
            if (cell->isWall())
                continue;

            glColor3f(0.0, 0.0, 0.0);

            glBegin(GL_QUADS);

            glVertex2i(j * cell_width + MARGIN, i * cell_height + MARGIN);
            glVertex2i((j + 1) * cell_width + MARGIN, i * cell_height + MARGIN);

            glVertex2i((j + 1) * cell_width + MARGIN, (i + 1) * cell_height + MARGIN);
            glVertex2i(j * cell_width + MARGIN, (i + 1) * cell_height + MARGIN);

            glEnd();

            if (cell->hasFlag(CellFlags::CELL_FLAG_PACMAN))
                glColor3f(1.0, 0.5, 0.0);
            else if (cell->hasFlag(CellFlags::CELL_FLAG_FOOD))
                glColor3f(0.0, 1.0, 1.0);
            else
                continue;

            glBegin(GL_QUADS);

            glVertex2i(j * cell_width + MARGIN + cell_width4, i * cell_height + MARGIN + cell_height4);
            glVertex2i(j * cell_width + MARGIN + ( 2 * cell_width4), i * cell_height + MARGIN + cell_height4);

            glVertex2i(j * cell_width + MARGIN + ( 2 * cell_width4), i * cell_height + MARGIN + (2 * cell_height4));
            glVertex2i(j * cell_width + MARGIN + cell_width4, i* cell_height + MARGIN + (2 * cell_height4));

            glEnd();
        }
    }

    glutSwapBuffers();
}

void Graphics::idle()
{
  long t;

  t=glutGet(GLUT_ELAPSED_TIME);

  if(last_t==0)
    last_t=t;
  else
  {
    printf("Entro en el deep web\n" );
    movePacman();
    last_t=t;
  }


  glutPostRedisplay();
}

void Graphics::redrawMap(  std::vector<Cell*> cells){

  /*float cell_width = (float)WIDTH / (float)s_columns;
  float cell_height = (float)HEIGHT / (float)s_rows;
  float cell_width4 = cell_width / 4;
  float cell_height4 = cell_height / 4;

  for(int i=0; i < cells.size(); i++){
    Cell* cell = cells[i];
    if (cell->isWall())
        continue;

        glColor3f(0.0, 0.0, 0.0);

        glBegin(GL_QUADS);

        glVertex2i(cell->y * cell_width + MARGIN, cell->x * cell_height + MARGIN);
        glVertex2i((cell->y + 1) * cell_width + MARGIN, cell->x * cell_height + MARGIN);

        glVertex2i((cell->y + 1) * cell_width + MARGIN, (cell->x + 1) * cell_height + MARGIN);
        glVertex2i(cell->y * cell_width + MARGIN, (cell->x + 1) * cell_height + MARGIN);

        glEnd();

        if (cell->hasFlag(CellFlags::CELL_FLAG_PACMAN))
            glColor3f(1.0, 0.5, 0.0);
        else if (cell->hasFlag(CellFlags::CELL_FLAG_FOOD))
            glColor3f(0.0, 1.0, 1.0);
        else
            continue;

        glBegin(GL_QUADS);

        glVertex2i(cell->y * cell_width + MARGIN + cell_width4, cell->x * cell_height + MARGIN + cell_height4);
        glVertex2i(cell->y * cell_width + MARGIN + ( 2 * cell_width4), cell->x * cell_height + MARGIN + cell_height4);

        glVertex2i(cell->y * cell_width + MARGIN + ( 2 * cell_width4), cell->x * cell_height + MARGIN + (2 * cell_height4));
        glVertex2i(cell->y * cell_width + MARGIN + cell_width4, cell->x* cell_height + MARGIN + (2 * cell_height4));

        glEnd();
  }*/
}

void Graphics::movePacman(){

  std::vector<std::vector<Cell> > grid = s_map.grid;
  Cell* pacman = &grid[s_map.pacman_x][s_map.pacman_y];
  static std::vector<std::vector<int>> movements = direct;
  Utils::RandomResize(movements, 1);

  for (auto itr = movements.begin(); itr != movements.end();)
  {
    int i_offset = (*itr)[0];
    int j_offset = (*itr)[1];
    Cell * cell = &grid[pacman->x + i_offset][pacman->y + j_offset];
    if(!cell->hasFlag(CellFlags::CELL_FLAG_WALL))
    {
        if(cell->hasFlag(CellFlags::CELL_FLAG_FOOD))
          pacman->removeFlag(CellFlags::CELL_FLAG_FOOD);
        pacman->removeFlag(CellFlags::CELL_FLAG_PACMAN);
        cell->addFlag(CellFlags::CELL_FLAG_PACMAN);

        std::vector<Cell*> toRedraw;
        toRedraw.push_back(pacman);
        toRedraw.push_back(cell);
        redrawMap(toRedraw);
        //glutPostRedisplay();
        //redrawMap(pacman, cell);
        /*CELL_FLAG_WALL = 0x01,
        CELL_FLAG_FOOD = 0x02,
        CELL_FLAG_DELETED = 0x04,
        CELL_FLAG_ADDED = 0x08,
        CELL_FLAG_VISITED = 0x10,
        CELL_FLAG_PACMAN = 0x20,
        CELL_FLAG_GHOST = 0x40*/

        break;
    }
    itr++;
  }
}

void Graphics::keyboard(unsigned char c, int x, int y)
{
    if (c == ' ')
    {
        s_map.reset();
        s_map.setWalls();

        glutPostRedisplay();
    }
    else
    {
      // direct = { {0, 1}, {0, -1}, {-1, 0}, {1, 0} };
      //movePacman();
      //glutPostRedisplay();


      /*for (int i = 0; i < )
      {
        int i_offset = (*itr)[0];
        int j_offset = (*itr)[1];
        Cell* cell = &grid[s_map.pacman.x + i_offset][s_map.pacman.y + j_offset];
        if(cell->hasFlag(CellFlags::CELL_FLAG_FOOD))
        {
            movements.push_back(cell);
        }
        itr++;
      }*/
    }
}
