#pragma once

#include <niven/Context.h>
#include <niven/Response.h>


namespace niven
{
	// Action to be performed, processes the request and returns a response.
	typedef std::function<Response(Context&)> Action;

	// Action that can modify a response after a request has been processed.
	typedef std::function<void(Context&, Response&)> AfterAction;


	// Stores a list of actions to be invoked.
	template <class T> class Pipeline
	{
		public:

			Pipeline &operator +=(const T action)
			{
				this->actions.push_back(action);
				return *this;
			}

		protected:

			std::vector<T> actions;
	};


	// Pipeline for actions that can be called before the specific route
	// action is invoked.
	class BeforePipeline : public Pipeline<Action>
	{
		public:

			Response Invoke(Context &context);
	};


	// Pipeline for actions that can be called after the specific route
	// action is invoked.
	class AfterPipeline : public Pipeline<AfterAction>
	{
		public:

			void Invoke(Context &context, Response &response);
	};
}
