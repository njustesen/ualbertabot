
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include "HttpClient.cpp"
#include "BuildOrderService.h"

using boost::asio::ip::tcp;

BuildOrderService::BuildOrderService(){
}

std::string BuildOrderService::call(){
	std::string message;
	try
	{
		std::string host = "10.28.6.237";
		std::string path = "/app/evolve/";

		//std::cout << "HOST " << host << "\n";
		//std::cout << "GET " << path << "\n";

		boost::asio::io_service io_service;
		HttpClient c(io_service, host, path);
		io_service.run();
		message = c.response;
			
	}
	catch (std::exception& e)
	{
		std::ostringstream out_stream;
		out_stream << "Exception: " << e.what() << "\n";
		message = out_stream.str();
	}
	return message;
}

