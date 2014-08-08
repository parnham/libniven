#pragma once

#include <string>
#include <niven/Http.h>
#include <entity/json.h>
#include <microhttpd.h>


namespace niven
{
	struct Response
	{
		Http status 								= Http::OK;
		std::map<std::string, std::string> headers	= {{ "Content-Type", "text/plain" }};
		std::string data;

		Response() {}
		Response(const ent::entity &data);
		Response(const Http &status)										: status(status) {}
		Response(const char *data, const Http &status = Http::OK)			: status(status), data(data) {}
		Response(const std::string &data, const Http &status = Http::OK)	: status(status), data(data) {}
		Response(std::nullptr_t null)										: status(Http::None), headers() {}

		Response &WithHeader(std::string key, std::string value);

		int Send(MHD_Connection *connection);
	};
}
