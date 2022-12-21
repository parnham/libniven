#include "niven/routing/nodes/GreedyNode.h"
#include <algorithm>
#include <numeric>

using namespace std;
using namespace emg;
using namespace ent;


namespace niven
{
	GreedyNode::GreedyNode(string segment) : TrieNode(segment, 0)
	{
		// Retrieve the capture name from the route segment.
		this->name = String::trim(String::trim(String::trim(segment, '{'), '}'), '*');
	}


	void GreedyNode::GetMatches(vector<RouteMatch> &results, const vector<string> &segments, int index, const tree &captures)
	{
		tree all = captures;

		// Add all remaining segments (stuck back together) as a capture.
		all.set(this->name, accumulate(
			segments.begin() + index, segments.end(), segments[index - 1],
			[](auto &a, auto &b) { return a + '/' + b; }
		));

		// A greedy node may still have children, retrieve any matches
		// for them (since they may have a higher score).
		if (this->children.size())
		{
			for (; index < (int)segments.size(); index++)
			{
				for (auto &c : this->children)
				{
					if (c.second->IsMatch(segments[index]))
					{
						c.second->GetMatches(results, segments, index + 1, c.second->GetCaptures(all, segments[index]));
					}
				}
			}
		}

		results.push_back({ this->route, all });
	}


	bool GreedyNode::IsMatch(const std::string &)
	{
		return true;
	}
}

