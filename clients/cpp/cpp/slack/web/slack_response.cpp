#include "../slackapi.h"
#include "../slackerrors.h"
#include "slack_response.h"   


namespace slack::web {
     
    SlackResponse& SlackResponse::validate() {
        json ok = this->json_val["ok"];
        if (this->status_code == 200 && ok.is_boolean() && ok == true) {
            return *this;
        }
        string output = this->json_val["error"].dump();
        std::string msg = "The request to the Slack API failed: " + output;
        throw SlackApiError(msg, to_string());
    };
};
