#pragma once

#include <niven/Module.h>
#include <niven/routing/nodes/RootNode.h>


namespace niven
{
	// Responsible for generating the route trie and resolving requests.
	class Router
	{
		public:

			// Generate the route trie from the given modules
			void Initialise(std::map<std::string, std::unique_ptr<Module>> &modules);

			// Find the best route match for the supplied request and then
			// apply the action (plus any module specific actions) returning
			// the resulting response.
			Response Resolve(Context &context);


		private:

			// The route trie containing root nodes for each for the HTTP
			// verbs in use.
			std::map<std::string, RootNode> trie;
	};

}
