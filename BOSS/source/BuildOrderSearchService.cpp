#include "BuildOrderSearchService.h"

#include <string>
#include <sstream>
#include <vector>

using namespace BOSS;

BuildOrderSearchService::BuildOrderSearchService(const GameState state)
	: _initialState(state), _service()
{

}


BuildOrder & BuildOrderSearchService::search()
{
	// Create new build order
	BuildOrder buildOrder;

	// Call service
	
	std::string message = _service.call();
	//std::string message = "";

	// Check for errors
	if (message.length() < 9 || message.substr(0, 8) == "Exception"){
		return buildOrder;
	}

	// split message
	std::vector<int> unitIDs;
	std::stringstream ss(message);
	int temp;
	while (ss >> temp)
		unitIDs.push_back(temp);

	// Convert to unit IDs to build order
	for (std::vector<int>::iterator it = unitIDs.begin(); it != unitIDs.end(); ++it) {
		
		BWAPI::UnitType* unitType = new BWAPI::UnitType(*it);
		ActionType* type = new ActionType(*unitType);
		buildOrder.add(*type);
		
	}

	return buildOrder;
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
