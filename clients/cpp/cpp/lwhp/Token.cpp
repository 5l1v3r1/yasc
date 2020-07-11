
#include "Token.h"
#include "Exception.h"
#include "Ascii.h"

#include <string>

namespace libwinhttp {


Token::Token()
{
}


Token::~Token()
{
}


bool Token::start(char c, std::istream& istr)
{
	_value = c;
	return false;
}


void Token::finish(std::istream& istr)
{
}


Token::Class Token::tokenClass() const
{
	return INVALID_TOKEN;
}

		
std::string Token::asString() const
{
	return _value;
}


#if defined(_HAVE_INT64)
Int64 Token::asInteger64() const
{
	try {
		return std::stoll(_value);
	}
	catch (std::invalid_argument const&) {
		throw SyntaxException("Not a valid integer", _value);
	}
	catch (std::out_of_range const&) {
		throw SyntaxException("Not a valid integer", _value);
	}
}


UInt64 Token::asUnsignedInteger64() const
{
	try {
		return std::stoull(_value);
	}
	catch (std::invalid_argument const&) {
		throw SyntaxException("Not a valid integer", _value);
	}
	catch (std::out_of_range const&) {
		throw SyntaxException("Not a valid integer", _value);
	}
}
#endif


int Token::asInteger() const
{
	try	{
		return std::stoi(_value);
	}
	catch (std::invalid_argument const&)	{
		throw SyntaxException("Not a valid integer", _value);
	}
	catch (std::out_of_range const&)	{
		throw SyntaxException("Not a valid integer", _value);
	}
}


unsigned Token::asUnsignedInteger() const
{
	try {
		return std::stoul(_value);
	}
	catch (std::invalid_argument const&) {
		throw SyntaxException("Not a valid unsigned integer", _value);
	}
	catch (std::out_of_range const&) {
		throw SyntaxException("Not a valid unsigned integer", _value);
	}
}


double Token::asFloat() const
{
	try {
		return std::stof(_value);
	}
	catch (std::invalid_argument const&) {
		throw SyntaxException("Not a valid floating point number", _value);
	}
	catch (std::out_of_range const&) {
		throw SyntaxException("Not a valid floating point number", _value);
	}
}


char Token::asChar() const
{
	return _value.empty() ? 0 : _value[0];
}


InvalidToken::InvalidToken()
{
}


InvalidToken::~InvalidToken()
{
}


Token::Class InvalidToken::tokenClass() const
{
	return INVALID_TOKEN;
}


EOFToken::EOFToken()
{
}


EOFToken::~EOFToken()
{
}


Token::Class EOFToken::tokenClass() const
{
	return EOF_TOKEN;
}


WhitespaceToken::WhitespaceToken()
{
}


WhitespaceToken::~WhitespaceToken()
{
}


Token::Class WhitespaceToken::tokenClass() const
{
	return WHITESPACE_TOKEN;
}


bool WhitespaceToken::start(char c, std::istream& istr)
{
	if (Ascii::isSpace(c))
	{
		_value = c;
		return true;
	}
	return false;
}


void WhitespaceToken::finish(std::istream& istr)
{
	int c = istr.peek();
	while (Ascii::isSpace(c))
	{
		istr.get();
		_value += (char) c;
		c = istr.peek();
	}
}


} 
