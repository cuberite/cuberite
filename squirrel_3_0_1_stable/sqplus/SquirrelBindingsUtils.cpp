#include "sqplus.h"

BOOL CreateStaticNamespace(HSQUIRRELVM v,ScriptNamespaceDecl *sn)
{
	int n = 0;
	sq_pushroottable(v);
	sq_pushstring(v,sn->name,-1);
	sq_newtable(v);
	const ScriptClassMemberDecl *members = sn->members;
	const ScriptClassMemberDecl *m = NULL;
	while(members[n].name) {
		m = &members[n];
		sq_pushstring(v,m->name,-1);
		sq_newclosure(v,m->func,0);
		sq_setparamscheck(v,m->params,m->typemask);
		sq_setnativeclosurename(v,-1,m->name);
		sq_createslot(v,-3);
		n++;
	}
	const ScriptConstantDecl *consts = sn->constants;
	const ScriptConstantDecl *c = NULL;
	n = 0;
	while(consts[n].name) {
		c = &consts[n];
		sq_pushstring(v,c->name,-1);
		switch(c->type) {
		case OT_STRING: sq_pushstring(v,c->val.s,-1);break;
		case OT_INTEGER: sq_pushinteger(v,c->val.i);break;
		case OT_FLOAT: sq_pushfloat(v,c->val.f);break;
		}
		sq_createslot(v,-3);
		n++;
	}
	if(sn->delegate) {
		const ScriptClassMemberDecl *members = sn->delegate;
		const ScriptClassMemberDecl *m = NULL;
		sq_newtable(v);
		while(members[n].name) {
			m = &members[n];
			sq_pushstring(v,m->name,-1);
			sq_newclosure(v,m->func,0);
			sq_setparamscheck(v,m->params,m->typemask);
			sq_setnativeclosurename(v,-1,m->name);
			sq_createslot(v,-3);
			n++;
		}
		sq_setdelegate(v,-2);
	}
	sq_createslot(v,-3);
	sq_pop(v,1);
	
	return TRUE;
}

BOOL CreateClass(HSQUIRRELVM v,SquirrelClassDecl *cd)
{
	int n = 0;
	int oldtop = sq_gettop(v);
	sq_pushroottable(v);
	sq_pushstring(v,cd->name,-1);
	if(cd->base) {
		sq_pushstring(v,cd->base,-1);
		if(SQ_FAILED(sq_get(v,-3))) { // Make sure the base exists if specified by cd->base name.
			sq_settop(v,oldtop);
			return FALSE;
		}
	}
	if(SQ_FAILED(sq_newclass(v,cd->base?1:0))) { // Will inherit from base class on stack from sq_get() above.
		sq_settop(v,oldtop);
		return FALSE;
	}
//  sq_settypetag(v,-1,(unsigned int)cd);
#ifdef _WIN32
#pragma warning(disable : 4311)
#endif
	sq_settypetag(v,-1,reinterpret_cast<SQUserPointer>(cd));
	const ScriptClassMemberDecl *members = cd->members;
	const ScriptClassMemberDecl *m = NULL;
  if (members) {
    while(members[n].name) {
      m = &members[n];
      sq_pushstring(v,m->name,-1);
      sq_newclosure(v,m->func,0);
      sq_setparamscheck(v,m->params,m->typemask);
      sq_setnativeclosurename(v,-1,m->name);
      sq_createslot(v,-3);
      n++;
    }
  } // if
	sq_createslot(v,-3);
	sq_pop(v,1);
	return TRUE;
}

BOOL CreateNativeClassInstance(HSQUIRRELVM v,
                               const SQChar *classname,
                               SQUserPointer ud,
                               SQRELEASEHOOK hook)
{
  // If we don't do this, SquirrelVM keeps an old pointer around and this 
  // will be used by SquirrelObject. That crashes when using several VMs.
  SquirrelVM::Init( v );
  
  int oldtop = sq_gettop(v);
  sq_pushroottable(v);
  sq_pushstring(v,classname,-1);
  if(SQ_FAILED(sq_rawget(v,-2))){ //Get the class (created with sq_newclass()).
    sq_settop(v,oldtop);
    return FALSE;
  }
  //sq_pushroottable(v);
  if(SQ_FAILED(sq_createinstance(v,-1))) {
    sq_settop(v,oldtop);
    return FALSE;
  }

#ifdef SQ_USE_CLASS_INHERITANCE
  HSQOBJECT ho;
  sq_getstackobj(v, -1, &ho); // OT_INSTANCE
  SquirrelObject instance(ho);
  SqPlus::PopulateAncestry(v, instance, ud);
#endif
    
  sq_remove(v,-3); //removes the root table
  sq_remove(v,-2); //removes the class
  if(SQ_FAILED(sq_setinstanceup(v,-1,ud))) {
    sq_settop(v,oldtop);
    return FALSE;
  }
  sq_setreleasehook(v,-1,hook);
  return TRUE;
}



// Create native class instance and leave on stack.
BOOL CreateConstructNativeClassInstance(HSQUIRRELVM v,const SQChar * className) {
  int oldtop = sq_gettop(v);
  sq_pushroottable(v);
  sq_pushstring(v,className,-1);
  if (SQ_FAILED(sq_rawget(v,-2))) { // Get the class (created with sq_newclass()).
    sq_settop(v,oldtop);
    return FALSE;
  } // if
#if 0
  sq_remove(v,-3); // Remove the root table.
  sq_push(v,1);    // Push the 'this'.
#else // Kamaitati's change. 5/28/06 jcs.
  sq_remove(v,-2); // Remove the root table.
  sq_pushroottable(v); // Push the 'this'.
#endif
  if (SQ_FAILED(sq_call(v,1,SQTrue,SQ_CALL_RAISE_ERROR))) { // Call ClassName(): creates new instance and calls constructor (instead of sq_createinstance() where constructor is not called).
    sq_settop(v,oldtop);
    return FALSE;
  } // if
  sq_remove(v,-2); // Remove the class.
  //  int newtop = sq_gettop(v);
  return TRUE;
} // CreateConstructNativeClassInstance


