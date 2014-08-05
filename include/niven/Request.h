#pragma once

#include <map>
#include <string>
#include <microhttpd.h>


namespace niven
{
	struct Request
	{
		std::string url;
		std::string method;
		std::string body;

		std::map<std::string, std::string> query;
		std::map<std::string, std::string> headers;
		std::map<std::string, std::string> cookies;


		Request() {}
		Request(std::string url, std::string method, MHD_Connection *connection);

		//void Set(std::string url, std::string method, MHD_Connection *connection);
	};
}
