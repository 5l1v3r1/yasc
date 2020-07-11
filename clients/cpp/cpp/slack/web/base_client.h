#pragma once

#include "../slackapi.h"
#include "../slackerrors.h"
#include "slack_response.h"

#include "../../lwhp/libwinhttp.h"
#include "../../lwhp/ClientSession.h"
#include "../../lwhp/NameValueCollection.h"
#include "../../lwhp/URI.h"
#include "../../json.hpp"


namespace slack::web {

using libwinhttp::NameValueCollection;
using libwinhttp::ClientSession;
using libwinhttp::URI;
using json = nlohmann::json;

class BaseClient {
    private:
        libwinhttp::ClientSession* _session;
        std::string token{};
        std::string base_url{};
        int timeout{ 60 };
        bool proxy{ false };
        NameValueCollection _headers{};

        std::string urljoin(const std::string &base_url, const std::string &add_part);

        static constexpr char useragent[] = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/74.0.3729.169 Safari/537.36";
        static constexpr char BASE_URL[] = "https://www.slack.com/api/";

    public:
        BaseClient(
            std::string token = "",
            std::string base_url = BASE_URL,
            int timeout = 30,
            bool proxy = false,
            NameValueCollection headers = NameValueCollection()) : token(token), base_url(base_url), timeout(timeout), proxy(proxy), _session(nullptr), _headers(headers)
        {
            URI uri{ base_url };
            if (uri.getScheme() == "https") {
                _session = new ClientSession(useragent);
            }
            else if (uri.getScheme() == "http") {
                _session = new ClientSession(useragent);
            }
            if (!token.empty()) {
                _headers.set("Authorization", "Bearer " + token);
            }
        };

        ~BaseClient() 
        {
            delete _session;
        };
    private:
        std::string _get_url(const std::string &api_method);

         
        NameValueCollection _get_headers(bool has_json, bool has_files, NameValueCollection request_specific_headers);

         
        SlackResponse _send(const std::string& http_verb, const std::string& api_url, const std::string& body, libwinhttp::WinHttpRequest &req_args);

         
        
        bool _request(WinHttpRequest& request);

        
        void _move_auth(json& json_val, NameValueCollection& auth) {
            if (json_val.find("auth") != json_val.end()) {
                auth.set("username", json_val["auth"]["client_id"]);
                auth.set("password", json_val["auth"]["client_secret"]);
                json_val.erase("auth");
            }
        }
    public:
        
        SlackResponse download_url(const std::string& url, const std::string& accept="*/*");

         
        SlackResponse api_call(
            std::string api_method,
            std::string http_verb = "POST",
            NameValueCollection files = NameValueCollection(),
            NameValueCollection form_data = NameValueCollection(),
            NameValueCollection params = NameValueCollection(),
            json json_val = json(),
            NameValueCollection headers = NameValueCollection(),
            NameValueCollection auth = NameValueCollection()
        );

         
        SlackResponse api_call(
            std::string api_method,
            json json_val
        ) {
            NameValueCollection auth;
            _move_auth(json_val, auth);
            return api_call(api_method, "POST", NameValueCollection(), NameValueCollection(), NameValueCollection(), json_val, NameValueCollection(), auth);
        };

         
        SlackResponse api_call(
            std::string api_method,
            NameValueCollection params
        ) {
            return api_call(api_method, "GET", NameValueCollection(), NameValueCollection(), params, json(), NameValueCollection(), NameValueCollection());
        };
    };
};