#include "niven/Request.h"


#include <iostream>

using namespace std;



namespace niven
{
	Request::Request(const string url, const string method, MHD_Connection *connection)
	{
		this->url			= url;
		this->method		= method;
		this->connection	= connection;
	}


	auto Request::Populate(void *cls, MHD_ValueKind, const char *key, const char *value)
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


	auto Request::IterateMultipart(void *cls, MHD_ValueKind, const char *key, const char *filename, const char *content_type, const char *transfer_encoding, const char *data, uint64_t off, size_t size)
	{
		printf("key: %s\nfilename: %s\ntype: %s\n", key, filename, content_type);
		printf("data: %ld\noff: %ld\nsize: %ld\n", (int64_t)data, off, size);


		return MHD_YES;
	}

	// #define MHD_STATICSTR_LEN_(macro) (sizeof(macro) / sizeof(char) - 1)

	void Request::MultipartData()
	{
		if (this->method != "POST")
		{
			printf("not post\n");
			return;
		}

		const std::string_view encoding = MHD_lookup_connection_value(this->connection, MHD_HEADER_KIND, MHD_HTTP_HEADER_CONTENT_TYPE);

		if (encoding.empty())
		{
			return;
		}

		std::cout << encoding << std::endl;

		// MHD_HTTP_POST_ENCODING_MULTIPART_FORMDATA

		if (encoding.substr(0, sizeof(MHD_HTTP_POST_ENCODING_MULTIPART_FORMDATA) - 1) != MHD_HTTP_POST_ENCODING_MULTIPART_FORMDATA)
		{
			printf("not multipart\n");
			return;
		}

		const auto boundary = encoding.substr(encoding.find("boundary=") + 9);

		if (boundary.empty())
		{
			printf("no boundary\n");
			return;
		}

		std::cout << boundary << std::endl;


		std::string_view data = this->body;

		size_t position = data.find(boundary, 0);

		while (position != std::string_view::npos)
		{
			std::cout << position << std::endl;

			// std::cout << (int)(data.data() + position + sizeof(boundary))[0] << std::endl;
			std::cout << data.substr(position + boundary.size(), 8) << std::endl;

			position = data.find(boundary, position + boundary.size());
		}






		// printf("encoding: %s\n", encoding);


		// auto pp = MHD_create_post_processor(this->connection, 64'000, &Request::IterateMultipart, nullptr);

		// printf("body: %ld, %ld\n", (int64_t)this->body.data(), this->body.size());

		// // MHD_post_process(struct MHD_PostProcessor *pp, const char *post_data, size_t post_data_len)
		// MHD_post_process(pp, this->body.data(), this->body.size());

		// MHD_destroy_post_processor(pp);
	}


}


// static int
// iterate_post (void *coninfo_cls, enum MHD_ValueKind kind, const char *key,
//               const char *filename, const char *content_type,
//               const char *transfer_encoding, const char *data, uint64_t off,
//               size_t size)

  // if (NULL == *con_cls)
  //   {
  //     struct connection_info_struct *con_info;
  //     if (nr_of_uploading_clients >= MAXCLIENTS)
  //       return send_page (connection, busypage, MHD_HTTP_SERVICE_UNAVAILABLE);
  //     con_info = malloc (sizeof (struct connection_info_struct));
  //     if (NULL == con_info)
  //       return MHD_NO;
  //     con_info->fp = NULL;
  //     if (0 == strcmp (method, "POST"))
  //       {
  //         con_info->postprocessor =
  //           MHD_create_post_processor (connection, POSTBUFFERSIZE,
  //                                      iterate_post, (void *) con_info);
  //         if (NULL == con_info->postprocessor)
  //           {
  //             free (con_info);
  //             return MHD_NO;
  //           }
  //         nr_of_uploading_clients++;
  //         con_info->connectiontype = POST;
  //         con_info->answercode = MHD_HTTP_OK;
  //         con_info->answerstring = completepage;
  //       }
  //     else
  //       con_info->connectiontype = GET;
  //     *con_cls = (void *) con_info;
  //     return MHD_YES;
