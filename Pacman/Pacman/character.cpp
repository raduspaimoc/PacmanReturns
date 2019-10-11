#include "ShareDefines.h"
#include "character.h"
#include <GL/glut.h>

Character::Character(float x, float y, int r, int c, unsigned int flags) : rows(r), columns(c), grid_x(x), grid_y(y), flags(flags)
{
    float cell_width = (float) WIDTH / (float) s_columns;
    float cell_height = (float) HEIGHT / (float) s_rows;

    this->x = (x * cell_width) - WIDTH_2;
    this->y = y * cell_height - HEIGHT_2;
    this->dir[0] = 0;
    this->dir[1] = 0;
    this->dir[2] = 1;
}

void Character::initMovement(float destination_x, float destination_y, float duration)
{
    vx = (destination_x - x)/duration;
    vy = (destination_y - y)/duration;
    state = MOVE;
    time_remaining = (long) duration;

}

void Character::integrate(long t)
{
    if (state==MOVE && t<time_remaining)
    {
        x = x + vx* (float)t;
        y = y + vy* (float)t;
        time_remaining-=t;
    }
    else if (state==MOVE && t>=time_remaining)
    {
        x = x + vx*(float)time_remaining;
        y = y + vy*(float)time_remaining;
        state=QUIET;

        if (hasFlag(CharacterFlags::CHARACTER_FLAG_PACMAN))
        {
            visited->removeFlag(CellFlags::CELL_FLAG_FOOD);
            visited->addFlag(CellFlags::CELL_FLAG_EMPTY);
        }
    }
}

void Character::draw() {

    float cell_width = (float)WIDTH / (float)columns;
    float cell_height = (float)HEIGHT / (float)rows;
    float cell_width4 = cell_width / 4;
    float cell_height4 = cell_height / 4;

    glShadeModel(GL_SMOOTH);
    GLfloat material[4] = {0.0, 0.0, 0.0, 1.0};

    if (hasFlag(CharacterFlags::CHARACTER_FLAG_PACMAN))
    {
        material[0]=1.0; material[1]=0.5; material[2]=0.0; material[3]=1.0;
    }
    if (hasFlag(CharacterFlags::CHARACTER_FLAG_GHOST))
    {
        material[0]=1.0; material[1]=0.0; material[2]=0.0; material[3]=1.0;
    }
    if (hasFlag(CharacterFlags::CHARACTER_FLAG_AUTO_GHOST))
    {
        material[0]=0.6; material[1]=0.0; material[2]=0.4; material[3]=1.0;
    }

    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE, material);

    glPopMatrix();
    glPushMatrix();
    glTranslatef(y + (2 * cell_width4), DEPTH, -x  + (2 * cell_height4) - 1*cell_height);
    gluSphere( gluNewQuadric(), cell_width4, 10, 10); // Tried r = 0.02f, 0.2f, 2.0f, 20.0f, none works
    glPopMatrix();
}