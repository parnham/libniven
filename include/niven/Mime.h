#pragma once

#include <string>
#include <map>


namespace niven
{
	// Lookup for mimetypes based on file extension.
	struct Mime
	{
		static const std::map<std::string, std::string> ByExtension;
	};
}


