#pragma once

#include <map>
#include <string>
#include <vector>
#include <microhttpd.h>


namespace niven
{
	// Representation of the request.
	class Request
	{
		public:
			struct Multipart
			{
				std::string_view type;		// content-type
				std::string_view name;		// content-disposition name
				std::string_view filename;	// content-disposition filename
				std::string_view encoding;	// content-encoding
				std::string_view data;		// file data
			};


			// The actual URL of the request (used for routing).
			std::string url;

			// The request method (GET/POST/DELETE/PUT).
			std::string method;

			// // The raw contents of the request.
			std::string body;


			// Retrieve a specific query parameter for the request (empty string if parameter key does not exist).
			std::string Parameter(const std::string &key);

			// Retrieve all query parameters associated with the request.
			std::map<std::string, std::string> Parameters();


			// Retrieve a specific header for the request (empty string if header key does not exist).
			std::string Header(const std::string &key);

			// Retrieve all headers associated with the request.
			std::map<std::string, std::string> Headers();


			// Retrieve a specific cookie for the request (empty string if cookie key does not exist).
			std::string Cookie(const std::string &key);

			// Retrieve all cookies associated with the request.
			std::map<std::string, std::string> Cookies();


			std::vector<Multipart> MultipartData();


			Request() {}
			Request(const std::string url, const std::string method, MHD_Connection *connection);

		private:

			// static auto Populate(void *cls, MHD_ValueKind kind, const char *key, const char *value);

			// static auto IterateMultipart(void *cls, MHD_ValueKind, const char *key, const char *filename, const char *content_type, const char *transfer_encoding, const char *data, uint64_t off, size_t size);

			MHD_Connection *connection = nullptr;
	};
}
