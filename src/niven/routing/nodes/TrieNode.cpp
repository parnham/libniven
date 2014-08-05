#include "niven/routing/nodes/TrieNode.h"
#include "niven/routing/nodes/LiteralNode.h"
#include "niven/routing/nodes/CaptureNode.h"
#include <emergent/Logger.h>
#include <algorithm>

using namespace std;
using namespace emg;

namespace niven
{

	void TrieNode::Add(shared_ptr<Route> route, int index, int score)
	{
		if (index >= route->segments.size())
		{
			route->score = score + this->score;
			this->routes.push_back(route);

			FLOG(info, "Added %s at %d with a score of %d", route->path, index, route->score);
		}
		else
		{
			auto seg	= route->segments[index];
			auto child	= this->children.count(seg) ? this->children[seg] : this->children[seg] = this->Create(seg);

			child->Add(route, index + 1, score + this->score);
		}
	}


	void TrieNode::GetMatches(vector<RouteMatch> &results, const vector<string> &segments, int index, const std::map<std::string, std::string> &parameters)
	{
		if (index >= segments.size())
		{
			for (auto &r : this->routes) results.push_back({ r, parameters });
		}
		else
		{
			for (auto &c : this->children)
			{
				auto match = c.second->Match(segments[index]);

				if (match.first)
				{
					match.second.insert(parameters.begin(), parameters.end());
					c.second->GetMatches(results, segments, index + 1, match.second);
				}
			}
		}
	}


	// Create different types of node based on the segment
	shared_ptr<TrieNode> TrieNode::Create(string segment)
	{
		char start 	= segment.front();
		char end	= segment.back();

		/*if (start == '(' && end == ')')
		{
			return make_shared<RegExNode>(segment);
		}*/

		if (start == '{' && end == '}' && count(segment.begin(), segment.end(), '{') == 1 && count(segment.begin(), segment.end(), '}') == 1)
		{
			/*if (count(segment.begin(), segment.end(), ':'))
			{
				return make_shared<ConstrainedCaptureNode>(segment);
			}

			if (segment.substr(segment.size() - 2) == "?}")
			{
				return make_shared<OptionalCaptureNode>(segment);
			}

			if (segment.substr(segment.size() - 2) == "*}")
			{
				return make_shared<GreedyCaptureNode>(segment);
			}

			if (count(segment.begin(), segment.end(), '?'))
			{
				return make_shared<CaptureNodeWithDefault>(segment);
			}*/

			return make_shared<CaptureNode>(segment);
		}

		/*if (segment.substr(0, 2) == "^(" && (end == ')' || segment.substr(segment.size() - 2) == ")$"))
		{
			return make_shared<GreedyRegExNode>(segment);
		}*/

		return make_shared<LiteralNode>(segment);
	}
}

