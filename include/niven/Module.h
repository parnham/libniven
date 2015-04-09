#pragma once

#include <emergent/Type.h>
#include <niven/Request.h>
#include <niven/Pipeline.h>
#include <niven/routing/Route.h>


// Helper macro for registering a module using the emergent Type system.
#define REGISTER_MODULE(name) REGISTER_TYPE(niven::Module, name)


namespace niven
{
	// The base class for niven modules. Simply derive your module from
	// this and then add GET/POST/PUT/DELETE routes in the constructor
	// (see the src/examples directory).
	class Module
	{
		friend class Router;

		public:

			// A derived module can specify a base path for itself. All
			// routes within that module will then begin with that path.
			Module(std::string path = "") :  path(path) {}


		protected:

			// Helper class for adding routes to the module.
			class RouteBuilder
			{
				public:

					RouteBuilder(Module *parent, std::string method) : method(method), parent(parent) {}

					Route &operator[](const std::string path);

				private:

					// Construct the full path of the route (including the module base path).
					std::string GetPath(std::string path);

					std::string method;
					Module *parent;
			};


			// Standard REST methods.
			RouteBuilder Get	= { this, "GET" };
			RouteBuilder Post	= { this, "POST" };
			RouteBuilder Put	= { this, "PUT" };
			RouteBuilder Delete	= { this, "DELETE" };


			// template <class T, class = typename std::enable_if<std::is_base_of<ent::entity, T>::value>::type> static T Bind(Context &c)
			// {
			// 	return ent::entity::decode<ent::json, T>(c.request.body);
			// }

			// template <class Interface> static std::shared_ptr<Interface> Resolve(Context &c)
			// {
			// 	return c.Resolve<Interface>();
			// }


			// Module specific pipeline allowing requests to be parsed/modified before
			// the route specific action is invoked.
			BeforePipeline Before;

			// Module specific pipeline allowing responses to be modified after the
			// route specific action is invoked.
			AfterPipeline After;


		private:

			std::vector<std::shared_ptr<Route>> routes;
			std::string path;
	};
}
