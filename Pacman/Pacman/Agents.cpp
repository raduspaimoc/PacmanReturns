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
#include <deque>          // std::deque
#include <queue>          // std::queue
#include <cmath>

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
/*double Agents::betterEvaluationFunction(Map map){
    std::vector<Cell> cellsWithFood = map.getCellsWithFood();
    if(cellsWithFood.size() == 0)
        return 10000;
    double score = 0;
    if(map.grid[(int)map.pacman.grid_x][(int)map.pacman.grid_y + 1].hasFlag(CellFlags::CELL_FLAG_WALL) && map.grid[(int)map.pacman.grid_x][(int)map.pacman.grid_y - 1].hasFlag(CellFlags::CELL_FLAG_WALL)
       && map.grid[(int)map.pacman.grid_x + 1][(int)map.pacman.grid_y].hasFlag(CellFlags::CELL_FLAG_WALL))
        score -= 100;
    if(map.grid[(int)map.pacman.grid_x][(int)map.pacman.grid_y + 1].hasFlag(CellFlags::CELL_FLAG_WALL) && map.grid[(int)map.pacman.grid_x][(int)map.pacman.grid_y - 1].hasFlag(CellFlags::CELL_FLAG_WALL)
       && map.grid[(int)map.pacman.grid_x - 1][(int)map.pacman.grid_y].hasFlag(CellFlags::CELL_FLAG_WALL))
        score -= 100;
    if(map.grid[(int)map.pacman.grid_x + 1][(int)map.pacman.grid_y].hasFlag(CellFlags::CELL_FLAG_WALL) && map.grid[(int)map.pacman.grid_x - 1][(int)map.pacman.grid_y].hasFlag(CellFlags::CELL_FLAG_WALL)
       && map.grid[(int)map.pacman.grid_x][(int)map.pacman.grid_y + 1].hasFlag(CellFlags::CELL_FLAG_WALL))
        score -= 100;
    if(map.grid[(int)map.pacman.grid_x + 1][(int)map.pacman.grid_y].hasFlag(CellFlags::CELL_FLAG_WALL) && map.grid[(int)map.pacman.grid_x - 1][(int)map.pacman.grid_y].hasFlag(CellFlags::CELL_FLAG_WALL)
       && map.grid[(int)map.pacman.grid_x][(int)map.pacman.grid_y - 1].hasFlag(CellFlags::CELL_FLAG_WALL))
        score -= 100;

    for (Character character: map.getGhosts()){
        int manhattan_distance = abs(map.pacman.grid_x - (int) character.x) + abs(map.pacman.grid_y - (int) character.y);
        if(manhattan_distance < 2)
            return -9999;
    }

    double minDis = 10000;
    for (Cell food : cellsWithFood){
        double manhattan_distance = abs(map.pacman.grid_x - (int) food.x) + abs(map.pacman.grid_y - (int) food.y);
        minDis = std::min(minDis, manhattan_distance);
    }

    return score + 5000.0 / ((cellsWithFood.size()+ 1) - minDis);
}*/

