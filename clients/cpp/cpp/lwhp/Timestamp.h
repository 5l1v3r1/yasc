#pragma once

#include <ctime>

namespace libwinhttp {

	class Timestamp {

	public:
		using TimeVal = Int64;
		
		

		using UtcTimeVal = Int64;
		
		

		using TimeDiff = Int64;
		

		static const TimeVal TIMEVAL_MIN; 
		static const TimeVal TIMEVAL_MAX; 

		Timestamp();
		

		Timestamp(TimeVal tv);
		
		

		Timestamp(const Timestamp& other);
		

		~Timestamp();
		

		void update();

#if defined(_WIN32)
		static Timestamp fromFileTimeNP(UInt32 fileTimeLow, UInt32 fileTimeHigh);
		void toFileTimeNP(UInt32& fileTimeLow, UInt32& fileTimeHigh) const;
#endif
	private:
		TimeVal _ts;
	};
}
