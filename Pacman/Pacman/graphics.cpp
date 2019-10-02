#include <GL/glut.h>
#include <iostream>
#include <algorithm>    // std::shuffle
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock
#include "graphics.h"
#include "ShareDefines.h"
#include "Utils.h"

long last_t = 0;
//int aux = 1;

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

            if (cell->hasFlag(CellFlags::CELL_FLAG_EMPTY))
                glColor3f(0.0, 0.0, 0.0);
            if (cell->hasFlag(CellFlags::CELL_FLAG_FOOD))
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

    // prova
    if(!s_map.pacman.set)
        s_map.pacman.set_position((s_map.pacman.x * cell_height), (s_map.pacman.y * cell_width));
    if(!s_map.ghost.set)
        s_map.ghost.set_position((s_map.ghost.x * cell_height), (s_map.ghost.y * cell_width));
    s_map.pacman.draw();
    s_map.ghost.draw();

    glutSwapBuffers();
}

/*void Graphics::reboot(int x){
  std::vector<Cell*> toRedraw;
  std::vector<std::vector<Cell> > grid = s_map.grid;
  Cell* cell = &grid[(int)s_map.pacman_x][(int)s_map.pacman_y];
  cell->addFlag(CellFlags::CELL_FLAG_PACMAN);
  toRedraw.push_back(cell);
  redrawMap(toRedraw);
  s_map.pacman_x = cell->x;
  s_map.pacman_y = cell->y;
}*/

void Graphics::idle()
{
  long t;

  t=glutGet(GLUT_ELAPSED_TIME);

  if(last_t==0)
    last_t=t;
  else
  {
    s_map.pacman.integrate(t-last_t);
    //s_map.ghost.integrate(t-last_t);
    last_t = t;
  }

  /* A lo mejor falta */
  glutPostRedisplay();
}

void Graphics::movePacman(int t){

  //std::vector<std::vector<Cell>>* grid = &s_map.grid;
  Cell* pacman = &s_map.grid[(int)s_map.pacman.grid_x][(int)s_map.pacman.grid_y];;
  static std::vector<std::vector<int>> movements = direct;
      shuffle(movements.begin(), movements.end(), std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count()));

  for (auto itr = movements.begin(); itr != movements.end();)
  {
    int i_offset = (*itr)[0];
    int j_offset = (*itr)[1];
    if ((i_offset + pacman->x < 0) || (j_offset + pacman->y < 0) || (i_offset + pacman->x >= s_map.grid.size()) || (j_offset + pacman->y >= s_map.grid[pacman->x].size()))
    {
        itr++;
        continue;
    }

    Cell* cell = &s_map.grid[(int)s_map.pacman.grid_x + i_offset][(int)s_map.pacman.grid_y + j_offset];

    if(!cell->hasFlag(CellFlags::CELL_FLAG_WALL))
    {
        printf("Pôs %f - %f\n", cell->x, cell->y);


        //pacman->setFlag(CellFlags::CELL_FLAG_EMPTY);
        //pacman->removeFlag(CellFlags::CELL_FLAG_PACMAN);


        //cell->setFlag(CellFlags::CELL_FLAG_PACMAN);

        /*pacman->removeFlag(CellFlags::CELL_FLAG_PACMAN);
        pacman->addFlag(CellFlags::CELL_FLAG_EMPTY);
        cell->addFlag(CellFlags::CELL_FLAG_PACMAN);*/





        //printf("Pacman x: %f  y: %f --- CEll to move x: %f y: %f", s_map.pacman_x, s_map.pacman_y, cell->x, cell->y);
        //CELL_FLAG_EMPTY
        s_map.pacman.grid_x = cell->x;
        s_map.pacman.grid_y = cell->y;

        float cell_width = (float)WIDTH / (float)s_columns;
        float cell_height = (float)HEIGHT / (float)s_rows;
        //s_map.pacman.init_movement(cell->x * cell_height + 50, cell->y * cell_width + 50, 1500);
        //grid[(int)cell->x][(int)cell->y] = cell;


        //s_map.ghost.grid_x--;
        //s_map.ghost.init_movement((s_map.ghost.x - 1) * cell_height, s_map.ghost.y * cell_width, 1000);

        s_map.pacman.init_movement(cell->x * cell_height, cell->y * cell_width, 1000);
        s_map.pacman.setCell(cell);

        break;
    }
    itr++;
  }
  glutTimerFunc(1000, movePacman, 0);
}

void Graphics::keyboard(unsigned char c, int x, int y)
{
    float cell_width = (float)WIDTH / (float)s_columns;
    float cell_height = (float)HEIGHT / (float)s_rows;
    if (c == ' ')
    {
        s_map.reset();
        s_map.setWalls();

        glutPostRedisplay();
    }
    else
    {
        //printf("C:: %c \n", c);
        if(toupper(c) == UP && !s_map.grid[s_map.ghost.grid_x - 1][s_map.ghost.grid_y].isWall()){
            //printf("up \n");
            s_map.ghost.grid_x--;
            s_map.ghost.init_movement((s_map.ghost.x - 1) * cell_height, s_map.ghost.y * cell_width, 1000);

        }
        if(toupper(c) == DOWN && !s_map.grid[s_map.ghost.grid_x + 1][s_map.ghost.grid_y].isWall()) {
            //printf("down \n");
            s_map.ghost.grid_x++;
            s_map.ghost.init_movement((s_map.ghost.x + 1) * cell_height, s_map.ghost.y * cell_width, 1000);
        }
        if(toupper(c) == LEFT && !s_map.grid[s_map.ghost.grid_x][s_map.ghost.grid_y - 1].isWall()) {
            //printf("left \n");
            s_map.ghost.grid_y--;
            s_map.ghost.init_movement(s_map.ghost.x  * cell_height, (s_map.ghost.y - 1) * cell_width, 1000);
        }
        if(toupper(c) == RIGHT && !s_map.grid[s_map.ghost.grid_x][s_map.ghost.grid_y + 1].isWall()) {
            //printf("right \n");
            s_map.ghost.grid_x++;
            s_map.ghost.init_movement(s_map.ghost.x  * cell_height, (s_map.ghost.y + 1) * cell_width, 1000);
        }
      //movePacman(0);
    }
}
