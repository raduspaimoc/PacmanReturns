//
// Created by radu on 27/12/19.
//

#include "Agents.h"
#include <limits.h>
#include <cstdlib>
#include <chrono>
#include <random>
#include <algorithm>
#include  <iterator>

/*Agents::Agents() :
{
    pacman.grid_y = pacman.grid_y = 0;

    for (size_t i = 0; i < r; i++) {

        std::vector<Cell> v;
        for (size_t j = 0; j < c; j++)
        {
            bool wall = true;
            if (i % 2 == 0 && j % 2 == 0)
                wall = false;
            if (i % 2 == 1 && j % 2 ==1)
                wall = false;

            v.emplace_back(i, j, wall);
        }

        grid.push_back(v);
    }
};*/


double Agents::evaluationFunction(Map map) {
    double total_score = 0;
    std::vector<Character> auto_ghosts = map.auto_ghosts;
    auto_ghosts.push_back(map.ghost);
    std::vector<Cell> cellsWithFood = map.getCellsWithFood();
    int verg = cellsWithFood.size();
    for (Cell cell : cellsWithFood) {
        if(cell.hasFlag(CellFlags::CELL_FLAG_FOOD))
        {
            Character pacman = map.getAgent(0);
            int manhattan_distance = (pacman.grid_x - (int) cell.x) + abs(pacman.grid_y - (int) cell.y);
            if(manhattan_distance == 0)
                total_score += 100;
            //else if (manhattan_distance == 1)
            //    total_score += 75;
            else
                total_score += 1.0/(manhattan_distance * manhattan_distance);
        }
    }

    for(Character ghost : auto_ghosts)
    {
        int manhattan_distance = (map.pacman.grid_x - (int) ghost.grid_x) + abs(map.pacman.grid_y - (int) ghost.grid_y);
        if(manhattan_distance <= 1)
            total_score -= 100;
        else
            total_score -= 1.0/(manhattan_distance * manhattan_distance);
        /*if(manhattan_distance == 0)
            total_score -= 100;
        else
            total_score -= 1.0/(manhattan_distance * manhattan_distance);*/
    }

    return total_score;
}

Map Agents::getResult(Map map, std::vector<int> action, int agent){
    //Cell* cell = &s_map.grid[(int)s_map.pacman.grid_x + random_action[0]][(int)s_map.pacman.grid_y + random_action[1]];
    //map.pacman.grid_x = map.pacman.grid_x + action[0];
    //map.pacman.grid_y = map.pacman.grid_y + action[1];
    map.generateSuccesor(agent, action);
    return map;
}

double Agents::getUtility(Map map){
    return evaluationFunction(map);
}

bool Agents::isTerminalState(int depth, Map map){
    return depth == 0 or map.pacmanWins() or map.pacmanLoses();
}

int Agents::maxValue(Map map, int agent, int depth, int alpha, int beta){
    if(isTerminalState(depth, map))
        return  getUtility(map);
    int v = -INT_MAX;
    for (std::vector<int> action : getLegalActions(map, map.getAgent(agent))){
        v = std::max(v, minValue(getResult(map, action, agent), 1, depth, alpha, beta));
        if(v > beta)
            return v;
        beta = std::max(alpha, v);
    }
    return v;
    //return 0;
}

int Agents::minValue(Map map, int agent, int depth, int alpha, int beta){
    if(isTerminalState(depth, map))
        return  getUtility(map);
    int v = INT_MAX;
    for (std::vector<int> action : getLegalActions(map, map.getAgent(agent))){
        if(agent == 3){
            v = std::min(v, maxValue(getResult(map, action, agent), 0, depth - 1, alpha, beta));
        } else {
            v = std::min(v, maxValue(getResult(map, action, agent), agent + 1, depth - 1, alpha, beta));
        }
        if(v < alpha)
           return v;
       beta = std::min(beta, v);
    }
    return v;
    //return 0;
}

std::vector<std::vector<int>> Agents::getLegalActions(Map map, Character agent){

    Cell* agentCell = &map.grid[(int)agent.grid_x][(int)agent.grid_y];;
    static std::vector<std::vector<int>> movements = { {0, 1}, {0, -1}, {-1, 0}, {1, 0} };;
    shuffle(movements.begin(), movements.end(), std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count()));
    std::vector<std::vector<int>> final_movements;

    for (auto itr = movements.begin(); itr != movements.end();)
    {
        int i_offset = (*itr)[0];
        int j_offset = (*itr)[1];
        if ((i_offset + agentCell->x < 0) || (j_offset + agentCell->y < 0) || (i_offset + agentCell->x >= map.grid.size()) || (j_offset + agentCell->y >= map.grid[agentCell->x].size()))
        {
            itr++;
            continue;
        }

        // Work in progress generateactions for each agent
        Cell* cell = &map.grid[(int)agent.grid_x + i_offset][(int)agent.grid_y + j_offset];

        if(!cell->hasFlag(CellFlags::CELL_FLAG_WALL))
        {
            //final_movements
            //movements.erase(std::remove(movements.begin(), movements.end(), {i_offset, j_offset}), movements.end());
            final_movements.push_back({i_offset, j_offset});
            //break;
            //movements.rem
        }
        itr++;
    }
    return final_movements;
}

std::vector<int> Agents::getAction(Map map, int depth){
        std::vector<std::vector<int>> final_actions;
        int u = 0;
        int v = -INT_MAX;
        int alpha =  -INT_MAX;
        for (std::vector<int> action : getLegalActions(map, map.getAgent(0))){
            u = minValue(getResult(map, action, 0), 1, depth, alpha, INT_MAX);
            if(u == v){
                final_actions.push_back(action);
            } else if (u >= v) {
                v = u;
                alpha = v;
                final_actions.clear();
                final_actions.push_back(action);
            }
        }
        shuffle(final_actions.begin(), final_actions.end(), std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count()));
        return final_actions[0];

        /*Cell* cell = &map.grid[(int)map.pacman.grid_x + random_action[0]][(int)map.pacman.grid_y + random_action[1]];
        if(map.pacman.hasFlag(CELL_FLAG_FOOD))
            map.total_food--;
        map.pacman.initMovement(cell->x * cell_width - WIDTH_2, cell->y * cell_height - HEIGHT_2, 1000);
        map.pacman.setCell(cell);

        if(map.total_food == 0)
            std::exit(0);*/
}