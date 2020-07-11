#pragma once

#include <cstdint>


namespace  {


using Int8    = std::int8_t;
using UInt8   = std::uint8_t;
using Int16   = std::int16_t;
using UInt16  = std::uint16_t;
using Int32   = std::int32_t;
using UInt32  = std::uint32_t;
using Int64   = std::int64_t;
using UInt64  = std::uint64_t;
using IntPtr  = std::intptr_t;
using UIntPtr = std::uintptr_t;


#if defined(_MSC_VER)
	#if defined(_WIN64)
		#define _PTR_IS_64_BIT 1
	#endif
	#define _HAVE_INT64 1
#elif defined(__GNUC__) || defined(__clang__)
	#if defined(_WIN64)
		#define _PTR_IS_64_BIT 1
	#else
		#if defined(__LP64__)
			#define _PTR_IS_64_BIT 1
			#define _LONG_IS_64_BIT 1
			#if _OS == _OS_LINUX || _OS == _OS_ANDROID
				#define _INT64_IS_LONG 1
			#endif
		#endif
	#endif
	#define _HAVE_INT64 1
#elif defined(__SUNPRO_CC)
	#if defined(__sparcv9)
		#define _PTR_IS_64_BIT 1
		#define _LONG_IS_64_BIT 1
	#endif
	#define _HAVE_INT64 1
#elif defined(__IBMCPP__)
	#if defined(__64BIT__)
		#define _PTR_IS_64_BIT 1
		#define _LONG_IS_64_BIT 1
	#endif
	#define _HAVE_INT64 1
#elif defined(_DIAB_TOOL)
	#define _HAVE_INT64 1
#endif


} 
