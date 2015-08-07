#pragma once

#include <vector>
#include <emergent/Path.hpp>


namespace niven
{
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
			Configuration &WithKey(const emergent::Path path);

			// Load the SSL certificate from file at the supplied path.
			Configuration &WithCertificate(const emergent::Path path);

		protected:

			int port	= 8090;
			int threads = 8;
			bool ssl	= false;
			std::string sslKey;
			std::string sslCertificate;
	};
}
