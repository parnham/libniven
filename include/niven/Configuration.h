#pragma once

#include <vector>

#if __has_include(<filesystem>)
	#include <filesystem>
#elif __has_include(<experimental/filesystem>)
	#include <experimental/filesystem>
#endif


namespace niven
{
	#ifdef __cpp_lib_filesystem
		namespace fs = std::filesystem;
	#elif __cpp_lib_experimental_filesystem
		namespace fs = std::experimental::filesystem;
	#endif

	// Fluent configuration for NivenHost
	class Configuration
	{
		public:

			// Set the port for the host to listen on (default is 8090).
			Configuration &Listen(int port);

			// Set the size of the thread pool. Recommended setting is
			// to match the number of processor cores (default is 8).
			Configuration &WithThreads(int poolSize);

			// Enable SSL with the given key and certificate (useful for embedding).
			Configuration &EnableSSL(const std::string &key = "", const std::string &certificate = "");

			// Load the SSL key from file at the supplied path.
			Configuration &WithKey(const fs::path &path);

			// Load the SSL certificate from file at the supplied path.
			Configuration &WithCertificate(const fs::path &path);

		protected:

			int port	= 8090;
			int threads = 8;
			bool ssl	= false;
			std::string sslKey;
			std::string sslCertificate;
	};
}
