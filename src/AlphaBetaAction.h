#pragma once
#ifndef ALPHABETAACTION_H
#define ALPHABETAACTION_H
#include "sc2api/sc2_api.h"
#include <memory>

class AlphaBetaUnit;
enum class AlphaBetaActionType { ATTACK, MOVE_BACK, MOVE_FORWARD, WAIT };

class AlphaBetaAction {
public:
	std::shared_ptr<AlphaBetaUnit> unit; // unit� faisant l'action
	std::shared_ptr<AlphaBetaUnit> target; // cible de l'action (si possible)
    sc2::Point2D position; // position du mouvement (si possible)
    float distance; // distance entre unit� et nouvelle position
    AlphaBetaActionType type;
    float time; // temps � laquelle l'action sera termin�e

    AlphaBetaAction(std::shared_ptr<AlphaBetaUnit> punit, std::shared_ptr<AlphaBetaUnit> ptarget, sc2::Point2D pposition, float pdistance, AlphaBetaActionType ptype, float ptime);
};

#endif 