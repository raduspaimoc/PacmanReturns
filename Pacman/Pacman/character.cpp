#include "character.h"
#include <GL/glut.h>
#include <limits.h>
#include <cstdlib>
#include <chrono>
#include <random>
#include <algorithm>

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

double Character::evaluationFunction(Map map) {
    double total_score = 0;
    std::vector<Character> auto_ghosts = map.auto_ghosts;
    auto_ghosts.push_back(map.ghost);

    for (Cell cell : map.getCellsWithFood()) {
        if(cell.hasFlag(CellFlags::CELL_FLAG_FOOD))
        {
            /*Manhattan fucking distance*/
            int manhattan_distance = (this->grid_x - (int) cell.x) + abs(this->grid_y - (int) cell.y);
            if(manhattan_distance == 0)
                total_score += 100;
            else
                total_score += 1.0/(manhattan_distance * manhattan_distance);
        }
    }

    for(Character ghost : auto_ghosts)
    {
        int manhattan_distance = (this->grid_x - (int) ghost.grid_x) + abs(this->grid_y - (int) ghost.grid_y);
        if(manhattan_distance == 0)
            total_score -= 200;
        else
            total_score -= 1.0/(manhattan_distance * manhattan_distance);
    }

    return total_score;
}

void Character::getResult(){

}

void Character::getUtility(){

}

bool Character::isTerminalState(int depth, Map map){
    return depth == 0 or map.pacmanWins() or map.pacmanLoses();
}

int Character::maxValue(){
    return 0;
}

int Character::minValue(){
    return 0;
}

std::vector<std::vector<int>> Character::getLegalActions(Map map){
    Cell* pacman = &s_map.grid[(int)map.pacman.grid_x][(int)map.pacman.grid_y];;
    static std::vector<std::vector<int>> movements = direct;
    shuffle(movements.begin(), movements.end(), std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count()));
    static std::vector<std::vector<int>> final_movements;

    for (auto itr = movements.begin(); itr != movements.end();)
    {
        int i_offset = (*itr)[0];
        int j_offset = (*itr)[1];
        if ((i_offset + pacman->x < 0) || (j_offset + pacman->y < 0) || (i_offset + pacman->x >= s_map.grid.size()) || (j_offset + pacman->y >= s_map.grid[pacman->x].size()))
        {
            itr++;
            continue;
        }

        Cell* cell = &s_map.grid[(int)s_map.pacman.grid_x + i_offset][(int)s_map.pacman.grid_y + j_offset];

        if(!cell->hasFlag(CellFlags::CELL_FLAG_WALL))
        {
            final_movements.push_back({i_offset, j_offset});
            /*s_map.pacman.grid_x = cell->x;
            s_map.pacman.grid_y = cell->y;

            float cell_width = (float)WIDTH / (float)s_columns;
            float cell_height = (float)HEIGHT / (float)s_rows;

            if(s_map.pacman.hasFlag(CELL_FLAG_FOOD))
                s_map.total_food--;*/

            /*s_map.pacman.initMovement(cell->x * cell_width - WIDTH_2, cell->y * cell_height - HEIGHT_2, 1000);
            s_map.pacman.setCell(cell);

            if(s_map.total_food == 0)
                std::exit(0);*/

            break;
        }
        itr++;
    }
    return final_movements;
}

void Character::getAction(Map map, std::vector<std::vector<int>> actions, int depth){
    /*
     import sys
        def result(gameState, agent, action):
            return gameState.generateSuccessor(agent, action)

        def utility(gameState):
            return self.evaluationFunction(gameState)

        def terminalTest(gameState, depth):
            return depth == 0 or gameState.isWin() or gameState.isLose()

        def max_value(gameState, agent, depth, alpha, beta):
            if terminalTest(gameState, depth): return utility(gameState)
            v = -sys.maxint
            for a in gameState.getLegalActions(agent):
                v = max(v,min_value(result(gameState,agent,a),1,depth, alpha, beta))
                if v > beta:
                    return v

                alpha = max(alpha, v)

            return v

        def min_value(gameState, agent, depth, alpha, beta):
            if terminalTest(gameState, depth): return utility(gameState)
            v = sys.maxint
            for a in gameState.getLegalActions(agent):
                if (agent == gameState.getNumAgents()-1):
                    v = min(v, max_value(result(gameState,agent,a), 0, depth-1, alpha, beta))
                else:
                    v = min(v, min_value(result(gameState,agent,a), agent+1, depth, alpha, beta))

                if v < alpha:
                    return v

                beta = min(beta, v)

            return v


        v = -sys.maxint
        actions = []
        alpha = -sys.maxint
        for a in gameState.getLegalActions(0):
            u = min_value(result(gameState, 0, a), 1, self.depth, alpha, sys.maxint)

            if u == v:
                actions.append(a)
            elif u >= v:
                v = u
                alpha = v
                actions = [a]

        return random.choice(actions)*/
        std::vector<std::vector<int>> final_actions;
        int u = 0;
        int v = -INT8_MAX;
        int alpha =  INT8_MAX;
        for (std::vector<int> action : getLegalActions(map)){
            u = minValue();
            if(u == v){
                final_actions.push_back(action);
            } else if (u >= v) {
                v = u;
                alpha = v;
                // Work in progress
                //actions =
            }
        }

        //s_map.pacman.initMovement(cell->x * cell_width - WIDTH_2, cell->y * cell_height - HEIGHT_2, 1000);
        //s_map.pacman.setCell(cell);
}

void Character::initMovement(float destination_x, float destination_y, float duration)
{

    //if(destination_x != 0 && destination_y != 0){
    vx = (destination_x - x)/duration;
    vy = (destination_y - y)/duration;
    dir[0] = vy;
    dir[1] = 0;
    dir[2] = -vx;

    state = MOVE;
    time_remaining = (long) duration;
    //}

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
            //if(s_map.pacman.hasFlag(CELL_FLAG_FOOD))
            if(visited->hasFlag(CellFlags::CELL_FLAG_FOOD)){
                s_map.total_food--;
                s_map.score++;
                printf("%d\n", s_map.total_food);
                visited->removeFlag(CellFlags::CELL_FLAG_FOOD);
            }
            if(s_map.total_food == 0)
                std::exit(0);
            visited->addFlag(CellFlags::CELL_FLAG_EMPTY);

        }
        //visited = s_map.grid[x][y];
        current_cell = s_map.grid[grid_x][grid_y];
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