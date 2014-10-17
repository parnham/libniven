#include "niven/Niven.h"
#include <signal.h>
#include <bits/signum.h>
#include <iostream>
#include <mutex>
#include <condition_variable>

#include <niven/crypto/Hash.h>

using namespace std;
using namespace emg;
using namespace ent;
using namespace niven;
using namespace niven::crypto;


#include <emergent/image/Image.hpp>

class ImageResource
{
	public:

		void Set(Image<byte> &image)
		{
			lock_guard<mutex> lock(this->cs);

			this->image = image;
		}


		Buffer<byte> Get()
		{
			lock_guard<mutex> lock(this->cs);

			if (this->image.Save(this->buffer, 2))
			{
				return this->buffer;
			}

			return {};
		}

	private:

		mutex cs;
		Image<byte> image;
		Buffer<byte> buffer;

};

class IResource
{
	public:
		virtual string Get() = 0;
};


class ConcreteResource : public IResource
{
	public:
		virtual string Get() { return "ConcreteResource"; }
};

class AnotherResource  : public IResource
{
	public:

		int number;

		AnotherResource(int needsNumber) : number(needsNumber) {}
		virtual string Get() { return "AnotherResource with number " + to_string(this->number); }
};


#include <entity/entity.hpp>
#include <entity/tree.hpp>

struct Test : entity
{
	string name = "Test";
	vector<int> list = { 1, 2, 3 };

	mapping describe()
	{
		return { eref(name), eref(list) };
	}
};


class SampleModule : public NivenModule
{
	public:

		SampleModule() : NivenModule("/sample")
		{
			//Before	+= [](Context &c) { cout << "Before module" << endl; return nullptr; };
			//After	+= [](Context &c, Response &r) { cout << "After module" << endl; };

			Get["/"]			= [](Context &c) { return "The moon's a balloon"; };
			Get["/test"]		= [](Context &c) { return Test(); };
			Get["/tree"]		= [](Context &c) { return tree { { "name", "Test" }, { "list", vector<tree> { 1, 2, 3 }}}; };	// Equivalent of anonymous object in C#
			Get["/resource"]	= [](Context &c) { return c.Resolve<IResource>()->Get(); };
			Get["/ok"]			= [](Context &c) { return Http::OK; };
			Get["/custom"]		= [](Context &c) { return Response().WithHeader("something", "else"); };
			Get["/test/{id}"]	= [](Context &c) { return "ID=" + c["id"]; };
			Get["/broken"]		= nullptr;

			Get["/cookie"]		= [](Context &c) { return Response().WithCookie({"test", "value", DateTime::Now().AddSeconds(30)}); };

			Get["/test/{id}/{name}"]	= [](Context &c) { return "ID=" + c["id"] + "  Name=" + c["name"]; };
			Get["/test/{id}/literal"]	= [](Context &c) { return "ID=" + c["id"]; };
			Get["/file/{name*}"]		= [](Context &c) { return Path("/home/dan/temp") / c["name"]; };
			Get["/file/{name*}/bar"]	= [](Context &c) { return c["name"]; };

			Get["/salt"]				= [](Context &c) { return Hash::Salt(); };
			Get["/hash/{value}"]		= [](Context &c) { return Hash::AsBase64(c["value"], Hash::Salt(), Algorithm::SHA256);  };

			Post["/bind"] = [](Context &c) { return "The name is: " + c.bind<Test>().name; };
		}
};

REGISTER_MODULE(SampleModule)


bool run = true;
condition_variable condition;

