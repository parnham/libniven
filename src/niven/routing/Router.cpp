#include "niven/routing/Router.h"
#include <emergent/logger/Logger.hpp>
#include <algorithm>

using namespace std;
using namespace emg;


namespace niven
{
	void Router::Initialise(map<string, unique_ptr<Module>> &modules)
	{
		// Build the trie
		for (auto &m : modules)
		{
			FLOG(info, "Registering routes for module %s", m.first);

			for (auto &r : m.second->routes)
			{
				this->trie[r->method].Add(r);
			}
		}
	}


	Response Router::Resolve(Context &context)
	{
		vector<RouteMatch> matches;
		auto method = context.request.method;
		auto path	= context.request.url;

		if (!path.empty() && this->trie.count(method))
		{
			// Find all relevant route matches for the request URL.
			this->trie[method].GetMatches(matches, String::explode(path, "/"));

			if (matches.size())
			{
				// Pick the best route by score, copy the captures and apply
				// any Before actions in the parent module.
				auto best 			= max_element(matches.begin(), matches.end(), [](auto &a, auto &b) { return a.route->score < b.route->score; });
				context.captures 	= best->captures;
				auto response		= best->route->parent->Before.Invoke(context);

				if (response.status == Http::None)
				{
					// If the Before actions did not return a response status
					// then run the route specific action. An error response is
					// generated in the event that an action throws an exception.
					try
					{
						if (best->route->action)
						{
							response = best->route->action(context);
						}
						else return { "Missing action for route: " + best->route->path, Http::InternalServerError };
					}
					catch (const std::exception &e)
					{
						return { e.what(), Http::InternalServerError };
					}
				}

				// Apply any After actions in the parent module.
				best->route->parent->After.Invoke(context, response);

				return response;
			}
		}

		return Http::NotFound;
	}
}

