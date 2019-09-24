#include <iostream>     // std::cout
#include <algorithm>    // std::shuffle
#include <array>        // std::array
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock
#include <GL/glut.h>

#define WIDTH 300
#define HEIGHT 300

#include "map.h"
#include "ShareDefines.h"
#include "Utils.h"

Map::Map(int r, int c) : rows(r), columns(c)
{
  for (size_t i = 0; i < r; i++) {

    std::vector<Cell> v;
    for (size_t j = 0; j < c; j++)
      v.push_back(Cell(i, j));

    grid.push_back(v);
  }
};

void Map::showInfo()
{
  printf("Rows: %zu Columns: %zu \n", grid.size(), grid[0].size());
  printf("Map: \n");

  for (auto row : grid)
  {
      for (auto column : row)
          std::cout << column;

      std::cout << std::endl;
  }

  printf("--------------------- \n");
}

void Map::setWalls()
{
    setWallsRec(0, 0);
    addWalls();
    createVerticalSymetry();
    addMiddle();
    removeTrees();
}

void Map::addWalls(){
    for (size_t i = 0; i < grid.size(); i++)
    {
        for (size_t j = 0; j < grid[0].size(); j++)
        {
            if ((j + 1 == grid[0].size() - 1) || (j + 2 >= grid[0].size()) || (j + 3 >= grid[0].size()))
                continue;

            if (grid[i][j].isWall() && grid[i][j + 3].isWall())
                grid[i][j + 2].setWall(true);

            if (grid[i][j].isWall() && grid[i][j + 2].isWall())
                grid[i][j + 1].setWall(true);
        }
    }
}

void Map::removeTrees()
{
    showInfo();
    for (int i = 0; i < grid.size(); i++)
    {
        for (int j = 0; j < grid[i].size(); j++)
        {
            if (isMiddle(i, j))
                continue;

            int walls = 0;
            std::vector<std::vector<int>> directc = direct;

            for (auto itr = directc.begin(); itr != directc.end();)
            {
                int i_offset = (*itr)[0];
                int j_offset = (*itr)[1];

                if ((i_offset == -1 && i == 0) ||
                    (j_offset == -1 && j == 0) ||
                    (i_offset == 1 && i == grid.size() -1 ) ||
                    (j_offset == 1 && j == grid[i].size() - 1) ||
                    isMiddle(i + i_offset, j + j_offset))
                {
                    itr = directc.erase(itr);
                    walls++;
                    continue;
                }

                if (grid[i + i_offset][j + j_offset].isWall())
                {
                    ++itr;
                    walls++;
                }
                else
                    itr = directc.erase(itr);
            }

            /// Check if the cell is alone WIP
            /*bool alone = true;
            for (auto const offset : direct_all)
            {
                int i_offset = offset[0];
                int j_offset = offset[1];

                if ((i_offset == -1 && i == 0) ||
                    (j_offset == -1 && j == 0) ||
                    (i_offset == 1 && i == grid.size() - 1) ||
                    (j_offset == 1 && j == grid[i].size() - 1))
                {
                    //printf("i: %d, j: %d, i_: %d, j_: %d, isWall:Border %d\n", i, j, i+i_offset, j_offset);
                    alone = false;
                    break;
                }
                if (grid[i + i_offset][j + j_offset].isWall())
                {
                    //printf("i: %d, j: %d, i_: %d, j_: %d, isWall: %d\n", i, j, i+i_offset, j_offset, grid[i + i_offset][j + j_offset].isWall());
                    alone = false;
                    break;
                }
            }*/

            /*if (alone)
            {
                printf("Entro en el alone, pos i: %d, j: %d\n", i, j);
                grid[i][j].setWall(true);
                grid[i][j].SetAdded(true);
                //a ver si funka
                //walls++;
            }*/

            if (walls <= 2)
                continue;

            Utils::RandomResize(directc, walls - 2);
            int sizea = directc.size() - (walls - 2);
            for (auto const& elem : directc)
            {
                int i_offset = elem[0];
                int j_offset = elem[1];
                int aux = grid[0].size();

                Cell& toDelete = grid[i + i_offset][j + j_offset];
                Cell* pair = getPairCell(toDelete);

                toDelete.setWall(false);
                pair->setWall(false);
            }
        }
    }
}

