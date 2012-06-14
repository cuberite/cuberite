#pragma once

#include "cMCLogger.h"
#include <time.h>
// tolua_begin

unsigned int GetTime()
{
	return (unsigned int)time(0);
}

std::string GetChar( std::string & a_Str, unsigned int a_Idx )
{
	return std::string(1, a_Str[ a_Idx ]);
}

// tolua_end
