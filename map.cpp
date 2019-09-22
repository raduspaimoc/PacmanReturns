#include <iostream>     // std::cout
#include <algorithm>    // std::shuffle
#include <array>        // std::array
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock

#include "map.h"

Map::Map(int r, int c)
{
  for (size_t i = 0; i < r; i++) {

    std::vector<Cell> v;
    for (size_t j = 0; j < c; j++) {
      v.push_back(Cell(i, j));
    }
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
  addMiddle();
}

void Map::addMiddle()
{
    int height_middle = grid.size() / 2;
    int width_middle = grid[0].size() / 2;


    grid[height_middle][width_middle].setWall(false);
    grid[height_middle - 1][width_middle].setWall(false);
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
  int direct[][2] = {{0,1}, {0,-1}, {-1,0}, {1,0}};
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
    int direct[][2] = {{1,0}, {-1,0}, {0,1}, {0,-1}};
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
