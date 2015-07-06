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

using namespace std;
using namespace emg;
using namespace niven;


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
