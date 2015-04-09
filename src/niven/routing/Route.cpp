#include "niven/routing/Route.h"

using namespace std;


namespace niven
{
	Route::Route(Module *parent, string method, string path)
	{
		this->method	= method;
		this->path		= path;
		this->parent	= parent;

		string segment;
		int depth = 0;	// Parenthesis depth

		// Extract route segments
		for (auto &c : path)
		{
			if (c == '(')			depth++;
			if (c == ')')			depth--;
			if (c != '/' || depth)	segment += c;

			if (c == '/' && segment.size() && !depth)
			{
				this->segments.push_back(segment);
				segment.clear();
			}
		}

		if (segment.size()) this->segments.push_back(segment);
	}


	Route &Route::operator=(const Action &action)
	{
		this->action = action;
		return *this;
	}
}
