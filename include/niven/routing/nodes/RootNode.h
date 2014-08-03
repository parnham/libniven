#pragma once

#include <niven/routing/nodes/TrieNode.h>


namespace niven
{
	class RootNode : public TrieNode
	{
		public:
			RootNode() : TrieNode("", 0) {}

			SegmentMatch Match(std::string segment)
			{
				return { true };
			}
	};
}
