#include <string>

class Service
{
public:
	Service();
	std::string call(std::string host, std::string path);
	std::string update(
		std::vector<int> own_units,
		std::vector<std::string> own_units_under_construction,
		std::vector<int> own_techs,
		std::vector<int> own_techs_under_construction,
		std::vector<int> own_upgrades,
		std::vector<int> own_upgrades_under_construction,
		std::vector<int> opp_units, std::string own_race,
		std::string opp_race, int frame, int min, int gas, bool new_game);
};