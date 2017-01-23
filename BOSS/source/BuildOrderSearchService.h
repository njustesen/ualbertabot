#pragma once

#include "Common.h"
#include "GameState.h"
#include "BuildOrder.h"
#include "Timer.hpp"
#include "..\..\SparCraft\source\Service.h"

namespace BOSS{
	class BuildOrderSearchService
	{
		RaceID								_race;
		
		Timer							    _searchTimer;

	public:

		GameState					        _initialState;
		Service								_service;

		BuildOrderSearchService(const GameState state);

		void setState(const GameState state);
		
		void print();

		void search(BuildOrder & buildOrder, std::vector<int> own_units,
			std::vector<std::string> own_units_under_construction,
			std::vector<int> own_techs,
			std::vector<int> own_techs_under_construction,
			std::vector<int> own_upgrades,
			std::vector<int> own_upgrades_under_construction,
			std::vector<int> opp_units, std::string own_race,
			std::string opp_race, int frame, int min, int gas, bool new_game);

		//const BuildOrder & getBuildOrder() const;
		
	};
}