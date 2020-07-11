#include "WinHttpRequest.h"
#include "Exceptions.h"

#include "String.h"

#include <sstream>

namespace libwinhttp {

    WinHttpRequest::WinHttpRequest(const string& verb, const URI& url) : WinHttpRequest(verb, url.getScheme(), url.getHost(), url.getPathAndQuery(), url.getPort()) {  };


    WinHttpRequest::WinHttpRequest(const string& verb, const string& scheme, const string& hostname, const string& url, unsigned short port)
    {
        
        this->requestVerb = utf8_to_wstr(verb);

        string low_scheme = toLower(scheme);
        if (low_scheme == "http") {
            this->requestScheme = INTERNET_SCHEME_HTTP;
            this->requestPort = port ? port : 80;
        }
        else if (low_scheme == "https") {
            this->requestScheme = INTERNET_SCHEME_HTTPS;
            this->requestPort = port ? port : 443;
        }
        else {
            throw LibWinHttpError("Invalid scheme", scheme);
        };
        wstring w_hostname = utf8_to_wstr(hostname);
        if (w_hostname.size() < MAX_PATH) {
            wcscpy_s(this->requestHost, w_hostname.c_str());
        }
        else {
            throw LibWinHttpError("Hostname length exceeds MAX_PATH.");
        }
        wstring w_url = utf8_to_wstr(url);
        if (w_url.size() < (MAX_PATH * 5)) {
            wcscpy_s(this->requestURL, w_url.c_str());
        }
        else {
            throw LibWinHttpError("Hostname length exceeds MAX_PATH*5.");
        }
    };


    void WinHttpRequest::set_additional_headers(NameValueCollection& headers)
    {
        
        if (headers.empty()) {
            additionalRequestHeaders.clear();
        }
        else
        {
            std::stringstream ss_additionalRequestHeaders;
            bool first = true;
            for (auto& key : headers) {
                if (!first)
                    ss_additionalRequestHeaders << "\r\n";
                ss_additionalRequestHeaders << key.first << ": " << key.second;
                first = false;
            }
            additionalRequestHeaders = utf8_to_wstr(ss_additionalRequestHeaders.str());
        }
    };

    void WinHttpRequest::setContentLength(size_t length) {
        
    };

    void WinHttpRequest::setURI(const URI& url) {
        wstring w_url = utf8_to_wstr(url.toString());
        URL_COMPONENTS urlComp;
        memset(&urlComp, 0, sizeof(urlComp));
        urlComp.dwStructSize = sizeof(urlComp);
        urlComp.lpszHostName = this->requestHost;
        urlComp.dwHostNameLength = MAX_PATH;
        urlComp.lpszUrlPath = this->requestURL;
        urlComp.dwUrlPathLength = MAX_PATH * 5;
        urlComp.dwSchemeLength = 1; 
        urlComp.nPort = 1; 

        
        if (pWinHttpCrackUrl(w_url.c_str(), (DWORD)w_url.size(), 0, &urlComp)) {
            this->requestPort = urlComp.nPort;
            this->requestScheme = urlComp.nScheme;

            if (urlComp.dwUrlPathLength == 0)
                throw LibWinHttpError("No URL Path specified in URL", url.toString());
        }
        else {
            int error = GetLastError();
            throw LibWinHttpError("Invalid URL supplied", url.toString());
        }
    }
}