void Map::createVerticalSymetry() {
    int aux = grid[0].size();
    for (int i = 0; i < grid.size(); i++)
    {
        if (s_columns % 2 != 0)
            grid[i].push_back(Cell(i, aux, false));

        for (int j = aux - 1; j >= 0; j--)
        {
            int new_x = i;
            int new_y = aux + std::abs(j - aux) - 1;

            if (s_columns % 2 != 0)
                new_x++;

            grid[i].push_back(Cell(new_x, new_y, grid[i][j].isWall()));
        }
    }
}

Cell* Map::getPairCell(Cell cell)
{
    return &grid[cell.x][s_columns - cell.y - 1];
}

bool Map::isMiddle(int i, int j){
  int height_middle = grid.size() / 2;
  int width_middle = grid[0].size() / 2;

  if((i == height_middle && j == width_middle) ||
     (i == height_middle - 1 && j == width_middle) ||
     (i == height_middle - 2 && j == width_middle) ||
     (i == height_middle && j == width_middle + 1) ||
     (i == height_middle && j == width_middle - 1) ||
     (i == height_middle && j == width_middle - 2) ||
     (i == height_middle && j == width_middle + 2) ||
     (i == height_middle - 1 && j == width_middle - 1) ||
     (i == height_middle - 1 && j == width_middle - 2) ||
     (i == height_middle - 1 && j == width_middle + 1) ||
     (i == height_middle - 1 && j == width_middle + 2) ||
     (i == height_middle + 1 && j == width_middle - 1) ||
     (i == height_middle + 1 && j == width_middle - 2) ||
     (i == height_middle + 1 && j == width_middle) ||
     (i == height_middle + 1 && j == width_middle + 1) ||
     (i == height_middle + 1 && j == width_middle + 2))
     {
       return true;
     }
  return false;
}

void Map::addMiddle()
{
    int height_middle = grid.size() / 2;
    int width_middle = grid[0].size() / 2;


    grid[height_middle][width_middle].setWall(false);
    grid[height_middle - 1][width_middle].setWall(false);
    grid[height_middle - 2][width_middle].setWall(false);
    grid[height_middle][width_middle - 1].setWall(false);
    grid[height_middle][width_middle + 1].setWall(false);

    grid[height_middle - 1][width_middle - 1].setWall(true);
    grid[height_middle - 1][width_middle - 2].setWall(true);
    grid[height_middle - 1][width_middle + 1].setWall(true);
    grid[height_middle - 1][width_middle + 2].setWall(true);

    //map[height_middle][width_middle] = 'X';
    grid[height_middle][width_middle - 2].setWall(true);
    grid[height_middle][width_middle + 2].setWall(true);

    grid[height_middle + 1][width_middle - 1].setWall(true);
    grid[height_middle + 1][width_middle - 2].setWall(true);
    grid[height_middle + 1][width_middle].setWall(true);
    grid[height_middle + 1][width_middle + 1].setWall(true);
    grid[height_middle + 1][width_middle + 2].setWall(true);
}

void Map::setWallsRec(int i, int j)
{
  std::vector<int> visitOrder = {0, 1, 2, 3};
  //out of boundary
  if (i < 0 || j < 0 || i >= grid.size() || j >= grid[0].size())
    return;

  //visited, go back the the coming direction, return
  if (!grid[i][j].isWall())
    return;

  //some neightbors are visited in addition to the coming direction, return
  //this is fill some circles in maze
  if (countVisitedNeighbor(i, j) > 2)
      return;

  grid[i][j].setWall(false);

  //shuffle the visitOrder

  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  shuffle (visitOrder.begin(), visitOrder.end(), std::default_random_engine(seed));

//  shuffle(visitOrder, 4);

  for (int k = 0; k < 4; ++k)
  {
      int ni = i + direct[visitOrder[k]][0];
      int nj = j + direct[visitOrder[k]][1];
      setWallsRec(ni, nj);
  }
}

int Map::countVisitedNeighbor(int i, int j){
    int count = 0;

    for (int k = 0; k < 4; ++k)
    {
        int ni = i + direct[k][0];
        int nj = j + direct[k][1];
        //out of boundary
        if (ni < 0 || nj < 0 || ni >= grid.size() || nj >= grid[0].size())
          continue;

        if (!grid[ni][nj].isWall())
          count++;
    }

    return count;
}

void Map::reset()
{
    for (auto& row : grid)
    {
        row.resize(s_columns/2);
        for (auto& cell : row)
            cell.setWall(true);
    }
}

std::ostream& operator<<(std::ostream& os, const Map& map)
{
    for (auto row : map.grid)
    {
      for (auto column : row)
        os << column;

      os << '\n';
    }

    return os;
}
