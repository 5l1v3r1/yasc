#include "ClientSession.h"
#include "WinHttpRequest.h"
#include "Exceptions.h"
#include "String.h"
#include "StringTokenizer.h"

#include <fstream>
#include <sstream>

using std::string;
using std::wstring;
using libwinhttp::WinHttpRequest;
using std::stringstream;


namespace libwinhttp {

	ClientSession::ClientSession(string useragent) : closed_(true) {
		wstring _useragent = utf8_to_wstr(useragent);

		
		session = pWinHttpOpen(
			_useragent.c_str(),
			WINHTTP_ACCESS_TYPE_AUTOMATIC_PROXY,
			WINHTTP_NO_PROXY_NAME,
			WINHTTP_NO_PROXY_BYPASS,
			WINHTTP_FLAG_ASYNC
		);

		if (session != NULL) {
			closed_ = false;
		}
	};

	ClientSession::~ClientSession() {
		close();
	}




	
	bool ClientSession::sendRequest(WinHttpRequest& req) {
		if (!session || closed_) {
			throw LibWinHttpError("Session is closed!");
		};

		auto connect = pWinHttpConnect(
			session,
			req.requestHost,
			req.requestPort,
			0);

		if (!connect) {
			throw LibWinHttpError("Session is closed!");
		}

		if (session == NULL)
		{
			session = pWinHttpOpen(utf8_to_wstr(useragent).c_str(),
				WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
				WINHTTP_NO_PROXY_NAME,
				WINHTTP_NO_PROXY_BYPASS,
				0);
			if (session == NULL)
			{
				
				throw LibWinHttpError("Could not Connect to host");
			}
		}

		pWinHttpSetTimeouts(session,
			req.resolveTimeout,
			req.connectTimeout,
			req.sendTimeout,
			req.receiveTimeout);

		
		bool result = _request(req);
		return result;
	};

	
	bool ClientSession::_request(WinHttpRequest& req)
	{
		wstring verb = req.requestVerb;
		if (!req.forcedCharset.empty())
			_forcedCharset = utf8_to_wstr(req.forcedCharset);
		bool bRetVal = true;

		
		if (_hConnect)
			this->reset();

		_hConnect = pWinHttpConnect(session, req.requestHost, req.requestPort, 0);
		if (!_hConnect)
		{
			
			return false;
		}

		DWORD dwOpenRequestFlag = (req.requestScheme == INTERNET_SCHEME_HTTPS) ? WINHTTP_FLAG_SECURE : 0;
		_hRequest = pWinHttpOpenRequest(
			_hConnect,
			verb.c_str(),
			req.requestURL,
			NULL,
			WINHTTP_NO_REFERER,
			WINHTTP_DEFAULT_ACCEPT_TYPES,
			dwOpenRequestFlag);
		if (!_hRequest)
		{
			
			return false;
		}

		
		
		if (!req.requireValidSsl && req.requestScheme == INTERNET_SCHEME_HTTPS)
		{
			DWORD options = SECURITY_FLAG_IGNORE_CERT_CN_INVALID
				| SECURITY_FLAG_IGNORE_CERT_DATE_INVALID
				| SECURITY_FLAG_IGNORE_UNKNOWN_CA;
			pWinHttpSetOption(_hRequest,
				WINHTTP_OPTION_SECURITY_FLAGS,
				(LPVOID)&options,
				sizeof(DWORD));
		}

		bool bGetReponseSucceed = false;
		unsigned int iRetryTimes = 0;

		if (!req.additionalRequestHeaders.empty())
		{
			if (!pWinHttpAddRequestHeaders(
				_hRequest,
				req.additionalRequestHeaders.c_str(),
				(DWORD)req.additionalRequestHeaders.size(),
				WINHTTP_ADDREQ_FLAG_COALESCE_WITH_SEMICOLON))
			{
				req.lastError = ::GetLastError();
				return false;
			}
		}

		if (!req.additionalRequestCookies.empty())
		{
			
			stringstream ss_additionalRequestCookies;
			bool first = true;
			ss_additionalRequestCookies << "Cookie: ";
			for (auto& key : req.additionalRequestCookies) {
				if (!first)
					ss_additionalRequestCookies << "; ";
				ss_additionalRequestCookies << key.first << "=" << key.second;
				first = false;
			}
			wstring cookies = utf8_to_wstr(ss_additionalRequestCookies.str());
			if (!pWinHttpAddRequestHeaders(
				_hRequest,
				cookies.c_str(),
				(DWORD)cookies.size(),
				WINHTTP_ADDREQ_FLAG_COALESCE_WITH_SEMICOLON))
			{
				req.lastError = ::GetLastError();
				return false;
			}
		}
		
		if (!req.proxy.empty())
		{
			WINHTTP_PROXY_INFO proxyInfo;
			memset(&proxyInfo, 0, sizeof(proxyInfo));
			proxyInfo.dwAccessType = WINHTTP_ACCESS_TYPE_NAMED_PROXY;
			
			
			
			wchar_t szProxy[MAX_PATH] = L"";
			wcscpy_s(szProxy, MAX_PATH, utf8_to_wstr(req.proxy).c_str());
			proxyInfo.lpszProxy = szProxy;

			if (!pWinHttpSetOption(_hRequest, WINHTTP_OPTION_PROXY, &proxyInfo, sizeof(proxyInfo)))
			{
				req.lastError = ::GetLastError();
				return false;
			}

			if (req.proxyUsername.size() > 0)
			{
				if (!pWinHttpSetOption(
					_hRequest,
					WINHTTP_OPTION_PROXY_USERNAME,
					(LPVOID)req.proxyUsername.c_str(),
					(DWORD)req.proxyUsername.size() * sizeof(wchar_t)))
				{
					req.lastError = ::GetLastError();
					return false;
				}
				if (req.proxyPassword.size() > 0)
				{
					if (!pWinHttpSetOption(
						_hRequest,
						WINHTTP_OPTION_PROXY_PASSWORD,
						(LPVOID)req.proxyPassword.c_str(),
						(DWORD)req.proxyPassword.size() * sizeof(wchar_t)))
					{
						req.lastError = ::GetLastError();
						return false;
					}
				}
			}
		}

		if (req.disableAutoRedirect)
		{
			DWORD dwDisableFeature = WINHTTP_DISABLE_REDIRECTS;
			if (!pWinHttpSetOption(_hRequest, WINHTTP_OPTION_DISABLE_FEATURE, &dwDisableFeature, sizeof(dwDisableFeature)))
			{
				req.lastError = ::GetLastError();
				return false;
			}
		}
		
		bool bSendRequestSucceed = false;
		if (pWinHttpSendRequest(_hRequest,
			WINHTTP_NO_ADDITIONAL_HEADERS,
			0,
			WINHTTP_NO_REQUEST_DATA,
			0,
			0,
			NULL))
		{
			bSendRequestSucceed = true;
		}
		else
		{
			
			WINHTTP_CURRENT_USER_IE_PROXY_CONFIG proxyConfig;
			memset(&proxyConfig, 0, sizeof(proxyConfig));
			if (pWinHttpGetIEProxyConfigForCurrentUser(&proxyConfig))
			{
				if (proxyConfig.lpszAutoConfigUrl != NULL)
				{
					WINHTTP_AUTOPROXY_OPTIONS autoProxyOptions;
					memset(&autoProxyOptions, 0, sizeof(autoProxyOptions));
					autoProxyOptions.dwFlags = WINHTTP_AUTOPROXY_AUTO_DETECT | WINHTTP_AUTOPROXY_CONFIG_URL;
					autoProxyOptions.dwAutoDetectFlags = WINHTTP_AUTO_DETECT_TYPE_DHCP;
					autoProxyOptions.lpszAutoConfigUrl = proxyConfig.lpszAutoConfigUrl;
					autoProxyOptions.fAutoLogonIfChallenged = TRUE;
					autoProxyOptions.dwReserved = 0;
					autoProxyOptions.lpvReserved = NULL;

					WINHTTP_PROXY_INFO proxyInfo;
					memset(&proxyInfo, 0, sizeof(proxyInfo));

					if (pWinHttpGetProxyForUrl(session, req.requestURL, &autoProxyOptions, &proxyInfo))
					{
						if (pWinHttpSetOption(_hRequest, WINHTTP_OPTION_PROXY, &proxyInfo, sizeof(proxyInfo)))
						{
							if (pWinHttpSendRequest(_hRequest,
								WINHTTP_NO_ADDITIONAL_HEADERS,
								0,
								WINHTTP_NO_REQUEST_DATA,
								0,
								0,
								NULL))
							{
								bSendRequestSucceed = true;
							}
						}
						if (proxyInfo.lpszProxy != NULL)
						{
							::GlobalFree(proxyInfo.lpszProxy);
						}
						if (proxyInfo.lpszProxyBypass != NULL)
						{
							::GlobalFree(proxyInfo.lpszProxyBypass);
						}
					}
					else
					{
						req.lastError = ::GetLastError();
					}
				}
				else if (proxyConfig.lpszProxy != NULL)
				{
					WINHTTP_PROXY_INFO proxyInfo;
					memset(&proxyInfo, 0, sizeof(proxyInfo));
					proxyInfo.dwAccessType = WINHTTP_ACCESS_TYPE_NAMED_PROXY;
					wchar_t szProxy[MAX_PATH] = L"";
					wcscpy_s(szProxy, MAX_PATH, proxyConfig.lpszProxy);
					proxyInfo.lpszProxy = szProxy;

					if (proxyConfig.lpszProxyBypass != NULL)
					{
						wchar_t szProxyBypass[MAX_PATH] = L"";
						wcscpy_s(szProxyBypass, MAX_PATH, proxyConfig.lpszProxyBypass);
						proxyInfo.lpszProxyBypass = szProxyBypass;
					}

					if (!pWinHttpSetOption(_hRequest, WINHTTP_OPTION_PROXY, &proxyInfo, sizeof(proxyInfo)))
					{
						req.lastError = ::GetLastError();
					}
				}

				if (proxyConfig.lpszAutoConfigUrl != NULL)
				{
					::GlobalFree(proxyConfig.lpszAutoConfigUrl);
				}
				if (proxyConfig.lpszProxy != NULL)
				{
					::GlobalFree(proxyConfig.lpszProxy);
				}
				if (proxyConfig.lpszProxyBypass != NULL)
				{
					::GlobalFree(proxyConfig.lpszProxyBypass);
				}

				if (req.lastError)
					return false;
			}
			else
			{
				req.lastError = ::GetLastError();
				return false;
			}
		}

		
		if (bSendRequestSucceed)
		{
			bRetVal = true;
			
			if (req.data_size > 0)
			{
				DWORD dwWritten = 0;
				if (!pWinHttpWriteData(_hRequest,
					req.data,
					req.data_size,
					&dwWritten))
				{
					req.lastError = ::GetLastError();
					return false;
				}
				
				
				
			}
		}

		return bRetVal;
	};

	
	bool ClientSession::receiveResponse(WinHttpResponse& response) {
		
		
		
		if (pWinHttpReceiveResponse(_hRequest, NULL))
		{
			DWORD dwSize = 0;
			BOOL bResult = FALSE;
			bResult = pWinHttpQueryHeaders(_hRequest,
				WINHTTP_QUERY_RAW_HEADERS_CRLF,
				WINHTTP_HEADER_NAME_BY_INDEX,
				NULL,
				&dwSize,
				WINHTTP_NO_HEADER_INDEX);
			if (bResult || (!bResult && (::GetLastError() == ERROR_INSUFFICIENT_BUFFER)))
			{
				wchar_t* szHeader = new wchar_t[dwSize];
				if (szHeader != NULL)
				{
					memset(szHeader, 0, dwSize * sizeof(wchar_t));
					if (pWinHttpQueryHeaders(_hRequest,
						WINHTTP_QUERY_RAW_HEADERS_CRLF,
						WINHTTP_HEADER_NAME_BY_INDEX,
						szHeader,
						&dwSize,
						WINHTTP_NO_HEADER_INDEX))
					{
						wstring w_responseCharset{ _forcedCharset };

						string responseHeader = wstr_to_utf8(szHeader);
						StringTokenizer tokens{ responseHeader, "\r\n" };

						
						for (auto& key : tokens) {
							size_t colon = key.find(":");
							if (colon != std::string::npos && colon+2 < key.length()) {
								string header_name = key.substr(0, colon);
								string header_value = key.substr(colon + 2, key.length() - (colon + 2));
								if (icompare(header_name, "set-cookie") == 0) {
									string key{ header_value.substr(0, header_value.find("=")) };
									response.responseCookies[key] = header_value;
								}
								else {
									response.responseHeaders[header_name] = header_value;
								}
							}
						}

						if (response.responseHeaders.find("Content-Type") != response.responseHeaders.end()) {
							size_t charset = response.responseHeaders["Content-Type"].find("charset=");
							if (charset != string::npos)
								response.responseCharset = response.responseHeaders["Content-Type"].substr(charset + 8);
						}

						if (response.responseHeaders.find("Content-Length") != response.responseHeaders.end()) {
							response.contentLength = (unsigned long)stoul(response.responseHeaders["Content-Length"]);
						}

						if (response.responseHeaders.find("Location") != response.responseHeaders.end()) {
							response.location = response.responseHeaders["Location"];
						}
					}
					delete[] szHeader;
				}
			}

			dwSize = 0;
			bResult = pWinHttpQueryHeaders(_hRequest,
				WINHTTP_QUERY_STATUS_CODE,
				WINHTTP_HEADER_NAME_BY_INDEX,
				NULL,
				&dwSize,
				WINHTTP_NO_HEADER_INDEX);
			if (bResult || (!bResult && (::GetLastError() == ERROR_INSUFFICIENT_BUFFER)))
			{
				wchar_t* szStatusCode = new wchar_t[dwSize];
				if (szStatusCode != NULL)
				{
					memset(szStatusCode, 0, dwSize * sizeof(wchar_t));
					if (pWinHttpQueryHeaders(_hRequest,
						WINHTTP_QUERY_STATUS_CODE,
						WINHTTP_HEADER_NAME_BY_INDEX,
						szStatusCode,
						&dwSize,
						WINHTTP_NO_HEADER_INDEX))
					{
						response.statusCode = static_cast<unsigned int>(_wtoi(szStatusCode));
					}
					delete[] szStatusCode;
				}
			}

			
			
			
			
			
			if (response.contentLength > DW_MAXCONTENTLENGTH)
				throw libwinhttp::MessageException("The HTTP response size is too large");

			_pResponse.resize(0);

			do
			{
				dwSize = 0;
				if (pWinHttpQueryDataAvailable(_hRequest, &dwSize))
				{
					if (dwSize > 0) {
						try
						{
							_readRequest(dwSize);
						}
						catch (const std::exception & e)
						{
							throw e;
						}
					}
				} 
				else
				{
					
					DWORD reason = ::GetLastError();
					switch (reason)
					{
					case ERROR_WINHTTP_CONNECTION_ERROR:
					case ERROR_WINHTTP_OPERATION_CANCELLED:
					case ERROR_WINHTTP_TIMEOUT:
						throw libwinhttp::ConnectionResetException("Unable to read data", reason);
						break;
					default:
						throw libwinhttp::HTTPException("Unknown exception", reason);
						break;
					}
				}
			} while (dwSize > 0);
			
			response.responseContent = _pResponse;
		} 
		else
		{
			
			DWORD reason = ::GetLastError();
			switch (reason)
			{
			case ERROR_WINHTTP_NAME_NOT_RESOLVED:
			case ERROR_WINHTTP_CANNOT_CONNECT:
				throw libwinhttp::ConnectionRefusedException("Could not connect to server while receiving response", reason);
				break;
			case ERROR_WINHTTP_CONNECTION_ERROR:
			case ERROR_WINHTTP_OPERATION_CANCELLED:
			case ERROR_WINHTTP_TIMEOUT:
				throw libwinhttp::ConnectionResetException("Could not receive response", reason);
				break;
			case ERROR_WINHTTP_CHUNKED_ENCODING_HEADER_SIZE_OVERFLOW:
				throw libwinhttp::PoolOverflowException("Overflow while processing chunked encoding", reason);
				break;
			case ERROR_WINHTTP_HEADER_COUNT_EXCEEDED:
			case ERROR_WINHTTP_HEADER_SIZE_OVERFLOW:
				throw libwinhttp::PoolOverflowException("Too many headers in response", reason);
			case ERROR_WINHTTP_CLIENT_AUTH_CERT_NEEDED:
			case ERROR_WINHTTP_LOGIN_FAILURE:
				throw libwinhttp::NotAuthenticatedException("Auth failed/Server requesting authentication", reason);
				break;
			case ERROR_WINHTTP_INVALID_SERVER_RESPONSE:
				throw libwinhttp::HTTPException("Invalid server response", reason);
			case ERROR_WINHTTP_SECURE_FAILURE:
				throw libwinhttp::HTTPException("Unable to secure connection", reason);
			default:
				throw libwinhttp::HTTPException("Unknown exception while receiving response", reason);
				break;
			}
		}
		pWinHttpCloseHandle(_hRequest);
		pWinHttpCloseHandle(_hConnect);
		return true;
	};

