
#pragma once

#ifdef USE_SQUIRREL




#include <sqrat.h>





HSQUIRRELVM OpenSquirrelVM();
void CloseSquirrelVM();

void sqPrint(SQChar * text);





#endif  // USE_SQUIRREL




