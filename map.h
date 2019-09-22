#include <vector>
#include "cell.h"

struct Map
{
  public:
    Map(int r, int c);
    void showInfo();
    void setWalls();
    void setWallsRec(int i, int j);
    int countVisitedNeighbor(int i, int j);

    friend std::ostream& operator<<(std::ostream& os, const Map& map);

    std::vector<std::vector<Cell> > grid;
};
