#include "character.h"
#include <GL/glut.h>
#include <iostream>
//#include "ShareDefines.h"

#define WIDTH 1000
#define HEIGHT 1000
#define WIDTH_2 WIDTH * 2
#define HEIGHT_2 HEIGHT * 2
#define MARGIN 20
#define MARGIN_2 MARGIN * 2

Character::Character() {}
Character::Character(float x, float y, int r, int c) : x(x), y(y), r(r), c(c) { }

void Character::set_position(float x, float y){
    this->x = x;
    this->y = y;
}

void Character::init_movement(float destination_x, float destination_y, float duration) {
    vx = (destination_x - x)/duration;
    vy = (destination_y - y)/duration;
    state = MOVE;
    time_remaining = (long) duration;

}

void Character::integrate(long t){
    if(state==MOVE && t<time_remaining)
    {
        x = x + vx* (float)t;
        y = y + vy* (float)t;
        time_remaining-=t;
    }
    else if(state==MOVE && t>=time_remaining)
    {
        x = x + vx*(float)time_remaining;
        y = y + vy*(float)time_remaining;
        state=QUIET;
    }
}


//[WIP]
/*void Character::draw() {

    float cell_width = (float)WIDTH / (float)c;
    float cell_height = (float)HEIGHT / (float)r;
    float cell_width4 = cell_width / 4;
    float cell_height4 = cell_height / 4;
    float j = this->y;
    //float i = this->x;
    float i = (r - ((this->x - MARGIN / cell_height)) - 1) * cell_height + MARGIN;

    glColor3f(0.0, 0.0, 0.0);

    glBegin(GL_QUADS);

    glVertex2i(j, i * cell_height + MARGIN);
    glVertex2i(j + cell_width, i);

    glVertex2i(j + cell_width, i + cell_height);
    glVertex2i(j, i + cell_height);

    glEnd();

    glColor3f(1.0, 0.5, 0.0);

    glBegin(GL_QUADS);

    glVertex2i(j + cell_width4, i + cell_height4);
    glVertex2i(j + ( 2 * cell_width4), i + cell_height4);

    glVertex2i(j + ( 2 * cell_width4), i + (2 * cell_height4));
    glVertex2i(j + cell_width4, i+ (2 * cell_height4));

    glEnd();
}*/

void Character::draw() {
    int j = this->y;
    //int i = this->x;
    int i = r - this->x - 1;
    float cell_width = (float)WIDTH / (float)c;
    float cell_height = (float)HEIGHT / (float)r;
    float cell_width4 = cell_width / 4;
    float cell_height4 = cell_height / 4;

    /*glColor3f(1.0, 0.5, 0.0);
    glBegin(GL_QUADS);
    glVertex2i(y * cell_width + MARGIN + cell_width4, x * cell_height + MARGIN + cell_height4);
    glVertex2i(y * cell_width + MARGIN + ( 2 * cell_width4), x * cell_height + MARGIN + cell_height4);
    glVertex2i(y * cell_width + MARGIN + ( 2 * cell_width4), x * cell_height + MARGIN + (2 * cell_height4));
    glVertex2i(y * cell_width + MARGIN + cell_width4, x * cell_height + MARGIN + (2 * cell_height4));
    glEnd();*/
    glColor3f(0.0, 0.0, 0.0);

    glBegin(GL_QUADS);

    glVertex2i(j * cell_width + MARGIN, i * cell_height + MARGIN);
    glVertex2i((j + 1) * cell_width + MARGIN, i * cell_height + MARGIN);

    glVertex2i((j + 1) * cell_width + MARGIN, (i + 1) * cell_height + MARGIN);
    glVertex2i(j * cell_width + MARGIN, (i + 1) * cell_height + MARGIN);

    glEnd();

    glColor3f(1.0, 0.5, 0.0);

    glBegin(GL_QUADS);

    glVertex2i(j * cell_width + MARGIN + cell_width4, i * cell_height + MARGIN + cell_height4);
    glVertex2i(j * cell_width + MARGIN + ( 2 * cell_width4), i * cell_height + MARGIN + cell_height4);

    glVertex2i(j * cell_width + MARGIN + ( 2 * cell_width4), i * cell_height + MARGIN + (2 * cell_height4));
    glVertex2i(j * cell_width + MARGIN + cell_width4, i* cell_height + MARGIN + (2 * cell_height4));

    glEnd();
}