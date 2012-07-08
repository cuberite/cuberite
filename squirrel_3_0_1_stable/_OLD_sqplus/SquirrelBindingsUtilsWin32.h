#ifndef SQUIRREL_BINDINGS_UTILS_WIN32_H
#define SQUIRREL_BINDINGS_UTILS_WIN32_H

#ifndef _INC_WINDOWS
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#endif
#ifndef __IUnknown_INTERFACE_DEFINED__
#include <unknwn.h>
#endif

#ifndef SQUIRREL_BINDINGS_UTILS_H
#include "SquirrelBindingsUtils.h"
#endif

#define _DECLARE_REFCOUNTED_NEW(cppclass,classname) \
	SquirrelObject new_##classname(cppclass *ptr) { \
		if(CreateRefCountedInstance(SquirrelVM::GetVMPtr(),_SC(#classname),ptr)) { \
			HSQOBJECT o; \
			sq_getstackobj(SquirrelVM::GetVMPtr(),-1,&o); \
			SquirrelObject tmp = o; \
			sq_pop(SquirrelVM::GetVMPtr(),1); \
			return tmp; \
		} \
		return SquirrelObject() ; \
	}

#define _RETURN_REFCOUNTED_INSTANCE(classname,ptr) \
	if(!CreateRefCountedInstance(SquirrelVM::GetVMPtr(),_SC(#classname),ptr)) { \
		return sa.ThrowError(_SC("cannot create the class instance")); \
	} \
	return 1;

BOOL CreateRefCountedInstance(HSQUIRRELVM v,const SQChar *classname,IUnknown *pRC);
BOOL CreateRefCountedInstanceChached(HSQUIRRELVM v,const SQChar *classname,IUnknown *pRC);
int refcounted_release_hook(SQUserPointer p, int size);
int construct_RefCounted(IUnknown *p);

#endif // SQUIRREL_BINDINGS_UTILS_WIN32_H

