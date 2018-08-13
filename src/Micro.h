#pragma once

#include "Common.h"
#include "sc2api/sc2_api.h"
#include "FocusFireFiniteStateMachine.h"
#include "KitingFiniteStateMachine.h"

class CCBot;
namespace Micro
{   
    void SmartStop          (const sc2::Unit * attacker,  CCBot & bot);
    void SmartAttackUnit    (const sc2::Unit * attacker,  const sc2::Unit * target, CCBot & bot);
    void SmartAttackMove    (const sc2::Unit * attacker,  const sc2::Point2D & targetPosition, CCBot & bot);
    void SmartMove          (const sc2::Unit * attacker,  const sc2::Point2D & targetPosition, CCBot & bot);
    void SmartRightClick    (const sc2::Unit * unit,      const sc2::Unit * target, CCBot & bot);
    void SmartRepair        (const sc2::Unit * unit,      const sc2::Unit * target, CCBot & bot);
    void SmartKiteTarget    (const sc2::Unit * rangedUnit,const sc2::Unit * target, CCBot & bot, std::unordered_map<sc2::Tag, KitingFiniteStateMachine*> &states);
	void SmartFocusFire     (
        const sc2::Unit * rangedUnit,
        const sc2::Unit * target, 
        const std::vector<const sc2::Unit *> * targets, 
        CCBot & bot, 
        std::unordered_map<sc2::Tag, FocusFireFiniteStateMachine*> &states,
        const std::vector<const sc2::Unit *> * units,
        std::unordered_map<sc2::Tag, float> &unitHealth
    );
    void SmartAbility       (const sc2::Unit * builder, const sc2::AbilityID & abilityID, CCBot & bot);
	void SmartAbility		(const sc2::Unit * builder, const sc2::AbilityID & abilityID, CCBot & bot, const sc2::Point2D& point);
};