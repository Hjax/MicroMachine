#include "Micro.h"
#include "Util.h"
#include "CCBot.h"

void Micro::SmartStop(const sc2::Unit * attacker, CCBot & bot)
{
    BOT_ASSERT(attacker != nullptr, "Attacker is null");
    bot.Actions()->UnitCommand(attacker, sc2::ABILITY_ID::STOP);
}

void Micro::SmartAttackUnit(const sc2::Unit * attacker, const sc2::Unit * target, CCBot & bot)
{
    BOT_ASSERT(attacker != nullptr, "Attacker is null");
    BOT_ASSERT(target != nullptr, "Target is null");
    bot.Actions()->UnitCommand(attacker, sc2::ABILITY_ID::ATTACK_ATTACK, target);
}

void Micro::SmartAttackMove(const sc2::Unit * attacker, const sc2::Point2D & targetPosition, CCBot & bot)
{
    BOT_ASSERT(attacker != nullptr, "Attacker is null");
    bot.Actions()->UnitCommand(attacker, sc2::ABILITY_ID::ATTACK_ATTACK, targetPosition);
}

void Micro::SmartMove(const sc2::Unit * attacker, const sc2::Point2D & targetPosition, CCBot & bot)
{
    BOT_ASSERT(attacker != nullptr, "Attacker is null");
    bot.Actions()->UnitCommand(attacker, sc2::ABILITY_ID::MOVE, targetPosition);
}

void Micro::SmartRightClick(const sc2::Unit * unit, const sc2::Unit * target, CCBot & bot)
{
    BOT_ASSERT(unit != nullptr, "Unit is null");
    bot.Actions()->UnitCommand(unit, sc2::ABILITY_ID::SMART, target);
}

void Micro::SmartRepair(const sc2::Unit * unit, const sc2::Unit * target, CCBot & bot)
{
    BOT_ASSERT(unit != nullptr, "Unit is null");
    bot.Actions()->UnitCommand(unit, sc2::ABILITY_ID::SMART, target);
}

void Micro::SmartKiteTarget(const sc2::Unit * rangedUnit, const sc2::Unit * target, CCBot & bot, std::unordered_map<sc2::Tag, KitingFiniteStateMachine*> &state)
{
    BOT_ASSERT(rangedUnit != nullptr, "RangedUnit is null");
    BOT_ASSERT(target != nullptr, "Target is null");

    KitingFiniteStateMachine* stateMachine;
    if (state.find(rangedUnit->tag) == state.end()) {
        stateMachine = new KitingFiniteStateMachine(rangedUnit, target);
    }
    else
        stateMachine = state[rangedUnit->tag];

    stateMachine->update(target, &bot);
    state.insert_or_assign(rangedUnit->tag, stateMachine);
}

void Micro::SmartFocusFire(const sc2::Unit * rangedUnit, const sc2::Unit * target, const std::vector<const sc2::Unit *> * targets, CCBot & bot, std::unordered_map<sc2::Tag, FocusFireFiniteStateMachine*> &state, std::unordered_map<sc2::Tag, float> &unitHealth)
{
    BOT_ASSERT(rangedUnit != nullptr, "RangedUnit is null");
    BOT_ASSERT(target != nullptr, "Target is null");

    FocusFireFiniteStateMachine* stateMachine;
    if (state.find(rangedUnit->tag) == state.end())
        stateMachine = new FocusFireFiniteStateMachine(rangedUnit, target, targets, &bot);
    else stateMachine = state[rangedUnit->tag];
    
    stateMachine->update(target, targets, &unitHealth, &bot);
    state.insert_or_assign(rangedUnit->tag, stateMachine);
}

void Micro::SmartBuild(const sc2::Unit * builder, const sc2::UnitTypeID & buildingType, sc2::Point2D pos, CCBot & bot)
{
    BOT_ASSERT(builder != nullptr, "Builder is null");
    bot.Actions()->UnitCommand(builder, bot.Data(buildingType).buildAbility, pos);
}

void Micro::SmartBuildTarget(const sc2::Unit * builder, const sc2::UnitTypeID & buildingType, const sc2::Unit * target, CCBot & bot)
{
    BOT_ASSERT(builder != nullptr, "Builder is null");
    BOT_ASSERT(target != nullptr, "Target is null");
    bot.Actions()->UnitCommand(builder, bot.Data(buildingType).buildAbility, target);
}

void Micro::SmartTrain(const sc2::Unit * builder, const sc2::UnitTypeID & buildingType, CCBot & bot)
{
    BOT_ASSERT(builder != nullptr, "Builder is null");
    bot.Actions()->UnitCommand(builder, bot.Data(buildingType).buildAbility);
}

void Micro::SmartAbility(const sc2::Unit * builder, const sc2::AbilityID & abilityID, CCBot & bot)
{
    BOT_ASSERT(builder != nullptr, "Builder is null");
    bot.Actions()->UnitCommand(builder, abilityID);
}