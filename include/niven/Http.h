#pragma once
// #include <array>
#include <emergent/String.hpp>


namespace niven
{
	// Lookup for HTTP response codes.
	enum class Http
	{
		None							= 0,

		Continue						= 100,
		SwitchingProtocols				= 101,
		Processing						= 102,

		OK								= 200,
		Created							= 201,
		Accepted						= 202,
		NonAuthoritativeInformation		= 203,
		NoContent						= 204,
		PartialContent					= 205,
		MultiStatus						= 207,

		MultipleChoices					= 300,
		MovedPermanently				= 301,
		Found							= 302,
		SeeOther						= 303,
		NotModified						= 304,
		UseProxy						= 305,
		SwitchProxy						= 306,
		TemporaryRedirect				= 307,

		BadRequest						= 400,
		Unauthorized					= 401,
		PaymentRequired					= 402,
		Forbidden						= 403,
		NotFound						= 404,
		MethodNotAllow					= 405,
		MethodNotAcceptable				= 406,
		ProxyAuthenticationRequired		= 407,
		RequestTimeout					= 408,
		Conflict						= 409,
		Gone							= 410,
		LengthRequired					= 411,
		PreconditionFailed				= 412,
		RequestEntityTooLarge			= 413,
		RequestUriTooLong				= 414,
		UnsupportedMediaType			= 415,
		RequestedRangeNotSatisfiable	= 416,
		ExpectationFailed				= 417,
		UnprocessableEntity				= 422,
		Locked							= 423,
		FailedDependency				= 424,
		UnorderedCollection				= 425,
		UpgradeRequired					= 426,
		NoResponse						= 444,
		RetryWith						= 449,
		UnavailableForLegalReasons		= 451,

		InternalServerError				= 500,
		NotImplemented					= 501,
		BadGateway						= 502,
		ServiceUnavailable				= 503,
		GatewayTimeout					= 504,
		HttpVersionNotSupported			= 505,
		VariantAlsoNegotiates			= 506,
		InsufficientStorage				= 507,
		BandwidthLimitExceeded			= 509,
		NotExtended						= 510
	};


	struct HttpTime
	{
		static constexpr const char *DAYS[]		= { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
		static constexpr const char *MONTHS[]	= { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

		// Convert the timestamp to the standard HTTP time format.
		static std::string Format(const time_t time) //, const bool cookie = false)
		{
			tm t;
			gmtime_r(&time, &t);

				// Convert the expiry timestamp to the standard HTTP header format.
			return emergent::String::format(
				// cookie ? "%s, %02d-%s-%04d %02d:%02d:%02d GMT" : "%s, %02d %s %04d %02d:%02d:%02d GMT",
				"%s, %02d %s %04d %02d:%02d:%02d GMT",
				DAYS[t.tm_wday], t.tm_mday, MONTHS[t.tm_mon], 1900+t.tm_year,
				t.tm_hour, t.tm_min, t.tm_sec
			);
		}
	};
}


