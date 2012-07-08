#pragma once


#define USE_SQUIRREL 1

#if USE_SQUIRREL

#include <squirrel.h>
#include <sqrat.h>

void BindSquirrel(HSQUIRRELVM vm);

#endif
