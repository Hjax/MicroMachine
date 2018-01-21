#pragma once
#ifndef ALPHABETAACTION_H
#define ALPHABETAACTION_H
#include "sc2api/sc2_api.h"

class AlphaBetaUnit;
enum class AlphaBetaActionType { ATTACK, MOVE, WAIT };

class AlphaBetaAction {
public:
    AlphaBetaUnit * unit; // unit� faisant l'action
    AlphaBetaUnit * target; // cible de l'action (si possible)
    sc2::Point2D position; // position du mouvement (si possible)
    float distance; // distance entre unit� et nouvelle position
    AlphaBetaActionType type;
    long time; // temps � laquelle l'action a �t� faite

    AlphaBetaAction(AlphaBetaUnit * punit, AlphaBetaUnit * ptarget, sc2::Point2D pposition, float pdistance, AlphaBetaActionType ptype, long ptime); 
};

#endif 