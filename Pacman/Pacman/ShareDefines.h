#include "map.h"

#define WIDTH 1000
#define HEIGHT 500

extern int s_columns, s_rows;
extern Map s_map;
static std::vector<std::vector<int>> direct = { {0, 1}, {0, -1}, {-1, 0}, {1, 0} };
static std::vector<std::vector<int>> direct_all = { {0, 1}, {1, 0}, {1, 1}, {0, -1}, {-1, 0}, {-1, -1}, {1, -1}, {-1, 1} };