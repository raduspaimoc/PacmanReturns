#include "map.h"

#define WIDTH 1000
#define HEIGHT 1000
#define WIDTH_2 WIDTH * 2
#define HEIGHT_2 HEIGHT * 2
#define MARGIN 20
#define MARGIN_2 MARGIN * 2
#define UP 'W'
#define DOWN 'S'
#define LEFT 'A'
#define RIGHT 'D'

extern int s_columns, s_rows;
extern Map s_map;
static std::vector<std::vector<int>> direct = { {0, 1}, {0, -1}, {-1, 0}, {1, 0} };
static std::vector<std::vector<int>> direct_all = { {0, 1}, {1, 0}, {1, 1}, {0, -1}, {-1, 0}, {-1, -1}, {1, -1}, {-1, 1} };
static std::vector<std::vector<int>> middle_empty_walls = { {0, 0}, {-1, 0}, {-2, 0}, {0, -1}, {0, -1}, {0, 1} };
static std::vector<std::vector<int>> middle_walls = { {-1, -1}, {-1, -2}, {-1, 1}, {-1, 2}, {0, -2}, {0, 2},{1, -1}, {1, -2}, {1, 0}, {1, 1}, {1, 2}};
