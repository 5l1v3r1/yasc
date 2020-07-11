#pragma once


#if defined(_WIN32)
	#if !defined(WIN32_LEAN_AND_MEAN) && !defined(_BLOATED_WIN32)
		#define WIN32_LEAN_AND_MEAN
	#endif
#endif


#if !defined(_NO_WINDOWS_H)
    #include <windows.h>
    #ifdef __MINGW32__
        #include <Winsock2.h>
        #include <Iphlpapi.h>
        #include <ws2tcpip.h>
    #endif 
#endif
