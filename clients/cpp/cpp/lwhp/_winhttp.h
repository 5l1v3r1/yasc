#pragma once

#include "UnWindows.h"
#include <winhttp.h>
#include <iphlpapi.h>


extern "C" {

	typedef BOOL(WINAPI* PWinHttpCloseHandle) (HINTERNET);
	extern PWinHttpCloseHandle pWinHttpCloseHandle;

	typedef BOOL(WINAPI* PWinHttpCrackUrl) (LPCWSTR, DWORD, DWORD, LPURL_COMPONENTS);
	extern PWinHttpCrackUrl pWinHttpCrackUrl;
	typedef HINTERNET(WINAPI* PWinHttpConnect) (HINTERNET, LPCWSTR, INTERNET_PORT, DWORD);
	extern PWinHttpConnect pWinHttpConnect;
	typedef BOOL(WINAPI* PWinHttpCreateUrl) (LPURL_COMPONENTS, DWORD, LPWSTR, LPDWORD);
	extern PWinHttpCreateUrl pWinHttpCreateUrl;
	typedef HINTERNET(WINAPI* PWinHttpOpen) (LPCWSTR, DWORD, LPCWSTR, LPCWSTR, DWORD);
	extern PWinHttpOpen pWinHttpOpen;
	typedef HINTERNET(WINAPI* PWinHttpOpenRequest) (HINTERNET, LPCWSTR, LPCWSTR, LPCWSTR, LPCWSTR, LPCWSTR*, DWORD);
	extern PWinHttpOpenRequest pWinHttpOpenRequest;
	typedef BOOL(WINAPI* PWinHttpQueryDataAvailable) (HINTERNET, LPDWORD);
	extern PWinHttpQueryDataAvailable pWinHttpQueryDataAvailable;
	typedef BOOL(WINAPI* PWinHttpQueryHeaders) (HINTERNET, DWORD, LPCWSTR, LPVOID, LPDWORD, LPDWORD);
	extern PWinHttpQueryHeaders pWinHttpQueryHeaders;
	typedef BOOL(WINAPI* PWinHttpReadData) (HINTERNET, LPVOID, DWORD, LPDWORD);
	extern PWinHttpReadData pWinHttpReadData;
	typedef BOOL(WINAPI* PWinHttpReceiveResponse) (HINTERNET, LPVOID);
	extern PWinHttpReceiveResponse pWinHttpReceiveResponse;
	typedef BOOL(WINAPI* PWinHttpSendRequest) (HINTERNET, LPCWSTR, DWORD, LPVOID, DWORD, DWORD, DWORD_PTR);
	extern PWinHttpSendRequest pWinHttpSendRequest;
	typedef BOOL(WINAPI* PWinHttpWriteData) (HINTERNET, LPCVOID, DWORD, LPDWORD);
	extern PWinHttpWriteData pWinHttpWriteData;

	typedef BOOL(WINAPI* PWinHttpSetTimeouts) (HINTERNET, int, int, int, int);
	extern PWinHttpSetTimeouts pWinHttpSetTimeouts;
	typedef BOOL(WINAPI* PWinHttpSetOption) (HINTERNET, DWORD, LPVOID, DWORD);
	extern PWinHttpSetOption pWinHttpSetOption;
	typedef BOOL(WINAPI* PWinHttpAddRequestHeaders) (HINTERNET, LPCWSTR, DWORD, DWORD);
	extern PWinHttpAddRequestHeaders pWinHttpAddRequestHeaders;
	typedef BOOL(WINAPI* PWinHttpGetIEProxyConfigForCurrentUser) (WINHTTP_CURRENT_USER_IE_PROXY_CONFIG*);
	extern PWinHttpGetIEProxyConfigForCurrentUser pWinHttpGetIEProxyConfigForCurrentUser;
	typedef BOOL(WINAPI* PWinHttpGetProxyForUrl) (HINTERNET, LPCWSTR, WINHTTP_AUTOPROXY_OPTIONS*, WINHTTP_PROXY_INFO*);
	extern PWinHttpGetProxyForUrl pWinHttpGetProxyForUrl;

	typedef BOOL(WINAPI* PGetAdaptersInfo) (PIP_ADAPTER_INFO, PULONG);
	extern PGetAdaptersInfo pGetAdaptersInfo;

}

