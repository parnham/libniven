#include "niven/Cookie.h"
#include "niven/Http.h"
#include <emergent/String.hpp>

using namespace std;
using namespace emergent;


namespace niven
{
	// Build the cookie string.
	string Cookie::Build() const
	{
		auto result = String::format("%s=%s; Path=%s", this->name, this->value, this->path);

		if (this->expires.timestamp >= 0)	result += "; Expires=" + HttpTime::Format(this->expires.timestamp); //this->CookieTime();
		if (this->domain.size())			result += "; Domain=" + this->domain;
		if (this->secure)					result += "; Secure";
		if (this->httpOnly)					result += "; HttpOnly";
		if (!this->sameSite)				result += "; SameSite=None";

		return result;
	}


	// Enable the Secure flag.
	Cookie &Cookie::Secure()
	{
		this->secure = true;
		return *this;
	}

	// Disable the SameSite option, this will set the Secure flag since it's required
	Cookie &Cookie::DisableSameSite()
	{
		this->sameSite	= false;
		this->secure	= true;
		return *this;
	}
}
