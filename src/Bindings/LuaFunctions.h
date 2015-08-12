#pragma once

#include "Logger.h"
#include <time.h>
#include <chrono>
// tolua_begin

inline unsigned int GetTime()
{
	// NB: For caveats, please see http://stackoverflow.com/a/14505248
	return static_cast<unsigned int>(std::chrono::seconds(time(0)).count());
}

inline std::string GetChar( std::string & a_Str, unsigned int a_Idx)
{
	return std::string(1, a_Str[ a_Idx ]);
}

// tolua_end
