#include <niven/Niven.h>
#include <entity/entity.hpp>
#include <entity/tree.hpp>

using namespace niven;



// An example entity. Please refer to https://github.com/emergent-design/libentity
// for further information.
struct Test
{
	std::string name			= "Test";
	std::vector<int> list	= { 1, 2, 3 };

	emap(eref(name), eref(list))
};


class EntitiesModule : public Module
{
	public:

		EntitiesModule() : Module("/entities")
		{
			// Returning an object derived from ent::entity will automatically
			// generate an "application/json" reponse containing the serialised
			// instance.
			Get["/"] = [](auto) { return Test(); };

			// The tree class is a simple object tree provided by libentity. It allows
			// you to construct and manipulate complex structures without creating
			// explicit entity structures. These also support JSON based serialisation and
			// deserialisation. Simply returning a tree will generate an appropriate
			// "application/json" response.
			Get["/tree"] = [](auto) {
				return ent::tree {
					{ "name", "Test" },
					{ "list", std::vector<ent::tree> { 1, 2, 3 }}
				};
			};

			// If the request contains JSON data it can be bound back to an entity
			// as follows.
			Post["/bind"] = [](Context &c) { return "The name is: " + c.Bind<Test>().name; };
		}
};

REGISTER_MODULE(EntitiesModule)
