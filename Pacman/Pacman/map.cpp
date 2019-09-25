#include <iostream>     // std::cout
#include <algorithm>    // std::shuffle
#include <array>        // std::array
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock
#include <GL/glut.h>

#include "map.h"
#include "ShareDefines.h"
#include "Utils.h"

Map::Map(int r, int c) : rows(r), columns(c)
{
  for (size_t i = 0; i < r; i++) {

    std::vector<Cell> v;
    for (size_t j = 0; j < c; j++)
    {
        bool wall = true;
        if (i % 2 == 0 && j % 2 == 0)
            wall = false;
        if (i % 2 == 1 && j % 2 ==1)
            wall = false;

        v.emplace_back(i, j, wall);
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
    DFS(0, 0);
    createVerticalSymetry();
    //removeTrees();
    addMiddle();
    // setWallsRec(0, 0);
  /*
   * addWalls();
    createVerticalSymetry();
    addMiddle();
    removeTrees();
    addAloneWalls();
    float percent = countWalls();
    bool addMore = percent < 0.35 ? true : false;
    printf("----| %f |-----\n", percent);
    while(addMore){
      addWalls();
      addMiddle();
      finalVerticalSymetryCheck();
      removeTrees();
      addAloneWalls();
      float new_percent = countWalls();
      printf("----| %f |----- es o no es: %d\n", new_percent, (int)new_percent == (int)percent);
      if((int) new_percent == (int)percent)
        addMore = false;
    }
    //addAloneWalls();
    //countAll();
    */
}

float Map::countWalls(){
  int walls = 0;
  int empty = 0;
  for (size_t i = 0; i < grid.size(); i++)
  {
      for (size_t j = 0; j < grid[0].size(); j++)
      {
        if(grid[i][j].isWall()){
          walls++;
        } else {
          empty++;
        }
      }
  }
  return (float)walls/(walls+empty);
  //rintf("Total: %d, Walls: %d, Percent: %f, Empty: %d, Percent: %f, Difference: %d\n", walls+empty, walls, (float)walls/(walls+empty), empty,(float)empty/(walls+empty), walls-empty);
}

void Map::addWalls(){
    for (size_t i = 0; i < grid.size(); i++)
    {
        for (size_t j = 0; j < grid[0].size(); j++)
        {
            if (isMiddle(i, j))
                continue;

            if ((j + 1 == grid[0].size() - 1) || (j + 2 >= grid[0].size()) || (j + 3 >= grid[0].size()))
                continue;

            if (grid[i][j].isWall() && grid[i][j + 3].isWall())
                grid[i][j + 2].setWall(true);

            if (grid[i][j].isWall() && grid[i][j + 2].isWall())
                grid[i][j + 1].setWall(true);


           /*if((i + 1 == grid.size()- 1) || (i + 2) >= grid.size() || (i + 3 >= grid.size()))
             continue;
           if(grid[i][j].isWall() && grid[i+3][j].isWall())
                grid[i+1][j].setWall(true);
           if(grid[i][j].isWall() && grid[i+2][j].isWall())
                grid[i+1][j].setWall(true);*/
        }
    }
}

void Map::addAloneWalls(){
  for (int i = 0; i < grid.size(); i++)
  {
      for (int j = 0; j < grid[i].size(); j++)
      {
          if (isMiddle(i, j))
              continue;

          bool alone = true;
          for (auto itr = direct_all.begin(); itr != direct_all.end();)
          {
              int i_offset = (*itr)[0];
              int j_offset = (*itr)[1];
              ++itr;

              if ((i_offset + i < 0) || (j_offset + j < 0) || (i_offset + i >= grid.size()) || (j_offset + j >= grid[i].size()))
              {
                  alone = false;
                  break;
              }
              if ((j_offset + j != s_columns / 2 ) && grid[i + i_offset][j + j_offset].isWall())
              {
                  alone = false;
                  break;
              }
          }

          if (alone && !grid[i][j].isWall())
          {
              grid[i][j].setWall(true);
              grid[i][j].SetAdded(true);
              printf("Añado i: %d, j: %d\n", i, j);

              Cell& toDelete = grid[i][j];
              Cell* pair = getPairCell(toDelete);
              printf("Miro par i: %d, j: %d\n", pair->x, pair->y);

              //toDelete.setWall(false);
              //toDelete.SetDeleted(true);
              if (!isMiddle(pair->x, pair->y) && !grid[pair->x][pair->y].isWall())
              {
                  grid[pair->x][pair->y].setWall(true);
                  grid[pair->x][pair->y].SetAdded(true);
                  //pair->setWall(false);
                  //pair->SetDeleted(true);
              }
          }

      }
  }
}

void Map::DFS(int i, int j)
{
    // Solo el DFS que te visita y quita muros de todos lados
    if (i < 0 || j < 0 || i >= grid.size() || j >= grid[i].size())
        return;

    if (grid[i][j].isVisited())
        return;

    grid[i][j].setVisited(true);

    std::vector<std::vector<int>> spaces = { {0, 1}, {0, -1}, {-1, 0}, {1, 0} };
    std::vector<Cell*> cells;
    std::vector<Cell*> walls;

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    shuffle(spaces.begin(), spaces.end(), std::default_random_engine(seed));

    int next_i = -1, next_j = -1;
    for (auto itr = spaces.begin(); itr != spaces.end();)
    {
        int new_i = (*itr)[0] + i;
        int new_j = (*itr)[1] + j;

        if (new_i < 0 || new_j < 0 || new_i >= grid.size() || new_j >= grid[new_i].size() || grid[new_i][new_j].isVisited())
        {
            ++itr;
            continue;
        }

        if (grid[new_i][new_j].isWall() && !grid[new_i][new_j].isVisited())
            walls.push_back(&grid[new_i][new_j]);

        if (!grid[new_i][new_j].isWall() && !grid[new_i][new_j].isVisited())
            cells.push_back(&grid[new_i][new_j]);

        ++itr;
    }

    for (int k = 0; k < cells.size(); k++)
    {
        Cell* cell = cells[k];
        cell->setWall(true);
        cell->setVisited(true);
    }

    if (!walls.empty())
        walls[0]->setWall(false);

    if (!cells.empty())
    {
        grid[i][j].setWall(false);
        cells[0]->setWall(false);
        cells[0]->setVisited(false);

        DFS(cells[0]->x, cells[0]->y);
    }

    for (auto const& wall : walls)
        DFS(wall->x, wall->y);
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

            if (grid[i][j].isWall())
                continue;

            int walls = 0;
            std::vector<std::vector<int>> directc = { {0, 1}, {0, -1}, {-1, 0}, {1, 0} };
            std::vector<Cell*> good;

            for (auto& direction : directc)
            {
                int i_offset = direction[0];
                int j_offset = direction[1];

                if ((i_offset == -1 && i == 0) ||
                    (j_offset == -1 && j == 0) ||
                    (i_offset == 1 && i == grid.size() -1 ) ||
                    (j_offset == 1 && j == grid[i].size() - 1) ||
                    isMiddle(i + i_offset, j + j_offset))
                {
                    walls++;
                    continue;
                }

                if (grid[i + i_offset][j + j_offset].isWall())
                {
                    good.push_back(&grid[i + i_offset][j + j_offset]);
                    walls++;
                }
            }

            if (walls < 3)
                continue;

            Utils::RandomResize(good, walls - 2);
            int sizea = good.size() - (walls - 2);
            for (auto& toDelete : good)
            {
                Cell* pair = getPairCell(toDelete);

                toDelete->setWall(false);
                toDelete->SetDeleted(true);

                if (!isMiddle(pair->x, pair->y))
                {
                    pair->setWall(false);
                    //pair->SetDeleted(true);
                }
            }
        }
    }
}

