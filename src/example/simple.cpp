#include "niven/Niven.h"
#include <signal.h>
#include <bits/signum.h>
#include <iostream>
#include <mutex>
#include <condition_variable>

#include <entity/json.h>

using namespace std;
using namespace emg;
using namespace ent;
using namespace niven;



class IResource
{
	public:
		virtual string Get() = 0;
};


class ConcreteResource : public IResource
{
	public:
		virtual string Get() { return "ConcreteResource"; }
};

class AnotherResource  : public IResource
{
	public:

		int number;

		AnotherResource(int needsNumber) : number(needsNumber) {}
		virtual string Get() { return "AnotherResource"; }
};


struct Test : entity
{
	string name = "Test";

	mapping map()
	{
		return mapping() << eref(name);
	}
};


class SampleModule : public NivenModule
{
	public:

		SampleModule() : NivenModule("/sample")
		{
			//Before += [](Context &c)
			//After +=	[](Context &c, Response &r)?? or response inside context?

			Get["/"]			= [](Context &c) { return "The moon's a balloon"; };
			Get["/test"]		= [](Context &c) { return Test(); };
			Get["/resource"]	= [](Context &c) { return c.Resolve<IResource>()->Get(); };
			Get["/ok"]			= [](Context &c) { return Http::OK; };
			Get["/custom"]		= [](Context &c) { return Response().WithHeader("something", "else"); };
			Get["/test/{id}"]	= [](Context &c) { return "ID=" + c["id"]; };
			Get["/broken"]		= nullptr;

			Get["/test/{id}/{name}"]	= [](Context &c) { return "ID=" + c["id"] + "  Name=" + c["name"]; };
			Get["/test/{id}/literal"]	= [](Context &c) { return "ID=" + c["id"]; };

			Post["/bind"] = [](Context &c) { return "The name is: " + c.bind<Test>().name; };
		}
};

REGISTER_MODULE(SampleModule)


bool run = true;
condition_variable condition;

int main(int argc, char **argv)
{
	/*auto resource = shared_ptr<IResource>(new ConcreteResource());

	cout << "Singleton" << endl;
	cout << "  Count = " << resource.use_count() << endl;
	{
		Dependencies dep;
		dep.Register<IResource>(resource);

		cout << "  Count = " << resource.use_count() << endl;

		{
			vector<shared_ptr<IResource>> resources;

			for (int i=0; i<10; i++)
			{
				resources.push_back(dep.Resolve<IResource>());

				cout << "  " << (long)resources[i].get() << endl;
				cout << "  Count = " << resource.use_count() << endl;
			}
		}

		cout << "  Count = " << resource.use_count() << endl;
	}
	cout << "  Count = " << resource.use_count() << endl;

	cout << endl << "Multiple" << endl;
	{
		Dependencies dep;

		dep.Register<IResource, ConcreteResource>();

		vector<shared_ptr<IResource>> resources;

		for (int i=0; i<10; i++)
		{
			resources.push_back(move(dep.Resolve<IResource>()));

			cout << "  " << (long)resources[i].get() << endl;
			cout << "  Count = " << resources[i].use_count() << endl;

			//cout << ((AnotherResource *)resources[i].get())->number << endl;
		}
	}

	return 0;
	//*/

	logger::instance().add(new sink::console());
	logger::instance().set_verbosity("info");
	signal(SIGINT,	[](int) { run = false; condition.notify_one(); });
	signal(SIGQUIT,	[](int) { run = false; condition.notify_one(); });

	mutex m;
	NivenHost host;

	//host.Configure()...

	host.Register<IResource, ConcreteResource>();
	//host.Register<IResource, AnotherResource>(42);

	if (host.Run(8090))
	{
		cout << "Listening on port 8090" << endl;

		unique_lock<mutex> lock(m);
		while (run) condition.wait(lock);

		host.Stop();
	}
	else cout << "Failed to initialise host" << endl;

	return 0;
}
