#include "niven/Request.h"

using namespace std;


namespace niven
{
	Request::Request(const string url, const string method, MHD_Connection *connection)
	{
		this->url			= url;
		this->method		= method;
		this->connection	= connection;
	}


	auto Request::Populate(void *cls, MHD_ValueKind kind, const char *key, const char *value)
	{
		static_cast<map<string, string> *>(cls)->emplace(key, value);
		return MHD_YES;
	}


	string Request::Parameter(const string &key)
	{
		auto result = MHD_lookup_connection_value(this->connection, MHD_GET_ARGUMENT_KIND, key.c_str());

		return result ? result : "";
	}


	map<string, string> Request::Parameters()
	{
		map<string, string> result;

		MHD_get_connection_values(this->connection, MHD_GET_ARGUMENT_KIND, &Request::Populate, &result);

		return result;
	}


	string Request::Header(const string &key)
	{
		auto result = MHD_lookup_connection_value(this->connection, MHD_HEADER_KIND, key.c_str());

		return result ? result : "";
	}


	map<string, string> Request::Headers()
	{
		map<string, string> result;

		MHD_get_connection_values(this->connection, MHD_HEADER_KIND, &Request::Populate, &result);

		return result;
	}


	string Request::Cookie(const string &key)
	{
		auto result = MHD_lookup_connection_value(this->connection, MHD_COOKIE_KIND, key.c_str());

		return result ? result : "";
	}


	map<string, string> Request::Cookies()
	{
		map<string, string> result;

		MHD_get_connection_values(this->connection, MHD_COOKIE_KIND, &Request::Populate, &result);

		return result;
	}
}
