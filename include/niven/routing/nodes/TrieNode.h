#pragma once

#include <niven/routing/Route.h>
#include <entity/entity.h>


namespace niven
{
	typedef std::pair<bool, std::map<std::string, std::string>> SegmentMatch;


	class TrieNode
	{
		public:

			TrieNode(std::string segment, int score) : score(score), routeSegment(segment) {}

			void Add(std::shared_ptr<Route> route, int index = 0, int score = 0);

			virtual void GetMatches(std::vector<RouteMatch> &results, const std::vector<std::string> &segments, int index = 0, const std::map<std::string, std::string> &parameters = {});
			//, ent::tree parameters = ent::tree());

			virtual SegmentMatch Match(std::string segment) = 0;

		protected:

			int score;
			std::string routeSegment;
			std::map<std::string, std::shared_ptr<TrieNode>> children;
			std::vector<std::shared_ptr<Route>> routes;	// Routes that end at this node

		private:

			std::shared_ptr<TrieNode> Create(std::string segment);
	};
}
