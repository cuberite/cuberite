#pragma once

#include <time.h>
// tolua_begin

inline unsigned int GetTime()
{
	// NB: For caveats, please see https://stackoverflow.com/a/14505248
	return static_cast<unsigned int>(std::chrono::seconds(time(nullptr)).count());
}

// tolua_end
