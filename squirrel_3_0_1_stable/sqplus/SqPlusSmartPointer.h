// SqPlusSmartPointer.h
// SqPlus smart pointer member function support created by James Whitworth.
// Modular integration 07/11/07 James Whitworth.
// you must define the function:
// unsigned char* getSmartPointerPointee(unsigned char*) {
// 		return &(MySmartPointer->pointee);
// }
// somewhere in your program so it can correctly lookup member variables on the class pointed to
// by your smart pointer.

#ifdef SQPLUS_SMARTPOINTER_ACCESSTYPE
#undef SQPLUS_SMARTPOINTER_ACCESSTYPE
enum VarAccessType {VAR_ACCESS_READ_WRITE=0,VAR_ACCESS_READ_ONLY=1<<0,VAR_ACCESS_CONSTANT=1<<1,VAR_ACCESS_STATIC=1<<2,VAR_ACCESS_SMARTPOINTER=1<<3};
#endif // #ifdef SQPLUS_SMARTPOINTER_ACCESSTYPE

#ifdef SQPLUS_SMARTPOINTER_REGISTER_VARIABLE
#undef SQPLUS_SMARTPOINTER_REGISTER_VARIABLE
// classType is the type of the member variable's containing class.
template<typename T>
void
RegisterSmartInstanceVariable(SquirrelObject & so,
                              ClassTypeBase *classType,
                              T *var,
                              const SQChar *scriptVarName,
                              VarAccessType access = VAR_ACCESS_READ_WRITE)
{
	VarRef *pvr = createVarRef(so,scriptVarName);
    
    // var must be passed in as &obj->var, where obj = 0 
    // (the address is the offset), or as static/global address.
	void *offsetOrAddrOrConst = static_cast<void*>(var);
	*pvr = VarRef(offsetOrAddrOrConst,
                  TypeInfo<T>(),
                  classType,
                  ClassType<T>::type(),
                  sizeof(*var),
                  access);
	pvr->m_access |= VAR_ACCESS_SMARTPOINTER;
	createInstanceSetGetHandlers(so);
}
#endif // #ifdef SQPLUS_SMARTPOINTER_REGISTER_VARIABLE

#ifdef SQPLUS_SMARTPOINTER_DISPATCH
#undef SQPLUS_SMARTPOINTER_DISPATCH
template<typename Callee,typename Pointee,typename Func>
class DirectCallSmartInstanceMemberFunction {
public:
  static inline int Dispatch(HSQUIRRELVM v) {
    DirectCallInstanceFuncPicker<Callee, Func> p(v);
    if (!p.instance || !p.func) {
      sq_throwerror(v, _SC("Invalid Instance Type"));
    }
	Pointee *pointeeInstance = static_cast<Pointee*>(p.instance->get());
    return
        !pointeeInstance ? sq_throwerror(v, _SC("SmartPointer Pointee NULL")) :
        Call(*pointeeInstance, *(p.func), v, 2);
  }
};
#endif // #ifdef SQPLUS_SMARTPOINTER_DISPATCH

#ifdef SQPLUS_SMARTPOINTER_DIRECT_CLOSURE
#undef SQPLUS_SMARTPOINTER_DIRECT_CLOSURE

template<typename Callee,typename Pointee,typename Func>
inline void sq_pushdirectsmartinstanceclosure(HSQUIRRELVM v,const Callee & callee,const Pointee & pointee,Func func,SQUnsignedInteger nupvalues) {
  unsigned char * up = (unsigned char *)sq_newuserdata(v,sizeof(func));  // Also pushed on stack.
  memcpy(up,&func,sizeof(func));
  sq_newclosure(v,DirectCallSmartInstanceMemberFunction<Callee,Pointee,Func>::Dispatch,nupvalues+1);
} // sq_pushdirectinstanceclosure

#endif // #ifdef SQPLUS_SMARTPOINTER_DIRECT_CLOSURE

#ifdef SQPLUS_SMARTPOINTER_REGISTER_INSTANCE
#undef SQPLUS_SMARTPOINTER_REGISTER_INSTANCE

template<typename Callee, typename Pointee, typename Func>
inline void RegisterSmartInstance(HSQUIRRELVM v,HSQOBJECT hclass,Callee & callee,Pointee & pointee,Func func,const SQChar * name) {
  sq_pushobject(v,hclass);
  sq_pushstring(v,name,-1);
  sq_pushdirectsmartinstanceclosure(v,callee,pointee,func,0);
  sq_createslot(v,-3);
  sq_poptop(v); // Remove hclass.
} // RegisterInstance

#endif // #ifdef SQPLUS_SMARTPOINTER_REGISTER_INSTANCE

#ifdef SQPLUS_SMARTPOINTER_CLASS_DEF_FUNC
#undef SQPLUS_SMARTPOINTER_CLASS_DEF_FUNC

  // Register a smartpointer member function.
  template<typename Pointee, typename Func>
  SQClassDefBase & smartFunc(Func pfunc,const SQChar * name) {
    RegisterSmartInstance(v,newClass.GetObjectHandle(),*(TClassType *)0,*(Pointee *)0,pfunc,name);
    return *this;
  } // func

#endif // #ifdef SQPLUS_SMARTPOINTER_CLASS_DEF_FUNC

#ifdef SQPLUS_SMARTPOINTER_CLASS_DEF_VAR
#undef SQPLUS_SMARTPOINTER_CLASS_DEF_VAR

  // Register a member variable.
  template<typename Pointee, typename VarType>
  SQClassDefBase & smartVar(VarType Pointee::* pvar,const SQChar * name,VarAccessType access=VAR_ACCESS_READ_WRITE) {
	  struct CV {
		  VarType Pointee::* var;
	  } cv; // Cast Variable helper.
	  cv.var = pvar;
	  RegisterSmartInstanceVariable(newClass,ClassType<TClassType>::type(),*(VarType **)&cv,name,access);
	  return *this;
  } // var

  // Register a member variable as a UserPointer (read only).
  template<typename Pointee, typename VarType>
  SQClassDefBase & smartVarAsUserPointer(VarType Pointee::* pvar,const SQChar * name) {
	  struct CV {
		  VarType Pointee::* var;
	  } cv; // Cast Variable helper.
	  cv.var = pvar;
	  RegisterSmartInstanceVariable(newClass,ClassType<TClassType>::type(),*(SQAnything **)&cv,name,VAR_ACCESS_READ_ONLY);
	  return *this;
  } // varAsUserPointer

#endif // #ifdef SQPLUS_SMARTPOINTER_CLASS_DEF_VAR


#ifdef SQPLUS_SMARTPOINTER_CPP_DECLARATION
#undef SQPLUS_SMARTPOINTER_CPP_DECLARATION
extern unsigned char* getSmartPointerPointee(unsigned char*);
#endif

#ifdef SQPLUS_SMARTPOINTER_INSTANCE_VARINFO
#undef SQPLUS_SMARTPOINTER_INSTANCE_VARINFO
	if(vr->m_access & VAR_ACCESS_SMARTPOINTER) {
		up = reinterpret_cast<char*>(
            getSmartPointerPointee(reinterpret_cast<unsigned char*>(up))
            );
	}
#endif // #ifdef SQPLUS_SMARTPOINTER_CPP_DECLARATION

// SqPlusSmartPointer.h
