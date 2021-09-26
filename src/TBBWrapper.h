#pragma once

// The new and free macros break tbb
#pragma push_macro("new")
#undef new
#pragma push_macro("free")
#undef free

// Disable warnings caused by TBB headers
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#endif

// TBB headers
#include "oneapi/tbb.h"

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#pragma pop_macro("free")
#pragma pop_macro("new")
