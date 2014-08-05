#pragma once

#include <emergent/Emergent.h>
#include <niven/routing/nodes/TrieNode.h>


namespace niven
{
	class CaptureNode : public TrieNode
	{
		public:

			CaptureNode(std::string segment) : TrieNode(segment, 1000)
			{
				this->name = emg::trim(emg::trim(segment, '{'), '}');
			}

			SegmentMatch Match(std::string segment)
			{
				return { true, {{ this->name, segment }} };
			}

		private:

			std::string name;
	};
}
