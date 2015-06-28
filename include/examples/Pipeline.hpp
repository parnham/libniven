#include <niven/Niven.h>

using namespace niven;


class PipelineModule : public Module
{
	public:

		PipelineModule() : Module("/pipeline")
		{
			// Add an action to be invoked before the route. This affects all routes in this module.
			// Returning "nullptr" will allow the pipeline to continue as normal, returning a response
			// will short-circuit anything else in the Before pipeline along with the route action
			// itself. Even in this case the After pipeline will be invoked as normal.
			// The request object can be modified here, however since the route has already been
			// parsed modifying the URL will have no effect on which route action is invoked.
			Before += [](auto &c) {
				c.request.body = "Before";
				return nullptr;
			};

			// Add an action to be invoked after the route. An action in this pipeline is able to
			// modify the response before it is sent to the client.
			After += [](auto &c, auto &r) {
				r.data += "After";

			};

			Get["/"] = [](auto &c) {
				return c.request.body + " - During - ";
			};



			// There are also BeforeRequest and AfterRequest pipelines belonging to the NivenHost.
			// The BeforeRequest pipeline is able to modify the request before routing and can therefore
			// modify the route. For example,
			//
			// 		host.BeforeRequest += [](auto &c) {
			//			c.request.url = "/login";
			//			return nullptr;
			//		};
			//
			// As with the module specific pipelines, BeforeRequest can short-circuit the rest of the
			// pipelines and route by returning a response. In this case even the AfterRequest is bypassed.
			// Actions in the AfterRequest pipeline are able to modify the response.
			//
			// 		host.AfterRequest += [](auto &c, auto &r) {
			//			r.status = Http::Unauthorized;
			//		};
		}
};

REGISTER_MODULE(PipelineModule)
