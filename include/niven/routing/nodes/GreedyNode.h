#pragma once

#include <emergent/Emergent.h>
#include <niven/routing/nodes/TrieNode.h>


namespace niven
{
	// A greedy node will match against all remaining URL segments.
	// The request URL from this point will be stored in the captures.
	class GreedyNode : public TrieNode
	{
		public:

			GreedyNode(std::string segment);

			virtual void GetMatches(std::vector<RouteMatch> &results, const std::vector<std::string> &segments, int index = 0, const ent::tree &captures = {});

			virtual bool IsMatch(const std::string &segment);


		private:

			std::string name;
	};
}
