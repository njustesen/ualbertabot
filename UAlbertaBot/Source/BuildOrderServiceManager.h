#pragma once

#include "Common.h"
#include "WorkerManager.h"
#include "../../BOSS/source/BOSS.h"
#include "StrategyManager.h"
#include <memory>
#include "InformationManager.h"

namespace UAlbertaBot
{
    
typedef std::shared_ptr<BOSS::BuildOrderSearchService> SearchPtr;

class BuildOrderServiceManager
{
	int                                     _previousSearchFrame;
	int                                     _nextSearchFrame;
	bool									_isSearchInProgress;

	BOSS::GameState				            getCurrentState();
	BOSS::GameState				            getStartState();
	BOSS::BuildOrder                        _previousBuildOrder;

	SearchPtr                               _searchService;

    // functions
	BOSS::BuildOrder						search();
	const BOSS::RaceID                      getRace() const;

    BuildOrderServiceManager();

	static std::vector<MetaType>			GetMetaVector(const BOSS::BuildOrder & buildOrder);
	static BOSS::ActionType					GetActionType(const MetaType & t);
	static MetaType					        GetMetaType(const BOSS::ActionType & a);

public:

	static BuildOrderServiceManager &	    Instance();

    BuildOrder                  getBuildOrder();
    
	void						update(int currentFrame, int frameSkip, bool newGame);

	bool                        isSearchInProgress();

	void						drawSearchInformation(int x, int y);
	void						drawStateInformation(int x, int y);

};

}