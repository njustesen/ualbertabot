#include <string>

class Service
{
public:
	Service();
	std::string call(std::vector<size_t> ownUnits, std::vector<size_t> oppUnits);
};