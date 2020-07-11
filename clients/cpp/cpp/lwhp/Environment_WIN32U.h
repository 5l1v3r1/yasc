#pragma once

#include <string>
#include "Types.h"


namespace libwinhttp {


class EnvironmentImpl
{
public:
	typedef UInt8 NodeId[6]; 

	static std::string getImpl(const std::string& name);	
	static bool hasImpl(const std::string& name);	
	static void setImpl(const std::string& name, const std::string& value);
	static std::string osNameImpl();	
	static std::string osDisplayNameImpl();
	static std::string osVersionImpl();
	static std::string osArchitectureImpl();
	static std::string nodeNameImpl();
	static void nodeIdImpl(NodeId& id);
	static unsigned processorCountImpl();
};


} 
