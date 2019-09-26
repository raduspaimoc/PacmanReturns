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
    cout << "Enter the number of columns: ";
    cin >> s_columns;
    cout << "Enter the number of rows: ";
    cin >> s_rows;

    if (s_columns < 10 || s_rows < 10)
    {
        printf("Min dimension is 20x20\n");
        return 0;
    }

    if (s_columns > 100 || s_rows > 100)
    {
        printf("Max dimension is 100x100\n");
        return 0;
    }

    s_map = Map(s_rows, s_columns/2);
    Graphics graph;

    s_map.setWalls();

    char fakeParam[] = "fake";
    char *fakeargv[] = { fakeParam, NULL };
    int fakeargc = 1;
    glutInit(&fakeargc, fakeargv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(WIDTH + MARGIN_2, HEIGHT + MARGIN_2);
    glutCreateWindow("Pac man");

    s_map.showInfo();
    glutDisplayFunc(graph.display);
    glutKeyboardFunc(graph.keyboard);

    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, WIDTH + MARGIN_2, 0, HEIGHT + MARGIN_2);

    glutMainLoop();

    return 0;
}
