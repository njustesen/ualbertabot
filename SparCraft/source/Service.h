#include <string>

class Service
{
public:
	Service();
	std::string call(std::vector<int> own_units, std::vector<int> own_techs, std::vector<int> own_upgrades, std::vector<int> opp_units, int frame, int min, int gas);
};