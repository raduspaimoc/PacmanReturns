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
  // Definition for singly-linked list.
    void maze(vector<vector<char>>& map){
        //U unvisited, ' ' visited
        for (int i = 0; i < map.size(); ++i)
        {
            for (int j = 0; j < map[0].size(); ++j)
            {
                map[i][j] = 'X';
            }
        }
        _maze(map, 0, 0);
    }

    void showMaze(vector<vector<char>>& map){
        for (int i = 0; i < map.size(); ++i)
        {
            for (int j = 0; j < map[0].size(); ++j)
            {
                cout<<map[i][j];
            }
            cout<<endl;
        }
    }

    //Use DFS
    void _maze(vector<vector<char>>& map, int i, int j){
        int direct[][2] = {{0,1}, {0,-1}, {-1,0}, {1,0}};
        int visitOrder[] = {0,1,2,3};
        //out of boundary
        if(i < 0 || j < 0 || i >= map.size() || j >= map[0].size()) return ;
        #ifdef DEBUG
        cout<<"("<<i<<", "<<j<<")"<<endl;
        #endif
        //visited, go back the the coming direction, return
        if(map[i][j] == ' ') return ;

        //some neightbors are visited in addition to the coming direction, return
        //this is fill some circles in maze
        if(countVisitedNeighbor(map, i, j) > 2) return ;

        map[i][j] = ' '; // visited

        //shuffle the visitOrder
        shuffle(visitOrder, 4);

        for (int k = 0; k < 4; ++k)
        {
            int ni = i + direct[visitOrder[k]][0];
            int nj = j + direct[visitOrder[k]][1];
            _maze(map, ni, nj);
        }
    }



    int countVisitedNeighbor(vector<vector<char>>& map, int i, int j){
        int direct[][2] = {{1,0}, {-1,0}, {0,1}, {0,-1}};
        int count = 0;
        for (int k = 0; k < 4; ++k)
        {
            int ni = i + direct[k][0];
            int nj = j + direct[k][1];
            //out of boundary
            if(ni < 0 || nj < 0 || ni >= map.size() || nj >= map[0].size()) continue;
            if(map[ni][nj] == ' ') count++;//visited
        }
        return count;
    }
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
          new_map[i+1][new_map[0].size() - j - 1] = map[i][j];
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

void addMiddle(vector<vector<char>>& map){
  int height_middle = map.size() /2;
  int width_middle = map[0].size() /2;

  /*
    ____ ___
    |______|

  */

  /*Empty start room */
  map[height_middle][width_middle] = ' ';
  map[height_middle - 1][width_middle] = ' ';
  map[height_middle][width_middle - 1] = ' ';
  map[height_middle][width_middle + 1] = ' ';

  map[height_middle - 1][width_middle - 1] = 'P';
  map[height_middle - 1][width_middle - 2] = 'P';
  map[height_middle - 1][width_middle + 1] = 'P';
  map[height_middle - 1][width_middle + 2] = 'P';

  //map[height_middle][width_middle] = 'X';
  map[height_middle][width_middle - 2] = 'P';
  map[height_middle][width_middle + 2] = 'P';

  map[height_middle + 1][width_middle - 1] = 'P';
  map[height_middle + 1][width_middle - 2] = 'P';
  map[height_middle + 1][width_middle] = 'P';
  map[height_middle + 1][width_middle + 1] = 'P';
  map[height_middle + 1][width_middle + 2] = 'P';

}


int main(int argc, char const *argv[])
{
	Solution s;

    int height = 20;
    int width = 41;
    srand(time(0));
    vector<char> row(width/2);
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
    addMiddle(new_map);
    s.showMaze(new_map);
    return 0;
}
