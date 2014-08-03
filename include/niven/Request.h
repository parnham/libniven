#pragma once

#include <map>
#include <string>
#include <microhttpd.h>

//Debug
#include <entity/entity.h>


namespace niven
{
	struct Request : ent::entity
	{
		std::string url;
		std::string method;
		std::string body;

		std::map<std::string, std::string> query;
		std::map<std::string, std::string> headers;
		std::map<std::string, std::string> cookies;


		Request() {}
		Request(std::string url, std::string method, MHD_Connection *connection);


		ent::mapping map()
		{
			return ent::mapping() << eref(url) << eref(method) << eref(query) << eref(headers) << eref(cookies);
		}
	};
}
