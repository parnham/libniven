#include "niven/Request.h"
#include "emergent/String.hpp"

#include <iostream>

// using namespace std;
using std::string;
using std::string_view;



namespace niven
{
	Request::Request(const string url, const string method, MHD_Connection *connection)
	{
		this->url			= url;
		this->method		= method;
		this->connection	= connection;
	}


	auto Populate(void *cls, MHD_ValueKind, const char *key, const char *value)
	{
		static_cast<std::map<string, string> *>(cls)->emplace(key, value);
		return MHD_YES;
	}


	string Request::Parameter(const string &key)
	{
		auto result = MHD_lookup_connection_value(this->connection, MHD_GET_ARGUMENT_KIND, key.c_str());

		return result ? result : "";
	}


	std::map<string, string> Request::Parameters()
	{
		std::map<string, string> result;

		MHD_get_connection_values(this->connection, MHD_GET_ARGUMENT_KIND, &Populate, &result);

		return result;
	}


	string Request::Header(const string &key)
	{
		auto result = MHD_lookup_connection_value(this->connection, MHD_HEADER_KIND, key.c_str());

		return result ? result : "";
	}


	std::map<string, string> Request::Headers()
	{
		std::map<string, string> result;

		MHD_get_connection_values(this->connection, MHD_HEADER_KIND, &Populate, &result);

		return result;
	}


	string Request::Cookie(const string &key)
	{
		auto result = MHD_lookup_connection_value(this->connection, MHD_COOKIE_KIND, key.c_str());

		return result ? result : "";
	}


	std::map<string, string> Request::Cookies()
	{
		std::map<string, string> result;

		MHD_get_connection_values(this->connection, MHD_COOKIE_KIND, &Populate, &result);

		return result;
	}


	// auto Request::IterateMultipart(void *cls, MHD_ValueKind, const char *key, const char *filename, const char *content_type, const char *transfer_encoding, const char *data, uint64_t off, size_t size)
	// {
	// 	printf("key: %s\nfilename: %s\ntype: %s\n", key, filename, content_type);
	// 	printf("data: %ld\noff: %ld\nsize: %ld\n", (int64_t)data, off, size);


	// 	return MHD_YES;
	// }

	// auto pp = MHD_create_post_processor(this->connection, 64'000, &Request::IterateMultipart, nullptr);

	// printf("body: %ld, %ld\n", (int64_t)this->body.data(), this->body.size());

	// // MHD_post_process(struct MHD_PostProcessor *pp, const char *post_data, size_t post_data_len)
	// MHD_post_process(pp, this->body.data(), this->body.size());

	// MHD_destroy_post_processor(pp);




	static constexpr string_view CONTENT_DISPOSITION	= "content-disposition: ";
	static constexpr string_view CONTENT_TYPE			= "content-type: ";
	static constexpr string_view CONTENT_ENCODING		= "content-transfer-encoding: ";


	// Case-insensitive comparison with a constant string.
	static inline bool Matches(string_view data, size_t offset, string_view constant)
	{
		// Sub-string from the current offset position in the data of the same size as the constant
		auto sub = data.substr(offset, constant.size());

		return std::equal(
			sub.begin(), sub.end(),
			constant.begin(),constant.end(),
			[](auto i, auto j) { return std::tolower(i) == std::tolower(j); }
		);
	}


	// Returns the value of the header and the offset position for the end of that line
	static inline std::pair<string_view, size_t> GetHeader(string_view data, size_t offset)
	{
		const auto end = data.find_first_of('\r', offset);

		return { data.substr(offset, end - offset), end };
	}


	/// Trim a string (both ends) of the given character - string_view version of the one from libemergent
	static inline std::string_view Trim(std::string_view text, const char c)
	{
		auto start = text.find_first_not_of(c);

		return start == std::string_view::npos ? "" : text.substr(start, text.find_last_not_of(c) - start + 1);
	}


	// Retrieve the name and filename values from a content-disposition header
	static std::pair<string_view, string_view> GetValues(string_view data)
	{
		string_view name, filename;

		for (auto section : emergent::String::explode(data, ";"))
		{
			auto kvp = emergent::String::explode(Trim(section, ' '), "=");

			if (kvp.size() == 2)
			{
				if (kvp[0] == "name")
				{
					name = Trim(kvp[1], '"');
				}
				else if (kvp[0] == "filename")
				{
					filename = Trim(kvp[1], '"');
				}
			}
		}

		return { name, filename };
	}

	// Starting from the offset position find consecutive newline characters.
	// Returns the number of newlines found and the new offset position at
	// the next non-newline character.
	std::pair<int, size_t> CountNewLines(const std::string_view data, size_t offset)
	{
		for (int count = 0; offset < data.size() - 1; offset += 2, count++)
		{
			if (data[offset] != '\r' || data[offset+1] != '\n')
			{
				return { count, offset };
			}
		}

		return { 0, std::string_view::npos };
	}


	std::pair<Request::Multipart, size_t> MultipartHeaders(string_view data, size_t offset)
	{
		int newlines = 0;
		Request::Multipart part;

		std::tie(newlines, offset) = CountNewLines(data, offset);

		// There is a newline before each header (\r\n) but there are 2 between the headers and the file data (\r\n\r\n)
		while (newlines == 1)
		{
			if (Matches(data, offset, CONTENT_DISPOSITION))
			{
				string_view disposition;

				std::tie(disposition, offset)		= GetHeader(data, offset + CONTENT_DISPOSITION.size());
				std::tie(part.name, part.filename)	= GetValues(disposition);
			}
			else if (Matches(data, offset, CONTENT_TYPE))
			{
				std::tie(part.type, offset) = GetHeader(data, offset + CONTENT_TYPE.size());
			}
			else if (Matches(data, offset, CONTENT_ENCODING))
			{
				std::tie(part.type, offset) = GetHeader(data, offset + CONTENT_ENCODING.size());
			}
			else
			{
				// Not a header we're interested in, so skip to the next one
				offset = data.find_first_of('\r', offset);
			}

			std::tie(newlines, offset) = CountNewLines(data, offset);
		}

		return { part, offset };
	}


	std::vector<Request::Multipart> Request::MultipartData()
	{
		if (this->method != "POST")
		{
			return {};
		}

		const string_view encoding	= MHD_lookup_connection_value(this->connection, MHD_HEADER_KIND, MHD_HTTP_HEADER_CONTENT_TYPE);
		const auto boundary			= encoding.substr(encoding.find("boundary=") + 9);

		if (encoding.empty() || boundary.empty() || !Matches(encoding, 0, MHD_HTTP_POST_ENCODING_MULTIPART_FORMDATA))
		{
			// Cannot parse the data without the boundary or if this is not multipart/formdata
			return {};
		}

		std::vector<Multipart> result;
		const string_view data	= this->body;
		size_t position			= data.find(boundary, 0);

		while (position != std::string_view::npos)
		{
			auto [part, offset]	= MultipartHeaders(data, position + boundary.size());
			const size_t next	= data.find(boundary, offset);

			if (next != std::string_view::npos)
			{
				// Need to move back 4 characters from the boundary to remove "\r\n--"
				part.data = data.substr(offset, next - offset - 4);

				result.push_back(part);
			}

			position = next;
		}

		return result;
	}
}
