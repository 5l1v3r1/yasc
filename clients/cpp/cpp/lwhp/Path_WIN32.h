















#ifndef Foundation_Path_WIN32_INCLUDED
#define Foundation_Path_WIN32_INCLUDED


#include "Poco/Foundation.h"
#include <vector>


namespace Poco {


class Foundation_API PathImpl
{
public:
	static std::string currentImpl();
	static std::string homeImpl();
	static std::string configHomeImpl();
	static std::string dataHomeImpl();
	static std::string cacheHomeImpl();
	static std::string tempHomeImpl();
	static std::string tempImpl();
	static std::string configImpl();
	static std::string nullImpl();
	static std::string systemImpl();
	static std::string expandImpl(const std::string& path);
	static void listRootsImpl(std::vector<std::string>& roots);
};


} 


#endif 
