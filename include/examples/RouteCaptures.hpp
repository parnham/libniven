#include <niven/Niven.h>

using namespace niven;


class RouteCapturesModule : public Module
{
	public:

		RouteCapturesModule() : Module("/captures")
		{
			// The route can contain captures referenced by name. An individual
			// capture can then be retrieved via the context using the array
			// subscript operator.
			Get["/{id}"] = [](auto &c) { return "Just an ID of " + c["id"]; };

			// The captures tree can be accessed directly to simplify the retrieval
			// of a capture as a numeric type.
			Get["number/{number}"] = [](auto &c) {
				return "The number multiplied by 42 is " + std::to_string(42 * c.captures["number"].as_long());
			};

			// Multiple captures can be defined for a route
			Get["{id}/{name}"] = [](auto &c) { return "An ID of " + c["id"] + " with a name of " + c["name"]; };

			// Routes with captures can also contain addition literal segments.
			Get["{id}/literal"] = [](auto &c) { return "An ID of " + c["id"] + " with a literal URL component"; };

			// A greedy segment will attempt to capture everything that follows
			Get["greedy/{remainder*}"] = [](auto &c) { return "A greedy capture of " + c["remainder"]; };
		}
};

REGISTER_MODULE(RouteCapturesModule)
