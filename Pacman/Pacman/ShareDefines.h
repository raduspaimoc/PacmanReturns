#include "map.h"

#define WIDTH 500
#define HEIGHT 300

extern int s_columns, s_rows;
extern Map s_map;
static std::vector<std::vector<int>> direct = { {0, 1}, {0, -1}, {-1, 0}, {1, 0} };
static std::vector<std::vector<int>> direct_all = { {0, 1}, {1, 0}, {1, 1}, {0, -1}, {-1, 0}, {-1, -1}, {1, -1}, {-1, 1} };