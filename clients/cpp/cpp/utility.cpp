#include "utility.h"

#ifdef _DEBUG
#include <iostream>
#include <fstream>
#include <sstream>
#include <functional>
#include <cstdarg>
#endif

#if (defined(WIN32) || defined(_WIN64)) && defined(_DEBUG)
  #if !defined(WIN32_LEAN_AND_MEAN) && !defined(_BLOATED_WIN32)
	#define WIN32_LEAN_AND_MEAN
  #endif

  #include <windows.h>
  #include <debugapi.h>
#endif


void Log(const char* format, ...) {
#ifdef _DEBUG
	char linebuf[1024];
	va_list v;
	va_start(v, format);
	vsnprintf(linebuf, 1024, format, v);
	va_end(v);
#ifdef WIN32
	
	OutputDebugStringA(linebuf);
#endif 
#endif 
}
