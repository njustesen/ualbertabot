
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

std::string Service::call(std::string host, std::string path){
	std::string res = "";
	std::string body = "";
	try{
		boost::asio::io_service io_service;

		// Get a list of endpoints corresponding to the server name.
		tcp::resolver resolver(io_service);
		tcp::resolver::query query(host, "8000");
		tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
		tcp::resolver::iterator end;

		// Try each endpoint until we successfully establish a connection.
		tcp::socket socket(io_service);
		boost::system::error_code error = boost::asio::error::host_not_found;
		while (error && endpoint_iterator != end){
			socket.close();
			socket.connect(*endpoint_iterator++, error);
		}

		if (error){ throw boost::system::system_error(error); }

		// Form the request. We specify the "Connection: close" header so that the server will close the socket 
		// after transmitting the response. This will allow us to treat all data up until the EOF as the content.
		boost::asio::streambuf request;
		std::ostream request_stream(&request);
		request_stream << "GET " << path << " HTTP/1.0\r\n";
		request_stream << "Host: " << host << "\r\n";
		request_stream << "Accept: */*\r\n";
		request_stream << "Connection: close\r\n\r\n";

		// Send the request.
		boost::asio::write(socket, request);

		// Read the response status line.
		boost::asio::streambuf response;
		boost::asio::read_until(socket, response, "\r\n");

		// Check that response is OK.
		std::istream response_stream(&response);

		std::string http_version;
		response_stream >> http_version;

		unsigned int status_code;
		response_stream >> status_code;

		std::string status_message;
		std::getline(response_stream, status_message);
		if (!response_stream || http_version.substr(0, 5) != "HTTP/"){
			std::cout << "Invalid response\n";
		}
		if (status_code != 200){
			std::cout << "Response returned with status code " << status_code << "\n";
		}

		// Read the response headers, which are terminated by a blank line.
		boost::asio::read_until(socket, response, "\r\n\r\n");

		// Write whatever content we already have to output.
		if (response.size() > 0){
			std::ostringstream oss;
			oss << &response;
			res = oss.str();
		}

		// Read until EOF, writing data to output as we go.
		while (boost::asio::read(socket, response, boost::asio::transfer_at_least(1), error)){
			std::ostringstream oss;
			oss << &response;
			res += oss.str();
		}

		if (error != boost::asio::error::eof){ throw boost::system::system_error(error); }

		// Only return body
		std::string delimiter = "\r\n\r";
		body = res.substr(res.find(delimiter)+4, res.length());

	}
	catch (std::exception& e){
		std::cout << "Exception: " << e.what() << "\n";
	}

	return body;
}

std::string Service::update(
	std::vector<int> own_units, 
	std::vector<std::string> own_units_under_construction,
	std::vector<int> own_techs, 
	std::vector<int> own_techs_under_construction,
	std::vector<int> own_upgrades, 
	std::vector<int> own_upgrades_under_construction,
	std::vector<int> opp_units, std::string own_race, 
	std::string opp_race, int frame, int min, int gas, bool new_game){
	
	std::string host = "127.0.0.1";
	std::string path = "/app/update/";
		
	path.append("?own_units=");
	for (int i = 0; i < own_units.size(); i++){
		std::string u = std::to_string(own_units[i]);
		path.append(u);
		if (i < own_units.size() - 1){
			path.append("-");
		}
	}

	path.append("&own_units_under_construction=");
	for (int i = 0; i < own_units_under_construction.size(); i++){
		std::string u = own_units_under_construction[i];
		path.append(u);
		if (i < own_units_under_construction.size() - 1){
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

	path.append("&own_techs_under_construction=");
	for (int i = 0; i < own_techs_under_construction.size(); i++){
		std::string u = std::to_string(own_techs_under_construction[i]);
		path.append(u);
		if (i < own_techs_under_construction.size() - 1){
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

	path.append("&own_upgrades_under_construction=");
	for (int i = 0; i < own_upgrades_under_construction.size(); i++){
		std::string u = std::to_string(own_upgrades_under_construction[i]);
		path.append(u);
		if (i < own_upgrades_under_construction.size() - 1){
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
	std::string new_game_string = "false";
	if (new_game){
		new_game_string = "true";
	}
	path.append("&new_game=" + new_game_string);
	path.append("&own_race=" + own_race);
	path.append("&opp_race=" + opp_race);
	path.append("&frame=" + std::to_string(frame));
	path.append("&minerals=" + std::to_string(min));
	path.append("&gas=" + std::to_string(gas));
		
	std::string message = call(host, path);

	return message;
}

