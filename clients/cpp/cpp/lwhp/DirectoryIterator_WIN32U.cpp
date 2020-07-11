
#include "DirectoryIterator_WIN32U.h"
#if defined(_WIN32_WCE)
#include "Poco/File_WINCE.h"
#else
#include "File_WIN32U.h"
#endif
#include "Path.h"

#include "String.h"
#include <cstring>


namespace libwinhttp {


DirectoryIteratorImpl::DirectoryIteratorImpl(const std::string& path): _fh(INVALID_HANDLE_VALUE), _rc(1)
{
	Path p(path);
	p.makeDirectory();
	std::string findPath = p.toString();
	findPath.append("*");
	std::wstring uFindPath;
	FileImpl::convertPath(findPath, uFindPath);

	_fh = FindFirstFileW(uFindPath.c_str(), &_fd);
	if (_fh == INVALID_HANDLE_VALUE)
	{
		if (GetLastError() != ERROR_NO_MORE_FILES)
			File::handleLastError(path);
	}
	else
	{
		_current = wstr_to_utf8(_fd.cFileName);
		if (_current == "." || _current == "..")	
			next();
	}
}


DirectoryIteratorImpl::~DirectoryIteratorImpl()
{
	if (_fh != INVALID_HANDLE_VALUE)
		FindClose(_fh);
}


const std::string& DirectoryIteratorImpl::next()
{
	do
	{
		_current.clear();
		if (FindNextFileW(_fh, &_fd) != 0)
		{
			_current = wstr_to_utf8(_fd.cFileName);
		}
	}
	while (_current == "." || _current == "..");
	return _current;
}


} 
