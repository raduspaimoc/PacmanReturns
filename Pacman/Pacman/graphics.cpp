#include <GL/glut.h>
#include <iostream>
#include <algorithm>    // std::shuffle
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock
#include "graphics.h"
#include "ShareDefines.h"
#include "Utils.h"

long last_t = 0;


void Graphics::display()
{
    glClearColor(0, 0, 255, 255);
    glClear(GL_COLOR_BUFFER_BIT);

    std::vector<std::vector<Cell> > grid = s_map.grid;

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

    if(!s_map.pacman.set)
        s_map.pacman.set_position((s_map.pacman.x * cell_height), (s_map.pacman.y * cell_width));
    if(!s_map.ghost.set){
        s_map.ghost.set_position((s_map.ghost.x * cell_height), (s_map.ghost.y * cell_width));
        int aux = s_map.auto_ghosts.size();
        for(int z=0; z < s_map.auto_ghosts.size(); z++){
            s_map.auto_ghosts[z].set_position(s_map.auto_ghosts[z].x * cell_height, s_map.auto_ghosts[z].y * cell_width);
        }
    }
    s_map.pacman.draw();
    s_map.ghost.draw();
    for(int z=0; z < s_map.auto_ghosts.size(); z++){
        s_map.auto_ghosts[z].draw();
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
    s_map.pacman.integrate(t-last_t);
    s_map.ghost.integrate(t-last_t);
    for (int i = 0; i < s_map.auto_ghosts.size() ; ++i) {
      s_map.auto_ghosts[i].integrate(t-last_t);
    }
    last_t = t;
  }

  glutPostRedisplay();
}

void Graphics::moveAutoGhosts(){
    for(int i=0; i < s_map.auto_ghosts.size(); i++){
        Character* ghost = &s_map.auto_ghosts[i];
        static std::vector<std::vector<int>> movements = direct;
        shuffle(movements.begin(), movements.end(), std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count()));

        for (auto itr = movements.begin(); itr != movements.end();) {
            int i_offset = (*itr)[0];
            int j_offset = (*itr)[1];
            if ((i_offset + ghost->grid_x < 0) || (j_offset + ghost->grid_y < 0) || (i_offset + ghost->grid_x >= s_map.grid.size()) ||
                (j_offset + ghost->grid_y >= s_map.grid[0].size())) {
                itr++;
                continue;
            }

            Cell *cell = &s_map.grid[(int) ghost->grid_x + i_offset][(int) ghost->grid_y + j_offset];

            if (!cell->hasFlag(CellFlags::CELL_FLAG_WALL)) {

                ghost->grid_x = cell->x;
                ghost->grid_y = cell->y;

                float cell_width = (float) WIDTH / (float) s_columns;
                float cell_height = (float) HEIGHT / (float) s_rows;

                ghost->init_movement(cell->x * cell_height, cell->y * cell_width, 1000);
                //ghost.setCell(cell);

                break;
            }
            itr++;
        }
    }
}

void Graphics::moveCharacters(int x){
    movePacman(x);
    moveAutoGhosts();
    glutTimerFunc(1000, moveCharacters, 0);
}

void Graphics::movePacman(int t){

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
        s_map.pacman.grid_x = cell->x;
        s_map.pacman.grid_y = cell->y;

        float cell_width = (float)WIDTH / (float)s_columns;
        float cell_height = (float)HEIGHT / (float)s_rows;

        s_map.pacman.init_movement(cell->x * cell_height, cell->y * cell_width, 1000);
        s_map.pacman.setCell(cell);

        break;
    }
    itr++;
  }
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
        if (s_map.ghost.state == QUIET){
            if(toupper(c) == Directions::UP && s_map.ghost.grid_x - 1 >= 0  && !s_map.grid[s_map.ghost.grid_x - 1][s_map.ghost.grid_y].isWall()){

                s_map.ghost.grid_x--;
                s_map.ghost.init_movement((s_map.ghost.x) - cell_height, s_map.ghost.y , 1000);

            }
            if(toupper(c) == Directions::DOWN && s_map.ghost.grid_x + 1 < s_map.grid.size() && !s_map.grid[s_map.ghost.grid_x + 1][s_map.ghost.grid_y].isWall()) {

                s_map.ghost.grid_x++;
                s_map.ghost.init_movement((s_map.ghost.x + 1) + cell_height, s_map.ghost.y, 1000);

            }
            if(toupper(c) == Directions::LEFT && s_map.ghost.grid_y - 1 >= 0 && !s_map.grid[s_map.ghost.grid_x][s_map.ghost.grid_y - 1].isWall()) {

                s_map.ghost.grid_y--;
                s_map.ghost.init_movement(s_map.ghost.x, (s_map.ghost.y - 1) - cell_width, 1000);

            }
            if(toupper(c) == Directions::RIGHT && s_map.ghost.grid_y + 1 < s_map.grid[0].size() && !s_map.grid[s_map.ghost.grid_x][s_map.ghost.grid_y + 1].isWall()) {

                s_map.ghost.grid_y++;
                s_map.ghost.init_movement(s_map.ghost.x, (s_map.ghost.y + 1) + cell_width, 1000);

            }
        }

    }
}
