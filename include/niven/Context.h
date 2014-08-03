#pragma once

#include <entity/json.h>
#include <niven/Request.h>
#include <niven/Dependencies.h>

//#include <iostream>

namespace niven
{
	class Context
	{
		public:
			Request request;
			ent::tree parameters;	// Any parameters that were part of the path
			//std::map<std::string, std::string> parameters;	// tree?

			std::string operator[](const std::string key) { return this->parameters.get<std::string>(key); }


			Context() {}
			Context(Dependencies *dependencies, const Request &request) : request(request), dependencies(dependencies) {}


			template <class T, class = typename std::enable_if<std::is_base_of<ent::entity, T>::value>::type> T bind()
			{
				// Check headers and type
				//if (this->request.headers["Content-Type"] == "application/json")
				//{
					return ent::entity::from<T, ent::json>(this->request.body);
				//}

				//return T();
			}

			template <class Interface> std::shared_ptr<Interface> Resolve()
			{
				return this->dependencies->Resolve<Interface>();
			}

		private:

			Dependencies *dependencies = nullptr;
	};
}
