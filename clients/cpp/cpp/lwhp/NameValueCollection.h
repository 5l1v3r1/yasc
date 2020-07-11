#pragma once

#include "Exception.h"

#include <cstddef>
#include <unordered_map>


using libwinhttp::NotFoundException;

namespace libwinhttp {
	class NameValueCollection
		
		
		
		
		
		
		
	{
	public:
		using HeaderMap = std::unordered_map<std::string, std::string>;
		using Iterator = HeaderMap::iterator;
		using ConstIterator = HeaderMap::const_iterator;

		NameValueCollection();
		

		NameValueCollection(const NameValueCollection& nvc);
		

		NameValueCollection(NameValueCollection&& nvc) noexcept;
		

		virtual ~NameValueCollection();
		

		NameValueCollection& operator = (const NameValueCollection& nvc);
		

		NameValueCollection& operator = (NameValueCollection&& nvc) noexcept;
		

		void swap(NameValueCollection& nvc);
		

		const std::string& operator [] (const std::string& name) const;
		
		
		

		std::string& operator [] (const std::string& name);
		
		
		

		void set(const std::string& name, const std::string& value);
		

		void insert(const ConstIterator& start, const ConstIterator& end);
		

		void add(const std::string& name, const std::string& value);
		

		const std::string& get(const std::string& name) const;
		
		
		

		const std::string& get(const std::string& name, const std::string& defaultValue) const;
		
		

		bool has(const std::string& name) const;
		
		

		ConstIterator find(const std::string& name) const;
		
		

		ConstIterator begin() const;
		
		

		ConstIterator end() const;
		
		

		bool empty() const;
		

		std::size_t size() const;
		
		

		void erase(const std::string& name);
		

		void clear();
		

	private:
		HeaderMap _map;
	};


	
	
	
	inline void swap(NameValueCollection& nvc1, NameValueCollection& nvc2)
	{
		nvc1.swap(nvc2);
	}
}