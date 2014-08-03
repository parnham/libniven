#pragma once

#include <niven/routing/Route.h>
#include <entity/entity.h>


namespace niven
{
		//TODO: Move to own header
	struct SegmentMatch
	{
		bool match = false;
		ent::tree parameters;	// or map<string, string>??

		SegmentMatch(bool match) : match(match) {}
		SegmentMatch(bool match, const ent::tree &parameters) : match(match), parameters(parameters) {}
	};


	class TrieNode
	{
		public:

			TrieNode(std::string segment, int score) : score(score), routeSegment(segment) {}

			void Add(std::shared_ptr<Route> route, int index = 0, int score = 0);

			std::vector<RouteMatch> GetMatches(const std::vector<std::string> &segments, int index = 0, ent::tree parameters = ent::tree());

		protected:

			virtual SegmentMatch Match(std::string segment) = 0;

			int score;
			std::string routeSegment;
			std::map<std::string, std::shared_ptr<TrieNode>> children;
			std::vector<std::shared_ptr<Route>> routes;	// Routes that end at this node

		private:

			std::shared_ptr<TrieNode> Create(std::string segment);
	};
}
