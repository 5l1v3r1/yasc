#pragma once

#include <string>

namespace RemoteAccessTool {

	using std::string;
	
	class Commands
	{
	public:
		Commands();
		~Commands();

		string process(const string& command_line);
	private:

	};

	Commands::Commands()
	{
	}

	Commands::~Commands()
	{
	}
}