#include "Exception.h"
#include <typeinfo>


namespace libwinhttp {


Exception::Exception(int code): _pNested(0), _code(code)
{
}


Exception::Exception(const std::string& msg, int code): _msg(msg), _pNested(0), _code(code)
{
}


Exception::Exception(const std::string& msg, const std::string& arg, int code): _msg(msg), _pNested(0), _code(code)
{
	if (!arg.empty())
	{
		_msg.append(": ");
		_msg.append(arg);
	}
}


Exception::Exception(const std::string& msg, const Exception& nested, int code): _msg(msg), _pNested(nested.clone()), _code(code)
{
}


Exception::Exception(const Exception& exc):
	std::exception(exc),
	_msg(exc._msg),
	_code(exc._code)
{
	_pNested = exc._pNested ? exc._pNested->clone() : 0;
}

	
Exception::~Exception() throw()
{
	delete _pNested;
}


Exception& Exception::operator = (const Exception& exc)
{
	if (&exc != this)
	{
		Exception* newPNested = exc._pNested ? exc._pNested->clone() : 0;
		delete _pNested;
		_msg     = exc._msg;
		_pNested = newPNested;
		_code    = exc._code;
	}
	return *this;
}


const char* Exception::name() const throw()
{
	return "Exception";
}


const char* Exception::className() const throw()
{
	return typeid(*this).name();
}

	
const char* Exception::what() const throw()
{
	return name();
}

	
std::string Exception::displayText() const
{
	std::string txt = name();
	if (!_msg.empty())
	{
		txt.append(": ");
		txt.append(_msg);
	}
	return txt;
}


void Exception::extendedMessage(const std::string& arg)
{
	if (!arg.empty())
	{
		if (!_msg.empty()) _msg.append(": ");
		_msg.append(arg);
	}
}


Exception* Exception::clone() const
{
	return new Exception(*this);
}


void Exception::rethrow() const
{
	throw *this;
}


LIBWINHTTP_IMPLEMENT_EXCEPTION(LogicException, Exception, "Logic exception")
LIBWINHTTP_IMPLEMENT_EXCEPTION(AssertionViolationException, LogicException, "Assertion violation")
LIBWINHTTP_IMPLEMENT_EXCEPTION(NullPointerException, LogicException, "Null pointer")
LIBWINHTTP_IMPLEMENT_EXCEPTION(NullValueException, LogicException, "Null value")
LIBWINHTTP_IMPLEMENT_EXCEPTION(BugcheckException, LogicException, "Bugcheck")
LIBWINHTTP_IMPLEMENT_EXCEPTION(InvalidArgumentException, LogicException, "Invalid argument")
LIBWINHTTP_IMPLEMENT_EXCEPTION(NotImplementedException, LogicException, "Not implemented")
LIBWINHTTP_IMPLEMENT_EXCEPTION(RangeException, LogicException, "Out of range")
LIBWINHTTP_IMPLEMENT_EXCEPTION(IllegalStateException, LogicException, "Illegal state")
LIBWINHTTP_IMPLEMENT_EXCEPTION(InvalidAccessException, LogicException, "Invalid access")
LIBWINHTTP_IMPLEMENT_EXCEPTION(SignalException, LogicException, "Signal received")
LIBWINHTTP_IMPLEMENT_EXCEPTION(UnhandledException, LogicException, "Unhandled exception")

LIBWINHTTP_IMPLEMENT_EXCEPTION(RuntimeException, Exception, "Runtime exception")
LIBWINHTTP_IMPLEMENT_EXCEPTION(NotFoundException, RuntimeException, "Not found")
LIBWINHTTP_IMPLEMENT_EXCEPTION(ExistsException, RuntimeException, "Exists")
LIBWINHTTP_IMPLEMENT_EXCEPTION(TimeoutException, RuntimeException, "Timeout")
LIBWINHTTP_IMPLEMENT_EXCEPTION(SystemException, RuntimeException, "System exception")
LIBWINHTTP_IMPLEMENT_EXCEPTION(RegularExpressionException, RuntimeException, "Error in regular expression")
LIBWINHTTP_IMPLEMENT_EXCEPTION(LibraryLoadException, RuntimeException, "Cannot load library")
LIBWINHTTP_IMPLEMENT_EXCEPTION(LibraryAlreadyLoadedException, RuntimeException, "Library already loaded")
LIBWINHTTP_IMPLEMENT_EXCEPTION(NoThreadAvailableException, RuntimeException, "No thread available")
LIBWINHTTP_IMPLEMENT_EXCEPTION(PropertyNotSupportedException, RuntimeException, "Property not supported")
LIBWINHTTP_IMPLEMENT_EXCEPTION(PoolOverflowException, RuntimeException, "Pool overflow")
LIBWINHTTP_IMPLEMENT_EXCEPTION(NoPermissionException, RuntimeException, "No permission")
LIBWINHTTP_IMPLEMENT_EXCEPTION(OutOfMemoryException, RuntimeException, "Out of memory")
LIBWINHTTP_IMPLEMENT_EXCEPTION(DataException, RuntimeException, "Data error")

LIBWINHTTP_IMPLEMENT_EXCEPTION(DataFormatException, DataException, "Bad data format")
LIBWINHTTP_IMPLEMENT_EXCEPTION(SyntaxException, DataException, "Syntax error")
LIBWINHTTP_IMPLEMENT_EXCEPTION(CircularReferenceException, DataException, "Circular reference")
LIBWINHTTP_IMPLEMENT_EXCEPTION(PathSyntaxException, SyntaxException, "Bad path syntax")
LIBWINHTTP_IMPLEMENT_EXCEPTION(IOException, RuntimeException, "I/O error")
LIBWINHTTP_IMPLEMENT_EXCEPTION(ProtocolException, IOException, "Protocol error")
LIBWINHTTP_IMPLEMENT_EXCEPTION(FileException, IOException, "File access error")
LIBWINHTTP_IMPLEMENT_EXCEPTION(FileExistsException, FileException, "File exists")
LIBWINHTTP_IMPLEMENT_EXCEPTION(FileNotFoundException, FileException, "File not found")
LIBWINHTTP_IMPLEMENT_EXCEPTION(PathNotFoundException, FileException, "Path not found")
LIBWINHTTP_IMPLEMENT_EXCEPTION(FileReadOnlyException, FileException, "File is read-only")
LIBWINHTTP_IMPLEMENT_EXCEPTION(FileAccessDeniedException, FileException, "Access to file denied")
LIBWINHTTP_IMPLEMENT_EXCEPTION(CreateFileException, FileException, "Cannot create file")
LIBWINHTTP_IMPLEMENT_EXCEPTION(OpenFileException, FileException, "Cannot open file")
LIBWINHTTP_IMPLEMENT_EXCEPTION(WriteFileException, FileException, "Cannot write file")
LIBWINHTTP_IMPLEMENT_EXCEPTION(ReadFileException, FileException, "Cannot read file")
LIBWINHTTP_IMPLEMENT_EXCEPTION(DirectoryNotEmptyException, FileException, "Directory not empty")
LIBWINHTTP_IMPLEMENT_EXCEPTION(UnknownURISchemeException, RuntimeException, "Unknown URI scheme")
LIBWINHTTP_IMPLEMENT_EXCEPTION(TooManyURIRedirectsException, RuntimeException, "Too many URI redirects")
LIBWINHTTP_IMPLEMENT_EXCEPTION(URISyntaxException, SyntaxException, "Bad URI syntax")

LIBWINHTTP_IMPLEMENT_EXCEPTION(ApplicationException, Exception, "Application exception")
LIBWINHTTP_IMPLEMENT_EXCEPTION(BadCastException, RuntimeException, "Bad cast exception")

LIBWINHTTP_IMPLEMENT_EXCEPTION(NetException, IOException, "Net Exception")
LIBWINHTTP_IMPLEMENT_EXCEPTION(InvalidAddressException, NetException, "Invalid address")
LIBWINHTTP_IMPLEMENT_EXCEPTION(InvalidSocketException, NetException, "Invalid socket")
LIBWINHTTP_IMPLEMENT_EXCEPTION(ServiceNotFoundException, NetException, "Service not found")
LIBWINHTTP_IMPLEMENT_EXCEPTION(ConnectionAbortedException, NetException, "Software caused connection abort")
LIBWINHTTP_IMPLEMENT_EXCEPTION(ConnectionResetException, NetException, "Connection reset by peer")
LIBWINHTTP_IMPLEMENT_EXCEPTION(ConnectionRefusedException, NetException, "Connection refused")
LIBWINHTTP_IMPLEMENT_EXCEPTION(DNSException, NetException, "DNS error")
LIBWINHTTP_IMPLEMENT_EXCEPTION(HostNotFoundException, DNSException, "Host not found")
LIBWINHTTP_IMPLEMENT_EXCEPTION(NoAddressFoundException, DNSException, "No address found")
LIBWINHTTP_IMPLEMENT_EXCEPTION(InterfaceNotFoundException, NetException, "Interface not found")
LIBWINHTTP_IMPLEMENT_EXCEPTION(NoMessageException, NetException, "No message received")
LIBWINHTTP_IMPLEMENT_EXCEPTION(MessageException, NetException, "Malformed message")
LIBWINHTTP_IMPLEMENT_EXCEPTION(MultipartException, MessageException, "Malformed multipart message")
LIBWINHTTP_IMPLEMENT_EXCEPTION(HTTPException, NetException, "HTTP Exception")
LIBWINHTTP_IMPLEMENT_EXCEPTION(NotAuthenticatedException, HTTPException, "No authentication information found")
LIBWINHTTP_IMPLEMENT_EXCEPTION(UnsupportedRedirectException, HTTPException, "Unsupported HTTP redirect (protocol change)")
LIBWINHTTP_IMPLEMENT_EXCEPTION(FTPException, NetException, "FTP Exception")
LIBWINHTTP_IMPLEMENT_EXCEPTION(SMTPException, NetException, "SMTP Exception")
LIBWINHTTP_IMPLEMENT_EXCEPTION(POP3Exception, NetException, "POP3 Exception")
LIBWINHTTP_IMPLEMENT_EXCEPTION(ICMPException, NetException, "ICMP Exception")
LIBWINHTTP_IMPLEMENT_EXCEPTION(ICMPFragmentationException, NetException, "ICMP Fragmentation needed")
LIBWINHTTP_IMPLEMENT_EXCEPTION(NTPException, NetException, "NTP Exception")
LIBWINHTTP_IMPLEMENT_EXCEPTION(HTMLFormException, NetException, "HTML Form Exception")
LIBWINHTTP_IMPLEMENT_EXCEPTION(WebSocketException, NetException, "WebSocket Exception")
LIBWINHTTP_IMPLEMENT_EXCEPTION(UnsupportedFamilyException, NetException, "Unknown or unsupported socket family")
LIBWINHTTP_IMPLEMENT_EXCEPTION(AddressFamilyMismatchException, NetException, "Address family mismatch")
LIBWINHTTP_IMPLEMENT_EXCEPTION(ContentTypeException, NetException, "Invalid Content-Type")

} 
