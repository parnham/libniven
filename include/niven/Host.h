#pragma once

#include <emergent/Logger.h>
#include <niven/Module.h>
#include <niven/Http.h>
#include <niven/Dependencies.h>
#include <niven/Pipeline.h>
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

			BeforePipeline BeforeRequest;
			AfterPipeline AfterRequest;

		private:

			Context *Claim(const char *url, const char *method, MHD_Connection *connection);
			void Release(Context *context);

			std::map<std::string, std::unique_ptr<NivenModule>> modules;

			MHD_Daemon *daemon = nullptr;
			Router router;
	};
}
