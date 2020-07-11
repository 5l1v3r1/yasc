#pragma once

#include "slackapi.h"
#include "web/slack_response.h"

#include <exception>
#include <string>

namespace slack::web {
    class SlackResponse;
};

namespace slack {
    class SlackClientError {
    public:
        SlackClientError() : msg_("Unknown Error") {};
        SlackClientError(std::string message) : msg_(message) {};
        virtual const std::string what() const noexcept {
            return msg_;
        };

    protected:
        std::string msg_;
    };

     
    class BotUserAccessError : public SlackClientError {
    public:
        BotUserAccessError() : SlackClientError() {};
        BotUserAccessError(std::string message) : SlackClientError(message) { };
    };

     

    class SlackRequestError : public SlackClientError {
    public:
        SlackRequestError() : SlackClientError() {};
        SlackRequestError(std::string message) : SlackClientError(message) { };
    };

     
    class SlackApiError : public SlackClientError {
    public:
        std::string server_response;
        SlackApiError(std::string response) : SlackClientError(), server_response(response) { };
        SlackApiError(std::string message, std::string response) : SlackClientError(message), server_response(response) { };
        virtual const std::string what() const noexcept;
    };
};
 