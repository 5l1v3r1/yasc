#pragma once

#include "../slackapi.h"
#include "../../json.hpp"

#include "../../lwhp/libwinhttp.h"
#include "../../lwhp/WinHttpRequest.h"
#include "../../lwhp/WinHttpResponse.h"
#include "../../lwhp/String.h"
#include "../../lwhp/Buffer.h"

#include <sstream>

namespace slack::web {
using libwinhttp::StringDict;
using libwinhttp::WinHttpRequest;
using libwinhttp::WinHttpResponse;
using libwinhttp::isubstr;
using libwinhttp::Buffer;

using std::string;
using std::stringstream;
using json = nlohmann::json;

class BaseClient;

class SlackResponse {
private:
    BaseClient& client;
    
public:
    string http_verb;
    string api_url;
    json json_val;
    Buffer<unsigned char> rawdata{ 0 };
    unsigned long rawdata_size{ 0 };
    StringDict headers;
    int status_code;

    SlackResponse(
        BaseClient& client,
        string http_verb,
        string api_url,
        int status_code,
        WinHttpResponse& response
    ) :
        client(client), http_verb(http_verb), api_url(api_url), status_code(status_code), rawdata_size(0)  
    {
        if (isubstr<std::string>(response.getContentType(), "application/json") == 0) {
            auto root = json::parse(response.responseContent);

            

            if (!root.is_null()) {
                json_val = root;
            }
        }
         
        else {
            this->rawdata_size = response.responseContent.sizeBytes();
            this->rawdata = response.responseContent;
        }
    };

    virtual ~SlackResponse() {
        
    };

    json operator[] (string item) {
        
        return this->json_val[item];
    };

    const string to_string() const {
        return this->json_val.dump();
    };

    
    
     

    SlackResponse& validate();
};

};