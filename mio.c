#include <iostream>
#include <stdlib.h>
#define ROWS 20
#define COLUMNS 40

void addStartBox(char map[ROWS][COLUMNS], int i, int j);

int main(int argc,char *argv[])
{
  char map[ROWS][COLUMNS];
  int middle_rows = ROWS / 2 - 1;
  int middle_columns = COLUMNS / 2 - 1;

  for ( int i = 0; i < ROWS; i++ ) {
    for (int j = 0; j < COLUMNS; j++) {

      /*if(i == middle_rows - 1 && j == middle_columns ) {
        map[i][j] = "W"
      }*/
      /*FILL MAP*/
      if(i == 0 || j == 0 || i == ROWS - 1 || j == COLUMNS/2 - 1){
        map[i][j] = 'X';
      } else {
        //int iSecret = rand() % 2 + 1;
        map[i][j] = 'X';
        /*if(iSecret == 1){
          map[i][j] = 'X';
        } else {
          map[i][j] = ' ';
        }*/
      }

      /*NOT COMPLETED SET MIDDLE*/
      addStartBox( map, i, j);
      /*if((i == middle_rows - 1 && j == middle_columns ) ||
         (i == middle_rows && j == middle_columns - 1) ||
         (i == middle_rows && j == middle_columns - 2) ||
         (i == middle_rows && j == middle_columns) ||
         (i == middle_rows + 1 && j == middle_columns - 1) ||
         (i == middle_rows + 1 && j == middle_columns - 2) ||
         (i == middle_rows + 1 && j == middle_columns)
       ){
           map[i][j] = 'W';
      }*/



    }
    //printf("\n");
      //n[ i ] = i + 100; // set element at location i to i + 100
   }

   // Check bros
   /*for ( int i = 0; i < ROWS; i++ ) {
     for (int j = 0; j < COLUMNS/2; j++) {
       //let's see if works
       if((i != 0) && (j != 0) && (i != ROWS-1)){
         int neighbour = 0;
         neighbour = map[i][j-1] == 'X' ? neighbour + 1 : neighbour;
         neighbour = map[i][j+1] == 'X' ? neighbour + 1 : neighbour;
         neighbour = map[i-1][j] == 'X' ? neighbour + 1 : neighbour;
         neighbour = map[i+1][j] == 'X' ? neighbour + 1 : neighbour;

         if(map[i][j] != 'W'){
           map[i][j] = neighbour >= 3 ? ' ' : map[i][j];
           map[i][j] = neighbour == 0 ? 'X' : map[i][j];
         }

       }
     }

   }*/

   for ( int i = 0; i < ROWS; i++ ) {
     for (int j = 0; j < COLUMNS/2; j++) {
       printf("%c", map[i][j]);
     }
     printf("\n");
       //n[ i ] = i + 100; // set element at location i to i + 100
    }



  return 0;
}

void generateMaze(){
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
  //this is to avoid circles in maze
  if(countVisitedNeighbor(map, i, j) > 1) return ;

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

void addStartBox(char map[ROWS][COLUMNS], int i, int j){
  int middle_rows = ROWS / 2 - 1;
  int middle_columns = COLUMNS / 2 - 1;
  if((i == middle_rows - 1 && j == middle_columns ) ||
     (i == middle_rows && j == middle_columns - 1) ||
     (i == middle_rows && j == middle_columns - 2) ||
     (i == middle_rows && j == middle_columns) ||
     (i == middle_rows + 1 && j == middle_columns - 1) ||
     (i == middle_rows + 1 && j == middle_columns - 2) ||
     (i == middle_rows + 1 && j == middle_columns) ||
     (i == middle_rows && j == middle_columns + 1) ||
     (i == middle_rows && j == middle_columns + 2) ||
     (i == middle_rows && j == middle_columns) ||
     (i == middle_rows + 1 && j == middle_columns + 1) ||
     (i == middle_rows + 1 && j == middle_columns + 2) ||
     (i == middle_rows + 1 && j == middle_columns)
   /*(i == middle_rows && j == middle_columns + 1)*/){
       map[i][j] = 'W';
  }
}
