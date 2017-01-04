#include <string>

class Service
{
public:
	Service();
	std::string call(std::vector<int> state, int frame, int min, int gas);
};