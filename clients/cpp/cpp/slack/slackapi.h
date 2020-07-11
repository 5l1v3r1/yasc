#pragma once

#include <string>
#include <fstream>
#include <unordered_map>


using std::unordered_map;
using std::string;
using std::unique_ptr;
using std::ifstream;

namespace slack {
    typedef unordered_map<string, unique_ptr<ifstream>> FileDict;
};
