#include <iostream>
#include "map.h"
#include <GL/glut.h>
#define WIDTH 300
#define HEIGHT 300

using namespace std;
int columns, rows;


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

  //srand(time(0));
  rows = atoi(argv[1]);
  columns = atoi(argv[2]);
  Map map(rows, columns);
  map.setWalls();
  cout << map;
  map.showInfo();

  char fakeParam[] = "fake";
  char *fakeargv[] = { fakeParam, NULL };
  int fakeargc = 1;
  glutInit(&fakeargc, fakeargv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowPosition(50, 50);
  glutInitWindowSize(WIDTH, HEIGHT);
  glutCreateWindow("Pac man");
  glutDisplayFunc(map.display);
  //glutKeyboardFunc(keyboard);
  glMatrixMode(GL_PROJECTION);
  gluOrtho2D(0,WIDTH-1,0,HEIGHT-1);

  glutMainLoop();



  return 0;
}

void display()
{
  int i,j;

  glClearColor(0.0,0.0,0.0,0.0);
  glClear(GL_COLOR_BUFFER_BIT);

  for(i=0;i<WIDTH;i++)
    for(j=0;j<HEIGHT;j++)
	{
    if(!grid[i][j].isWall()){
      glColor3f(0.8,0.8,0.8);
      glBegin(GL_QUADS);

     glVertex2i(i*WIDTH/grid.size(),j*HEIGHT/grid[0].size());
     glVertex2i((i+1)*WIDTH/grid.size(),j*HEIGHT/grid[0].size());
     glVertex2i((i+1)*WIDTH/grid.size(),(j+1)*HEIGHT/grid[0].size());
     glVertex2i(i*WIDTH/grid.size(),(j+1)*HEIGHT/grid[0].size());

     glEnd();
    }
	}

  glutSwapBuffers();

}
