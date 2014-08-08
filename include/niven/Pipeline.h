#pragma once

#include <niven/Context.h>
#include <niven/Response.h>


namespace niven
{
	typedef std::function<Response(Context&)> Action;
	typedef std::function<void(Context&, Response&)> AfterAction;


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


	class BeforePipeline : public Pipeline<Action>
	{
		public:

			Response Invoke(Context &context);
	};


	class AfterPipeline : public Pipeline<AfterAction>
	{
		public:

			void Invoke(Context &context, Response &response);
	};
}
