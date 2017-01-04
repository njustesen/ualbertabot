#include "BuildOrderSearchService.h"

#include <string>
#include <sstream>
#include <vector>

using namespace BOSS;

BuildOrderSearchService::BuildOrderSearchService(const GameState state)
	: _initialState(state), _service()
{

}

void BuildOrderSearchService::search(BuildOrder & buildOrder, std::vector<int> state, int frame, int min, int gas)
{
	
	// Call service
	std::string message = _service.call(state, frame, min, gas);
	//std::string message = "";

	// Check for errors
	if (message.length() >= 9 && message.substr(0, 8) == "Exception"){
		return;
	}

	// Clear build order
	buildOrder = BOSS::BuildOrder();

	// If service returns -1, return with empty build order
	if (message == "-1"){
		return;
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
		//std::string name = unitType->getName();
		buildOrder.add(*type);

	}
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
