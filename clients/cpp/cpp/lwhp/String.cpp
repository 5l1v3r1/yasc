
#include "String.h"
#include "Exception.h"

#ifdef _OS_FAMILY_WINDOWS
#include "UnWindows.h"
#include <WinNls.h>
#endif


namespace libwinhttp {

	std::wstring utf8_to_wstr(const std::string& utf8) {
		
		
		std::vector<unsigned long> unicode;
		size_t i = 0;
		while (i < utf8.size())
		{
			unsigned long uni;
			size_t todo;
			bool error = false;
			unsigned char ch = utf8[i++];
			if (ch <= 0x7F)
			{
				uni = ch;
				todo = 0;
			}
			else if (ch <= 0xBF)
			{
				throw std::logic_error("not a UTF-8 string");
			}
			else if (ch <= 0xDF)
			{
				uni = ch & 0x1F;
				todo = 1;
			}
			else if (ch <= 0xEF)
			{
				uni = ch & 0x0F;
				todo = 2;
			}
			else if (ch <= 0xF7)
			{
				uni = ch & 0x07;
				todo = 3;
			}
			else
			{
				throw std::logic_error("not a UTF-8 string");
			}
			for (size_t j = 0; j < todo; ++j)
			{
				if (i == utf8.size())
					throw std::logic_error("not a UTF-8 string");
				unsigned char ch = utf8[i++];
				if (ch < 0x80 || ch > 0xBF)
					throw std::logic_error("not a UTF-8 string");
				uni <<= 6;
				uni += ch & 0x3F;
			}
			if (uni >= 0xD800 && uni <= 0xDFFF)
				throw std::logic_error("not a UTF-8 string");
			if (uni > 0x10FFFF)
				throw std::logic_error("not a UTF-8 string");
			unicode.push_back(uni);
		}
		std::wstring utf16;
		for (size_t i = 0; i < unicode.size(); ++i)
		{
			unsigned long uni = unicode[i];
			if (uni <= 0xFFFF)
			{
				utf16 += (wchar_t)uni;
			}
			else
			{
				uni -= 0x10000;
				utf16 += (wchar_t)((uni >> 10) + 0xD800);
				utf16 += (wchar_t)((uni & 0x3FF) + 0xDC00);
			}
		}
		return utf16;
	}
	
	std::string wstr_to_utf8(const std::wstring& wstr) {
		
		
#ifdef _OS_FAMILY_WINDOWS
		std::string retStr;
		if (!wstr.empty())
		{
			int sizeRequired = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
			if (sizeRequired > 0)
			{
				std::vector<char> utf8String(sizeRequired);
				int bytesConverted = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &utf8String[0], static_cast<int>(utf8String.size()), NULL, NULL);
				if (bytesConverted != 0)
					retStr = &utf8String[0];
				else
					throw libwinhttp::DataFormatException("wstr_to_utf8 failed to convert wstring");
			}
		}
		return retStr;
#else
		throw libwinhttp::ApplicationException("wstr_to_utf8 not allowed on Non-Windows");
#endif
	}

} 
