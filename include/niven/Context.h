#pragma once

#include <entity/entity.hpp>
#include <entity/json.hpp>
#include <niven/Request.h>
#include <niven/Dependencies.h>


namespace niven
{
	// The context is passed to a matched route action. It contains the request information,
	// any captures from the URL, access to dependency resolution and binding helpers.
	class Context
	{
		public:

			Context(Dependencies *dependencies, const Request &request) : request(request), dependencies(dependencies) {}


			// Helper function to bind the request to an entity. It assumes that the body
			// of the request contains JSON.
			template <class T, class = typename std::enable_if<std::is_base_of<ent::entity, T>::value>::type> T Bind()
			{
				// Check headers and type
				// if (this->request.headers["Content-Type"] == "application/json")
				// {
					return ent::entity::decode<ent::json, T>(this->request.body);
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


		private:

			// Reference to the dependency container (actually the NivenHost).
			Dependencies *dependencies = nullptr;
	};
}
