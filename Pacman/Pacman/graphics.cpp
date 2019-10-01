#include <GL/glut.h>
#include <iostream>
#include <algorithm>    // std::shuffle
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock
#include "graphics.h"
#include "ShareDefines.h"
#include "Utils.h"

long last_t = 0;
int aux = 1;

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

void Graphics::reboot(int x){
  std::vector<Cell*> toRedraw;
  std::vector<std::vector<Cell> > grid = s_map.grid;
  Cell* cell = &grid[(int)s_map.pacman_x][(int)s_map.pacman_y + aux];
  cell->addFlag(CellFlags::CELL_FLAG_PACMAN);
  toRedraw.push_back(cell);
  redrawMap(toRedraw);
  s_map.pacman_x = cell->x;
  s_map.pacman_y = cell->y;
}

void Graphics::idle()
{
  long t;

  t=glutGet(GLUT_ELAPSED_TIME);

  if(last_t==0)
    last_t=t;
  else
  {
    printf("T: %ld, LAST_t: %ld \n", t, last_t);
    //if(aux == 1){
        if(t - last_t > 2000){
            printf("Entro en el deep web\n" );
            movePacman();
            last_t=t;
        }
        //aux++;
    //}

  }


  glutPostRedisplay();
}

void Graphics::redrawMap(  std::vector<Cell*> cells){

  float cell_width = (float)WIDTH / (float)s_columns;
  float cell_height = (float)HEIGHT / (float)s_rows;
  float cell_width4 = cell_width / 4;
  float cell_height4 = cell_height / 4;

  for(int i=0; i < cells.size(); i++){
    Cell* cell = cells[i];
    //int real_i = s_rows - i - 1;
    int fake_i = s_rows - cell->x - 1;

    if(cell->hasFlag(CellFlags::CELL_FLAG_PACMAN)){
        glBegin(GL_QUADS);

        glVertex2i(cell->y * cell_width + MARGIN, fake_i * cell_height + MARGIN);
        glVertex2i((cell->y + 1) * cell_width + MARGIN, fake_i * cell_height + MARGIN);

        glVertex2i((cell->y + 1) * cell_width + MARGIN, (fake_i + 1) * cell_height + MARGIN);
        glVertex2i(cell->y * cell_width + MARGIN, (fake_i + 1) * cell_height + MARGIN);

        glEnd();

        glColor3f(1.0, 0.5, 0.0);

        glBegin(GL_QUADS);

        glVertex2i(cell->y * cell_width + MARGIN + cell_width4, fake_i * cell_height + MARGIN + cell_height4);
        glVertex2i(cell->y * cell_width + MARGIN + ( 2 * cell_width4), fake_i * cell_height + MARGIN + cell_height4);

        glVertex2i(cell->y * cell_width + MARGIN + ( 2 * cell_width4), fake_i * cell_height + MARGIN + (2 * cell_height4));
        glVertex2i(cell->y * cell_width + MARGIN + cell_width4, fake_i * cell_height + MARGIN + (2 * cell_height4));

        glEnd();

    } else {
        glColor3f(0.0, 0.0, 0.0);
        glBegin(GL_QUADS);

        glVertex2i(cell->y * cell_width + MARGIN, fake_i * cell_height + MARGIN);
        glVertex2i((cell->y + 1) * cell_width + MARGIN, fake_i * cell_height + MARGIN);

        glVertex2i((cell->y + 1) * cell_width + MARGIN, (fake_i + 1) * cell_height + MARGIN);
        glVertex2i(cell->y * cell_width + MARGIN, (fake_i + 1) * cell_height + MARGIN);

        glEnd();

        glColor3f(0.0, 0.0, 0.0);

        glBegin(GL_QUADS);

        glVertex2i(cell->y * cell_width + MARGIN + cell_width4, fake_i * cell_height + MARGIN + cell_height4);
        glVertex2i(cell->y * cell_width + MARGIN + ( 2 * cell_width4), fake_i * cell_height + MARGIN + cell_height4);

        glVertex2i(cell->y * cell_width + MARGIN + ( 2 * cell_width4), fake_i * cell_height + MARGIN + (2 * cell_height4));
        glVertex2i(cell->y * cell_width + MARGIN + cell_width4, fake_i * cell_height + MARGIN + (2 * cell_height4));

        glEnd();
    }
    cell->removeFlag(MovementFlags::MOVEMENT_FLAG_MOVING);
  }
    glutSwapBuffers();
}

void Graphics::movePacman(){

  std::vector<std::vector<Cell> > grid = s_map.grid;
  Cell* pacman = &grid[(int)s_map.pacman_x][(int)s_map.pacman_y];;
  static std::vector<std::vector<int>> movements = direct;
      shuffle(movements.begin(), movements.end(), std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count()));

  for (auto itr = movements.begin(); itr != movements.end();)
  {
    int i_offset = (*itr)[0];
    int j_offset = (*itr)[1];
    if ((i_offset + pacman->x < 0) || (j_offset + pacman->y < 0) || (i_offset + pacman->x >= grid.size()) || (j_offset + pacman->y >= grid[pacman->x].size()))
    {
        itr++;
        continue;
    }

    Cell * cell = &grid[(int)s_map.pacman_x + i_offset][(int)s_map.pacman_y + j_offset];

    if(!cell->hasFlag(CellFlags::CELL_FLAG_WALL) && !cell->hasFlag(MovementFlags::MOVEMENT_FLAG_MOVING) && !pacman->hasFlag(MovementFlags::MOVEMENT_FLAG_MOVING))
    {
        printf("Pôs %f - %f\n", cell->x, cell->y);

        pacman->addFlag(CellFlags::CELL_FLAG_EMPTY);
        pacman->addFlag(MovementFlags::MOVEMENT_FLAG_MOVING);
        pacman->removeFlag(CellFlags::CELL_FLAG_PACMAN);

        cell->addFlag(CellFlags::CELL_FLAG_PACMAN);
        cell->addFlag(MovementFlags::MOVEMENT_FLAG_MOVING);
        if(cell->hasFlag(CellFlags::CELL_FLAG_FOOD))
          cell->removeFlag(CellFlags::CELL_FLAG_FOOD);


        //CELL_FLAG_EMPTY
        s_map.pacman_x = cell->x;
        s_map.pacman_y = cell->y;

        std::vector<Cell*> toRedraw;
        toRedraw.push_back(pacman);
        toRedraw.push_back(cell);
        redrawMap(toRedraw);


        //pacman = cell;
        //glutPostRedisplay();
        //redrawMap(pacman, cell);
        /*CELL_FLAG_WALL = 0x01,
        CELL_FLAG_FOOD = 0x02,
        CELL_FLAG_DELETED = 0x04,
        CELL_FLAG_ADDED = 0x08,
        CELL_FLAG_VISITED = 0x10,
        CELL_FLAG_PACMAN = 0x20,
        CELL_FLAG_GHOST = 0x40*/
        printf("Sale del ghetto\n" );
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
      movePacman();
      //glutPostRedisplay();
      //aux++;
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
