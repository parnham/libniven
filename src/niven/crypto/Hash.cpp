#include "niven/crypto/Hash.h"

#include <gcrypt.h>
#include <iomanip>
#include <emergent/Logger.h>
#include <emergent/Uuid.hpp>
#include <entity/utilities.hpp>

using namespace emergent;
using namespace ent;
using namespace std;


namespace niven { namespace crypto
{
	string Hash::Salt()
	{
		vector<byte> result(8);

		auto a	= uuid().a;
		auto *p	= result.data() + 7;

		for (int i=0; i<8; i++, a = a >> 8) *p-- = a & 0xff;

		return base64::encode(result);
	}


	const vector<byte> Hash::AsBinary(const string &data, const string &salt, Algorithm algorithm)
	{
		int algo = GCRY_MD_SHA256;

		switch (algorithm)
		{
			case Algorithm::MD5: 		algo = GCRY_MD_MD5;			break;
			case Algorithm::SHA224:		algo = GCRY_MD_SHA224;		break;
			case Algorithm::SHA256:		algo = GCRY_MD_SHA256;		break;
			case Algorithm::SHA384:		algo = GCRY_MD_SHA384;		break;
			case Algorithm::SHA512:		algo = GCRY_MD_SHA512;		break;
			case Algorithm::Whirlpool:	algo = GCRY_MD_WHIRLPOOL;	break;
		}

		auto size		= gcry_md_get_algo_dlen(algo);
		auto combined	= base64::decode(salt);
		combined.insert(combined.end(), data.begin(), data.end());

		vector<byte> result(size);

		gcry_md_hash_buffer(algo, (void *)result.data(), combined.data(), combined.size());

		return result;
	}


	const string Hash::AsHex(const string &data, const string &salt, Algorithm algorithm)
	{
		stringstream result;
		result << hex << setfill('0');

		for (auto &c : AsBinary(data, salt, algorithm))
		{
			result << setw(2) << (int)c;
		}

		return result.str();
	}


	const string Hash::AsBase64(const string &data, const string &salt, Algorithm algorithm)
	{
		return base64::encode(AsBinary(data, salt, algorithm));
	}
}}
