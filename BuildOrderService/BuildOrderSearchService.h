#pragma once

#include "Common.h"
#include "GameState.h"
#include "BuildOrder.h"
#include "Timer.hpp"

namespace BOSS{
class BuildOrderSearchService
{
	RaceID								_race;
	//GameState					        _initialState;
	/*
	BOSS::BuildOrder					_buildOrder;
	
	Timer							    _searchTimer;
	
	void doSearch();
	*/
public:
	
	BuildOrderSearchService(const RaceID race);
	
	//void setState(const GameState state);
	
	/*
	void print();
	
	void search();

	const BuildOrder & getBuildOrder() const;
	*/
	
};
}