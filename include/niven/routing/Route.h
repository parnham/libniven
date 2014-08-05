#pragma once

#include <functional>
#include <vector>
#include <string>
#include <niven/Context.h>
#include <niven/Response.h>


namespace niven
{
	typedef std::function<Response(Context&)> Action;


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
	};


	struct RouteMatch
	{
		std::shared_ptr<Route> route;
		std::map<std::string, std::string> parameters;
	};
}
