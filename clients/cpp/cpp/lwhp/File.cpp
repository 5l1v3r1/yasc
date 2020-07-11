
#include "File.h"
#include "Path.h"
#include <thread>
#include <chrono>
#include "Exceptions.h"
#include "DirectoryIterator.h"


#if defined(_OS_FAMILY_WINDOWS)
#if defined(_WIN32_WCE)
#include "File_WINCE.cpp"
#else
#include "File_WIN32U.cpp"
#endif
#elif defined(_VXWORKS)
#include "File_VX.cpp"
#elif defined(_OS_FAMILY_UNIX)
#include "File_UNIX.cpp"
#endif


namespace libwinhttp {

using namespace std::chrono_literals;

File::File()
{
}


File::File(const std::string& path): FileImpl(path)
{
}


File::File(const char* path): FileImpl(std::string(path))
{
}


File::File(const Path& path): FileImpl(path.toString())
{
}


File::File(const File& file): FileImpl(file.getPathImpl())
{
}


File::~File()
{
}


File& File::operator = (const File& file)
{
	setPathImpl(file.getPathImpl());
	return *this;
}


File& File::operator = (const std::string& path)
{
	setPathImpl(path);
	return *this;
}


File& File::operator = (const char* path)
{
	_check_ptr (path);
	setPathImpl(path);
	return *this;
}


File& File::operator = (const Path& path)
{
	setPathImpl(path.toString());
	return *this;
}


void File::swap(File& file)
{
	swapImpl(file);
}


bool File::exists() const
{
	return existsImpl();
}


bool File::canRead() const
{
	return canReadImpl();
}


bool File::canWrite() const
{
	return canWriteImpl();
}


bool File::canExecute() const
{
	return canExecuteImpl();
}


bool File::isFile() const
{
	return isFileImpl();
}


bool File::isDirectory() const
{
	return isDirectoryImpl();
}


bool File::isLink() const
{
	return isLinkImpl();
}


bool File::isDevice() const
{
	return isDeviceImpl();
}


bool File::isHidden() const
{
	return isHiddenImpl();
}


Timestamp File::created() const
{
	return createdImpl();
}


Timestamp File::getLastModified() const
{
	return getLastModifiedImpl();
}


File& File::setLastModified(const Timestamp& ts)
{
	setLastModifiedImpl(ts);
	return *this;
}


File::FileSize File::getSize() const
{
	return getSizeImpl();
}


File& File::setSize(FileSizeImpl size)
{
	setSizeImpl(size);
	return *this;
}


File& File::setWriteable(bool flag)
{
	setWriteableImpl(flag);
	return *this;
}


File& File::setReadOnly(bool flag)
{
	setWriteableImpl(!flag);
	return *this;
}


File& File::setExecutable(bool flag)
{
	setExecutableImpl(flag);
	return *this;
}


void File::copyTo(const std::string& path, int options) const
{
	Path src(getPathImpl());
	Path dest(path);
	File destFile(path);
	if ((destFile.exists() && destFile.isDirectory()) || dest.isDirectory())
	{
		dest.makeDirectory();
		dest.setFileName(src.getFileName());
	}
	if (isDirectory())
		copyDirectory(dest.toString(), options);
	else
		copyToImpl(dest.toString(), options);
}


void File::copyDirectory(const std::string& path, int options) const
{
	File target(path);
	target.createDirectories();

	Path src(getPathImpl());
	src.makeFile();
	DirectoryIterator it(src);
	DirectoryIterator end;
	for (; it != end; ++it)
	{
		it->copyTo(path, options);
	}
}


void File::moveTo(const std::string& path, int options)
{
	copyTo(path, options);
	remove(true);
	setPathImpl(path);
}


void File::renameTo(const std::string& path, int options)
{
	renameToImpl(path, options);
	setPathImpl(path);
}


void File::linkTo(const std::string& path, LinkType type) const
{
	linkToImpl(path, type);
}


void File::remove(bool recursive)
{
	if (recursive && !isLink() && isDirectory())
	{
		std::vector<File> files;
		list(files);
		for (auto& f: files)
		{
			f.remove(true);
		}

		
		
		
		
		
		
		
		

		int retry = 8;
		std::chrono::milliseconds sleep{ 10 };
		while (retry > 0)
		{
			try
			{
				removeImpl();
				retry = 0;
			}
			catch (DirectoryNotEmptyException&)
			{
				if (--retry == 0) throw;
				std::this_thread::sleep_for(sleep);
				sleep *= 2;
			}
		}
	}
	else
	{
		removeImpl();
	}
}


bool File::createFile()
{
	return createFileImpl();
}


bool File::createDirectory()
{
	return createDirectoryImpl();
}


void File::createDirectories()
{
	if (!exists())
	{
		Path p(getPathImpl());
		p.makeDirectory();
		if (p.depth() > 1)
		{
			p.makeParent();
			File f(p);
			f.createDirectories();
		}
		try
		{
			createDirectoryImpl();
		}
		catch (FileExistsException&)
		{
		}
	}
}


void File::list(std::vector<std::string>& files) const
{
	files.clear();
	DirectoryIterator it(*this);
	DirectoryIterator end;
	while (it != end)
	{
		files.push_back(it.name());
		++it;
	}
}


File::FileSize File::totalSpace() const
{
	return totalSpaceImpl();
}


File::FileSize File::usableSpace() const
{
	return usableSpaceImpl();
}


File::FileSize File::freeSpace() const
{
	return freeSpaceImpl();
}


void File::list(std::vector<File>& files) const
{
	files.clear();
	DirectoryIterator it(*this);
	DirectoryIterator end;
	while (it != end)
	{
		files.push_back(*it);
		++it;
	}
}


void File::handleLastError(const std::string& path)
{
	handleLastErrorImpl(path);
}


} 