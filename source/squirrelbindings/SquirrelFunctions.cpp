
#include "Globals.h"





#ifdef USE_SQUIRREL





#include "SquirrelFunctions.h"
#include "SquirrelBindings.h"





static HSQUIRRELVM squirrelvm = NULL;





SQInteger runtimeErrorHandler(HSQUIRRELVM a_VM)
{
	const SQChar *sErr = 0;
	if(sq_gettop(a_VM) >= 1)
	{
		if(SQ_SUCCEEDED(sq_getstring(a_VM, 2, &sErr)))
		{
			LOGERROR("Squirrel Error: %s", sErr);
		}
		else
		{
			LOGERROR("Squirrel Error: Unknown Error");
		}
	}
	return 0;
}

void compilerErrorHandler(HSQUIRRELVM v,
	const SQChar* a_Desc,
	const SQChar* a_Source,
	SQInteger a_Line,
	SQInteger a_Column)
{
	
	LOGERROR("Squirrel Error: %s (%d:%d) %s", a_Source, a_Line, a_Column, a_Desc);
}

HSQUIRRELVM OpenSquirrelVM()
{
	if(!squirrelvm)
	{
		squirrelvm = sq_open(1024);
		Sqrat::DefaultVM::Set(squirrelvm);
		
		sq_newclosure(squirrelvm, runtimeErrorHandler, 0);
		sq_seterrorhandler(squirrelvm);
		
		sq_setcompilererrorhandler(squirrelvm, compilerErrorHandler);
		
		BindSquirrel(squirrelvm);
	}
	return squirrelvm;
}

void CloseSquirrelVM()
{
	if(squirrelvm)
	{
		sq_close(squirrelvm);
		squirrelvm = NULL;
	}
}


void sqPrint(SQChar * text)
{
	LOGINFO("%s", text);
}





#endif  // USE_SQUIRREL




