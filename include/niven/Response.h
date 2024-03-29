#pragma once

#include <string>
#include <ctime>
#include <niven/Http.h>
#include <niven/Cookie.h>
#include <entity/entity.hpp>
#include <entity/json.hpp>
#include <microhttpd.h>

#if __has_include(<filesystem>)
	#include <filesystem>
#elif __has_include(<experimental/filesystem>)
	#include <experimental/filesystem>
#endif


namespace niven
{
	#ifdef __cpp_lib_filesystem
		namespace fs = std::filesystem;
	#elif __cpp_lib_experimental_filesystem
		namespace fs = std::experimental::filesystem;
	#endif


	// Allows for simple construction of responses that can be returned from actions.
	// It contains a selection of constructor overloads which let you return anything
	// from an HTTP status code to a file on the system.
	struct Response
	{
		Http status 								= Http::OK;
		std::map<std::string, std::string> headers	= {{ "Content-Type", "text/plain" }};
		std::vector<Cookie> cookies;
		std::string data;
		fs::path path;

		// An empty response will return the default HTTP status with no data.
		Response() {}

		// An entity response will be serialised to JSON.
		// Response(const ent::entity &data);

		// A tree response will also be serialised to JSON.
		Response(const ent::tree &data);

		// A vector of tree will be serialised as a top-level JSON array.
		Response(const std::vector<ent::tree> &data);


		template <typename T, typename = ent::if_entity<T>> Response(const T &data)
		{
			this->headers["Content-Type"]	= "application/json; charset=utf-8";
			this->data						= ent::encode<ent::json>(const_cast<T&>(data));
		}


		template <typename T> Response(const std::map<std::string, T> &data)
		{
			this->headers["Content-Type"]	= "application/json; charset=utf-8";
			this->data						= ent::encode<ent::json>(
				const_cast<std::map<std::string, T>&>(data)
			);
		}


		// A vector of entities will be serialised as a top-level JSON array
		template <typename T> Response(const std::vector<T> &data)
		{
			this->headers["Content-Type"]	= "application/json; charset=utf-8";
			this->data						= ent::encode<ent::json>(
				const_cast<std::vector<T>&>(data)
			);
		}


		// A path response will return the file specified (if it exists).
		// If the file cannot be found or the mimetype cannot be determined
		// from the extension the response will only contain the HTTP status
		// code "NotFound".
		Response(const fs::path &path);

		// A response can simply be an HTTP status code.
		Response(const Http &status) : status(status) {}

		// A raw text response is sent to the client as is.
		Response(const char *data, const Http &status = Http::OK)			: status(status), data(data) {}
		Response(const std::string &data, const Http &status = Http::OK)	: status(status), data(data) {}

		// Binary data
		Response(const std::vector<uint8_t> &data, const Http &status = Http::OK)
			: status(status), data((char *)data.data(), data.size()) {}

		// A null response results in an undefined status code allowing other actions to be performed.
		Response(std::nullptr_t) : status(Http::None), headers() {}

		// Fluent helper function to add a response header.
		Response &WithHeader(const std::string &key, const std::string &value);

		// Fluent helper function to add a response cookie.
		Response &WithCookie(const Cookie &cookie);


		// Send this response to the client (should only be invoked by NivenHost).
		int Send(MHD_Connection *connection);
	};
}

