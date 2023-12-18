#include "niven/Mime.h"

namespace niven
{
	const std::map<std::string, std::string> Mime::ByExtension = {
		// JS
		{ ".json",	"application/json" },
		{ ".js",	"application/javascript" },
		{ ".map",	"application/json" },

		// Images
		{ ".avif",	"image/avif" },
		{ ".gif",	"image/gif" },
		{ ".ico",	"image/vnd.microsoft.icon" },
		{ ".jpg",	"image/jpeg" },
		{ ".jpeg",	"image/jpeg" },
		{ ".png",	"image/png" },
		{ ".svg",	"image/svg+xml"},
		{ ".tif",	"image/tiff" },
		{ ".webp",	"image/webp" },

		// Text
		{ ".css", 	"text/css" },
		{ ".csv",	"text/csv" },
		{ ".html", 	"text/html" },
		{ ".txt",	"text/plain" },
		{ ".xml", 	"text/xml" },

		// Documents
		{ ".pdf",	"application/pdf" },

		// Fonts
		{ ".ttf",	"font/ttf" },
		{ ".otf",	"font/otf" },
		{ ".woff",	"font/woff" },
		{ ".woff2",	"font/woff2" },

		// Audio
		{ ".mp3",	"audio/mpeg" },
		{ ".oga",	"audio/ogg" },
		{ ".wav",	"audio/wav" },
		{ ".weba",	"audio/webm" },

		// Video
		{ ".avi", "video/x-msvideo" },
		{ ".mp4",	"video/mp4" },
		{ ".mpeg",	"video/mpeg" },
		{ ".ogv",	"video/ogg" },
		{ ".ts",	"video/mp2t" },
		{ ".webm",	"video/webm" },

		// Archives
		{ ".bz",	"application/x-bzip" },
		{ ".bz2",	"application/x-bzip2" },
		{ ".gz",	"application/gzip" },
		{ ".rar",	"application/vnd.rar" },
		{ ".tar",	"application/x-tar" },
		{ ".zip",	"application/zip" },
		{ ".7z",	"application/x-7z-compressed" }
	};
}





