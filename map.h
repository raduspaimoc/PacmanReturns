#include <vector>
#include "cell.h"

struct Map
{
  public:
    Map(int r, int c);
    void Show();
    friend std::ostream& operator<<(std::ostream& os, const Map& map);

    int rows;
    int columns;
    std::vector<std::vector<Cell> > grid;
};
