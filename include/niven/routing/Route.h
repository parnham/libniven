#pragma once

#include <functional>
#include <vector>
#include <string>
#include <niven/Context.h>
#include <niven/Response.h>


namespace niven
{
	typedef std::function<Response(Context)> Action;


	/*class RouteDescription
	{
		public:

			RouteDescription(std::string method, std::string path) : name(name), method(method), path(path) {}

		protected:

			std::string method;
			std::string path;

			std::vector<std::string> segments;
	};*/



	class Route
	{
		friend class Router;
		friend class TrieNode;

		public:

			Route(std::string method, std::string path);

			Route &operator=(const Action action);

		private:
			Action action = nullptr;

			//std::string module;
			std::string method;
			std::string path;
			std::vector<std::string> segments;	// Created from the full path
			int score;							// Assigned when building the trie
			//RouteDescription description;
			//

	};


	struct RouteMatch
	{
		std::shared_ptr<Route> route;
		ent::tree parameters;	// or map<string, string>??
	};
}
