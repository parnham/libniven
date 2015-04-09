#pragma once

#include <functional>
#include <vector>
#include <string>
#include <niven/Context.h>
#include <niven/Response.h>
#include <niven/Pipeline.h>


namespace niven
{
	class Module;

	// Represents a route and contains an action to be invoked
	// if a URL matching this route is requested. The router uses
	// the score to determine the best match for the request.
	class Route
	{
		friend class Router;
		friend class TrieNode;
		friend class GreedyNode;

		public:

			Route(Module *parent, std::string method, std::string path);

			// Assign an action to this route. Allows for a clean syntax
			// when used via the RouteBuilder in a module.
			Route &operator=(const Action &action);


		private:

			Action action = nullptr;
			Module *parent;

			std::string method;
			std::string path;
			std::vector<std::string> segments;	// Created from the full path
			int score = 0;						// Assigned when building the trie
	};


	// A result structure used during a search of the trie. It
	// also contains any captures from the URL.
	struct RouteMatch
	{
		std::shared_ptr<Route> route;
		ent::tree captures;
	};
}
