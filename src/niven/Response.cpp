#include "niven/Response.h"

using namespace ent;


namespace niven
{
	Response::Response(const entity &data)
	{
		this->headers["Content-Type"]	= "application/json";
		this->data						= const_cast<entity &>(data).to<json>();
	}


	Response &Response::WithHeader(std::string key, std::string value)
	{
		this->headers[key] = value;
		return *this;
	}


	int Response::Send(MHD_Connection *connection)
	{
		auto response = MHD_create_response_from_data(this->data.size(), (void *)this->data.data(), MHD_NO, MHD_YES);

		for (auto &h : this->headers) MHD_add_response_header(response, h.first.c_str(), h.second.c_str());

		MHD_add_response_header(response, "Server", "niven/0.1");
		MHD_add_response_header(response, "Keep-Alive", "timeout=60");

		int result = MHD_queue_response(connection, (int)this->status, response);

		MHD_destroy_response(response);

		return result;
	}
}
