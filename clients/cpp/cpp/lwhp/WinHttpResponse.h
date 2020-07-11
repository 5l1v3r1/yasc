#pragma once

#include "libwinhttp.h"
#include "../json.hpp"
#include "Buffer.h"



namespace libwinhttp {
    using json = nlohmann::json;
    using libwinhttp::Buffer;

    class WinHttpResponse {
    public:
        string responseURL{};
        string location{};
        string responseHost{};
        Buffer<unsigned char> responseContent { 0 };
        StringDict responseHeaders{};
        StringDict responseCookies{};
        bool dataWritten{ false };
        string responseCharset{};
        unsigned int statusCode{ 0 };
        unsigned long contentLength{ 0 };

        
        unsigned long lastError{ 0 };

        WinHttpResponse() { };

         
        json toJson(const string& encoding = "", const string& content_type = "application/json");
        bool _is_expected_content_type(const string& response_content_type, const string& expected_content_type);
        std::string& getContentType();
    };
} 