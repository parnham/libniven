#pragma once

#include <vector>
#include <emergent/Path.h>


namespace niven
{
	class Configuration
	{
		public:

			Configuration &Listen(int port);
			Configuration &WithThreads(int poolSize);
			Configuration &EnableSSL(const std::string &key = "", const std::string &certificate = "");
			Configuration &WithKey(const emergent::Path key);
			Configuration &WithCertificate(const emergent::Path certificate);

		protected:

			int port	= 8090;
			int threads = 8;
			bool ssl	= false;
			std::string sslKey;
			std::string sslCertificate;
	};
}
