#include "map.h"

#define WIDTH 700
#define HEIGHT 700
#define DEPTH 20
#define DEPTH_2 20 / 2
#define WIDTH_2 WIDTH / 2
#define HEIGHT_2 HEIGHT / 2
#define MARGIN 20
#define MARGIN_2 MARGIN * 2
#define PI 3.1416

enum Directions : unsigned char
{
    UP = 'W',
    DOWN = 'S',
    LEFT = 'A',
    RIGHT = 'D'
};

extern int s_columns, s_rows;
extern Map s_map;
static std::vector<std::vector<int>> direct = { {0, 1}, {0, -1}, {-1, 0}, {1, 0} };
static std::vector<std::vector<int>> direct_all = { {0, 1}, {1, 0}, {1, 1}, {0, -1}, {-1, 0}, {-1, -1}, {1, -1}, {-1, 1} };
static std::vector<std::vector<int>> middle_empty_walls = { {0, 0}, {-1, 0}, {-2, 0}, {0, -1}, {0, -1}, {0, 1} };
static std::vector<std::vector<int>> middle_walls = { {-1, -1}, {-1, -2}, {-1, 1}, {-1, 2}, {0, -2}, {0, 2},{1, -1}, {1, -2}, {1, 0}, {1, 1}, {1, 2}};
