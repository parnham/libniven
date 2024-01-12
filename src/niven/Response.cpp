#include "niven/Response.h"
#include "niven/Mime.h"
#include <emergent/Io.hpp>

#include <iostream>

namespace niven
{
	// Response::Response(const entity &data)
	// {
	// 	this->headers["Content-Type"]	= "application/json; charset=utf-8";
	// 	this->data						= entity::encode<json>(const_cast<entity&>(data));
	// }


	Response::Response(const ent::tree &data)
	{
		this->headers["Content-Type"]	= "application/json; charset=utf-8";
		this->data						= ent::encode<ent::json>(data);
	}


	Response::Response(const std::vector<ent::tree> &data)
	{
		this->headers["Content-Type"]	= "application/json; charset=utf-8";
		this->data						= ent::encode<ent::json>(ent::tree { data });
	}


	Response::Response(const fs::path &path)
	{
		auto extension = path.extension();

		if (fs::exists(path) && Mime::ByExtension.count(extension))
		{
			this->headers["Content-Type"]	= Mime::ByExtension.at(extension);
			this->headers["Last-Modified"]	= HttpTime::Format(
				emergent::time::to_time_t(
					fs::last_write_time(path)
				)
			);

			// emg::Io::Load(this->data, path);
			this->path = path;
		}
		else this->status = Http::NotFound;
	}


	Response &Response::WithHeader(const std::string &key, const std::string &value)
	{
		this->headers[key] = value;
		return *this;
	}


	Response &Response::WithCookie(const Cookie &cookie)
	{
		this->cookies.push_back(cookie);
		return *this;
	}


	int Response::Send(MHD_Connection *connection)
	{
		MHD_Response *response = nullptr;

		if (this->path.empty())
		{
			// auto response = MHD_create_response_from_buffer(this->data.size(), (void *)this->data.data(), MHD_RESPMEM_PERSISTENT);
			response = MHD_create_response_from_buffer(this->data.size(), (void *)this->data.data(), MHD_RESPMEM_MUST_COPY);
		}
		else if (auto file = fopen(this->path.c_str(), "rb"))
		{
			response = MHD_create_response_from_callback(
				fs::file_size(this->path),
				32 * 1024,
				[](void *file, uint64_t pos, char *buffer, size_t max) {
					fseek((FILE *)file, pos, SEEK_SET);
					return (ssize_t)fread(buffer, 1, max, (FILE *)file);
				},
				file,
				[](void *file) { fclose((FILE *)file); }
			);
		}
		else
		{
			response		= MHD_create_response_from_buffer(0, nullptr, MHD_RESPMEM_MUST_COPY);
			this->status	= Http::NotFound;
		}

		for (const auto &[name, value] : this->headers)
		{
			MHD_add_response_header(response, name.c_str(), value.c_str());
		}
		for (const auto &c : this->cookies)
		{
			MHD_add_response_header(response, "Set-Cookie", c.Build().c_str());
		}

		MHD_add_response_header(response, "Server", "niven/0.1");
		MHD_add_response_header(response, "Keep-Alive", "timeout=60");

		int result = MHD_queue_response(connection, (int)this->status, response);

		MHD_destroy_response(response);

		return result;
	}
}
