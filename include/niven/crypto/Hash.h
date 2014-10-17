#pragma once

#include <emergent/Emergent.h>


namespace niven { namespace crypto
{
	enum class Algorithm
	{
		MD5,
		SHA224,
		SHA256,
		SHA384,
		SHA512,
		Whirlpool
	};


	class Hash
	{
		public:
			static std::string Salt();
			static std::vector<byte> AsBinary(const std::string &data, const std::string &salt, Algorithm algorithm);
			static std::string AsHex(const std::string &data, const std::string &salt, Algorithm algorithm);
			static std::string AsBase64(const std::string &data, const std::string &salt, Algorithm algorithm);
	};

}}
