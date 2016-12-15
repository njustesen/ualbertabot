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

		BuildOrder & search();

		//const BuildOrder & getBuildOrder() const;
		

	};
}