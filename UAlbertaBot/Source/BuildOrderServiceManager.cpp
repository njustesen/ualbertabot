#include "Common.h"
#include "BuildOrderServiceManager.h"
#include "UnitUtil.h"

using namespace UAlbertaBot;

// get an instance of this
BuildOrderServiceManager & BuildOrderServiceManager::Instance() 
{
	static BuildOrderServiceManager instance;
	return instance;
}

// constructor
BuildOrderServiceManager::BuildOrderServiceManager() 
{
	
}

void BuildOrderServiceManager::update(int currentFrame, int frameSkip, bool newGame)
{

	// Search
	std::vector<int> own_units(256);
	std::vector<std::string> own_units_under_construction(256);
	std::vector<int> own_upgrades_under_construction(64);
	std::vector<int> own_techs_under_construction(64);
	int workers = 0;
	int bases = 0;
	int geysers = 0;
	for (const auto & kv : InformationManager::Instance().getUnitData(BWAPI::Broodwar->self()).getUnits())
	{
		const UnitInfo & ui(kv.second);
		if (ui.type.isWorker()){
			workers++;
		}
		else if (ui.type.isResourceDepot()){
			bases++;
		}
		else if (ui.type.getID() == 157 || ui.type.getID() == 110){
			geysers++;
		}
		if (ui.unit->getRemainingBuildTime() > 0){
			if (own_units_under_construction[ui.type.getID()] == ""){
				own_units_under_construction[ui.type.getID()] = std::to_string(ui.unit->getRemainingBuildTime());
			}
			else {
				own_units_under_construction[ui.type.getID()] += "," + std::to_string(ui.unit->getRemainingBuildTime());
			}
		}
		else {
			own_units[ui.type.getID()]++;
			own_units[85] += ui.unit->getScarabCount();
			own_units[73] += ui.unit->getInterceptorCount();
			if (ui.unit->canCancelTrain() && ui.unit->getType() == BWAPI::UnitTypes::Protoss_Reaver){
				own_units[85] += 1;
			}
			if (ui.unit->canCancelTrain() && ui.unit->getType() == BWAPI::UnitTypes::Protoss_Carrier){
				own_units[73] += 1;
			}
			if (ui.unit->getTech() != BWAPI::TechTypes::None){
				own_techs_under_construction[ui.unit->getTech().getID()] = ui.unit->getRemainingResearchTime();
			}
			if (ui.unit->getUpgrade() != BWAPI::UpgradeTypes::None){
				own_upgrades_under_construction[ui.unit->getUpgrade().getID()] = ui.unit->getRemainingUpgradeTime();
			}
		}

	}
	// Buildings assigned to be build, count as being build
	for (const BWAPI::UnitType & type : BuildingManager::Instance().buildingsQueued())
	{
		if (own_units_under_construction[type.getID()] == ""){
			own_units_under_construction[type.getID()] = std::to_string(type.buildTime());
		}
		else {
			own_units_under_construction[type.getID()] += "," + std::to_string(type.buildTime());
		}
	}

	std::vector<int> opp_units(256);
	for (const auto & kv : InformationManager::Instance().getUnitData(BWAPI::Broodwar->enemy()).getUnits())
	{
		const UnitInfo & ui(kv.second);
		opp_units[ui.type.getID()]++;
	}
	std::vector<int> own_techs(64);
	for (const BWAPI::TechType & techType : BWAPI::TechTypes::allTechTypes())
	{
		if (BWAPI::Broodwar->self()->hasResearched(techType)){
			own_techs[techType.getID()] = 1;
		}
	}
	std::vector<int> own_upgrades(64);
	for (const BWAPI::UpgradeType & upgradeType : BWAPI::UpgradeTypes::allUpgradeTypes())
	{
		if (BWAPI::Broodwar->self()->getUpgradeLevel(upgradeType) > 0 || BWAPI::Broodwar->self()->isUpgrading(upgradeType)){
			own_upgrades[upgradeType.getID()] = 1;
		}
	}

	std::string own_race = BWAPI::Broodwar->self()->getRace().getName();
	std::string opp_race = BWAPI::Broodwar->enemy()->getRace().getName();
	if (bases == 0 || (workers - geysers * 3 < 1 && BWAPI::Broodwar->self()->minerals() < 50)){
		_previousBuildOrder = BOSS::BuildOrder();
	}
	else{
		_searchService->search(_previousBuildOrder, own_units, own_units_under_construction, own_techs, own_techs_under_construction, own_upgrades, own_upgrades_under_construction, opp_units, own_race, opp_race, BWAPI::Broodwar->getFrameCount(), BWAPI::Broodwar->self()->minerals(), BWAPI::Broodwar->self()->gas(), newGame);
	}
	_nextSearchFrame = currentFrame + frameSkip;

}

