#include "niven/Mime.h"

namespace niven
{
	const std::map<std::string, std::string> Mime::ByExtension = {
		{ "json",	"application/json" },
		{ "js",		"application/javascript" },

		{ "gif",	"image/gif" },
		{ "jpg",	"image/jpeg" },
		{ "jpeg",	"image/jpeg" },
		{ "png",	"image/png" },

		{ "css", 	"text/css" },
		{ "html", 	"text/html" },
		{ "txt",	"text/plain" },
		{ "xml", 	"text/xml" },

		{ "gz",		"application/gzip" }
	};
}
