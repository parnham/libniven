#include <niven/Niven.h>

using namespace niven;

class FilesModule : public Module
{
	public:

		FilesModule() : Module("/files")
		{
			// Return a file by name relative to the path of execution.
			// By returning a path as the response, niven will automatically
			// attempt to load the file in question.
			Get["/{name*}"] = [](auto &c) {
				return fs::path(c["name"]);
			};

			// Adding a standard "Content-Disposition" header allows us
			// to inform the browser to download the file.
			Get["download/{name*}"] = [](auto &c) {
				return Response(fs::path(c["name"])).WithHeader(
					"Content-Disposition", "attachment; filename=" + c["name"]
				);
			};

			// Be aware that niven does not provide security measures
			// for limiting access to files. That is the responsibility
			// of the application using this library.
		}
};

REGISTER_MODULE(FilesModule)
