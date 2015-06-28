#include <niven/Niven.h>

using namespace niven;


class QueryModule : public Module
{
	public:

		QueryModule() : Module("/query")
		{
			// Access a URL query parameter by name. Alternatively the function
			// Parameters() will return a map<string, string> containing all
			// of the parameters.
			Get["/"] = [](auto c) {
				return c.request.Parameter("test");
			};
		}
};

REGISTER_MODULE(QueryModule)
