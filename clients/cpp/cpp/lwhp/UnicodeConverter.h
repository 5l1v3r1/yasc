#pragma once

namespace libwinhttp {


class UnicodeConverter
	
	
	
	
	
	
{
public:
	 

	template <typename F, typename T>
	static void toUTF32(const F& f, T& t)
	{
		convert(f, t);
	}

	template <typename F, typename T>
	static void toUTF32(const F& f, std::size_t l, T& t)
	{
		convert(f, l, t);
	}

	template <typename F, typename T>
	static void toUTF16(const F& f, T& t)
	{
		convert(f, t);
	}

	template <typename F, typename T>
	static void toUTF16(const F& f, std::size_t l, T& t)
	{
		convert(f, l, t);
	}

	template <typename F, typename T>
	static void toUTF8(const F& f, T& t)
	{
		convert(f, t);
	}

	template <typename F, typename T>
	static void toUTF8(const F& f, std::size_t l, T& t)
	{
		convert(f, l, t);
	}

	template <typename T>
	static T to(const char* pChar)
	{
		T utfStr;
		Poco::UnicodeConverter::convert(pChar, utfStr);
		return utfStr;
	}


	template <typename T>
	static T to(const std::string& str)
	{
		T utfStr;
		Poco::UnicodeConverter::convert(str, utfStr);
		return utfStr;
	}

	template <typename T>
	static std::size_t UTFStrlen(const T* ptr)
		
	{
		if (ptr == 0) return 0;
		const T* p;
		for (p = ptr; *p; ++p);
		return p - ptr;
	}
};


} 


#endif 
