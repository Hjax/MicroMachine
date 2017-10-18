#pragma once
#include "sc2api/sc2_api.h"
#include "FocusFireFSM.h"

class ShouldPullBackTransition : public FocusFireFSMTransition
{
private:
    FocusFireFSMState* m_nextState;
    const sc2::Unit * m_unit;
    const sc2::Unit * m_target;

public:
    ShouldPullBackTransition(const sc2::Unit * unit, const sc2::Unit * target, FocusFireFSMState* nextState);
    bool isValid(std::map<sc2::Tag, float> *, CCBot*);
    FocusFireFSMState* getNextState();
    void onTransition();
};