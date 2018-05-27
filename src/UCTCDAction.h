#pragma once

#include "sc2api/sc2_api.h"
#include "UCTCDUnit.h"

enum class UCTCDActionType { ATTACK, MOVE_BACK, MOVE_FORWARD, WAIT };

class UCTCDAction {
public:
    UCTCDUnit unit; // unit� faisant l'action
    UCTCDUnit target; // cible de l'action (si possible)
    sc2::Point2D position; // position du mouvement (si possible)
    float distance; // distance entre unit� et nouvelle position
    UCTCDActionType type;
    float time; // temps � laquelle l'action sera termin�e

    UCTCDAction();
    UCTCDAction(UCTCDUnit punit, UCTCDUnit ptarget, sc2::Point2D pposition, float pdistance, UCTCDActionType ptype, float ptime);
};