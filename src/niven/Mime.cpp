#include "niven/Mime.h"

namespace niven
{
	const std::map<std::string, std::string> Mime::ByExtension = {
		// JS
		{ ".json",	"application/json" },
		{ ".js",	"application/javascript" },
		{ ".map",	"application/json" },

		// Images
		{ ".gif",	"image/gif" },
		{ ".jpg",	"image/jpeg" },
		{ ".jpeg",	"image/jpeg" },
		{ ".png",	"image/png" },
		{ ".svg",	"image/svg+xml"},

		// Text
		{ ".css", 	"text/css" },
		{ ".html", 	"text/html" },
		{ ".txt",	"text/plain" },
		{ ".xml", 	"text/xml" },

		// Fonts
		{ ".ttf",	"font/ttf" },
		{ ".otf",	"font/otf" },
		{ ".woff",	"font/woff" },
		{ ".woff2",	"font/woff2" },

		{ ".gz",	"application/gzip" }
	};
}


