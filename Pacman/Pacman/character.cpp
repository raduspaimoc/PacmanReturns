#include "ShareDefines.h"
#include "character.h"
#include <GL/glut.h>

#define MARGIN 20
#define MARGIN_2 MARGIN * 2

Character::Character(float x, float y, int r, int c) : x(x), y(y), rows(r), columns(c)
{
    float cell_width = (float) WIDTH / (float) s_columns;
    float cell_height = (float) HEIGHT / (float) s_rows;

    this->x = x * cell_width;
    this->y = y * cell_height;
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
    float cell_width = (float) WIDTH / (float)columns;
    float cell_height = (float) HEIGHT / (float)rows;
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
        else if (hasFlag(CharacterFlags::CHARACTER_FLAG_GHOST))
        {
            x = (float)grid_x * cell_height;
            y = (float)grid_y * cell_width;
        }
    }
}

void Character::draw() {

    float cell_width = (float)WIDTH / (float)columns;
    float cell_height = (float)HEIGHT / (float)rows;
    float cell_width4 = cell_width / 4;
    float cell_height4 = cell_height / 4;

    float j = this->y;
    float i = (rows * cell_height)  - (this->x) - cell_height;

    glColor3f(0.0, 0.0, 0.0);

    if (hasFlag(CharacterFlags::CHARACTER_FLAG_PACMAN))
        glColor3f(1.0, 0.5, 0.0);
    if (hasFlag(CharacterFlags::CHARACTER_FLAG_GHOST))
        glColor3f(1.0, 0.0, 0.0);
    if (hasFlag(CharacterFlags::CHARACTER_FLAG_AUTO_GHOST))
        glColor3f(0.6, 0.0, 0.4);

    glBegin(GL_QUADS);

    glVertex2i(j + cell_width4 + MARGIN, i + cell_height4 + MARGIN);
    glVertex2i(j + ( 2 * cell_width4) + MARGIN, i + cell_height4 + MARGIN);

    glVertex2i(j + ( 2 * cell_width4) + MARGIN, i + (2 * cell_height4) + MARGIN);
    glVertex2i(j + cell_width4 + MARGIN, i+ (2 * cell_height4) + MARGIN);

    glEnd();
}