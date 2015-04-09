#pragma once

#include <emergent/Emergent.h>


namespace niven { namespace crypto
{
	// Available hashing algorithms.
	enum class Algorithm
	{
		MD5,
		SHA224,
		SHA256,
		SHA384,
		SHA512,
		Whirlpool
	};


	// Useful hashing functions.
	class Hash
	{
		public:

			// Generate a random salt for use when hashing.
			static std::string Salt();

			// Hash the supplied data and return as a binary blob.
			static const std::vector<byte> AsBinary(const std::string &data, const std::string &salt, Algorithm algorithm);

			// Hash the supplied data and return as a hex string.
			static const std::string AsHex(const std::string &data, const std::string &salt, Algorithm algorithm);

			// Hash the supplied data and return the binary encoded as base64.
			static const std::string AsBase64(const std::string &data, const std::string &salt, Algorithm algorithm);
	};

}}
