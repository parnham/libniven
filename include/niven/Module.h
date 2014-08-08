#pragma once

#include <emergent/Type.h>
#include <niven/Request.h>
#include <niven/Pipeline.h>
#include <niven/routing/Route.h>


#define REGISTER_MODULE(name) REGISTER_TYPE(niven::NivenModule, name)


namespace niven
{
	class NivenModule
	{
		friend class Router;

		public:

			NivenModule(std::string path = "") :  path(path) {}

		protected:

			class RouteBuilder
			{
				public:

					RouteBuilder(NivenModule *parent, std::string method) : method(method), parent(parent) {}

					Route &operator[](const std::string path);

				private:

					std::string GetPath(std::string path);

					std::string method;
					NivenModule *parent;
			};

			RouteBuilder Get	= { this, "GET" };
			RouteBuilder Post	= { this, "POST" };
			RouteBuilder Put	= { this, "PUT" };
			RouteBuilder Delete	= { this, "DELETE" };

			BeforePipeline Before;
			AfterPipeline After;

		private:

			std::vector<std::shared_ptr<Route>> routes;
			std::string path;
	};
}
