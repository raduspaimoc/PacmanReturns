#include <iostream>
#include "map.h"

using namespace std;

int main()
{
  int height = 20;
  int width = 20;

  srand(time(0));
  Map map(height, width);
  cout << map;
  map.showInfo();

  return 0;
}
