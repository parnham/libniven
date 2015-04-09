#include "niven/Pipeline.h"


namespace niven
{
	Response BeforePipeline::Invoke(Context &context)
	{
		for (auto &action : this->actions)
		{
			try
			{
				if (action)
				{
					auto response = action(context);

					if (response.status != Http::None)
					{
						// If any actions in this pipeline return a response status
						// other than None then drop out immediately. This response
						// will then be sent to the client and the route specific
						// action will be bypassed along with any additional pipelines.
						return response;
					}
				}
			}
			catch (const std::exception &e)
			{
				return { e.what(), Http::InternalServerError };
			}
		}

		return nullptr;
	}


	void AfterPipeline::Invoke(Context &context, Response &response)
	{
		for (auto &action : this->actions)
		{
			try
			{
				if (action) action(context, response);
			}
			catch (const std::exception &e)
			{
				response = { e.what(), Http::InternalServerError };

				return;
			}
		}
	}
}
