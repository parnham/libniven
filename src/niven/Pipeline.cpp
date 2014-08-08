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
