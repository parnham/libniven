#include "niven/Niven.h"
#include <signal.h>
#include <bits/signum.h>
#include <iostream>
#include <mutex>
#include <condition_variable>


#include "examples/Simple.hpp"
#include "examples/Entities.hpp"
#include "examples/Dependencies.hpp"
#include "examples/Pipeline.hpp"
#include "examples/Cookies.hpp"
#include "examples/Headers.hpp"
#include "examples/Query.hpp"
#include "examples/RouteCaptures.hpp"
#include "examples/Hashing.hpp"
#include "examples/Files.hpp"

// #include <niven/crypto/Hash.h>

using namespace std;
using namespace emg;
using namespace ent;
using namespace niven;
// using namespace niven::crypto;



class SampleModule : public Module
{
	public:

		SampleModule() : Module("/sample")
		{
			//Before	+= [](Context &c) { cout << "Before module" << endl; return nullptr; };
			//After	+= [](Context &c, Response &r) { cout << "After module" << endl; };

			//Get["/"]			= [](auto) { return "The moon's a balloon"; };
			// Get["/ok"]			= [](auto) { return Http::OK; };


			// Get["/test"]		= [](auto) { return Test(); };
			// Get["/tree"]		= [](auto) { return tree { { "name", "Test" }, { "list", vector<tree> { 1, 2, 3 }}}; };	// Equivalent of anonymous object in C#
			// // Post["/bind"] = [](auto &c) { return "The name is: " + Bind<Test>(c).name; };
			// Post["/bind"] = [](Context &c) { return "The name is: " + c.Bind<Test>().name; };


			// Get["/resource"]	= [](Context &c) { return c.Resolve<IResource>()->Get(); };
			// // Get["/resource"]	= [](auto &c) { return Resolve<IResource>(c)->Get(); };
			// Get["/redis/{key}"] 	= [](Context &c) { return c.Resolve<Redis>()->Get(c["key"]); };
			// Post["/redis/{key}"]	= [](Context &c) { c.Resolve<Redis>()->Set(c["key"], c.request.body); return Http::OK; };


			// Get["/query"] = [](auto &c) { return c.request.Parameter("test"); };

			// Get["/custom"]		= [](auto) { return Response().WithHeader("something", "else"); };

			// Get["/broken"]		= nullptr;

			// Get["/cookie"]		= [](auto) { return Response().WithCookie({"test", "value", DateTime::Now().AddSeconds(30)}); };
			// Get["/cookie/show"]	= [](auto &c) {

			// 	auto cookies = c.request.Cookies();

			// 	return std::accumulate(cookies.begin(), cookies.end(), string(), [](auto &acc, auto &i) {
			// 		return acc + i.first + " = " + i.second + "\n";
			// 	});
			// };

			// Get["/test/{id}"]	= [](auto &c) { return "ID=" + c["id"]; };
			// Get["/test/{id}/{name}"]	= [](auto &c) { return "ID=" + c["id"] + "  Name=" + c["name"]; };
			// Get["/test/{id}/literal"]	= [](auto &c) { return "ID=" + c["id"]; };


			Get["/file/{name*}"]		= [](auto &c) { return Path("/home/dan/temp") / c["name"]; };
			Get["/file/{name*}/bar"]	= [](auto &c) { return c["name"]; };

			// Get["/salt"]				= [](auto) { return Hash::Salt(); };
			// Get["/hash/{value}"]		= [](auto &c) { return Hash::AsBase64(c["value"], Hash::Salt(), Algorithm::SHA256);  };
		}
};

// REGISTER_MODULE(SampleModule)


bool run = true;
condition_variable condition;

int main(int argc, char **argv)
{
	// Enable logging to stdout
	logger::instance().add(new sink::console());
	logger::instance().set_verbosity("info");

	// Catch the interrupt/quit signals and handle appropriately
	signal(SIGINT,	[](int) { run = false; condition.notify_one(); });
	signal(SIGQUIT,	[](int) { run = false; condition.notify_one(); });


	NivenHost host;

	// Configure the host to listen on port 8090
	host.Listen(8090);


	//host.BeforeRequest	+= [](Context &c) { cout << "Before routing" << endl; return nullptr; };
	//host.AfterRequest	+= [](Context &c, Response &r) { cout << "The end" << endl; };


	// Register a RedisMultiplexer singleton with the dependency container.
	// Refer to https://github.com/emergent-design/libemergent/ for more
	// information about the C++ Redis wrappers. This Redis connection is
	// used in the "include/examples/Dependencies.hpp" example.
	host.Register<Redis>(make_shared<RedisMultiplexer>());

	// Resolve the singleton and invoke the Initialise function which will
	// connect to redis using the default parameters (127.0.0.1:6379).
	host.Resolve<Redis>()->Initialise();


	// Start the host running. This function will return true if successfully
	// started and since the host has its own threading we will use a condition
	// variable to wait for an interrupt or quit signal from the user.
	if (host.Run())
	{
		cout << "Listening on port 8090" << endl;

		mutex m;
		unique_lock<mutex> lock(m);
		while (run) condition.wait(lock);

		// Tell the host to stop listening.
		host.Stop();
	}
	else cout << "Failed to initialise host" << endl;

	return 0;
}
