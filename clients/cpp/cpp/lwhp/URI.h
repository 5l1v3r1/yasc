#pragma once

#include "Path.h"

#include <vector>
#include <string>


namespace libwinhttp {

class URI
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
{
public:
	using QueryParameters = std::vector<std::pair<std::string, std::string>>;

	URI();
		

	explicit URI(const std::string& uri);
		
		
		
	explicit URI(const char* uri);
		
		
		
	URI(const std::string& scheme, const std::string& pathEtc);
		
		
	URI(const std::string& scheme, const std::string& authority, const std::string& pathEtc);
		

	URI(const std::string& scheme, const std::string& authority, const std::string& path, const std::string& query);
		

	URI(const std::string& scheme, const std::string& authority, const std::string& path, const std::string& query, const std::string& fragment);
		

	URI(const URI& uri);
		

	URI(URI&& uri) noexcept;
		
		
	URI(const URI& baseURI, const std::string& relativeURI);
		
		

	explicit URI(const Path& path);
		
		
		
		

	~URI();
		
	
	URI& operator = (const URI& uri);
		

	URI& operator = (URI&& uri) noexcept;
		

	URI& operator = (const std::string& uri);
		
		

	URI& operator = (const char* uri);
		
		
		
	void swap(URI& uri);
		
		
	void clear();
		
	
	std::string toString() const;
		
		
		
		
	
	const std::string& getScheme() const;
		
		
	void setScheme(const std::string& scheme);
		
		
		
		
		
		
	const std::string& getUserInfo() const;
		
		
	void setUserInfo(const std::string& userInfo);
		
		
	const std::string& getHost() const;
		
		
	void setHost(const std::string& host);
		
		
	unsigned short getPort() const;
		
		
		
		
		
		
		
	void setPort(unsigned short port);
		
		
	std::string getAuthority() const;
		
		
		
		
		
		
		
	void setAuthority(const std::string& authority);
		
		
		
	const std::string& getPath() const;
		
		
	void setPath(const std::string& path);
		
	
	std::string getQuery() const;
		
		
		
		
		
		
		
		
		
	void setQuery(const std::string& query);	
		
		
		
		
		
		
		
		
		
		
		
		
		
		

	void addQueryParameter(const std::string& param, const std::string& val = "");
		
		
		
		
		

	const std::string& getRawQuery() const;
		
		
		
	void setRawQuery(const std::string& query);	
		
		
		
	
	QueryParameters getQueryParameters() const;
		
		

	void setQueryParameters(const QueryParameters& params);
		
		
		
		

	const std::string& getFragment() const;
		
		
	void setFragment(const std::string& fragment);
		
		
	void setPathEtc(const std::string& pathEtc);
		
		
	std::string getPathEtc() const;
		
		
	std::string getPathAndQuery() const;
		
		
	void resolve(const std::string& relativeURI);
		
		

	void resolve(const URI& relativeURI);
		
		

	bool isRelative() const;
		
		
		
		
		
	
	bool empty() const;
		
		
	bool operator == (const URI& uri) const;
		
		
		
		

	bool operator == (const std::string& uri) const;
		
		

	bool operator != (const URI& uri) const;
		

	bool operator != (const std::string& uri) const;
		
		
		
	void normalize();
		
		
		
		
		
	
	void getPathSegments(std::vector<std::string>& segments);
		
		
	
	static void encode(const std::string& str, const std::string& reserved, std::string& encodedStr);
		
		
		
	static void decode(const std::string& str, std::string& decodedStr, bool plusAsSpace = false);
		
		
		
		
		
		

protected:
	bool equals(const URI& uri) const;
		
	
	bool isWellKnownPort() const;
		
		
		
	unsigned short getWellKnownPort() const;
		
		
		
	void parse(const std::string& uri);
		
		

	void parseAuthority(std::string::const_iterator& it, const std::string::const_iterator& end);
		

	void parseHostAndPort(std::string::const_iterator& it, const std::string::const_iterator& end);
		

	void parsePath(std::string::const_iterator& it, const std::string::const_iterator& end);
		

	void parsePathEtc(std::string::const_iterator& it, const std::string::const_iterator& end);
		

	void parseQuery(std::string::const_iterator& it, const std::string::const_iterator& end);
		

	void parseFragment(std::string::const_iterator& it, const std::string::const_iterator& end);
		

	void mergePath(const std::string& path);
		

	void removeDotSegments(bool removeLeading = true);
		

	static void getPathSegments(const std::string& path, std::vector<std::string>& segments);
		
		

	void buildPath(const std::vector<std::string>& segments, bool leadingSlash, bool trailingSlash);
		

	static const std::string RESERVED_PATH;
	static const std::string RESERVED_QUERY;
	static const std::string RESERVED_QUERY_PARAM;
	static const std::string RESERVED_FRAGMENT;
	static const std::string ILLEGAL;
	
private:
	std::string    _scheme;
	std::string    _userInfo;
	std::string    _host;
	unsigned short _port;
	std::string    _path;
	std::string    _query;
	std::string    _fragment;
};





inline const std::string& URI::getScheme() const
{
	return _scheme;
}
	

inline const std::string& URI::getUserInfo() const
{
	return _userInfo;
}

	
inline const std::string& URI::getHost() const
{
	return _host;
}

	
inline const std::string& URI::getPath() const
{
	return _path;
}

	
inline const std::string& URI::getRawQuery() const
{
	return _query;
}

	
inline const std::string& URI::getFragment() const
{
	return _fragment;
}


inline void swap(URI& u1, URI& u2)
{
	u1.swap(u2);
}

	
} 

