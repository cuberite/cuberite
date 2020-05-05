#pragma once

#ifdef __clang__
	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-Wunknown-pragmas"
	#pragma clang diagnostic ignored "-Wunknown-warning-option"
	#pragma clang diagnostic ignored "-Wsigned-enum-bitfield"
	#pragma clang diagnostic ignored "-Wundefined-func-template"
	#pragma clang diagnostic ignored "-Wc++2a-compat"
#endif

#include <fmt/format.h>
#include <fmt/printf.h>

#ifdef __clang__
	#pragma clang diagnostic pop
#endif