// start a new search for a new goal
bool BuildOrderServiceManager::isSearchInProgress()
{
	if (_nextSearchFrame == NULL || _nextSearchFrame == 0){
		return false;
	}
	return true;
}

void BuildOrderServiceManager::drawSearchInformation(int x, int y) 
{
	if (!Config::Debug::DrawBuildOrderSearchInfo)
    {
        return;
    }

    // draw the background
    int width = 155;
    int height = 80;
    BWAPI::Broodwar->drawBoxScreen(BWAPI::Position(x-5,y), BWAPI::Position(x+width, y+height), BWAPI::Colors::Black, true);

    x += 5; y+=3;

    BWAPI::Broodwar->drawTextScreen(BWAPI::Position(x, y), "%cBuildOrderSearch:", '\x04');
    y += 10;
    
	// Draw build order
	for (size_t i(0); i < _previousBuildOrder.size(); ++i)
    {
		if (int(_previousBuildOrder[i].buildTime()) > 0)
        {
            y += 10;
			BWAPI::Broodwar->drawTextScreen(BWAPI::Position(x, y), "%d %s", int(_previousBuildOrder[i].buildTime()), _previousBuildOrder[i].getName().c_str());
        }
    }

	BWAPI::Broodwar->drawTextScreen(BWAPI::Position(x, y + 45), "BO Size: %d", (int)_previousBuildOrder.size());
}

void BuildOrderServiceManager::drawStateInformation(int x, int y) 
{
	if (!Config::Debug::DrawBOSSStateInfo)
    {
        return;
    }

    BOSS::GameState currentState(BWAPI::Broodwar, BWAPI::Broodwar->self(), BuildingManager::Instance().buildingsQueued());
    BWAPI::Broodwar->drawTextScreen(BWAPI::Position(x-100, y+30), "\x04%s", currentState.getBuildingData().toString().c_str());
    BWAPI::Broodwar->drawTextScreen(BWAPI::Position(x+150, y), "\x04%s", currentState.toString().c_str());
    
}

// gets the StarcraftState corresponding to the beginning of a Melee game
BOSS::GameState BuildOrderServiceManager::getStartState()
{
    BOSS::GameState state(getRace());
    state.setStartingState();

	return state;
}

const BOSS::RaceID BuildOrderServiceManager::getRace() const
{
    BWAPI::Race r = BWAPI::Broodwar->self()->getRace();
    if (r == BWAPI::Races::Protoss)
    {
        return BOSS::Races::Protoss;
    }
    else if (r == BWAPI::Races::Terran)
    {
        return BOSS::Races::Terran;
    }
    else if (r == BWAPI::Races::Zerg)
    {
        return BOSS::Races::Zerg;
    }
    else
    {
        BOSS_ASSERT(false, "We should have had a valid race from BWAPI");
        return BOSS::Races::None;
    }
}

// converts SearchResults.buildOrder vector into vector of MetaType
std::vector<MetaType> BuildOrderServiceManager::GetMetaVector(const BOSS::BuildOrder & buildOrder)
{
	std::vector<MetaType> metaVector;
    	
	for (size_t i(0); i<buildOrder.size(); ++i)
	{
		metaVector.push_back(GetMetaType(buildOrder[i]));
	}

	return metaVector;
}


BuildOrder BuildOrderServiceManager::getBuildOrder()
{
    return BuildOrder(BWAPI::Broodwar->self()->getRace(), GetMetaVector(_previousBuildOrder));
}

BOSS::ActionType BuildOrderServiceManager::GetActionType(const MetaType & t)
{
	// set the appropriate type
	if (t.isUnit())
	{
		return BOSS::ActionType(t.getUnitType());
	}
	else if (t.isUpgrade())
	{
		return BOSS::ActionType(t.getUpgradeType());
	} 
	else if (t.isTech())
	{
		return BOSS::ActionType(t.getTechType());
	}
	else
	{
		UAB_ASSERT(false, "Should have found a valid type here");
	}
	
	return BOSS::ActionType();
}

MetaType BuildOrderServiceManager::GetMetaType(const BOSS::ActionType & a)
{
	// set the appropriate type
	if (a.isUnit())
	{
		return MetaType(a.getUnitType());
	}
	else if (a.isUpgrade())
	{
		return MetaType(a.getUpgradeType());
	} 
	else if (a.isTech())
	{
		return MetaType(a.getTechType());
	}
	else
	{
		UAB_ASSERT(false, "Should have found a valid type here");
	}
	
	return MetaType();
}
