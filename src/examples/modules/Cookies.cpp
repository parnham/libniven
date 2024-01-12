#include <niven/Niven.h>

using namespace niven;


class CookieModule : public Module
{
	public:

		CookieModule() : Module("/cookies")
		{
			// A default Response object has a status of Http::OK.
			// The response can be modified fluently and a custom cookie appended. In this example
			// the cookie is set to expire 30 seconds after the request is made.
			Get["/"] = [](auto) {
				return Response().WithCookie({ "niven-test", "chocolate chip", DateTime::Now().AddSeconds(30) });
			};

			// The request object can extract cookie values by key if they exist.
			// The Cookies() function will return a map of all cookies in the request.
			Get["/show"] = [](Context &c) {
				return c.request.Cookie("niven-test");
			};
		}
};

REGISTER_MODULE(CookieModule)
