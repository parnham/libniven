#pragma once

#include <emergent/logger/Logger.hpp>
#include <emergent/Dependencies.hpp>
#include <niven/Configuration.h>
#include <niven/Module.h>
#include <niven/Http.h>
#include <niven/Dependencies.h>
#include <niven/Pipeline.h>
#include <niven/routing/Router.h>
#include <microhttpd.h>



namespace niven
{
	// The host is responsible for creating the modules, configuring
	// the daemon, acting as a dependency container and handling
	// requests.
	class NivenHost : public emg::Dependencies, public Configuration
	{
		public:

			NivenHost();

			~NivenHost();

			// Creates the daemon (if it has not been already). The daemon uses its own
			// thread pool and so this function returns immediately. Will return true
			// if the daemon was created successfully.
			bool Run();

			// Stops the daemon if there is one running.
			void Stop();

			// Global pipeline allowing a request to be parsed/ modified before routing is invoked.
			// If any functions in the pipeline return a status other than Http::None then
			// the response is sent immediately to the client bypassing the routing, modules
			// and AfterRequest pipeline.
			BeforePipeline BeforeRequest;

			// Global pipeline allowing a response to be modified before sending to the client.
			AfterPipeline AfterRequest;

		private:

			static auto OnAccess(void *cls, MHD_Connection *connection, const char *url, const char *method, const char *version, const char *upload_data, size_t *upload_data_size, void **ptr);

			std::map<std::string, std::unique_ptr<Module>> modules;
			MHD_Daemon *daemon = nullptr;
			Router router;
	};
}
