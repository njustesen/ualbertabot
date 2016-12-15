#include "BuildOrderService.h"
#include <iostream>
#include <istream>
#include <ostream>
#include <string>

int main(int argc, char* argv[])
{
	BuildOrderService* service = new BuildOrderService();
	std::string message = service->call();

	std::cout << message;

	return 0;
}
