#pragma once

#include "Types.h"

#include <istream>


namespace libwinhttp {


class Token
	
	
{
public:
	enum Class
	{
		IDENTIFIER_TOKEN,
		KEYWORD_TOKEN,
		SEPARATOR_TOKEN,
		OPERATOR_TOKEN,
		STRING_LITERAL_TOKEN,
		CHAR_LITERAL_TOKEN,
		INTEGER_LITERAL_TOKEN,
		LONG_INTEGER_LITERAL_TOKEN,
		FLOAT_LITERAL_TOKEN,
		DOUBLE_LITERAL_TOKEN,
		COMMENT_TOKEN,
		SPECIAL_COMMENT_TOKEN,
		PREPROCESSOR_TOKEN,
		WHITESPACE_TOKEN,
		EOF_TOKEN,
		INVALID_TOKEN,
		USER_TOKEN
	};
	
	Token();
		

	virtual ~Token();
		
		
	virtual bool start(char c, std::istream& istr);
		
		
		
		
		
		
		
		
		
		
		

	virtual void finish(std::istream& istr);
		
		
		
	virtual Class tokenClass() const;
		
	
	const std::string& tokenString() const;
		
	
	virtual std::string asString() const;
		
	
#if defined(_HAVE_INT64)
	virtual Int64 asInteger64() const;
        

	virtual UInt64 asUnsignedInteger64() const;
        
#endif

	virtual int asInteger() const;
		

	virtual unsigned asUnsignedInteger() const;
		

	virtual double asFloat() const;
		

	virtual char asChar() const;
		

	bool is(Class tokenClass) const;
		

protected:
	std::string _value;
	
private:
	Token(const Token&);
	Token& operator = (const Token&);
};


class InvalidToken: public Token
	
	
	
{
public:
	InvalidToken();
	~InvalidToken();
	Class tokenClass() const;
};


class EOFToken: public Token
	
	
{
public:
	EOFToken();
	~EOFToken();
	Class tokenClass() const;
};


class WhitespaceToken: public Token
	
	
{
public:
	WhitespaceToken();
	~WhitespaceToken();
	Class tokenClass() const;
	bool start(char c, std::istream& istr);
	void finish(std::istream& istr);
};





inline const std::string& Token::tokenString() const
{
	return _value;
}


inline bool Token::is(Token::Class cls) const
{
	return tokenClass() == cls;
}


} 

