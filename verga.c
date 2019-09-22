#include "string"
#include "vector"
#include "unordered_map"
#include "queue"
#include "cstdlib"
#include "ctime"
#include <iostream>
#include "algorithm"
#include "stack"
using namespace std;

class Solution{
public:


    void shuffle(int a[], int n){
        for (int i = 0; i < n; ++i)
        {
            swap(a[i], a[rand() % n]);
        }
    }
    void swap(int & a, int &b){
        int c = a;
        a = b;
        b = c;
    }
};

void addBorders(vector<vector<char>>& new_map, vector<vector<char>>& map){
  for (int i = 0; i < map.size(); ++i)
  {
      for (int j = 0; j < map[0].size(); ++j)
      {
          new_map[i+1][j+1] = map[i][j];
          new_map[new_map.size() - i -1][new_map[0].size() - j - 1] = map[i][j];
      }
  }

  for (int i = 0; i < new_map[0].size(); i++) {
    new_map[0][i] = 'X';
    new_map[new_map.size()-1][i] = 'X';
  }
  for (int i = 0; i < new_map.size(); i++) {
    new_map[i][0] = 'X';
    new_map[i][new_map[0].size()-1] = 'X';
  }
}


int main(int argc, char const *argv[])
{
	Solution s;

    int height = 20;
    int width = 40;
    srand(time(0));
    vector<char> row(width/2 - 1);
    vector<vector<char>> map;
    for (int i = 0; i < height-2; ++i)
    {
        map.push_back(row);
    }
    s.maze(map);
    vector<char> new_row(width);
    vector<vector<char>> new_map;
    for(int i=0; i < height; ++i ){
      new_map.push_back(new_row);
    }
    s.showMaze(map);
    addBorders(new_map, map);
    s.showMaze(new_map);
    return 0;
}
