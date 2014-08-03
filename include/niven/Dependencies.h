#pragma once

#include <functional>
#include <utility>
#include <memory>
#include <map>

#include <iostream>

namespace niven
{
	class Dependencies
	{
		public:

			template <class Interface, class Concrete, class... Parameters> void Register(Parameters... parameters)
			{
				static_assert(std::is_base_of<Interface, Concrete>::value, "Concrete class must be derived from Interface type");

				// The bind is a workaround until the next version of GCC which fixes an
				// issue with captured parameter pack expansion within a lambda.
				this->containers[typeid(Interface).name()].Set(std::bind(
					[](Parameters... parameters) {
						return new Concrete(std::forward<Parameters>(parameters)...);
					},
					std::forward<Parameters>(parameters)...
				));
			}


			template <class Interface> void Register(std::shared_ptr<Interface> singleton)
			{
				auto copy = new std::shared_ptr<Interface>(singleton);

				this->containers[typeid(Interface).name()].Set([=]() { return copy; }, [=]() { delete copy; });
			}


			template <class Interface> std::shared_ptr<Interface> Resolve()
			{
				auto name 	= typeid(Interface).name();
				auto &con	= this->containers[name];

				if (con.retrieve)
				{
					return con.dispose
						? *static_cast<std::shared_ptr<Interface> *>(this->containers[name].retrieve())
						: std::shared_ptr<Interface>((Interface *)this->containers[name].retrieve());
				}

				return nullptr;
			}


		private:

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
