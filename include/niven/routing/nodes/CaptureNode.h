#pragma once

#include <emergent/Emergent.h>
#include <niven/routing/nodes/TrieNode.h>


namespace niven
{
	// A capture node will always match agains a segment and that
	// segment will be stored in the captures by name.
	class CaptureNode : public TrieNode
	{
		public:

			CaptureNode(std::string segment) : TrieNode(segment, 1000)
			{
				// Determine the name of this capture from the route segment.
				this->name = emg::String::trim(emg::String::trim(segment, '{'), '}');
			}


			bool IsMatch(const std::string &segment)
			{
				return true;
			}


			const ent::tree GetCaptures(const ent::tree &captures, const std::string &segment)
			{
				// The capture is simply the segment.
				return ent::tree(captures).set(this->name, segment);
			}


		private:

			std::string name;
	};
}
