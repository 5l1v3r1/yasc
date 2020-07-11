
#include "libwinhttp.h"
#include "Environment.h"
#include <cstdlib>
#include <cstdio> 


#if defined(_VXWORKS)
#include "Environment_VX.cpp"
#elif defined(_OS_FAMILY_UNIX)
#include "Environment_UNIX.cpp"
#elif defined(_OS_FAMILY_WINDOWS)
#if defined(_WIN32_WCE)
#include "Environment_WINCE.cpp"
#else
#include "Environment_WIN32U.cpp"
#endif
#endif


namespace libwinhttp {

std::string Environment::get(const std::string& name)
{
	return EnvironmentImpl::getImpl(name);
}


std::string Environment::get(const std::string& name, const std::string& defaultValue)
{
	if (has(name))
		return get(name);
	else
		return defaultValue;
}


bool Environment::has(const std::string& name)
{
	return EnvironmentImpl::hasImpl(name);
}


void Environment::set(const std::string& name, const std::string& value)
{
	EnvironmentImpl::setImpl(name, value);
}


std::string Environment::osName()
{
	return EnvironmentImpl::osNameImpl();
}


std::string Environment::osDisplayName()
{
	return EnvironmentImpl::osDisplayNameImpl();
}


std::string Environment::osVersion()
{
	return EnvironmentImpl::osVersionImpl();
}


std::string Environment::osArchitecture()
{
	return EnvironmentImpl::osArchitectureImpl();
}


std::string Environment::nodeName()
{
	return EnvironmentImpl::nodeNameImpl();
}


std::string Environment::nodeId()
{
	NodeId id;
	nodeId(id);
	char result[18];
	std::sprintf(result, "%02x:%02x:%02x:%02x:%02x:%02x",
		id[0],
		id[1],
		id[2],
		id[3],
		id[4],
		id[5]);
	return std::string(result);
}


void Environment::nodeId(NodeId& id)
{
	return EnvironmentImpl::nodeIdImpl(id);
}


unsigned Environment::processorCount()
{
	return EnvironmentImpl::processorCountImpl();
}


UInt32 Environment::libraryVersion()
{
	
	return 1;
}


Int32 Environment::os()
{
	return _OS;
}


Int32 Environment::arch()
{
	return _ARCH;
}


bool Environment::isUnix()
{
#if defined(_OS_FAMILY_UNIX)
	return true;
#else
	return false;
#endif
}


bool Environment::isWindows()
{
#if defined(_OS_FAMILY_WINDOWS)
	return true;
#else
	return false;
#endif
}


} 
