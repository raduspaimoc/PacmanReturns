#include <iostream>
#include "map.h"

using namespace std;

int main(int argc, char const *argv[])
{
  if (argc < 2)
  {
    printf("Not enough arguments\n");
    return 0;
  }

  if (atoi(argv[1]) < 10 || atoi(argv[2]) < 10)
  {
    printf("Min dimension is 10x10\n");
    return 0;
  }

  srand(time(0));
  Map map(atoi(argv[1]), atoi(argv[2]));
  cout << map;
  map.showInfo();

  return 0;
}
