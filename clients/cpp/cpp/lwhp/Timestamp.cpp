
#include "libwinhttp.h"
#ifdef _OS_FAMILY_WINDOWS
#include "UnWindows.h"
#endif 
#include "Timestamp.h"
#undef min
#undef max
#include <limits>


namespace libwinhttp {

	const Timestamp::TimeVal Timestamp::TIMEVAL_MIN = std::numeric_limits<Timestamp::TimeVal>::min();
	const Timestamp::TimeVal Timestamp::TIMEVAL_MAX = std::numeric_limits<Timestamp::TimeVal>::max();

	Timestamp::Timestamp()
	{
		update();
	}


	Timestamp::Timestamp(TimeVal tv)
	{
		_ts = tv;
	}


	Timestamp::Timestamp(const Timestamp& other)
	{
		_ts = other._ts;
	}


	Timestamp::~Timestamp()
	{
	}

	void Timestamp::update()
	{
#if defined(_OS_FAMILY_WINDOWS)

		FILETIME ft;
#if defined(_WIN32_WCE) && defined(_WINCE_TIMESTAMP_HACK)
		GetSystemTimeAsFileTimeWithMillisecondResolution(&ft);
#else
		GetSystemTimeAsFileTime(&ft);
#endif

		ULARGE_INTEGER epoch; 
		epoch.LowPart = 0xD53E8000;
		epoch.HighPart = 0x019DB1DE;

		ULARGE_INTEGER ts;
		ts.LowPart = ft.dwLowDateTime;
		ts.HighPart = ft.dwHighDateTime;
		ts.QuadPart -= epoch.QuadPart;
		_ts = ts.QuadPart / 10;

#elif defined(_HAVE_CLOCK_GETTIME)

		struct timespec ts;
		if (clock_gettime(CLOCK_REALTIME, &ts))
			throw SystemException("cannot get time of day");
		_ts = TimeVal(ts.tv_sec) * resolution() + ts.tv_nsec / 1000;

#else

		struct timeval tv;
		if (gettimeofday(&tv, NULL))
			throw SystemException("cannot get time of day");
		_ts = TimeVal(tv.tv_sec) * resolution() + tv.tv_usec;

#endif
	}

#if defined(_WIN32)
	Timestamp Timestamp::fromFileTimeNP(UInt32 fileTimeLow, UInt32 fileTimeHigh)
	{
		ULARGE_INTEGER epoch; 
		epoch.LowPart = 0xD53E8000;
		epoch.HighPart = 0x019DB1DE;

		ULARGE_INTEGER ts;
		ts.LowPart = fileTimeLow;
		ts.HighPart = fileTimeHigh;
		ts.QuadPart -= epoch.QuadPart;

		return Timestamp(ts.QuadPart / 10);
	}


	void Timestamp::toFileTimeNP(UInt32& fileTimeLow, UInt32& fileTimeHigh) const
	{
		ULARGE_INTEGER epoch; 
		epoch.LowPart = 0xD53E8000;
		epoch.HighPart = 0x019DB1DE;

		ULARGE_INTEGER ts;
		ts.QuadPart = _ts * 10;
		ts.QuadPart += epoch.QuadPart;
		fileTimeLow = ts.LowPart;
		fileTimeHigh = ts.HighPart;
	}


#endif

}