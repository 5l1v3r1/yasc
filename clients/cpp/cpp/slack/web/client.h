#pragma once

#include "../slackapi.h"
#include "base_client.h"
#include "slack_response.h"
#include "../slackerrors.h"

#include "../../lwhp/NameValueCollection.h"

#include "../../json.hpp"

namespace slack::web {
    using json = nlohmann::json;

     
    class WebClient : public BaseClient {
    public:
        WebClient(std::string token) : BaseClient(token) {

        };

         
        SlackResponse channels_leave(std::string channel, json opt_params = json());
        SlackResponse channels_list(NameValueCollection opt_params = NameValueCollection());
        SlackResponse channels_history(std::string channel, NameValueCollection opt_params = NameValueCollection());
        SlackResponse chat_postMessage(std::string channel, std::string text, json opt_params = json());
        SlackResponse chat_postMessage(std::string channel, json blocks, json opt_params = json());
        SlackResponse files_upload(std::string filepath, json opt_params = json(), bool is_content = false);
        SlackResponse files_list(json opt_params = json());
    };
};