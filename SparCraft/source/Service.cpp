
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

std::string Service::call(std::vector<int> own_units, std::vector<int> own_techs, std::vector<int> own_upgrades, std::vector<int> opp_units, int frame, int min, int gas){
	std::string message;
	try
	{
		std::string host = "127.0.0.1";
		
		std::string path = "/app/update/?own_units=";
		for (int i = 0; i < own_units.size(); i++){
			std::string u = std::to_string(own_units[i]);
			path.append(u);
			if (i < own_units.size() - 1){
				path.append("-");
			}
		}

		path.append("&own_techs=");
		for (int i = 0; i < own_techs.size(); i++){
			std::string u = std::to_string(own_techs[i]);
			path.append(u);
			if (i < own_techs.size() - 1){
				path.append("-");
			}
		}

		path.append("&own_upgrades=");
		for (int i = 0; i < own_upgrades.size(); i++){
			std::string u = std::to_string(own_upgrades[i]);
			path.append(u);
			if (i < own_upgrades.size() - 1){
				path.append("-");
			}
		}

		path.append("&opp_units=");
		for (int i = 0; i < opp_units.size(); i++){
			std::string u = std::to_string(opp_units[i]);
			path.append(u);
			if (i < opp_units.size() - 1){
				path.append("-");
			}
		}

		path.append("&frame=" + std::to_string(frame));
		path.append("&minerals=" + std::to_string(min));
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