int main(int argc, char **argv)
{

	/*{
		auto start 	= steady_clock::now();

		for (int i=0; i<1000000; i++) uuid().to_string();

		long duration = duration_cast<milliseconds>(steady_clock::now() - start).count();
		cout << "Time taken was " << duration << "ms" << endl;
	}*/
	/*{
		auto start = steady_clock::now();
		uuid u;
		string data = encode64(u.to_binary());

		for (int i=0; i<1000000; i++) decode64(data); //encode64(u.to_binary());

		long duration = duration_cast<milliseconds>(steady_clock::now() - start).count();
		cout << "Time taken was " << duration << "ms" << endl;
	}*/

	//cout << uuid().to_string() << endl;
	//cout << encode64(uuid().to_binary()) << endl;
	//cout << uuid64() << endl;

	//return 0;

	/*auto resource = shared_ptr<IResource>(new ConcreteResource());

	cout << "Singleton" << endl;
	cout << "  Count = " << resource.use_count() << endl;
	{
		Dependencies dep;
		dep.Register<IResource>(resource);

		cout << "  Count = " << resource.use_count() << endl;

		{
			vector<shared_ptr<IResource>> resources;

			for (int i=0; i<10; i++)
			{
				resources.push_back(dep.Resolve<IResource>());

				cout << "  " << (long)resources[i].get() << endl;
				cout << "  Count = " << resource.use_count() << endl;
			}
		}

		cout << "  Count = " << resource.use_count() << endl;
	}
	cout << "  Count = " << resource.use_count() << endl;

	cout << endl << "Multiple" << endl;
	{
		Dependencies dep;

		dep.Register<IResource, ConcreteResource>();

		vector<shared_ptr<IResource>> resources;

		for (int i=0; i<10; i++)
		{
			resources.push_back(move(dep.Resolve<IResource>()));

			cout << "  " << (long)resources[i].get() << endl;
			cout << "  Count = " << resources[i].use_count() << endl;

			//cout << ((AnotherResource *)resources[i].get())->number << endl;
		}
	}

	return 0;
	//*/

	//cout << Cookie("key", "test", DateTime::Now().AddHours(1)).Build() << endl;
	//cout << DateTime::Today().FormatISO() << endl;

	//return 0;

	auto KEY = R"ssl(
		-----BEGIN RSA PRIVATE KEY-----
		MIICXAIBAAKBgQDfTaRcdcfQfgzC8pQS1X/i8yxij30selV3iqJShPKPjLBlIXZ3
		PtI6u7goKWQZciynr8i2iA3qlRCpxY7g4Vvp/1oXY2J/anjS1VgSVK6gUAJ7DAFR
		xU5YS9wq23tojA+UQgPmtuEwtXz5yr82XDKswIeAmFoYpxI/HASAqEmQQQIDAQAB
		AoGATK5T17BWamEurq8wa/tGzD9nKX1OpbxopwFDVoTy6pqMZF4GWVKITMTzq07z
		Yc2USg0SJqQ36ys/Kp19v/mDXwYMom6yhF+hM5fMnxy7EP42gfwh/mdLnkCZCufq
		IqoexjVK1VewLlpM4067UmPW+d+3q6GEKHrsEArh5wdq9HUCQQD4lzO1Od6co2r7
		1N3fx2HN5BgJQ7GLRTlXTu+i8FYxXjKFztpeDE8vjnsjltnYKJqvdmGArxsnDhWg
		VbOYBUF7AkEA5fV+DNAPRHWlclYo8N5luhobf6QmjcHZR4fhFXkO3ttgHoASeOpJ
		6Ip6BCSm1BUy5i0OG2mscGokPWGWOy+ScwJAMdSVT+GOxxVW764m36Gm8E+lpc3E
		/CbfLKotDRtiG7lHJV1mlm2fPegwtNybRGI3R67SMqpnJfgDy8UCIBoFmwJBAJQM
		bh+vn/SyTVcEM42HkTS0dDnEI/Il+E4+I+JgMvu1fveD+uCP7g9hf9bFBgcKst9P
		6IIrkZI6M6S0/DSUlxMCQFq1jfyARuSzLVPdydJzehbSETPXxn+69PlOqQvwt3rK
		hIF4O0PHZTwhVCyCngzpfj9CdbQ1+ctUfDM08Q8NHE8=
		-----END RSA PRIVATE KEY-----
	)ssl";

	auto CERTIFICATE = R"ssl(
		-----BEGIN CERTIFICATE-----
		MIIC6jCCAlOgAwIBAgIJANTs6h3imif9MA0GCSqGSIb3DQEBCwUAMIGNMQswCQYD
		VQQGEwJHQjEQMA4GA1UECAwHTm9yZm9sazEQMA4GA1UEBwwHTm9yd2ljaDEcMBoG
		A1UECgwTRW1lcmdlbnQgRGVzaWduIEx0ZDESMBAGA1UEAwwJbG9jYWxob3N0MSgw
		JgYJKoZIhvcNAQkBFhlkYW5AZW1lcmdlbnQtZGVzaWduLmNvLnVrMB4XDTE0MDkx
		NjIxMzExNVoXDTE1MDYwODIxMzExNVowgY0xCzAJBgNVBAYTAkdCMRAwDgYDVQQI
		DAdOb3Jmb2xrMRAwDgYDVQQHDAdOb3J3aWNoMRwwGgYDVQQKDBNFbWVyZ2VudCBE
		ZXNpZ24gTHRkMRIwEAYDVQQDDAlsb2NhbGhvc3QxKDAmBgkqhkiG9w0BCQEWGWRh
		bkBlbWVyZ2VudC1kZXNpZ24uY28udWswgZ8wDQYJKoZIhvcNAQEBBQADgY0AMIGJ
		AoGBAN9NpFx1x9B+DMLylBLVf+LzLGKPfSx6VXeKolKE8o+MsGUhdnc+0jq7uCgp
		ZBlyLKevyLaIDeqVEKnFjuDhW+n/WhdjYn9qeNLVWBJUrqBQAnsMAVHFTlhL3Crb
		e2iMD5RCA+a24TC1fPnKvzZcMqzAh4CYWhinEj8cBICoSZBBAgMBAAGjUDBOMB0G
		A1UdDgQWBBQa2kWJBE4LhDfqflJVVI3a0UODQjAfBgNVHSMEGDAWgBQa2kWJBE4L
		hDfqflJVVI3a0UODQjAMBgNVHRMEBTADAQH/MA0GCSqGSIb3DQEBCwUAA4GBACtn
		goHh5t09JzK4Yl3jL4PpxcnbKoFZU7dikLcUIm+aCGHnjIh7LLDdyG2K6faUXNSG
		D8sULRXg8TQMrqw1LaZucHKX8mvmP6KRiC5u4rqiCA1YtOBrkbBiR8N4cWNDNPiZ
		aKtM9aTy9iCFMPxtH3NqL9bleR0ikPLKe9sEP7Rx
		-----END CERTIFICATE-----
	)ssl";


	logger::instance().add(new sink::console());
	logger::instance().set_verbosity("info");
	signal(SIGINT,	[](int) { run = false; condition.notify_one(); });
	signal(SIGQUIT,	[](int) { run = false; condition.notify_one(); });

	mutex m;
	NivenHost host;

	host.Listen(8090).EnableSSL(KEY, CERTIFICATE);
	// 	.EnableSSL().WithKey("server.key").WithCertificate("server.pem");

	host.Register<IResource, ConcreteResource>();
	//host.Register<IResource, AnotherResource>(42);

	//host.BeforeRequest	+= [](Context &c) { cout << "Before routing" << endl; return nullptr; };
	//host.AfterRequest	+= [](Context &c, Response &r) { cout << "The end" << endl; };

	if (host.Run())
	{
		cout << "Listening on port 8090" << endl;

		unique_lock<mutex> lock(m);
		while (run) condition.wait(lock);

		host.Stop();
	}
	else cout << "Failed to initialise host" << endl;

	return 0;
}
