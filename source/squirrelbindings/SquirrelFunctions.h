#pragma once
#include <sqrat.h>
HSQUIRRELVM OpenSquirrelVM();
void CloseSquirrelVM();

void sqPrint(SQChar * text);