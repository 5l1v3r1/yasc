#include "client.h"


namespace slack::web {

     
    SlackResponse WebClient::channels_leave(std::string channel, json opt_params) {
        opt_params["channel"] = channel;
        return api_call("channels.leave", opt_params);
    };

     
    SlackResponse WebClient::channels_list(NameValueCollection opt_params) {
        
        
        if (opt_params.get("types", "") == "")
            
            opt_params.set("types", "public_channel");
        return api_call("conversations.list", "GET", NameValueCollection(), NameValueCollection(), opt_params);
    };

     
    SlackResponse WebClient::channels_history(std::string channel, NameValueCollection opt_params) {
        opt_params.set("channel", channel);
        return api_call("conversations.history", "GET", NameValueCollection(), NameValueCollection(), opt_params);
    };

     
    SlackResponse WebClient::chat_postMessage(std::string channel, std::string text, json opt_params) {
        opt_params["channel"] = channel;
        opt_params["text"] = text;
        return api_call("chat.postMessage", opt_params);
    }

    SlackResponse WebClient::chat_postMessage(std::string channel, json blocks, json opt_params) {
        opt_params["channel"] = channel;
        opt_params["blocks"] = blocks;
        return api_call("chat.postMessage", opt_params);
    }

     
    SlackResponse WebClient::files_upload(std::string file_or_content, json opt_params, bool is_content) {
        if (file_or_content.empty() && !is_content) {
            throw SlackRequestError("Filepath must be specified when not using content.");
        }
        if (is_content) {
            opt_params["content"] = file_or_content;
            return api_call("files.upload", "POST", NameValueCollection(), NameValueCollection(), NameValueCollection(), opt_params);
        }
        else {
            NameValueCollection files{};
            NameValueCollection form_data{};
            for (auto& item : opt_params.items()) {
                
                if (!item.key().empty())
                    form_data.set(item.key(), item.value().get<string>());
            }
            files.set("file", file_or_content);
            return api_call("files.upload", "POST", files, form_data);
        }
    }

     
    SlackResponse WebClient::files_list(json opt_params)
    {
        return api_call("files.list", opt_params);
    };
};