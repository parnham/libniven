#include "niven/Host.h"

using namespace std;
using namespace emg;


namespace niven
{
	NivenHost::NivenHost()
	{
		this->modules = Type<Module>::CreateAll();
		this->router.Initialise(this->modules);
	}


	int NivenHost::OnAccess(void *cls, MHD_Connection *connection, const char *url, const char *method, const char *version, const char *upload_data, size_t *upload_data_size, void **ptr)
	{
		if (!*ptr)
		{
			// First time this is called construct the context, microhttpd expects an immediate return.
			*ptr = new Context(static_cast<Dependencies *>(cls), { url, method, connection });

			return MHD_YES;
		}

		auto *context = static_cast<Context *>(*ptr);

		if (*upload_data_size > 0)
		{
			// If there is pending data then append it to the request body and return
			// as there may be more data to receive.
			context->request.body.append(upload_data, *upload_data_size);

			*upload_data_size = 0;

			return MHD_YES;
		}

		auto response = static_cast<NivenHost *>(cls)->BeforeRequest.Invoke(*context);

		if (response.status == Http::None)
		{
			// Invoke the router
			response = static_cast<NivenHost *>(cls)->router.Resolve(*context);

			static_cast<NivenHost *>(cls)->AfterRequest.Invoke(*context, response);
		}

		// Send the response to the client and clean up
		int result	= response.Send(connection);
		*ptr		= nullptr;

		delete context;
		return result;
	}


	bool NivenHost::Run()
	{
		if (!this->daemon)
		{
			// Logging wrapper function
			auto log = [](void *p, const char *s, va_list a) {
				char buffer[1024];
				vsnprintf(buffer, 1024, s, a);
				LOG(error, buffer);
			};

			if (this->ssl)
			{
				this->daemon = MHD_start_daemon(
					MHD_USE_SELECT_INTERNALLY | MHD_USE_EPOLL_LINUX_ONLY | MHD_USE_SSL | MHD_USE_DEBUG | MHD_USE_DUAL_STACK,
					this->port, nullptr, nullptr, &NivenHost::OnAccess, this,
					MHD_OPTION_THREAD_POOL_SIZE,	this->threads,
					MHD_OPTION_HTTPS_MEM_KEY,		this->sslKey.c_str(),
					MHD_OPTION_HTTPS_MEM_CERT,		this->sslCertificate.c_str(),
					MHD_OPTION_EXTERNAL_LOGGER, 	(MHD_LogCallback)log, nullptr,
					MHD_OPTION_END
				);
			}
			else
			{
				this->daemon = MHD_start_daemon(
					MHD_USE_SELECT_INTERNALLY | MHD_USE_EPOLL_LINUX_ONLY | MHD_USE_DEBUG | MHD_USE_DUAL_STACK,
					this->port, nullptr, nullptr, &NivenHost::OnAccess, this,
					MHD_OPTION_THREAD_POOL_SIZE,	this->threads,
					MHD_OPTION_EXTERNAL_LOGGER,		(MHD_LogCallback)log, nullptr,
					MHD_OPTION_END
				);
			}
		}

		return this->daemon;
	}


	void NivenHost::Stop()
	{
		if (this->daemon) MHD_stop_daemon(this->daemon);

		this->daemon = nullptr;
	}
}

