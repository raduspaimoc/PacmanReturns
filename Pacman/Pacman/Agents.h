//
// Created by radu on 27/12/19.
//
#pragma once
//#include "ShareDefines.h"
#include "character.h"
#include "map.h"


struct Agents {
    //Character pacman;
    //std::vector<Character> agents;

public:
        Agents(){};
        Agents(Character pacman);
        std::vector<int> getAction(Map map, int depth);
        Map getResult(Map map, std::vector<int> action, int agent);
        double getUtility(Map map);
        bool isTerminalState(int depth, Map map);
        int maxValue(Map map, int agent, int depth, int alpha, int beta);
        int minValue(Map map, int agent, int depth, int alpha, int beta);
        double evaluationFunction(Map map);
        double betterEvaluationFunction(Map map);
        std::vector<std::vector<int>> getLegalActions(Map map, Character agent);
};
