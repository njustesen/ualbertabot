#include <string>

class Service
{
public:
	Service();
	std::string cal(std::string host, std::string path);
	std::string call(std::vector<int> own_units, std::vector<int> own_techs, std::vector<int> own_upgrades, std::vector<int> opp_units, std::string own_race, std::string opp_race, int frame, int min, int gas, bool new_game);
};