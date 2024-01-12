#include "niven/Niven.h"

#include <mutex>
#include <signal.h>
#include <iostream>
// #include <bits/signum.h>
#include <condition_variable>

// #include "examples/Simple.hpp"


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


bool run = true;
std::condition_variable condition;

int main(int argc, char **argv)
{
	// Enable logging to stdout
	emg::Log::Initialise({ std::make_unique<emg::logger::Console>() });
	emg::Log::Verbosity("info");

	// Catch the interrupt/quit signals and handle appropriately
	signal(SIGINT,	[](int) { run = false; condition.notify_one(); });
	signal(SIGQUIT,	[](int) { run = false; condition.notify_one(); });


	niven::NivenHost host;

	// Listen on port 8090 and enable SSL using an embedded key and certificate.
	host.Listen(8090).EnableSSL(KEY, CERTIFICATE);

	// Alternatively the key and certificate could be loaded from file as follows:
	// host.Listen(8090).EnableSSL().WithKey("server.key").WithCertificate("server.pem");


	// Start the host running. This function will return true if successfully
	// started and since the host has its own threading we will use a condition
	// variable to wait for an interrupt or quit signal from the user.
	if (host.Run())
	{
		std::cout << "Listening on port 8090\n";

		std::mutex m;
		std::unique_lock lock(m);

		while (run)
		{
			condition.wait(lock);
		}

		host.Stop();
	}
	else
	{
		std::cout << "Failed to initialise host\n";
	}

	return 0;
}
