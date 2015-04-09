#include "niven/Host.h"

using namespace std;
using namespace emg;

#ifdef NIVEN_USE_H2O
	#include <h2o/http1.h>
	//#include <h2o/http2.h>
	#include <netinet/in.h>
#endif


// In header
	// #include <uv.h>
	// #include <h2o.h>
	// #include <thread>
// With members
	// h2o_context_t context;
	// h2o_globalconf_t config;
	// uv_tcp_t listener;
	// std::thread _thread;


namespace niven
{

#ifdef NIVEN_USE_H2O
	bool NivenHost::Run()
	{
		h2o_config_init(&this->config);

		auto host 		= h2o_config_register_host(&this->config, "default");
		auto path 		= h2o_config_register_path(host, "/");
		auto handler	= h2o_create_handler(path, sizeof(h2o_handler_t));
		handler->on_req	= [](auto *self, auto *request)->int {

			//cout << "Request received: "<< string(request->method.base, request->method.len) << " to " << string(request->path_normalized.base, request->path_normalized.len) << endl;

			static h2o_generator_t generator = { nullptr, nullptr };
			request->res.status = 200;
			request->res.reason = "OK";
			h2o_add_header(&request->pool, &request->res.headers, H2O_TOKEN_CONTENT_TYPE, H2O_STRLIT("text/plain; charset=utf-8"));
			h2o_start_response(request, &generator);
			h2o_send(request, &request->entity, 1, 1);

			return 0;
		};


		auto on_accept = [](uv_stream_t *listener, int status) {
			if (status != 0) return;

			uv_tcp_t *connection = (uv_tcp_t *)h2o_mem_alloc(sizeof(uv_tcp_t));
			uv_tcp_init(listener->loop, connection);

			if (uv_accept(listener, (uv_stream_t *)connection) != 0)
			{
				uv_close((uv_handle_t *)connection, (uv_close_cb)free);
				return;
			}

			auto socket		= h2o_uv_socket_create((uv_stream_t *)connection, nullptr, 0, (uv_close_cb)free);
			auto context	= (h2o_context_t *)listener->data;

			// if (ssl)
			// h2o_accept_ssl(&ctx, ctx.globalconf->hosts, sock, ssl_ctx);
			// else
			h2o_http1_accept(context, context->globalconf->hosts, socket);

		};


		this->_thread = thread([&]{

			uv_loop_t loop;
			uv_loop_init(&loop);
			h2o_context_init(&this->context, &loop, &this->config);

			int r;
			sockaddr_in address;

			uv_tcp_init(&loop, &this->listener);
			uv_ip4_addr("127.0.0.1", this->port, &address);

			this->listener.data = &this->context;

			r = uv_tcp_bind(&this->listener, (sockaddr *)&address, 0);
			if (r != 0)
			{
				FLOG(error, "Binding issue: %s", uv_strerror(r));
				uv_close((uv_handle_t *)&this->listener, nullptr);
				return;
			}

			r = uv_listen((uv_stream_t *)&this->listener, 128, on_accept);

			if (r != 0)
			{
				FLOG(error, "Listen issue: %s", uv_strerror(r));
				uv_close((uv_handle_t *)&this->listener, nullptr);
				return;
			}

			uv_run(this->context.loop, UV_RUN_DEFAULT);

			uv_loop_close(&loop);
		});

		return true;
	}



	void NivenHost::Stop()
	{
		cout << "attempting close" << endl;
		uv_close((uv_handle_t *)&this->listener, nullptr);

		this->_thread.join();

		cout << "closing" << endl;
		// uv_stop(this->context.loop);
		//uv_loop_close(this->context.loop);
		//this->_thread.join();
	}
#endif

}
