#include "niven/Configuration.h"
#include <emergent/Emergent.hpp>
#include <emergent/logger/Logger.hpp>


using namespace std;
using namespace emergent;


namespace niven
{

	Configuration &Configuration::Listen(int port)
	{
		this->port = port;
		return *this;
	}


	Configuration &Configuration::WithThreads(int poolSize)
	{
		this->threads = poolSize;
		return *this;
	}


	Configuration &Configuration::EnableSSL(const std::string &key, const std::string &certificate)
	{
		this->ssl 				= true;
		this->sslKey			= key;
		this->sslCertificate	= certificate;
		return *this;
	}


	Configuration &Configuration::WithKey(const fs::path &key)
	{
		if (fs::exists(key))
		{
			this->sslKey = String::load(key);
		}
		else Log::Error("Path to key file does not exist: %s", key);

		return *this;
	}


	Configuration &Configuration::WithCertificate(const fs::path &certificate)
	{
		if (fs::exists(certificate))
		{
			this->sslCertificate = String::load(certificate);
		}
		else Log::Error("Path to certificate file does not exist: %s", certificate);

		return *this;
	}
}
