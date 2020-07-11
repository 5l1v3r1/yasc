#pragma once

#include "libwinhttp.h"
#include <stdexcept>


namespace libwinhttp {


class Exception: public std::exception
	
	
{
public:
	Exception(const std::string& msg, int code = 0);
		

	Exception(const std::string& msg, const std::string& arg, int code = 0);
		

	Exception(const std::string& msg, const Exception& nested, int code = 0);
		
		

	Exception(const Exception& exc);
		
		
	~Exception() throw();
		

	Exception& operator = (const Exception& exc);
		

	virtual const char* name() const throw();
		
		
	virtual const char* className() const throw();
		
		
	virtual const char* what() const throw();
		
		
		
		
	const Exception* nested() const;
		
		
			
	const std::string& message() const;
		
			
	int code() const;
		
		
	std::string displayText() const;
		
		

	virtual Exception* clone() const;
		
		
		
		
		
	virtual void rethrow() const;
		
		
		
		
		

protected:
	Exception(int code = 0);
		

	void message(const std::string& msg);
		

	void extendedMessage(const std::string& arg);
		
		
private:
	std::string _msg;
	Exception*  _pNested;
	int			_code;
};





inline const Exception* Exception::nested() const
{
	return _pNested;
}


inline const std::string& Exception::message() const
{
	return _msg;
}


inline void Exception::message(const std::string& msg)
{
	_msg = msg;
}


inline int Exception::code() const
{
	return _code;
}








#define LIBWINHTTP_DECLARE_EXCEPTION_CODE(CLS, BASE, CODE) \
	class CLS: public BASE														    \
	{																				\
	public:																			\
		CLS(int code = CODE);														\
		CLS(const std::string& msg, int code = CODE);								\
		CLS(const std::string& msg, const std::string& arg, int code = CODE);		\
		CLS(const std::string& msg, const libwinhttp::Exception& exc, int code = CODE);	\
		CLS(const CLS& exc);														\
		~CLS() throw();																\
		CLS& operator = (const CLS& exc);											\
		const char* name() const throw();											\
		const char* className() const throw();										\
		libwinhttp::Exception* clone() const;												\
		void rethrow() const;														\
	};

#define LIBWINHTTP_DECLARE_EXCEPTION(CLS, BASE) \
	LIBWINHTTP_DECLARE_EXCEPTION_CODE(CLS, BASE, 0)

#define LIBWINHTTP_IMPLEMENT_EXCEPTION(CLS, BASE, NAME)													\
	CLS::CLS(int code): BASE(code)																	\
	{																								\
	}																								\
	CLS::CLS(const std::string& msg, int code): BASE(msg, code)										\
	{																								\
	}																								\
	CLS::CLS(const std::string& msg, const std::string& arg, int code): BASE(msg, arg, code)		\
	{																								\
	}																								\
	CLS::CLS(const std::string& msg, const libwinhttp::Exception& exc, int code): BASE(msg, exc, code)	\
	{																								\
	}																								\
	CLS::CLS(const CLS& exc): BASE(exc)																\
	{																								\
	}																								\
	CLS::~CLS() throw()																				\
	{																								\
	}																								\
	CLS& CLS::operator = (const CLS& exc)															\
	{																								\
		BASE::operator = (exc);																		\
		return *this;																				\
	}																								\
	const char* CLS::name() const throw()															\
	{																								\
		return NAME;																				\
	}																								\
	const char* CLS::className() const throw()														\
	{																								\
		return typeid(*this).name();																\
	}																								\
	libwinhttp::Exception* CLS::clone() const																\
	{																								\
		return new CLS(*this);																		\
	}																								\
	void CLS::rethrow() const																		\
	{																								\
		throw *this;																				\
	}





LIBWINHTTP_DECLARE_EXCEPTION(LogicException, Exception)
LIBWINHTTP_DECLARE_EXCEPTION(AssertionViolationException, LogicException)
LIBWINHTTP_DECLARE_EXCEPTION(NullPointerException, LogicException)
LIBWINHTTP_DECLARE_EXCEPTION(NullValueException, LogicException)
LIBWINHTTP_DECLARE_EXCEPTION(BugcheckException, LogicException)
LIBWINHTTP_DECLARE_EXCEPTION(InvalidArgumentException, LogicException)
LIBWINHTTP_DECLARE_EXCEPTION(NotImplementedException, LogicException)
LIBWINHTTP_DECLARE_EXCEPTION(RangeException, LogicException)
LIBWINHTTP_DECLARE_EXCEPTION(IllegalStateException, LogicException)
LIBWINHTTP_DECLARE_EXCEPTION(InvalidAccessException, LogicException)
LIBWINHTTP_DECLARE_EXCEPTION(SignalException, LogicException)
LIBWINHTTP_DECLARE_EXCEPTION(UnhandledException, LogicException)

LIBWINHTTP_DECLARE_EXCEPTION(RuntimeException, Exception)
LIBWINHTTP_DECLARE_EXCEPTION(NotFoundException, RuntimeException)
LIBWINHTTP_DECLARE_EXCEPTION(ExistsException, RuntimeException)
LIBWINHTTP_DECLARE_EXCEPTION(TimeoutException, RuntimeException)
LIBWINHTTP_DECLARE_EXCEPTION(SystemException, RuntimeException)
LIBWINHTTP_DECLARE_EXCEPTION(RegularExpressionException, RuntimeException)
LIBWINHTTP_DECLARE_EXCEPTION(LibraryLoadException, RuntimeException)
LIBWINHTTP_DECLARE_EXCEPTION(LibraryAlreadyLoadedException, RuntimeException)
LIBWINHTTP_DECLARE_EXCEPTION(NoThreadAvailableException, RuntimeException)
LIBWINHTTP_DECLARE_EXCEPTION(PropertyNotSupportedException, RuntimeException)
LIBWINHTTP_DECLARE_EXCEPTION(PoolOverflowException, RuntimeException)
LIBWINHTTP_DECLARE_EXCEPTION(NoPermissionException, RuntimeException)
LIBWINHTTP_DECLARE_EXCEPTION(OutOfMemoryException, RuntimeException)
LIBWINHTTP_DECLARE_EXCEPTION(DataException, RuntimeException)

LIBWINHTTP_DECLARE_EXCEPTION(DataFormatException, DataException)
LIBWINHTTP_DECLARE_EXCEPTION(SyntaxException, DataException)
LIBWINHTTP_DECLARE_EXCEPTION(CircularReferenceException, DataException)
LIBWINHTTP_DECLARE_EXCEPTION(PathSyntaxException, SyntaxException)
LIBWINHTTP_DECLARE_EXCEPTION(IOException, RuntimeException)
LIBWINHTTP_DECLARE_EXCEPTION(ProtocolException, IOException)
LIBWINHTTP_DECLARE_EXCEPTION(FileException, IOException)
LIBWINHTTP_DECLARE_EXCEPTION(FileExistsException, FileException)
LIBWINHTTP_DECLARE_EXCEPTION(FileNotFoundException, FileException)
LIBWINHTTP_DECLARE_EXCEPTION(PathNotFoundException, FileException)
LIBWINHTTP_DECLARE_EXCEPTION(FileReadOnlyException, FileException)
LIBWINHTTP_DECLARE_EXCEPTION(FileAccessDeniedException, FileException)
LIBWINHTTP_DECLARE_EXCEPTION(CreateFileException, FileException)
LIBWINHTTP_DECLARE_EXCEPTION(OpenFileException, FileException)
LIBWINHTTP_DECLARE_EXCEPTION(WriteFileException, FileException)
LIBWINHTTP_DECLARE_EXCEPTION(ReadFileException, FileException)
LIBWINHTTP_DECLARE_EXCEPTION(DirectoryNotEmptyException, FileException)
LIBWINHTTP_DECLARE_EXCEPTION(UnknownURISchemeException, RuntimeException)
LIBWINHTTP_DECLARE_EXCEPTION(TooManyURIRedirectsException, RuntimeException)
LIBWINHTTP_DECLARE_EXCEPTION(URISyntaxException, SyntaxException)

LIBWINHTTP_DECLARE_EXCEPTION(ApplicationException, Exception)
LIBWINHTTP_DECLARE_EXCEPTION(BadCastException, RuntimeException)

LIBWINHTTP_DECLARE_EXCEPTION(NetException, libwinhttp::IOException)
LIBWINHTTP_DECLARE_EXCEPTION(InvalidAddressException, NetException)
LIBWINHTTP_DECLARE_EXCEPTION(InvalidSocketException, NetException)
LIBWINHTTP_DECLARE_EXCEPTION(ServiceNotFoundException, NetException)
LIBWINHTTP_DECLARE_EXCEPTION(ConnectionAbortedException, NetException)
LIBWINHTTP_DECLARE_EXCEPTION(ConnectionResetException, NetException)
LIBWINHTTP_DECLARE_EXCEPTION(ConnectionRefusedException, NetException)
LIBWINHTTP_DECLARE_EXCEPTION(DNSException, NetException)
LIBWINHTTP_DECLARE_EXCEPTION(HostNotFoundException, DNSException)
LIBWINHTTP_DECLARE_EXCEPTION(NoAddressFoundException, DNSException)
LIBWINHTTP_DECLARE_EXCEPTION(InterfaceNotFoundException, NetException)
LIBWINHTTP_DECLARE_EXCEPTION(NoMessageException, NetException)
LIBWINHTTP_DECLARE_EXCEPTION(MessageException, NetException)
LIBWINHTTP_DECLARE_EXCEPTION(MultipartException, MessageException)
LIBWINHTTP_DECLARE_EXCEPTION(HTTPException, NetException)
LIBWINHTTP_DECLARE_EXCEPTION(NotAuthenticatedException, HTTPException)
LIBWINHTTP_DECLARE_EXCEPTION(UnsupportedRedirectException, HTTPException)
LIBWINHTTP_DECLARE_EXCEPTION(FTPException, NetException)
LIBWINHTTP_DECLARE_EXCEPTION(SMTPException, NetException)
LIBWINHTTP_DECLARE_EXCEPTION(POP3Exception, NetException)
LIBWINHTTP_DECLARE_EXCEPTION(ICMPException, NetException)
LIBWINHTTP_DECLARE_EXCEPTION(ICMPFragmentationException, NetException)
LIBWINHTTP_DECLARE_EXCEPTION(NTPException, NetException)
LIBWINHTTP_DECLARE_EXCEPTION(HTMLFormException, NetException)
LIBWINHTTP_DECLARE_EXCEPTION(WebSocketException, NetException)
LIBWINHTTP_DECLARE_EXCEPTION(UnsupportedFamilyException, NetException)
LIBWINHTTP_DECLARE_EXCEPTION(AddressFamilyMismatchException, NetException)
LIBWINHTTP_DECLARE_EXCEPTION(ContentTypeException, NetException)

} 
