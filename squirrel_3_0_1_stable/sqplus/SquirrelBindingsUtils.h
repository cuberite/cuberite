#ifndef SQUIRREL_BINDINGS_UTILS_H
#define SQUIRREL_BINDINGS_UTILS_H

struct ScriptClassMemberDecl  {
	const SQChar *name;
	SQFUNCTION func;
	int params;
	const SQChar *typemask;
};

struct SquirrelClassDecl  {
	const SQChar *name;
	const SQChar *base;
	const ScriptClassMemberDecl *members;
};

struct ScriptConstantDecl  {
	const SQChar *name;
	SQObjectType type;
	union value {
		value(float v){ f = v; }
		value(int v){ i = v; }
		value(long int v){ li = v; }
		value(const SQChar *v){ s = v; }
		float f;
		int i;
		long int li;
		const SQChar *s;
	} val;
};

struct ScriptNamespaceDecl  {
	const SQChar *name;
	const ScriptClassMemberDecl *members;
	const ScriptConstantDecl *constants;
	const ScriptClassMemberDecl *delegate;
};

#define _BEGIN_CLASS(classname)  \
		int __##classname##__typeof(HSQUIRRELVM v) \
		{ \
			sq_pushstring(v,_SC(#classname),-1); \
			return 1; \
		} \
		struct ScriptClassMemberDecl __##classname##_members[] = { \
		{_SC("_typeof"),__##classname##__typeof,1,NULL},

#define _BEGIN_NAMESPACE(xnamespace) struct ScriptClassMemberDecl __##xnamespace##_members[] = {
#define _BEGIN_NAMESPACE_CONSTANTS(xnamespace) {NULL,NULL,0,NULL}}; \
	struct ScriptConstantDecl __##xnamespace##_constants[] = {

#define _BEGIN_DELEGATE(xnamespace) struct ScriptClassMemberDecl __##xnamespace##_delegate[] = {
#define _DELEGATE(xnamespace) __##xnamespace##_delegate
#define _END_DELEGATE(classname) {NULL,NULL,NULL,NULL}};

#define _CONSTANT(name,type,val) {_SC(#name),type,val},
#define _CONSTANT_IMPL(name,type) {_SC(#name),type,name},

#define _MEMBER_FUNCTION(classname,name,nparams,typemask) \
	{_SC(#name),__##classname##_##name,nparams,typemask},

#define _END_NAMESPACE(classname,delegate) {NULL,OT_NULL,0}}; \
struct ScriptNamespaceDecl __##classname##_decl = {   \
	_SC(#classname), __##classname##_members,__##classname##_constants,delegate };

#define _END_CLASS(classname) {NULL,NULL,0,NULL}}; \
struct SquirrelClassDecl __##classname##_decl = {  \
	_SC(#classname), NULL, __##classname##_members };


#define _END_CLASS_INHERITANCE(classname,base) {NULL,NULL,NULL,NULL}}; \
struct SquirrelClassDecl __##classname##_decl = {  \
	_SC(#classname), _SC(#base), __##classname##_members };

#define _MEMBER_FUNCTION_IMPL(classname,name) \
	int __##classname##_##name(HSQUIRRELVM v)

#define _INIT_STATIC_NAMESPACE(classname) CreateStaticNamespace(SquirrelVM::GetVMPtr(),&__##classname##_decl);
#define _INIT_CLASS(classname)CreateClass(SquirrelVM::GetVMPtr(),&__##classname##_decl);

#define _DECL_STATIC_NAMESPACE(xnamespace) extern struct ScriptNamespaceDecl __##xnamespace##_decl;
#define _DECL_CLASS(classname) extern struct SquirrelClassDecl __##classname##_decl;

#define _CHECK_SELF(cppclass,scriptclass) \
	cppclass *self = NULL; \
	if(SQ_FAILED(sq_getinstanceup(v,1,(SQUserPointer*)&self,(SQUserPointer)&__##scriptclass##_decl))) { \
		return sq_throwerror(v,_SC("invalid instance type"));\
	}

#define _CHECK_INST_PARAM(pname,idx,cppclass,scriptclass)  \
	cppclass *pname = NULL; \
	if(SQ_FAILED(sq_getinstanceup(v,idx,(SQUserPointer*)&pname,(SQUserPointer)&__##scriptclass##_decl))) { \
		return sq_throwerror(v,_SC("invalid instance type"));\
	} \

#define _CHECK_INST_PARAM_BREAK(pname,idx,cppclass,scriptclass)  \
	cppclass *pname = NULL; \
	if(SQ_FAILED(sq_getinstanceup(v,idx,(SQUserPointer*)&pname,(SQUserPointer)&__##scriptclass##_decl))) { \
		break; \
	} \

#define _CLASS_TAG(classname) ((unsigned int)&__##classname##_decl)


#define _DECL_NATIVE_CONSTRUCTION(classname,cppclass) \
	BOOL push_##classname(cppclass &quat); \
	SquirrelObject new_##classname(cppclass &quat);

#define _IMPL_NATIVE_CONSTRUCTION(classname,cppclass) \
static int classname##_release_hook(SQUserPointer p, int size) \
{ \
	if(p) { \
		cppclass *pv = (cppclass *)p; \
		delete pv; \
	} \
	return 0; \
} \
BOOL push_##classname(cppclass &quat) \
{ \
	cppclass *newquat = new cppclass; \
	*newquat = quat; \
	if(!CreateNativeClassInstance(SquirrelVM::GetVMPtr(),_SC(#classname),newquat,classname##_release_hook)) { \
		delete newquat; \
		return FALSE; \
	} \
	return TRUE; \
} \
SquirrelObject new_##classname(cppclass &quat) \
{ \
	SquirrelObject ret; \
	if(push_##classname(quat)) { \
		ret.AttachToStackObject(-1); \
		sq_pop(SquirrelVM::GetVMPtr(),1); \
	} \
	return ret; \
} \
int construct_##classname(cppclass *p) \
{ \
	sq_setinstanceup(SquirrelVM::GetVMPtr(),1,p); \
	sq_setreleasehook(SquirrelVM::GetVMPtr(),1,classname##_release_hook); \
	return 1; \
}

BOOL CreateStaticClass(HSQUIRRELVM v,SquirrelClassDecl *cd);
BOOL CreateStaticNamespace(HSQUIRRELVM v,ScriptNamespaceDecl *sn);
BOOL CreateClass(HSQUIRRELVM v,SquirrelClassDecl *cd);
BOOL InitScriptClasses(HSQUIRRELVM v);
BOOL CreateNativeClassInstance(HSQUIRRELVM v,const SQChar *classname,SQUserPointer ud,SQRELEASEHOOK hook);
BOOL CreateConstructNativeClassInstance(HSQUIRRELVM v,const SQChar * className);

#endif // SQUIRREL_BINDINGS_UTILS_H

