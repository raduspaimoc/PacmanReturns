#include <iostream>     // std::cout
#include <algorithm>    // std::shuffle
#include <array>        // std::array
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock
#include <GL/glut.h>

#include "map.h"
#include "Utils.h"
#include "ShareDefines.h"

Map::Map(int r, int c) : rows(r), columns(c)
{
  //pacman.grid_y = pacman.grid_y = 0;
  total_food = 0;

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

  for (const auto& row : grid)
  {
      for (auto column : row)
          std::cout << column;

      std::cout << std::endl;
  }

  printf("--------------------- \n");
}

void Map::setWalls()
{
    DFS(&grid[0][0]);
    createVerticalSymetry();
    removeTrees();
    addMiddle();

    cleanMiddle();
    addAloneWalls();

    initCells();
}

void Map::initCells()
{
    std::vector<Cell*> emptyCells;
    total_food = 0;

    for (auto& row : grid)
    {
        for (auto& cell : row)
        {
            if (!cell.isWall() && !isMiddle(cell.x, cell.y))
            {
                cell.setFlag(CellFlags::CELL_FLAG_FOOD);
                total_food++;
                emptyCells.push_back(&cell);
            }
        }
    }

    if (emptyCells.empty())
        return;

    total_food -= 1;
    Utils::RandomResize(emptyCells, 1);
    emptyCells[0]->setFlag(CellFlags::CELL_FLAG_PACMAN);
    initCharacters(emptyCells[0]->x, emptyCells[0]->y);

}

void Map::initCharacters(int pacman_x, int pacman_y)
{
    pacman = Character(pacman_x, pacman_y, grid.size(), grid[0].size(), CharacterFlags::CHARACTER_FLAG_PACMAN);
    Character v_ghost = Character(grid.size() / 2, grid[0].size() / 2, grid.size(), grid[0].size(), CharacterFlags::CHARACTER_FLAG_GHOST);
    //v_ghost.setXY(grid.size()/2, grid[0].size() / 2);
    ghost = v_ghost;

    auto_ghosts.clear();
    //auto_ghosts.push_back(v_ghost);
    auto_ghosts.push_back(Character(grid.size() / 2, (grid[0].size() / 2) - 1, grid.size(), grid[0].size(), CharacterFlags::CHARACTER_FLAG_AUTO_GHOST));
    auto_ghosts.push_back(Character(grid.size() / 2, (grid[0].size() / 2) + 1, grid.size(), grid[0].size(), CharacterFlags::CHARACTER_FLAG_AUTO_GHOST));
}

void Map::checkGameState(long t, long last_t){
    s_map.pacman.integrate(t-last_t);
    s_map.ghost.integrate(t-last_t);

    int pacman_x = (int) s_map.pacman.x;
    int pacman_y = (int) s_map.pacman.y;
    int ghost_x = (int) s_map.ghost.x;
    int ghost_y = (int) s_map.ghost.y;

    if (pacman_x == ghost_x && pacman_y == ghost_y)
        std::exit(0);
    if (abs(pacman_x - ghost_x) <= 10 && abs(pacman_y - ghost_y) <= 10 )
        std::exit(0);

    for (auto & auto_ghost : s_map.auto_ghosts)
    {
        ghost_x = (int) auto_ghost.x;
        ghost_y = (int) auto_ghost.y;

        if (pacman_x == ghost_x && pacman_y == ghost_y)
            std::exit(0);

        if (abs(pacman_x - ghost_x) <= 10 && abs(pacman_y - ghost_y) <= 10 )
            std::exit(0);

        auto_ghost.integrate(t-last_t);
    }
}

int Map::getTotalScore() {
    return this->score;
}

bool Map::pacmanWins(){
    return total_food == 0;
}

