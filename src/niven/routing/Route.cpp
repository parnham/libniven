#include "niven/routing/Route.h"

using namespace std;


namespace niven
{

	// Route::Route(std::string module, std::string method, std::string path)
	Route::Route(string method, string path)
	{
		//this->module	= module;
		this->method	= method;
		this->path		= path;

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


	Route &Route::operator=(const Action action)
	{
		this->action = action;
		return *this;
	}
}
