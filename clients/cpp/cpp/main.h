#pragma once

 

#include "utility.h"
#include "lwhp/UnWindows.h"

#ifdef _DLL
HANDLE  hThread;
DWORD   dwThreadId;
#endif

HANDLE ghExitProcess; 

BOOL lookup_done = FALSE;
BOOL funcs_found = FALSE;

