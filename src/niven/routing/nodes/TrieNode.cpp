#include "niven/routing/nodes/TrieNode.h"
#include "niven/routing/nodes/LiteralNode.h"
#include "niven/routing/nodes/CaptureNode.h"
#include "niven/routing/nodes/GreedyNode.h"
#include <emergent/logger/Logger.hpp>
#include <algorithm>

using namespace std;
using namespace emg;
using namespace ent;

namespace niven
{

	void TrieNode::Add(shared_ptr<Route> route, int index, int score)
	{
		// If this is a leaf node for the route then generate a score
		// and store the route. Otherwise find/create the correct type
		// of child node and pass the route on down.
		if (index >= (int)route->segments.size())
		{
			route->score = score + this->score;

			if (!this->route || route->score > this->route->score)
			{
				this->route = route;
				Log::Info("  - added %s %s at %d (score = %d)", route->method, route->path, index, route->score);
			}
			else
			{
				Log::Info("  - ignored %s %s because a route already exists at this node", route->method, route->path);
			}
		}
		else
		{
			auto seg	= route->segments[index];
			auto child	= this->children.count(seg) ? this->children[seg] : this->children[seg] = this->Create(seg);

			child->Add(route, index + 1, score + this->score);
		}
	}


	void TrieNode::GetMatches(vector<RouteMatch> &results, const vector<string> &segments, int index, const tree &captures)
	{
		// If this is a leaf node then add its route to the results otherwise
		// retrieve matches for any child nodes.
		if (index >= (int)segments.size())
		{
			if (this->route)
			{
				results.push_back({ this->route, captures });
			}
		}
		else
		{
			auto &segment = segments[index];

			for (auto &c : this->children)
			{
				if (c.second->IsMatch(segment))
				{
					c.second->GetMatches(results, segments, index + 1, c.second->GetCaptures(captures, segment));
				}
			}
		}
	}


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
			}*/

			if (segment.substr(segment.size() - 2) == "*}")
			{
				return make_shared<GreedyNode>(segment);
			}

			/*if (count(segment.begin(), segment.end(), '?'))
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