void Map::finalVerticalSymetryCheck(){
  for (size_t i = 0; i < grid.size(); i++) {
    for (size_t j = 0; j < grid[0].size()/2; j++) {
      Cell& cell = grid[i][j];
      Cell* pair = getPairCell(cell);
      if(!isMiddle(pair->x, pair->y))
        pair->setWall(cell.isWall());
    }
  }
}

void Map::createVerticalSymetry() {
    int aux = grid[0].size();
    for (int i = 0; i < grid.size(); i++)
    {
        if (s_columns % 2 != 0)
            grid[i].push_back(Cell(i, aux, true)); ///< change

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

Cell* Map::getPairCell(Cell* cell)
{
    return &grid[cell->x][s_columns - cell->y - 1];
}


bool Map::isMiddle(int i, int j)
{
  int height_middle = grid.size() / 2;
  int width_middle = grid[0].size() / 2;

  for (auto const& elem : middle_walls)
  {
      int i_offset = elem[0];
      int j_offset = elem[1];
      if((i == height_middle + i_offset) && (j == width_middle + j_offset))
        return true;
  }
  for (auto const& elem : middle_empty_walls)
  {
      int i_offset = elem[0];
      int j_offset = elem[1];
      if((i == height_middle + i_offset) && (j == width_middle + j_offset))
        return true;
  }
  return false;
}

void Map::addMiddle()
{
    int height_middle = grid.size() / 2;
    int width_middle = grid[0].size() / 2;
    for (auto const& elem : middle_empty_walls)
    {
        int i_offset = elem[0];
        int j_offset = elem[1];
        grid[height_middle + i_offset][width_middle + j_offset].setWall(false);


    }
    for (auto const& elem : middle_walls)
    {
        int i_offset = elem[0];
        int j_offset = elem[1];
        grid[height_middle + i_offset][width_middle + j_offset].setWall(true);
    }
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
  if (countVisitedNeighbor(i, j) > 1)
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
        {
            bool wall = true;
            if (cell.y % 2 == 0 && cell.x % 2 == 0)
                wall = false;
            if (cell.y % 2 == 1 && cell.x % 2 ==1)
                wall = false;

            cell.setWall(wall);
            cell.SetAdded(false);
            cell.setVisited(false);
        }
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
