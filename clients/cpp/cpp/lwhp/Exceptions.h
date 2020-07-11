#pragma once

#include <string>
#include <stdexcept>

using std::string;
using std::runtime_error;

namespace libwinhttp {
    class LibWinHttpError : public runtime_error {
    public:
        LibWinHttpError() : runtime_error("Unknown Error") {};
        LibWinHttpError(const string& message) : runtime_error(message) {};
        LibWinHttpError(string message, string param) : runtime_error(message + ": " + param) {};
         

     
    };

     
}