bool Map::pacmanLoses(){

    int pacman_x = (int) s_map.pacman.x;
    int pacman_y = (int) s_map.pacman.y;
    int ghost_x = (int) s_map.ghost.x;
    int ghost_y = (int) s_map.ghost.y;

    if (pacman_x == ghost_x && pacman_y == ghost_y)
        return true;

    for (auto & auto_ghost : s_map.auto_ghosts)
    {
        ghost_x = (int) s_map.ghost.x;
        ghost_y = (int) s_map.ghost.y;

        if (pacman_x == ghost_x && pacman_y == ghost_y)
            return true;
    }
    return false;
}

std::vector<Character> Map::getGhosts(){
    std::vector<Character> ghosts;
    ghosts.push_back(ghost);
    for (Character ghos : auto_ghosts){
        ghosts.push_back(ghos);
    }
    return ghosts;
}

std::vector<Cell> Map::getCellsWithFood(){
    std::vector<Cell> cellsWithFood;
    for (auto& row : grid)
    {
        for (auto& cell : row)
        {
            if(cell.hasFlag(CellFlags::CELL_FLAG_FOOD)){
                cellsWithFood.push_back(cell);
            }
        }
    }
    return cellsWithFood;

}

std::vector<Cell> Map::getWalls(){
    std::vector<Cell> cellWall;
    for (auto& row : grid)
    {
        for (auto& cell : row)
        {
            if(cell.hasFlag(CellFlags::CELL_FLAG_WALL)){
                cellWall.push_back(cell);
            }
        }
    }
    return cellWall;

}

std::vector<Character> getAllAgents(){

    std::vector<Character> all_characters;
    all_characters.push_back(s_map.ghost);
    all_characters.push_back(s_map.pacman);
    all_characters.insert(all_characters.end(), std::begin(s_map.auto_ghosts), std::end(s_map.auto_ghosts));
    //all_characters.push_back(s_map.auto_ghosts);
    //= s_map.auto_ghosts;
    //all_characters.push_back(s_map.pacman);
    return all_characters;
}

Character Map::getAgent(int agent){
    if(agent == 0)
        return pacman;
    else if(agent == 1)
        return ghost;
    else if(agent == 2)
        return auto_ghosts[0];
    else
        return auto_ghosts[1];
}

void Map::generateSuccesor(int agent, std::vector<int> action){
   switch (agent) {
       case 0:
           pacman.grid_x = pacman.grid_x + action[0];
           pacman.grid_y = pacman.grid_y + action[1];
           break;
       case 1:
           ghost.grid_x = pacman.grid_x + action[0];
           ghost.grid_y = pacman.grid_y + action[1];
           break;
       case 2:
           auto_ghosts[0].grid_x = pacman.grid_x + action[0];
           auto_ghosts[0].grid_y = pacman.grid_y + action[1];
           break;
       case 3:
           auto_ghosts[1].grid_x = pacman.grid_x + action[0];
           auto_ghosts[1].grid_y = pacman.grid_y + action[1];
       default:
           break;
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

              Cell toDelete = grid[i][j];
              Cell* pair = getPairCell(toDelete);

              if (!isMiddle(pair->x, pair->y) && !grid[pair->x][pair->y].isWall())
                  grid[pair->x][pair->y].setWall(true);
          }

      }
  }
}

void Map::DFS(Cell* cell)
{
    if (cell->isVisited())
        return;

    cell->setVisited(true);

    std::vector<int8_t> directions = { 0, 1, 2, 3 };
    std::vector<Cell*> cells;
    std::vector<Cell*> walls;

    shuffle(directions.begin(), directions.end(), std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count()));

    for (auto& direction : directions)
    {
        int8_t new_i = direct[direction][0] + cell->x;
        int8_t new_j = direct[direction][1] + cell->y;

        if (new_i < 0 || new_j < 0 || new_i >= grid.size() || new_j >= grid[new_i].size() || grid[new_i][new_j].isVisited())
            continue;

        if (grid[new_i][new_j].isWall())
            walls.push_back(&grid[new_i][new_j]);
        else
            cells.push_back(&grid[new_i][new_j]);
    }

    for(auto& l_cell: cells)
    {
        l_cell->setWall(true);
        l_cell->setVisited(true);
    }

    if (!cells.empty())
    {
        cell->setWall(false);
        cells[0]->setWall(false);
        cells[0]->setVisited(false);

        DFS(cells[0]);
        cells.clear();
    }

    for (auto& wall : walls)
        if (!wall->isVisited())
            DFS(wall);
}

