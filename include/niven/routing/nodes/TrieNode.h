#pragma once

#include <niven/routing/Route.h>
#include <entity/tree.hpp>


namespace niven
{
	// Base class for all nodes in the route trie.
	class TrieNode
	{
		public:

			TrieNode(std::string segment, int score) : score(score), routeSegment(segment) {}

			// Add a route to this node (will generate child nodes where necessary).
			void Add(std::shared_ptr<Route> route, int index = 0, int score = 0);

			// Find all matches for the request URL (split into string segments).
			virtual void GetMatches(std::vector<RouteMatch> &results, const std::vector<std::string> &segments, int index = 0, const ent::tree &captures = {});

			// Does this node type match the current URL segment.
			virtual bool IsMatch(const std::string &segment) = 0;

			// Where appropriate, retrieve the captures for this node type.
			virtual const ent::tree GetCaptures(const ent::tree &captures, const std::string &segment) { return captures; }

		protected:

			int score;
			std::string routeSegment;
			std::map<std::string, std::shared_ptr<TrieNode>> children;
			std::shared_ptr<Route> route;	// Route that ends at this node.

		private:

			// Create a new node type from the route segment.
			std::shared_ptr<TrieNode> Create(std::string segment);
	};
}
