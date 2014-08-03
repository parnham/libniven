#pragma once

#include <emergent/Logger.h>
#include <niven/Module.h>
#include <niven/Http.h>
#include <niven/Dependencies.h>
#include <niven/routing/Router.h>

#include <microhttpd.h>


namespace niven
{


	class NivenHost : public Dependencies
	{
		public:

			NivenHost();

			bool Run(int port, int poolSize = 8);
			void Stop();

			//Configuration &Configure()...

		private:

			std::map<std::string, std::unique_ptr<NivenModule>> modules;

			MHD_Daemon *daemon = nullptr;
			Router router;
	};
}
