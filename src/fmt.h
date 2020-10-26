#pragma once

#ifdef __clang__
	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-Wsign-conversion"
	#pragma clang diagnostic ignored "-Wsigned-enum-bitfield"
#endif

#include <fmt/format.h>
#include <fmt/printf.h>

#ifdef __clang__
	#pragma clang diagnostic pop
#endif

