#pragma once

#include <string>
#include <ctime>
#include <niven/Http.h>
#include <niven/Cookie.h>
#include <entity/entity.hpp>

#include <emergent/Path.hpp>
#include <microhttpd.h>


namespace niven
{
	using emergent::Path;

	// Allows for simple construction of responses that can be returned from actions.
	// It contains a selection of constructor overloads which let you return anything
	// from an HTTP status code to a file on the system.
	struct Response
	{
		Http status 								= Http::OK;
		std::map<std::string, std::string> headers	= {{ "Content-Type", "text/plain" }};
		std::vector<Cookie> cookies;
		std::string data;

		// An empty response will return the default HTTP status with no data.
		Response() {}

		// An entity response will be serialised to JSON.
		Response(const ent::entity &data);

		// A tree response will also be serialised to JSON.
		Response(const ent::tree &data);

		// A vector of tree will be serialised as a top-level JSON array.
		Response(const std::vector<ent::tree> &data);

		// A path response will return the file specified (if it exists).
		// If the file cannot be found or the mimetype cannot be determined
		// from the extension the response will only contain the HTTP status
		// code "NotFound".
		Response(const Path &path);

		// A response can simply be an HTTP status code.
		Response(const Http &status) : status(status) {}

		// A raw text response is sent to the client as is.
		Response(const char *data, const Http &status = Http::OK)			: status(status), data(data) {}
		Response(const std::string &data, const Http &status = Http::OK)	: status(status), data(data) {}

		// Binary data
		Response(const std::vector<byte> &data, const Http &status = Http::OK)
			: status(status), data((char *)data.data(), data.size()) {}

		// A null response results in an undefined status code allowing other actions to be performed.
		Response(std::nullptr_t null) : status(Http::None), headers() {}

		// Fluent helper function to add a response header.
		Response &WithHeader(std::string key, std::string value);

		// Fluent helper function to add a response cookie.
		Response &WithCookie(const Cookie &cookie);


		// Send this response to the client (should only be invoked by NivenHost).
		int Send(MHD_Connection *connection);
	};
}

