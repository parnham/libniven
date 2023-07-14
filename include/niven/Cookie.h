#pragma once

#include <string>
#include <emergent/DateTime.hpp>


namespace niven
{
	using emergent::DateTime;


	// Structure to assist with buidling cookie strings.
	struct Cookie
	{
		std::string name;
		std::string value;
		std::string domain;
		std::string path	= "/";
		bool httpOnly		= true;
		bool secure			= false;
		DateTime expires;


		Cookie(std::string name, std::string value) : name(name), value(value) {}
		Cookie(std::string name, std::string value, const DateTime &expires) : name(name), value(value), expires(expires) {}

		// Build the cookie string.
		std::string Build();

		// Enable the Secure flag.
		Cookie &Secure();
	};
}

