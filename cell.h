#include <iostream>

struct Cell
{
  Cell(int x, int y);
  void setWall(bool apply);
  bool isWall();
  friend std::ostream& operator<<(std::ostream& os, const Cell& cell);

  int x;
  int y;
  bool wall;
};
