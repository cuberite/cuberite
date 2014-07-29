#pragma once

#include "../MCLogger.h"
#include <time.h>
// tolua_begin

inline unsigned int GetTime()
{
	return (unsigned int)time(0);
}

inline std::string GetChar( std::string & a_Str, unsigned int a_Idx)
{
	return std::string(1, a_Str[ a_Idx ]);
}

// tolua_end
