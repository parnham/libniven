#pragma once

#include <niven/routing/nodes/TrieNode.h>


namespace niven
{
	// The route trie contains a root node for each
	// of the HTTP verbs in use.
	class RootNode : public TrieNode
	{
		public:

			RootNode() : TrieNode("", 0) {}


			bool IsMatch(const std::string &segment)
			{
				return true;
			}
	};
}
