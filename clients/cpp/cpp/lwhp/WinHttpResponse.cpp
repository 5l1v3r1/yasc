
#include "WinHttpResponse.h"
#include "Exception.h"
#include "String.h"


namespace libwinhttp {
    bool WinHttpResponse::_is_expected_content_type(const string& response_content_type, const string& expected_content_type) {
        if (expected_content_type == "application/json")
            
            
            return response_content_type.find("json");
        return response_content_type.find(expected_content_type) != string::npos;
    }

    json WinHttpResponse::toJson(const string& encoding, const string& content_type)
    {
        
        if (responseContent.size() == 0)
            return json();

        string ctype = toLower(responseHeaders["Content-Type"]);
        if (!_is_expected_content_type(ctype, content_type))
            throw ContentTypeException("Cannot convert to json with wrong content-type", ctype);
        json root{};

        try {
            root = json::parse(this->responseContent);
        }
        catch (std::exception&) {
            throw libwinhttp::DataFormatException("JSON data was not properly formatted");
        }
        if (root.is_null()) {
            throw libwinhttp::DataFormatException("JSON data was not properly formatted");
        }
        return root;
    };

    std::string& WinHttpResponse::getContentType() {
        return responseHeaders["Content-Type"];
    }
}