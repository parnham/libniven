#pragma once

#include <niven/routing/nodes/TrieNode.h>


namespace niven
{
	class LiteralNode : public TrieNode
	{
		public:
			LiteralNode(std::string segment) : TrieNode(segment, 10000) {}

			SegmentMatch Match(std::string segment)
			{
				return {  segment == this->routeSegment };
			}
	};
}
