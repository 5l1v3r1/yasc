#pragma once

#include <string>
#include "Types.h"

namespace libwinhttp {


class Environment
	
	
{
public:
	typedef UInt8 NodeId[6]; 

	static std::string get(const std::string& name);
		
		
		

	static std::string get(const std::string& name, const std::string& defaultValue);
		
		
		

	static bool has(const std::string& name);
		
		

	static void set(const std::string& name, const std::string& value);
		
		

	static std::string osName();
		

	static std::string osDisplayName();
		
		
		
		
		
		
		
		

	static std::string osVersion();
		

	static std::string osArchitecture();
		

	static std::string nodeName();
		

	static void nodeId(NodeId& id);
		
		
		
		

	static std::string nodeId();
		
		
		
		

	static unsigned processorCount();
		
		
		

	static UInt32 libraryVersion();
		
		
		
		
		
		
		
		
		
		
		

	static Int32 os();
		
		

	static Int32 arch();
		
		

	static bool isUnix();
		

	static bool isWindows();
		
};


} 
