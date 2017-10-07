#include "RangedManager.h"
#include "Util.h"
#include "CCBot.h"
#include "Behavior.h"

// TODO: BEHAVIORS TO IMPLEMENT
/*
- search enemies
- Focus fire w / o overkill
- kiting
- send injured units to back
- flee
*/

/*
Le BT ressemblerait  quelque chose comme:
BehaviorTree* bt = BehaviorTreeBuilder()
    .sequence() // attack
        .sequence()
            .condition(EnemyInSight)
        .condition(EnemyInRange)
            .selector()
            .condition(IsEnemyMelee)
                .action(Kite)
            .condition(IsUnitHurt)
                .sequence()
                    .action(SendToBack)
                    .action(Focus)
            .action(Focus)
    .sequence()
        .action(search)
    .end();
*/

RangedManager::RangedManager(CCBot & bot)
    : MicroManager(bot)
{

}

void RangedManager::executeMicro(const std::vector<const sc2::Unit *> & targets)
{
	// build bt tree here
    assignTargets(targets);
}

void RangedManager::assignTargets(const std::vector<const sc2::Unit *> & targets)
{
    const std::vector<const sc2::Unit *> & rangedUnits = getUnits();

    // figure out targets
    std::vector<const sc2::Unit *> rangedUnitTargets;
    for (auto target : targets)
    {
        if (!target) { continue; }
        if (target->unit_type == sc2::UNIT_TYPEID::ZERG_EGG) { continue; }
        if (target->unit_type == sc2::UNIT_TYPEID::ZERG_LARVA) { continue; }

        rangedUnitTargets.push_back(target);
    }

    // for each rangedUnit
    for (auto rangedUnit : rangedUnits)
    {
        BOT_ASSERT(rangedUnit, "ranged unit is null");

        // if the order is to attack or defend
        if (order.getType() == SquadOrderTypes::Attack || order.getType() == SquadOrderTypes::Defend)
        {
            if (!rangedUnitTargets.empty())
            {
                // find the best target for this rangedUnit
                const sc2::Unit * target = getTarget(rangedUnit, rangedUnitTargets);

                // attack it
                if (m_bot.Config().KiteWithRangedUnits)
                {
                    Micro::SmartKiteTarget(rangedUnit, target, m_bot);
                }
                else
                {
                    Micro::SmartAttackUnit(rangedUnit, target, m_bot);
                }
            }
            // if there are no targets
            else
            {
                // if we're not near the order position
                if (Util::Dist(rangedUnit->pos, order.getPosition()) > 4)
                {
                    // move to it
                    Micro::SmartMove(rangedUnit, order.getPosition(), m_bot);
                }
            }
        }

        if (m_bot.Config().DrawUnitTargetInfo)
        {
            // TODO: draw the line to the unit's target
        }
    }
}

// get a target for the ranged unit to attack
// TODO: this is the melee targeting code, replace it with something better for ranged units
const sc2::Unit * RangedManager::getTarget(const sc2::Unit * rangedUnit, const std::vector<const sc2::Unit *> & targets)
{
    BOT_ASSERT(rangedUnit, "null ranged unit in getTarget");

    int highPriority = 0;
    double closestDist = std::numeric_limits<double>::max();
    const sc2::Unit * closestTarget = nullptr;

    // for each target possiblity
    for (auto targetUnit : targets)
    {
        BOT_ASSERT(targetUnit, "null target unit in getTarget");

        int priority = getAttackPriority(rangedUnit, targetUnit);
        float distance = Util::Dist(rangedUnit->pos, targetUnit->pos);

        // if it's a higher priority, or it's closer, set it
        if (!closestTarget || (priority > highPriority) || (priority == highPriority && distance < closestDist))
        {
            closestDist = distance;
            highPriority = priority;
            closestTarget = targetUnit;
        }
    }

    return closestTarget;
}

// TODO: instead of returning an hardcoded int, calculate a threat score based on dps, distance and other factors
float RangedManager::getAttackPriority(const sc2::Unit * attacker, const sc2::Unit * unit)
{
    BOT_ASSERT(unit, "null unit in getAttackPriority");

    if (Util::IsCombatUnit(unit, m_bot))
    {
        sc2::UnitTypeData unitTypeData = Util::GetUnitTypeDataFromUnitTypeId(unit->unit_type, m_bot);
        float dps = 0.f;
        for (sc2::Weapon & weapon : unitTypeData.weapons)
        {
            float weaponDps = weapon.attacks * weapon.damage_ * (1 / weapon.speed);
            if (weaponDps > dps)
                dps = weaponDps;
        }
        return dps;
        /*if (unit->unit_type == sc2::UNIT_TYPEID::ZERG_BANELING)
            return 11;
        return 10;*/
    }

    if (Util::IsWorker(unit))
    {
        return 2;
    }

    return 1;
}