	void ClientSession::reset() {
		if (_hRequest)
			pWinHttpCloseHandle(_hRequest);
		if (_hConnect)
			pWinHttpCloseHandle(_hConnect);
		_hConnect = NULL;
		_hRequest = NULL;
	}

	DWORD ClientSession::_readRequest(DWORD dwSize)
	{
		DWORD dwRead = 0;
		BYTE* tmpBuffer = new BYTE[dwSize + 1ul];
		memset(tmpBuffer, 0, dwSize + 1ul);
		if (!pWinHttpReadData(_hRequest,
			tmpBuffer,
			dwSize,
			&dwRead))
		{
			
			DWORD reason = ::GetLastError();
			switch (reason)
			{
			case ERROR_WINHTTP_CONNECTION_ERROR:
			case ERROR_WINHTTP_OPERATION_CANCELLED:
			case ERROR_WINHTTP_TIMEOUT:
			case ERROR_WINHTTP_RESPONSE_DRAIN_OVERFLOW:
				throw libwinhttp::ConnectionResetException("Unable to read data", reason);
				break;
			default:
				throw libwinhttp::HTTPException("Unknown exception", reason);
				break;
			}
		}
		_pResponse.append(tmpBuffer, dwRead);
		delete[] tmpBuffer;
		return dwRead;
	};


}