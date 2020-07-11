#pragma once

#include "Platform.h"









#if defined(_WIN32) && defined(LIBWINHTTP_DLL)
#if defined(libwinhttp_EXPORTS)
#define libwinhttp_API __declspec(dllexport)
#else
#define libwinhttp_API __declspec(dllimport)
#endif
#endif


#include "Types.h"
#include "BugCheck.h"

#include <unordered_map>
#include <cstring>

using std::string;
using std::wstring;


namespace libwinhttp {
    
    class StringDict : public std::unordered_map<string, string>
    {
    public:
        const string& get(const string& key, const string& def) const {
            if (count(key) == 0)
                return def;
            return at(key);
        }
    };
    typedef std::unordered_map<std::string, std::ifstream*> FileDict;
};