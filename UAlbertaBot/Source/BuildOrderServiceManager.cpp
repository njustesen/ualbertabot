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

// start a new search for a new goal
void BuildOrderServiceManager::startNewSearch(int currentFrame, int frameSkip)
{
    // convert from UAlbertaBot's meta goal type to BOSS ActionType goal
    try
    {
        BOSS::GameState initialState(BWAPI::Broodwar, BWAPI::Broodwar->self(), BuildingManager::Instance().buildingsQueued());
		_searchService = SearchPtr(new BOSS::BuildOrderSearchService(initialState));
		_previousBuildOrder = _searchService->search();
		_nextSearchFrame = currentFrame + frameSkip;
    }
    catch (const BOSS::BOSSException)
    {
        BWAPI::BroodwarPtr->printf("Exception in BOSS::GameState constructor, will try again next frame");
    }
}

void BuildOrderServiceManager::update(int currentFrame, int frameSkip)
{
	// Only call service every frameSkip frames
	if (currentFrame >= _nextSearchFrame){
		_previousBuildOrder = _searchService->search();
		_nextSearchFrame = currentFrame + frameSkip;
	}
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
