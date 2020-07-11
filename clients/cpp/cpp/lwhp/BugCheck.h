#pragma once

#include <cstdlib> 
#define _assert_dbg(cond)           do { if (!(cond)) std::abort(); } while (0)
#define _assert_msg_dbg(cond, text) do { if (!(cond)) std::abort(); } while (0)
#define _assert(cond)               do { if (!(cond)) std::abort(); } while (0)
#define _assert_msg(cond, text)     do { if (!(cond)) std::abort(); } while (0)
#define _check_ptr(ptr)             do { if (!(ptr)) std::abort(); } while (0)
#define _bugcheck()                 do { std::abort(); } while (0)
#define _bugcheck_msg(msg)          do { std::abort(); } while (0)
