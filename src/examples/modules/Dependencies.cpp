#include <niven/Niven.h>
#include <emergent/redis/RedisMultiplexer.hpp>

using namespace niven;
using namespace emg::redis;


// Niven contains a simple dependency container which supports both singletons and
// multi-instance objects. Refer to the "src/examples/example.cpp" to see how the
// RedisMultiplexer singleton used in this example was registered with the host.
class DependenciesModule : public Module
{
	public:

		DependenciesModule() : Module("/dependencies")
		{
			// Using a route capture to provide the key, this will resolve the Redis
			// singleton and use it to retrieve the string at that key.
			Get["/{key}"] = [](Context &c) { return c.Resolve<Redis>()->Get(c["key"]); };

			// Set the string in Redis to the value of the POST body at the supplied key.
			Post["/{key}"] = [](Context &c) {
				c.Resolve<Redis>()->Set(c["key"], c.request.body);
				return Http::OK;
			};
		}
};

REGISTER_MODULE(DependenciesModule)
