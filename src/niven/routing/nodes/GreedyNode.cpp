#include "niven/routing/nodes/GreedyNode.h"
#include <algorithm>

using namespace std;
using namespace emg;

namespace niven
{
	GreedyNode::GreedyNode(string segment) : TrieNode(segment, 0)
	{
		this->name = String::trim(String::trim(String::trim(segment, '{'), '}'), '*');
	}


	SegmentMatch GreedyNode::Match(string segment)
	{
		return { true, {} };
	}


	void GreedyNode::GetMatches(vector<RouteMatch> &results, const vector<string> &segments, int index, const map<string, string> &parameters)
	{
		auto p			= parameters;
		p[this->name]	= accumulate(
			segments.begin() + index - 1, segments.end(), string(),
			[](const string &a, const string &b) { return a + '/' + b; }
		);

		if (this->children.size())
		{
			for (; index < segments.size(); index++)
			{
				for (auto &c : this->children)
				{
					auto match = c.second->Match(segments[index]);

					if (match.first)
					{
						match.second.insert(p.begin(), p.end());
						c.second->GetMatches(results, segments, index + 1, match.second);
					}
				}
			}
		}

		for (auto &r : this->routes) results.push_back({r, p});
	}
}

