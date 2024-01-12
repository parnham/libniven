#include <niven/Niven.h>

using namespace niven;


class HeaderModule : public Module
{
	public:

		HeaderModule() : Module("/headers")
		{
			// Access the request headers by name. Alternatively the Headers() function
			// will return a map<string, string> containing all of the headers in the
			// request.
			Get["/request"] = [](auto c) {
				return c.request.Header("Custom");
			};

			// Return a response with a custom header.
			Get["/response"] = [](auto) {
				return Response().WithHeader("Custom", "The moon's a balloon");
			};
		}
};

REGISTER_MODULE(HeaderModule)
