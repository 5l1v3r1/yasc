















#ifndef Foundation_DirectoryIterator_UNIX_INCLUDED
#define Foundation_DirectoryIterator_UNIX_INCLUDED


#include "Poco/Foundation.h"
#include <dirent.h>


namespace Poco {


class Foundation_API DirectoryIteratorImpl
{
public:
	DirectoryIteratorImpl(const std::string& path);
	~DirectoryIteratorImpl();
	
	void duplicate();
	void release();
	
	const std::string& get() const;
	const std::string& next();
	
private:
	DIR*        _pDir;
	std::string _current;
	int _rc;
};





const std::string& DirectoryIteratorImpl::get() const
{
	return _current;
}


inline void DirectoryIteratorImpl::duplicate()
{
	++_rc;
}


inline void DirectoryIteratorImpl::release()
{
	if (--_rc == 0)
		delete this;
}


} 


#endif 
