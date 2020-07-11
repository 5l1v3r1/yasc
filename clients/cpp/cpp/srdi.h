#pragma once

#include "lwhp/UnWindows.h"

#define DEREF_64( name )*(DWORD64 *)(name)
#define DEREF_32( name )*(DWORD *)(name)
#define DEREF_16( name )*(WORD *)(name)
#define DEREF_8( name )*(BYTE *)(name)

typedef UINT_PTR(WINAPI* RDI)();
typedef void(WINAPI* Function)();
typedef BOOL(__cdecl* EXPORTEDFUNCTION)(LPVOID, DWORD);

FARPROC GetProcAddressR(UINT_PTR uiLibraryAddress, LPCSTR lpProcName);

UINT_PTR srdi(LPSTR finalShellcode);