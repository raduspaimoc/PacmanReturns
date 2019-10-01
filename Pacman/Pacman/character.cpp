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
        draw();
        //last_t = 0;
    }
}

/*void Character::emptyCell(){
    Cell* cell = visited;
    float cell_width = (float)WIDTH / (float)c;
    float cell_height = (float)HEIGHT / (float)r;
    float cell_width4 = cell_width / 4;
    float cell_height4 = cell_height / 4;

    int fake_i = r - cell->x - 1;

    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_QUADS);

    glVertex2i(cell->y * cell_width + MARGIN, fake_i * cell_height + MARGIN);
    glVertex2i((cell->y + 1) * cell_width + MARGIN, fake_i * cell_height + MARGIN);

    glVertex2i((cell->y + 1) * cell_width + MARGIN, (fake_i + 1) * cell_height + MARGIN);
    glVertex2i(cell->y * cell_width + MARGIN, (fake_i + 1) * cell_height + MARGIN);

    glEnd();

    glColor3f(0.0, 0.0, 0.0);

    glBegin(GL_QUADS);

    glVertex2i(cell->y * cell_width + MARGIN + cell_width4, fake_i * cell_height + MARGIN + cell_height4);
    glVertex2i(cell->y * cell_width + MARGIN + ( 2 * cell_width4), fake_i * cell_height + MARGIN + cell_height4);

    glVertex2i(cell->y * cell_width + MARGIN + ( 2 * cell_width4), fake_i * cell_height + MARGIN + (2 * cell_height4));
    glVertex2i(cell->y * cell_width + MARGIN + cell_width4, fake_i * cell_height + MARGIN + (2 * cell_height4));

    glEnd();
}*/


//[WIP]
void Character::draw() {

    float cell_width = (float)WIDTH / (float)c;
    float cell_height = (float)HEIGHT / (float)r;
    float cell_width4 = cell_width / 4;
    float cell_height4 = cell_height / 4;
    //float j = 0;
    //if(this->y > 0)
    float j = this->y;
    float i = (r * cell_height)  - (this->x) - cell_height;

    glColor3f(0.0, 0.0, 0.0);

    if (!set){
        glColor3f(0.0, 0.0, 0.0);

        glBegin(GL_QUADS);

        glVertex2i(j + MARGIN, i + MARGIN);
        glVertex2i(j + cell_width + MARGIN, i + MARGIN);

        glVertex2i(j + cell_width + MARGIN, i + cell_height + MARGIN);
        glVertex2i(j + MARGIN, i + cell_height + MARGIN);

        glEnd();
        set = true;
    }

    /*if(state == QUIET){
        glColor3f(0.0, 0.0, 0.0);

        glBegin(GL_QUADS);

        glVertex2i(j + cell_width4 + MARGIN, i + cell_height4 + MARGIN);
        glVertex2i(j + ( 2 * cell_width4) + MARGIN, i + cell_height4 + MARGIN);

        glVertex2i(j + ( 2 * cell_width4) + MARGIN, i + (2 * cell_height4) + MARGIN);
        glVertex2i(j + cell_width4 + MARGIN, i+ (2 * cell_height4) + MARGIN);

        glEnd();
    }*/


    glColor3f(1.0, 0.5, 0.0);

    glBegin(GL_QUADS);

    glVertex2i(j + cell_width4 + MARGIN, i + cell_height4 + MARGIN);
    glVertex2i(j + ( 2 * cell_width4) + MARGIN, i + cell_height4 + MARGIN);

    glVertex2i(j + ( 2 * cell_width4) + MARGIN, i + (2 * cell_height4) + MARGIN);
    glVertex2i(j + cell_width4 + MARGIN, i+ (2 * cell_height4) + MARGIN);

    glEnd();
}

/*void Character::draw() {
    int j = this->y;
    //int i = this->x;
    int i = r - this->x - 1;
    float cell_width = (float)WIDTH / (float)c;
    float cell_height = (float)HEIGHT / (float)r;
    float cell_width4 = cell_width / 4;
    float cell_height4 = cell_height / 4;

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
}*/