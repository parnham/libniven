#pragma once

#include <niven/Module.h>
#include <niven/routing/nodes/RootNode.h>


namespace niven
{

	class Router
	{
		public:

			void Initialise(std::map<std::string, std::unique_ptr<NivenModule>> &modules);

			Response Resolve(Context &context);

		private:

			std::map<std::string, RootNode> trie;
	};

}
