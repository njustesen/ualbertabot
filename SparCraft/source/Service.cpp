
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include "HttpClient.cpp"
#include "Service.h"

using boost::asio::ip::tcp;

Service::Service(){
}

std::string Service::call(std::vector<int> state, int frame, int min, int gas){
	std::string message;
	try
	{
		std::string host = "127.0.0.1";
		std::string path = "/app/update/?s=";

		for (int i = 0; i < state.size(); i++){
			std::string u = std::to_string(state[i]);
			path.append(u);
			if (i < state.size() - 1){
				path.append("-");
			}
		}

		path.append("&frame=" + std::to_string(frame));
		path.append("&min=" + std::to_string(min));
		path.append("&gas=" + std::to_string(gas));

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