int Map::getWallCount(Cell* cell)
{
    std::vector<std::vector<int>> spaces = { {0, 1}, {0, -1}, {-1, 0}, {1, 0} };
    int walls = 0;

    for (auto const& itr : spaces)
    {
        int new_i = itr[0] + cell->x;
        int new_j = itr[1] + cell->y;

        if (new_i < 0 || new_j < 0 || new_i >= grid.size() || new_j >= grid[new_i].size() || grid[new_i][new_j].isVisited())
        {
            walls++;
            continue;
        }

        if (grid[new_i][new_j].isWall())
            walls++;
    }

    return walls;
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
            for (auto& toDelete : good)
            {
                Cell* pair = getPairCell(toDelete);

                toDelete->setWall(false);
                toDelete->SetDeleted(true);
                CheckTreesRec(toDelete);

                if (!isMiddle(pair->x, pair->y))
                {
                    pair->setWall(false);
                }
            }
        }
    }
}

void Map::CheckTreesRec(Cell* cell)
{
    if (isMiddle(cell->x, cell->y))
        return;

    if (cell->isWall())
        return;

    int walls = 0;
    std::vector<std::vector<int>> directc = { {0, 1}, {0, -1}, {-1, 0}, {1, 0} };
    std::vector<Cell*> good;

    for (auto& direction : directc)
    {
        int i_offset = direction[0];
        int j_offset = direction[1];

        if ((i_offset == -1 && cell->x == 0) ||
            (j_offset == -1 && cell->y == 0) ||
            (i_offset == 1 && cell->x == grid.size() - 1) ||
            (j_offset == 1 && cell->y == grid[cell->x].size() - 1) ||
            isMiddle(cell->x + i_offset, cell->y + j_offset))
        {
            walls++;
            continue;
        }

        if (grid[cell->x + i_offset][cell->y + j_offset].isWall())
        {
            good.push_back(&grid[cell->x + i_offset][cell->y + j_offset]);
            walls++;
        }
    }

    if (walls < 3)
        return;

    Utils::RandomResize(good, walls - 2);
    for (auto& toDelete : good)
    {
        Cell* pair = getPairCell(toDelete);

        toDelete->setWall(false);
        toDelete->SetDeleted(true);
        CheckTreesRec(toDelete);

        if (!isMiddle(pair->x, pair->y))
        {
            pair->setWall(false);
            //pair->SetDeleted(true);
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
                new_y++;

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

void Map::cleanMiddle(){
  int height_middle = grid.size() / 2;
  int width_middle = grid[0].size() / 2;
  for(int i = 1; i < grid.size()-1; i++){
    grid[i][width_middle].setWall(false);
    if(i == height_middle+1){
      grid[i][width_middle].setWall(true);
    }
  }
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

void Map::reset()
{
    for (auto& row : grid)
    {
        row.resize(s_columns/2);
        for (auto& cell : row)
        {
            bool wall = true;
            if ((int) cell.y % 2 == 0 && (int) cell.x % 2 == 0)
                wall = false;
            if ((int) cell.y % 2 == 1 && (int) cell.x % 2 ==1)
                wall = false;

            cell.setWall(wall);
            cell.SetAdded(false);
            cell.setVisited(false);
        }
    }
    total_food = getCellsWithFood().size() - 1;
}

std::vector<Cell> Map::getCellsWithFood(){
    std::vector<Cell> cellsWithFood;
    for (auto& row : grid)
    {
        for (auto& cell : row)
        {
            if(cell.hasFlag(CellFlags::CELL_FLAG_FOOD)){
                cellsWithFood.push_back(cell);
            }
        }
    }
    return cellsWithFood;

}

std::ostream& operator<<(std::ostream& os, const Map& map)
{
    for (const auto& row : map.grid)
    {
      for (const auto& column : row)
        os << column;

      os << '\n';
    }

    return os;
}
