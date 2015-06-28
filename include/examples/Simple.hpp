#include <niven/Niven.h>

using namespace niven;


class SimpleModule : public Module
{
	public:

		SimpleModule() : Module("/simple")
		{
			// A basic text response
			Get["/"] = [](auto) { return "The moon's a balloon"; };

			// An HTTP status code response
			Get["/ok"] = [](auto) { return Http::OK; };
		}
};

REGISTER_MODULE(SimpleModule)
