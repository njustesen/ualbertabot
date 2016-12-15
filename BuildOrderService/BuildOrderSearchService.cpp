#include "BuildOrderSearchService.h"
#include "BuildOrderService.cpp"

#include <string>
#include <sstream>
#include <vector>

using namespace BOSS;

BuildOrderSearchService::BuildOrderSearchService(const RaceID race)
	: _race(race)
{
}
/*
void BuildOrderSearchService::setState(const GameState state)
{
	//_initialState = state;
}
*/
/*
void BuildOrderSearchService::doSearch()
{

	// Create new build order
	BuildOrder* buildOrder = new BuildOrder();

    // Call service
	BuildOrderService* service = new BuildOrderService();
	std::string message = service->call();

	// Check for errors
	if (message.length() < 9 || message.substr(0,8) == "Exception"){
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
		
		//BWAPI::UnitType* unitType = new BWAPI::UnitType(*it);
		//ActionType* type = new ActionType(*unitType);
		//buildOrder->add(*type);
		
	}

	// Save new build order
	_buildOrder = *buildOrder;

}

void BuildOrderSearchService::search()
{
    doSearch();
}

const BuildOrder & BuildOrderSearchService::getBuildOrder() const
{
    return _buildOrder;
}

void BuildOrderSearchService::print()
{
    //initialState.printData();
    printf("\n\n");
}
*/