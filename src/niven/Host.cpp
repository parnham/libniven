#include "niven/Host.h"
#include <microhttpd.h>

// Debug
#include <entity/json.h>

using namespace std;
using namespace emg;

// #define INITIAL_CONTEXT_POOL 256


namespace niven
{
	NivenHost::NivenHost()
	{
		this->modules = Type<NivenModule>::CreateAll();
		this->router.Initialise(this->modules);

		//for (int i=0; i<INITIAL_CONTEXT_POOL; i++) this->contextPool.push_back(new Context(this));
	}


	/*Context *NivenHost::Claim(const char *url, const char *method, MHD_Connection *connection)
	{
		Context *result = nullptr;

		this->cs.lock();

			if (this->contextPool.size())
			{
				result = this->contextPool.back();
				this->contextPool.pop_back();
			}
			else result = new Context(this); //, {url, method, connection});

		this->cs.unlock();

		result->Set(url, method, connection);

		return result;
	}


	void NivenHost::Release(Context *context)
	{
		this->cs.lock();
		this->contextPool.push_back(context);
		this->cs.unlock();
	}*/


	bool NivenHost::Run(int port, int poolSize)
	{
		if (!this->daemon)
		{
			auto handler = [](void *cls, MHD_Connection *connection, const char *url, const char *method, const char *version, const char *upload_data, size_t *upload_data_size, void **ptr) {

				if (!*ptr)
				{
					*ptr = new Context(static_cast<Dependencies *>(cls), {url, method, connection});
					//*ptr = static_cast<NivenHost *>(cls)->Claim(url, method, connection);

					return MHD_YES;
				}

				Context *context = static_cast<Context *>(*ptr);

				if (*upload_data_size > 0)
				{
					context->request.body.append(upload_data, *upload_data_size);

					*upload_data_size = 0;

					return MHD_YES;
				}

				int result	= static_cast<NivenHost *>(cls)->router.Resolve(*context).Send(connection);
				*ptr		= nullptr;

				//static_cast<NivenHost *>(cls)->Release(context);
				delete context;
				return result;
			};

			this->daemon = MHD_start_daemon(
				MHD_USE_SELECT_INTERNALLY | MHD_USE_EPOLL_LINUX_ONLY,
				port, nullptr, nullptr, handler, this,
				MHD_OPTION_THREAD_POOL_SIZE, poolSize,
				MHD_OPTION_END
			);
		}

		return this->daemon;
	}


	void NivenHost::Stop()
	{
		if (this->daemon) MHD_stop_daemon(this->daemon);

		this->daemon = nullptr;

		//cout << this->contextPool.size() << endl;
	}
}
