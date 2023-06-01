#pragma once

#include <niven/routing/nodes/TrieNode.h>


namespace niven
{
	// A literal node must match a segment of the request URL precisely.
	class LiteralNode : public TrieNode
	{
		public:

			LiteralNode(std::string segment) : TrieNode(segment, 10000) {}


			bool IsMatch(const std::string &segment) override
			{
				return segment == this->routeSegment;
			}
	};
}