double Agents::betterEvaluationFunction(Map map) {
    int pac_x = map.pacman.grid_x;
    int pac_y = map.pacman.grid_y;
    int currentScore = map.getTotalScore();

    if(map.pacmanLoses())
        return -INT_MAX;

    if(map.pacmanWins())
        return INT_MAX;

    std::vector<Cell> cellsWithFood = map.getCellsWithFood();
    int distanceToClosestFood = INT_MAX;
    for(Cell food : cellsWithFood){
        int manhattan_distance = abs(pac_x - (int) food.x) + abs(pac_y - (int) food.y);
        distanceToClosestFood = std::min(distanceToClosestFood, manhattan_distance);
    }

    int numberOfFoodLeft = cellsWithFood.size();

    int distanceToClosestGhost = INT_MAX;
    for(Character ghost  : map.getGhosts()){
        int manhattan_distance = abs(pac_x - (int) ghost.grid_x) + abs(pac_y - (int) ghost.grid_y);
        distanceToClosestGhost = std::min(distanceToClosestGhost, manhattan_distance);
    }

    double score =  1    * currentScore + \
          -1.5 * distanceToClosestFood + \
          -2    * (1./distanceToClosestGhost) + \
          -4    * numberOfFoodLeft;
    return score;

   /*

  # number of big dots
  # if we only count the number fo them, he'll only care about
  # them if he has the opportunity to eat one.
  numberOfCapsulesLeft = len(currentGameState.getCapsules())

  # number of foods left
  numberOfFoodsLeft = len(foodlist)

  # ghost distance

  # active ghosts are ghosts that aren't scared.
  scaredGhosts, activeGhosts = [], []
  for ghost in currentGameState.getGhostStates():
    if not ghost.scaredTimer:
      activeGhosts.append(ghost)
    else:
      scaredGhosts.append(ghost)

  def getManhattanDistances(ghosts):
    return map(lambda g: util.manhattanDistance(pos, g.getPosition()), ghosts)

  distanceToClosestActiveGhost = distanceToClosestScaredGhost = 0

  if activeGhosts:
    distanceToClosestActiveGhost = min(getManhattanDistances(activeGhosts))
  else:
    distanceToClosestActiveGhost = float("inf")
  distanceToClosestActiveGhost = max(distanceToClosestActiveGhost, 5)

  if scaredGhosts:
    distanceToClosestScaredGhost = min(getManhattanDistances(scaredGhosts))
  else:
    distanceToClosestScaredGhost = 0 # I don't want it to count if there aren't any scared ghosts

  outputTable = [["dist to closest food", -1.5*distanceToClosestFood],
                 ["dist to closest active ghost", 2*(1./distanceToClosestActiveGhost)],
                 ["dist to closest scared ghost", 2*distanceToClosestScaredGhost],
                 ["number of capsules left", -3.5*numberOfCapsulesLeft],
                 ["number of total foods left", 2*(1./numberOfFoodsLeft)]]

  score = 1    * currentScore + \
          -1.5 * distanceToClosestFood + \
          -2    * (1./distanceToClosestActiveGhost) + \
          -2    * distanceToClosestScaredGhost + \
          -20 * numberOfCapsulesLeft + \
          -4    * numberOfFoodsLeft
  return score*/
}

/*double Agents::betterEvaluationFunction(Map map) {
    double foodDistance = 9999999;
    double ghostDistance = 9999999;
    double averageDistance = 0;
    double manhattan_distance;

    for(Cell food : map.getCellsWithFood()){
        manhattan_distance = abs(map.pacman.grid_x - (int) food.x) + abs(map.pacman.grid_y - (int) food.y);
        foodDistance = std::min(manhattan_distance, foodDistance);
    }

    foodDistance += 1;
    int foodCount = map.getCellsWithFood().size() + 1;

    for (Character character: map.getGhosts()){
        manhattan_distance = abs(map.pacman.grid_x - (int) character.x) + abs(map.pacman.grid_y - (int) character.y);
        averageDistance += manhattan_distance;
        ghostDistance = std::min(manhattan_distance, ghostDistance);
    }

    double currentScore = evaluationFunction(map) + 1;
    if(ghostDistance < 2)
        ghostDistance = -100;
    else
        ghostDistance = 0;

    return ghostDistance + 1.0/foodDistance + currentScore;


}*/


double Agents::evaluationFunction(Map map) {
    double total_score = 0;
    std::vector<Character> auto_ghosts = map.auto_ghosts;
    auto_ghosts.push_back(map.ghost);
    for (Cell cell : map.getCellsWithFood()) {
        if(cell.hasFlag(CellFlags::CELL_FLAG_FOOD))
        {
            Character pacman = map.getAgent(0);
            int manhattan_distance = abs(pacman.current_cell.x - (int) cell.x) + abs(pacman.current_cell.y - (int) cell.y);
            if(manhattan_distance == 0)
                total_score += 100;
            else
                total_score += 1.0/(manhattan_distance * manhattan_distance);
        }
    }

    for(Character ghost : auto_ghosts)
    {
        //double manhattan_distance = abs((map.pacman.x - ghost.x)) + abs(map.pacman.y - ghost.y);
        double manhattan_distance = abs((map.pacman.current_cell.x - ghost.current_cell.x)) + abs(map.pacman.current_cell.y - ghost.current_cell.y);
        if(manhattan_distance <= 2)
            total_score -= 100;
        else
            total_score -= 1.0/(manhattan_distance * manhattan_distance);
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
    //return betterEvaluationFunction(map);
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
        //if(agentCell)
        if ((i_offset + agent.grid_x < 0) || (j_offset + agent.grid_y < 0) || (i_offset + agent.grid_x >= map.grid.size()) || (j_offset + agent.grid_y >= map.grid[agent.grid_x].size()))
        {
            itr++;
            continue;
        }

        // Work in progress generateactions for each agent
        Cell* cell = &map.grid[agent.grid_x + i_offset][agent.grid_y + j_offset];
        //bool available = (cell != NULL && cell->flags != NULL && cell->x!= NULL && cell->y!= NULL && cell->movementFlags!= NULL);
        if(cell != NULL && cell->flags != NULL && !cell->hasFlag(CellFlags::CELL_FLAG_WALL))
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