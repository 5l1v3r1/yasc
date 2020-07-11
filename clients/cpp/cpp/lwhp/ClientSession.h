#pragma once

#include "libwinhttp.h"
#include "WinHttpRequest.h"
#include "Buffer.h"

using libwinhttp::WinHttpRequest;

namespace libwinhttp {
    class ClientSession {
    private:
        static const unsigned int INT_RETRYTIMES = 3;
        static const DWORD DW_MAXCONTENTLENGTH = 10*1024*1024;  
        static constexpr char useragent[] = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/74.0.3729.169 Safari/537.36";
        HINTERNET session = nullptr;
        bool closed_{ true };
        Buffer<BYTE> _pResponse{ 10*1024 };
        DWORD _dwResponseBufferSize = 0;

        HINTERNET _hRequest = nullptr;
        HINTERNET _hConnect = nullptr;
        wstring _forcedCharset{ };

    public:
        ClientSession(string useragent = useragent);
        ~ClientSession();

        bool closed() {
            return closed_;
        };

        void close() {
            if (!closed_) {
                closed_ = true;
                pWinHttpCloseHandle(session);
            };
        };

        
        
        bool sendRequest(WinHttpRequest& req);
        bool _request(WinHttpRequest& req);

        
        bool receiveResponse(WinHttpResponse& response);
        DWORD _readRequest(DWORD dwSize);
        void _resizeResponseBuffer(DWORD dwNewSize);

        bool request(const string& http_verb, WinHttpRequest& req);
        bool _request(const string & http_verb, WinHttpRequest & req);

        void reset();
    };
}