#pragma once

#include <emergent/Type.h>
#include <niven/Request.h>
#include <niven/routing/Route.h>


#define REGISTER_MODULE(name) REGISTER_TYPE(niven::NivenModule, name)


namespace niven
{
	class NivenModule
	{
		friend class Router;

		public:

			NivenModule(std::string path = "") :  path(path) {}

			//std::vector<Route> &Routes() { return this->routes; }

		protected:

			class RouteBuilder
			{
				public:

					RouteBuilder(NivenModule *parent, std::string method) : method(method), parent(parent) {}

					Route &operator[](const std::string path)
					{
						auto route = std::make_shared<Route>(this->method, this->GetPath(path));

						this->parent->routes.push_back(route);

						return *route;
					}

				private:

					std::string GetPath(std::string path)
					{
						auto relative	= emg::trim(path, '/');
						auto parent		= emg::trim(this->parent->path, '/');

						return "/" + (parent.empty() ? relative : relative.empty() ? parent : parent + "/" + relative);
					}

					std::string method;
					NivenModule *parent;
			};

			RouteBuilder Get	= { this, "GET" };
			RouteBuilder Post	= { this, "POST" };
			RouteBuilder Put	= { this, "PUT" };
			RouteBuilder Delete	= { this, "DELETE" };

		private:

			std::vector<std::shared_ptr<Route>> routes;
			std::string path;
	};
}
