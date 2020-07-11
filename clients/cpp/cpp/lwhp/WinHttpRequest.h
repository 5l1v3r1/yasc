#pragma once

#include "libwinhttp.h"
#include "UnWindows.h"
#include "WinHttpResponse.h"
#include "NameValueCollection.h"
#include "URI.h"

#include "_winhttp.h"


namespace libwinhttp {

    class WinHttpRequest {
    public:
        wstring requestVerb = L"GET";
        wchar_t requestURL[MAX_PATH * 5] = L"";
        wchar_t requestHost[MAX_PATH] = L"";
        INTERNET_PORT requestPort;
        INTERNET_SCHEME requestScheme;
    public:
        wstring additionalRequestHeaders{};       
        NameValueCollection additionalRequestCookies{};    
        NameValueCollection params{};
        json json_val{};
        NameValueCollection form_data{};
        const char* data{ 0 };
        DWORD data_size{ 0 };
        FileDict files{};
        bool ssl{ true };
        bool requireValidSsl{ true };
        bool disableAutoRedirect{ false };
        string forcedCharset{};

        
        string proxy{};                      
        string proxyUsername{};
        string proxyPassword{};

        
        unsigned long lastError{ 0 };

        
        unsigned int resolveTimeout{ 0 };
        unsigned int connectTimeout{ 15000 };
        unsigned int sendTimeout{ 10000 };
        unsigned int receiveTimeout{ 10000 };

        bool response_valid{ false };
        std::unique_ptr<WinHttpResponse> response = std::make_unique<WinHttpResponse>();

        WinHttpRequest(const string& verb, const URI& url);
        WinHttpRequest(const string& verb, const string& scheme, const string& hostname, const string& url, unsigned short port = 80);

         
        void set_additional_headers(NameValueCollection& headers);
        void setContentLength(size_t length);

        void setURI(const URI& url);

         
        void add_part() {};
    };

} 