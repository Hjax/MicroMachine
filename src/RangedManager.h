#pragma once

#include "Common.h"
#include "MicroManager.h"
#include "UCTCDAction.h"

class CCBot;

class RangedManager : public MicroManager
{
public:

    RangedManager(CCBot & bot);
	inline void setHarassMode(bool harass)
	{
		m_harassMode = harass;
	}
    void setTargets(const std::vector<Unit> & targets);
    void executeMicro();
	bool isTargetRanged(const sc2::Unit * target);

private:
	void RunBehaviorTree(sc2::Units &rangedUnits, sc2::Units &rangedUnitTargets);
	void HarassLogic(sc2::Units &rangedUnits, sc2::Units &rangedUnitTargets);
	CCTilePosition FindSafestPathWithInfluenceMap(const sc2::Unit * rangedUnit, const std::vector<const sc2::Unit *> & threats);
	float getAttackPriority(const sc2::Unit * rangedUnit, const sc2::Unit * target);
	const sc2::Unit * getTarget(const sc2::Unit * rangedUnit, const std::vector<const sc2::Unit *> & targets);
	const std::vector<const sc2::Unit *> getThreats(const sc2::Unit * rangedUnit, const std::vector<const sc2::Unit *> & targets);
	void UCTCD(std::vector<const sc2::Unit *> rangedUnits, std::vector<const sc2::Unit *> rangedUnitTargets);
	void AlphaBetaPruning(std::vector<const sc2::Unit *> rangedUnits, std::vector<const sc2::Unit *> rangedUnitTargets);
	float getThreatRange(const sc2::Unit * rangedUnit, const sc2::Unit * threat);
    std::vector<const sc2::Unit *> lastUnitCommand;
    std::map<const sc2::Unit *, UCTCDAction> command_for_unit;
	std::map<const sc2::Unit *, uint32_t> lastCommandFrameForUnit;
    bool isCommandDone = false;
	bool m_harassMode = false;
};
