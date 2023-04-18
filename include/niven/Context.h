#pragma once

#include <entity/entity.hpp>
#include <entity/json.hpp>
#include <emergent/Dependencies.hpp>
#include <niven/Request.h>
// #include <niven/Dependencies.h>
// #include <experimental/any>

namespace niven
{
	using emergent::Dependencies;

	// The context is passed to a matched route action. It contains the request information,
	// any captures from the URL, access to dependency resolution and binding helpers.
	class Context
	{
		public:

			Context(Dependencies *dependencies, const Request &request) : request(request), dependencies(dependencies) {}


			// Helper function to bind the request to an entity. It assumes that the body
			// of the request contains JSON.
			// template <class T, class = typename std::enable_if<std::is_base_of<ent::entity, T>::value>::type> T Bind()
			template <class T> T Bind()
			{
				// Check headers and type
				// if (this->request.headers["Content-Type"] == "application/json")
				// {
					// return ent::entity::decode<ent::json, T>(this->request.body);
					return ent::decode<ent::json, T>(this->request.body);
				// }

				// return T();
			}


			// Resolve the relevant singleton or instance from the dependency container.
			template <class Interface> std::shared_ptr<Interface> Resolve()
			{
				return this->dependencies->Resolve<Interface>();
			}


			// The current request
			Request request;

			// Captures from the URL (where appropriate). The tree structures acts like
			// a map but simplifies the conversion of values to primitive types.
			ent::tree captures;

			// Array subscript operator for shorthand access to the captures. Allows
			// you to treat the context as a map.
			std::string operator[](const std::string key) { return this->captures[key].as_string(); }

			// Storage for any per-request data - for example a "before" pipeline could
			// retrieve the user from an authentication module and then insert it into
			// the context here so that any module route can make use of this information.
			// std::map<std::string, std::experimental::any> data;

		private:

			// Reference to the dependency container (actually the NivenHost).
			Dependencies *dependencies = nullptr;
	};
}
