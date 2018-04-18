#include "AttackFSMState.h"
#include "FleeFSMState.h"
#include "ShouldFleeTransition.h"
#include "CCBot.h"

AttackFSMState::AttackFSMState(const sc2::Unit * unit, const sc2::Unit * target)
{
    m_unit = unit;
    m_target = target;
    actionSent = false;
}

void AttackFSMState::onEnter()
{
    KitingFSMState* flee = new FleeFSMState(m_unit, m_target);
    KitingFSMTransition* shouldFlee = new ShouldFleeTransition(m_unit, m_target, flee);
    transitions = { shouldFlee };
}
void AttackFSMState::onExit() {
    actionSent = false;
}
std::vector<KitingFSMTransition*> AttackFSMState::getTransitions()
{
    return transitions;
}
void AttackFSMState::onUpdate(const sc2::Unit * target, CCBot* bot)
{
    //TODO move in another FSMState
    if (m_unit->buffs.size() < 1)
    {
        auto query = bot->Query();
        auto abilities = bot->Observation()->GetAbilityData();
        sc2::AvailableAbilities available_abilities = query->GetAbilitiesForUnit(m_unit);
        if (available_abilities.abilities.size() < 1)
        {
            std::cout << "No abilities available for this unit" << std::endl;
        }
        else
        {
            for (const sc2::AvailableAbility & available_ability : available_abilities.abilities)
            {
                if (available_ability.ability_id >= abilities.size()) { continue; }
                const sc2::AbilityData & ability = abilities[available_ability.ability_id];
                if (ability.ability_id == sc2::ABILITY_ID::EFFECT_STIM) {
                    bot->Actions()->UnitCommand(m_unit, ability.ability_id);
                    return;
                }
            }
        }
    }


    if (!actionSent)
    {
        m_target = target;
        bot->Actions()->UnitCommand(m_unit, sc2::ABILITY_ID::ATTACK_ATTACK, target);
        actionSent = true;
    }
}
