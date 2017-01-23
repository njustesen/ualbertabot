#include "BuildOrderSearchService.h"

#include <string>
#include <sstream>
#include <vector>

using namespace BOSS;

BuildOrderSearchService::BuildOrderSearchService(const GameState state)
	: _initialState(state), _service()
{

}

void BuildOrderSearchService::search(BuildOrder & buildOrder, std::vector<int> own_units, std::vector<int> own_techs, std::vector<int> own_upgrades, std::vector<int> opp_units, std::string own_race, std::string opp_race, int frame, int min, int gas, bool new_game)
{
	
	// Call service
	std::string message = _service.call(own_units, own_techs, own_upgrades, opp_units, own_race, opp_race, frame, min, gas, new_game);
	//std::string message = _service.cal();

	// Check for errors
	if (message.length() >= 9 && message.substr(0, 8) == "Exception"){
		return;
	}

	// Clear build order
	buildOrder = BOSS::BuildOrder();

	// If service returns -1, return with empty build order
	if (message == "" || message == "-1"){
		return;
	}

	std::string prefix = message.substr(0, 4);
	std::string substring = message.substr(4, message.length() - 1);
	int id;
	std::stringstream(substring) >> id;
	if (prefix == "unit"){
		BWAPI::UnitType* unitType = new BWAPI::UnitType(id);
		ActionType* type = new ActionType(*unitType);
		buildOrder.add(*type);
	}
	else if (prefix == "tech"){
		BWAPI::TechType* techType = new BWAPI::TechType(id);
		ActionType* type = new ActionType(*techType);
		buildOrder.add(*type);
	}
	else if (prefix == "upgr"){
		BWAPI::UpgradeType* upgradeType = new BWAPI::UpgradeType(id);
		ActionType* type = new ActionType(*upgradeType);
		buildOrder.add(*type);
	}

	/*
	// split message
	std::vector<int> unitIDs;
	std::stringstream ss(message);
	int temp;
	while (ss >> temp)
		unitIDs.push_back(temp);

	// Convert to unit IDs to build order
	for (std::vector<int>::iterator it = unitIDs.begin(); it != unitIDs.end(); ++it) {
		
		BWAPI::UnitType* unitType = new BWAPI::UnitType(*it);
		//BWAPI::TechType* techType = new BWAPI::TechType(*it);
		ActionType* type = new ActionType(*unitType);
		//std::string name = unitType->getName();
		buildOrder.add(*type);

	}
	*/
}


void BuildOrderSearchService::setState(const GameState state)
{
	_initialState = state;
}

void BuildOrderSearchService::print()
{
	//initialState.printData();
	printf("\n\n");
}
