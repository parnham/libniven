#include "niven/Module.h"


namespace niven
{
	Route &NivenModule::RouteBuilder::operator[](const std::string path)
	{
		auto route = std::make_shared<Route>(this->parent, this->method, this->GetPath(path));

		this->parent->routes.push_back(route);

		return *route;
	}


	std::string NivenModule::RouteBuilder::GetPath(std::string path)
	{
		auto relative	= emg::String::trim(path, '/');
		auto parent		= emg::String::trim(this->parent->path, '/');

		return "/" + (parent.empty() ? relative : relative.empty() ? parent : parent + "/" + relative);
	}
}
