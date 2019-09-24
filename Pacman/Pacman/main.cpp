#include <iostream>
#include <functional>
#include <GL/glut.h>

#include "map.h"
#include "ShareDefines.h"
#include "graphics.h"

using namespace std;

int s_columns, s_rows;
Map s_map;

int main(int argc, char const *argv[])
{
    /*
  if (argc < 2)
  {
    printf("Not enough arguments\n");
    return 0;
  }

  if (atoi(argv[1]) < 10 || atoi(argv[2]) < 10)
  {
    printf("Min dimension is 10x10\n");
    return 0;
  }*/

  //srand(time(0));

  //rows = atoi(argv[1]);
  //columns = atoi(argv[2]);

    cout << "Enter the number of columns: ";
    cin >> s_columns;
    cout << "Enter the number of rows: ";
    cin >> s_rows;

    if (s_columns < 10 || s_rows < 10)
    {
        printf("Min dimension is 10x10\n");
        return 0;
    }

    s_rows = 20;
    s_columns = 41;
    s_map = Map(s_rows, s_columns/2);
    Graphics graph;

    s_map.setWalls();

    char fakeParam[] = "fake";
    char *fakeargv[] = { fakeParam, NULL };
    int fakeargc = 1;
    glutInit(&fakeargc, fakeargv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Pac man");

    s_map.showInfo();
    glutDisplayFunc(graph.display);
    glutKeyboardFunc(graph.keyboard);

    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, WIDTH + (WIDTH / s_columns), 0, HEIGHT);

    glutMainLoop();

    return 0;
}
