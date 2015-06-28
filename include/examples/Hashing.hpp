#include <niven/Niven.h>
#include <niven/crypto/Hash.h>

using namespace niven;
using namespace niven::crypto;


class HashingModule : public Module
{
	public:

		HashingModule() : Module("/hashing")
		{
			// Return a base64 encoded random salt
			Get["/salt"] = [](auto) { return Hash::Salt(); };

			// Hash the supplied value using a random salt and returnas base64.
			Get["/{value}"] = [](auto &c) {
				return Hash::AsBase64(c["value"], Hash::Salt(), Algorithm::SHA256);
			};
		}
};

REGISTER_MODULE(HashingModule)
