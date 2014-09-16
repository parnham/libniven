#pragma once

#include <string>
#include <emergent/DateTime.h>


namespace niven
{
	using emergent::DateTime;


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


		std::string Build()
		{
			auto result = tfm::format("%s=%s; Path=%s", this->name, this->value, this->path);

			if (this->expires.timestamp >= 0)	result += "; Expires=" + this->CookieTime();
			if (this->domain.size())			result += "; Domain=" + this->domain;
			if (this->secure)					result += "; Secure";
			if (this->httpOnly)					result += "; HttpOnly";

			return result;
		}


		std::string CookieTime()
		{
			static std::string DAYS[]	= { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
			static std::string MONTHS[]	= { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

			tm t;
			gmtime_r(&this->expires.timestamp, &t);

			return tfm::format(
				"%s, %02d-%s-%04d %02d:%02d:%02d GMT",
				DAYS[t.tm_wday], t.tm_mday, MONTHS[t.tm_mon], 1900+t.tm_year,
				t.tm_hour, t.tm_min, t.tm_sec
			);
		}

		Cookie &Secure() { this->secure = true; return *this; }
	};
}
