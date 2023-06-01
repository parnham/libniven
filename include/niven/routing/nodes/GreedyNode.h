#pragma once

#include <emergent/Emergent.hpp>
#include <niven/routing/nodes/TrieNode.h>


namespace niven
{
	// A greedy node will match against all remaining URL segments.
	// The request URL from this point will be stored in the captures.
	class GreedyNode : public TrieNode
	{
		public:

			GreedyNode(std::string segment);

			void GetMatches(std::vector<RouteMatch> &results, const std::vector<std::string> &segments, int index = 0, const ent::tree &captures = {}) override;

			bool IsMatch(const std::string &segment) override;


		private:

			std::string name;
	};
}
