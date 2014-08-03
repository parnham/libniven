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
		Response(const Http &status)										: status(status) {}
		Response(const char *data, const Http &status = Http::OK)			: status(status), data(data) {}
		Response(const std::string &data, const Http &status = Http::OK)	: status(status), data(data) {}

		Response(const ent::entity &data) : data(const_cast<ent::entity &>(data).to<ent::json>())
		{
			this->headers["Content-Type"] = "application/json";
		}

		//Response(const Http &status, const std::string &data) : status(status), data(data) {}


		int Send(MHD_Connection *connection)
		{
			auto response = MHD_create_response_from_data(this->data.size(), (void *)this->data.data(), MHD_NO, MHD_YES);

			for (auto &h : this->headers) MHD_add_response_header(response, h.first.c_str(), h.second.c_str());

			MHD_add_response_header(response, "Server", "niven/0.1");
			//MHD_add_response_header(response, "Keep-Alive", "timeout=60,max=1000");
			MHD_add_response_header(response, "Keep-Alive", "timeout=60");

			int result = MHD_queue_response(connection, (int)this->status, response);

			MHD_destroy_response(response);

			return result;
		}
	};
}
