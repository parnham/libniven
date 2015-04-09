#pragma once

#include <functional>
#include <utility>
#include <memory>
#include <map>


namespace niven
{
	// A dependency container implementation with basic functionality.
	class Dependencies
	{
		public:

			// Register a concrete class type with the container. Instances will be created (using the supplied
			// constructor parameters) each time it is resolved.
			template <class Interface, class Concrete, class... Parameters> void Register(Parameters... parameters)
			{
				static_assert(std::is_base_of<Interface, Concrete>::value, "Concrete class must be derived from Interface type");

				this->containers[typeid(Interface).name()].Set([parameters...]() { return new Concrete(parameters...); });
			}


			// Register a singleton with the container. Any registered singleton must be thread safe since
			// it will be accessed from concurrent request handlers.
			template <class Interface> void Register(std::shared_ptr<Interface> singleton)
			{
				auto copy = new std::shared_ptr<Interface>(singleton);

				this->containers[typeid(Interface).name()].Set([=]() { return copy; }, [=]() { delete copy; });
			}


			// Resolve the required type. Use the same function to retrieve singletons
			// and standard instances alike.
			template <class Interface> std::shared_ptr<Interface> Resolve()
			{
				auto &con = this->containers[typeid(Interface).name()];

				if (con.retrieve)
				{
					return con.dispose
						? *static_cast<std::shared_ptr<Interface> *>(con.retrieve())
						: std::shared_ptr<Interface>((Interface *)con.retrieve());
				}

				return nullptr;
			}


		private:

			// Container for a given base type. It simply contains functions
			// for dependency retrieval and cleaning up.
			struct Container
			{
				std::function<void *()> retrieve	= nullptr;
				std::function<void()> dispose		= nullptr;	// Singleton has a dispose


				void Set(std::function<void *()> retrieve, std::function<void()> dispose = nullptr)
				{
					if (this->dispose) this->dispose();

					this->retrieve	= retrieve;
					this->dispose	= dispose;
				}


				~Container()
				{
					if (this->dispose) this->dispose();
				}
			};


			std::map<std::string, Container> containers;
	};
}
