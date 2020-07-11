#include "base_client.h"
#include "../../lwhp/WinHttpRequest.h"
#include "../../lwhp/WinHttpResponse.h"
#include "../../lwhp/NameValueCollection.h"
#include "../../lwhp/Exceptions.h"
#include "../../lwhp/URI.h"
#include "../../lwhp/String.h"
#include "../../utility.h"

#include <vector>
#include <fstream>

namespace slack::web {

	string BaseClient::urljoin(const string& base_url, const string& add_part) {
		
		
		if (base_url[base_url.length() - 1] != '/') {
			return base_url + "/" + add_part;
		}
		return base_url + add_part;
	};

	string BaseClient::_get_url(const string& api_method) {
		return urljoin(base_url, api_method);
	};

	using std::string;
	using libwinhttp::NameValueCollection;
	using libwinhttp::LibWinHttpError;
	using libwinhttp::URI;
	using libwinhttp::WinHttpResponse;
	using libwinhttp::isubstr;

	 
	NameValueCollection BaseClient::_get_headers(bool has_json, bool has_files, NameValueCollection request_specific_headers) {
		NameValueCollection final_headers = NameValueCollection();
		
		final_headers.set("Content-Type", "application/x-www-form-urlencoded;charset=utf-8");

		
		final_headers.insert(request_specific_headers.begin(), request_specific_headers.end());

		
		final_headers.insert(_headers.begin(), _headers.end());

		if (has_json)
			final_headers["Content-Type"] = "application/json;charset=utf-8";

		if (has_files)
			final_headers.erase("Content-Type");

		return final_headers;
	};

	SlackResponse BaseClient::download_url(const std::string& url, const std::string& accept) {
		WinHttpRequest request{ "GET", URI(url) };
		NameValueCollection final_headers = _headers;

		final_headers.add("Accept", accept);
		
		
		request.set_additional_headers(_headers);

		
		
		
		try {
			auto result = _send("GET", url, "", request);
			return result;
		}
		catch (SlackClientError & e) {
			throw e;
		}

		 
	};

	SlackResponse BaseClient::api_call(
		std::string api_method,
		std::string http_verb,
		NameValueCollection files,
		NameValueCollection form_data,
		NameValueCollection params,
		json json_val,
		NameValueCollection headers,
		NameValueCollection auth
	)
	{
		string body;

		bool has_json = json_val.empty() ? false : true;
		bool has_files = files.empty() ? false : true;
		bool has_params = params.empty() ? false : true;
		bool has_form_data = form_data.empty() ? false : true;
		if (has_json && http_verb != "POST") {
			string msg = "Json data can only be submitted as POST requests. GET requests should use the 'params' argument.";
			throw slack::SlackRequestError(msg);
		};

		if ((has_json && (has_params || has_form_data) || (has_params && has_form_data))) {
			string msg = "You cannot use JSON, Query String Parameter, and/or Form Data at the same time. Choose one per request.";
			throw slack::SlackRequestError(msg);
		};

		URI api_url{ this->_get_url(api_method) };

		if (!auth.empty()) {
			throw slack::SlackRequestError("auth is not implemented yet! Use oath token.");
			
			
		};

		WinHttpRequest request{ http_verb, api_url };
		NameValueCollection full_headers = _get_headers(has_json, has_files, headers);

		if (http_verb == "POST") {
			if (has_params) {
				URI uri_build{};
				for (auto& param : params)
					uri_build.addQueryParameter(param.first, param.second);
				body = uri_build.getQuery();
				full_headers["Content-Length"] = std::to_string(body.length());
				
			}
			else if (has_json) {
				body = json_val.dump();
				full_headers["Content-Length"] = std::to_string(body.length());
				
			}
			else if (has_files || has_form_data) {
				 
			}
		}
		else if (http_verb == "GET") {
			full_headers.erase("Content-Type");
			for (auto& param : params)
				api_url.addQueryParameter(param.first, param.second);

			request.setURI(api_url);
		}
		else {
			throw slack::SlackRequestError("Unsupported HTTP Verb: " + http_verb);
		}

		
		request.set_additional_headers(full_headers);


		
		
		
		string api_url_str = api_url.toString();
		try {
			auto result = _send(http_verb, api_url_str, body, request);
			return result;
		}
		catch (SlackClientError & e) {
			throw e;
		}

		 
	};


	 
	SlackResponse BaseClient::_send(const std::string& http_verb, const std::string& api_url, const std::string& body, WinHttpRequest& request) {
		string message;
		bool result{ false };
		
		try {
			request.data = body.c_str();
			request.data_size = (DWORD)body.length();
			
			if (!_request(request)) {
				throw SlackRequestError("Request failed: Last Error:" + request.lastError);
			}
			 
		}
		catch (SlackRequestError & e) {
			
			
			throw SlackClientError(e.what());
		}

		
		WinHttpResponse response{};
		try {
			
			if (!_session->receiveResponse(response)) {
				throw SlackRequestError("Response failed...IDK WHY!");
			}

			
			switch (response.statusCode)
			{
			case 200:
				
				
				return { *this, http_verb, api_url, (int)response.statusCode, response };
			case 401:
				
				message = "Not Authorized! Check authorization token.";
			default:
				if (message == "")
					message = "Unknown Error! Reason: " + response.lastError;

				throw SlackRequestError(message);
			}
		}
		catch (libwinhttp::LibWinHttpError& e) {
			
			
			throw SlackClientError(e.what());
		}
		catch (SlackRequestError & e) {
			
			
			throw SlackClientError(e.what());
		}
	};

	 
	
	bool BaseClient::_request(WinHttpRequest& request) {
		
		try {
			return _session->sendRequest(request);
		}
		catch (libwinhttp::LibWinHttpError & e) {
			_session->reset();
			
			throw SlackRequestError(e.what());
		}
	};
}