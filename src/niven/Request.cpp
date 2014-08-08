#include "niven/Request.h"

using namespace std;


namespace niven
{
	Request::Request(string url, string method, MHD_Connection *connection)
	{
		this->url		= url;
		this->method	= method;

		static auto populate = [](void *cls, MHD_ValueKind kind, const char *key, const char *value) {
			static_cast<std::map<string, string> *>(cls)->emplace(key, value);
			return MHD_YES;
		};

		MHD_get_connection_values(connection, MHD_GET_ARGUMENT_KIND, populate, &this->query);
		MHD_get_connection_values(connection, MHD_HEADER_KIND, populate, &this->headers);
		MHD_get_connection_values(connection, MHD_COOKIE_KIND, populate, &this->cookies);
	}
}
