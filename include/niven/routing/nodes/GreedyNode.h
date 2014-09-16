#pragma once

#include <emergent/Emergent.h>
#include <niven/routing/nodes/TrieNode.h>


namespace niven
{
	class GreedyNode : public TrieNode
	{
		public:

			GreedyNode(std::string segment);

			virtual void GetMatches(std::vector<RouteMatch> &results, const std::vector<std::string> &segments, int index = 0, const std::map<std::string, std::string> &parameters = {});

			virtual SegmentMatch Match(std::string segment);


		private:

			std::string name;
	};
}
