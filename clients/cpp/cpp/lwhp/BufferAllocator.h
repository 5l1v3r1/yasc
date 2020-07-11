#pragma once

#include "libwinhttp.h"
#include <ios>
#include <cstddef>


namespace libwinhttp {


template <typename ch>
class BufferAllocator
	
	
{
public:
	typedef ch char_type;

	static char_type* allocate(std::streamsize size)
	{
		return new char_type[static_cast<std::size_t>(size)];
	}
	
	static void deallocate(char_type* ptr, std::streamsize  ) noexcept
	{
		delete [] ptr;
	}
};


} 