#pragma once

#include <string>
#include <ctime>
#include <niven/Http.h>
#include <niven/Cookie.h>
#include <entity/json.h>
#include <entity/json2.h>

#include <emergent/Path.h>
#include <microhttpd.h>


namespace niven
{
	using emergent::Path;

	struct Response
	{
		Http status 								= Http::OK;
		std::map<std::string, std::string> headers	= {{ "Content-Type", "text/plain" }};
		std::vector<Cookie> cookies;
		std::string data;

		Response() {}
		Response(const ent::entity &data);
		Response(const ent::entity2 &data);
		Response(const ent::tree &data);
		Response(const Path &path);
		Response(const Http &status)										: status(status) {}
		Response(const char *data, const Http &status = Http::OK)			: status(status), data(data) {}
		Response(const std::string &data, const Http &status = Http::OK)	: status(status), data(data) {}
		Response(std::nullptr_t null)										: status(Http::None), headers() {}

		Response &WithHeader(std::string key, std::string value);
		Response &WithCookie(const Cookie &cookie);

		int Send(MHD_Connection *connection);
	};
}

