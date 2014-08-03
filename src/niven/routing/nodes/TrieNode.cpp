#include "niven/routing/nodes/TrieNode.h"
#include "niven/routing/nodes/LiteralNode.h"
#include "niven/routing/nodes/CaptureNode.h"
#include <algorithm>

using namespace std;


namespace niven
{

	void TrieNode::Add(shared_ptr<Route> route, int index, int score)
	{
		if (index >= route->segments.size())
		{
			route->score = score + this->score;
			this->routes.push_back(route);

			cout << "Added " << route->path << " at " << index << " with a score of " << route->score << endl;
		}
		else
		{
			auto seg	= route->segments[index];
			auto child	= this->children.count(seg) ? this->children[seg] : this->children[seg] = this->Create(seg);

			child->Add(route, index + 1, score + this->score);
		}
	}


	vector<RouteMatch> TrieNode::GetMatches(const vector<string> &segments, int index, ent::tree parameters)
	{
		vector<RouteMatch> result;

		if (index >= segments.size())
		{
			for (auto &r : this->routes) result.push_back({ r, parameters });
		}
		else
		{
			for (auto &c : this->children)
			{
				auto match = c.second->Match(segments[index]);

				if (match.match)
				{
					match.parameters.properties.insert(parameters.properties.begin(), parameters.properties.end());

					for (auto &m : c.second->GetMatches(segments, index + 1, match.parameters))
					{
						result.push_back(m);
					}
				}
			}
		}

		return result;
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


/*
        public virtual TrieNode GetNodeForSegment(TrieNode parent, string segment)
        {
            if (parent == null)
            {
                return new RootNode(this);
            }

            var chars = segment.ToCharArray();
            var start = chars[0];
            var end = chars[chars.Length - 1];

            if (start == '(' && end == ')')
            {
                return new RegExNode(parent, segment, this);
            }

            if (start == '{' && end == '}' && chars.Count(c => c == '{' || c == '}') == 2)
            {
                return this.GetCaptureNode(parent, segment);
            }

            if (segment.StartsWith("^(") && (segment.EndsWith(")") || segment.EndsWith(")$")))
            {
                return new GreedyRegExCaptureNode(parent, segment, this);
            }

            if (CaptureNodeWithMultipleParameters.IsMatch(segment))
            {
                return new CaptureNodeWithMultipleParameters(parent, segment, this);
            }

            return new LiteralNode(parent, segment, this);
        }

    }
}
*/
