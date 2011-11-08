// SqPlusConst.h
// SqPlus constant type and constant member function support created by Simon Michelmore.
// Modular integration 11/14/05 jcs.

#ifdef SQPLUS_DECLARE_INSTANCE_TYPE_CONST
#undef SQPLUS_DECLARE_INSTANCE_TYPE_CONST

// Kamaitati's NULL_INSTANCE support. 5/28/06 jcs

#ifdef SQPLUS_SUPPORT_NULL_INSTANCES

#define DECLARE_INSTANCE_TYPE_NAME_CONST_BASE(TYPE,NAME) \
inline bool Match(TypeWrapper<const TYPE &>,HSQUIRRELVM v,int idx) { return  GetInstance<TYPE,false>(v,idx) != NULL; } \
inline const TYPE & Get(TypeWrapper<const TYPE &>,HSQUIRRELVM v,int idx) { return *GetInstance<TYPE,true>(v,idx); } 

// Ordinary case
#define DECLARE_INSTANCE_TYPE_NAME_CONST(TYPE,NAME) \
  DECLARE_INSTANCE_TYPE_NAME_(TYPE,NAME) \
  namespace SqPlus { \
  DECLARE_INSTANCE_TYPE_NAME_CONST_BASE(TYPE,NAME) \
  template<> inline void Push(HSQUIRRELVM v,const TYPE * value) { \
    if (!value) sq_pushnull(v); \
    else if (!CreateNativeClassInstance(v,GetTypeName(*value),(TYPE*)value,0)) \
      throw SquirrelError(_SC("Push(): could not create INSTANCE (check registration name)")); } \
  template<> inline void Push(HSQUIRRELVM v,const TYPE & value) { if (!CreateCopyInstance(v,GetTypeName(value),value)) throw SquirrelError(_SC("Push(): could not create INSTANCE copy (check registration name)")); } \
  } // nameSpace SqPlus

// Case for custom Push implementation (covariant return type)
#define DECLARE_INSTANCE_TYPE_NAME_CONST_CUSTOM(TYPE,NAME) \
  DECLARE_INSTANCE_TYPE_NAME_CUSTOM_(TYPE,NAME) \
  namespace SqPlus { \
  DECLARE_INSTANCE_TYPE_NAME_CONST_BASE(TYPE,NAME) \
  template<> void Push(HSQUIRRELVM v,const TYPE * value); \
  template<> void Push(HSQUIRRELVM v,const TYPE & value); \
  } // nameSpace SqPlus


#else

#define DECLARE_INSTANCE_TYPE_NAME_CONST(TYPE,NAME) \
  DECLARE_INSTANCE_TYPE_NAME_(TYPE,NAME) \
  namespace SqPlus { \
  template<> inline void Push(HSQUIRRELVM v,const TYPE * value) { if (!CreateNativeClassInstance(v,GetTypeName(*value),(TYPE*)value,0)) throw SquirrelError(_SC("Push(): could not create INSTANCE (check registration name)")); } \
  template<> inline void Push(HSQUIRRELVM v,const TYPE & value) { if (!CreateCopyInstance(v,GetTypeName(value),value)) throw SquirrelError(_SC("Push(): could not create INSTANCE copy (check registration name)")); } \
  template<> inline bool	Match(TypeWrapper<const TYPE &>,HSQUIRRELVM v,int idx) { return GetInstance<TYPE,false>(v,idx) != NULL; } \
  template<> inline const TYPE & Get(TypeWrapper<const TYPE &>,HSQUIRRELVM v,int idx) { return *GetInstance<TYPE,true>(v,idx); } \
  } // nameSpace SqPlus

#endif

#define DECLARE_INSTANCE_TYPE(TYPE)           DECLARE_INSTANCE_TYPE_NAME_CONST(TYPE,TYPE)
#define DECLARE_INSTANCE_TYPE_NAME(TYPE,NAME) DECLARE_INSTANCE_TYPE_NAME_CONST(TYPE,NAME)
#define DECLARE_INSTANCE_TYPE_CUSTOM(TYPE)           DECLARE_INSTANCE_TYPE_NAME_CONST_CUSTOM(TYPE,TYPE)
#define DECLARE_INSTANCE_TYPE_NAME_CUSTOM(TYPE,NAME) DECLARE_INSTANCE_TYPE_NAME_CONST_CUSTOM(TYPE,NAME)
#endif

#define SQPLUS_APPLY_CONST
#include "SqPlusCallTemplates.h"


#ifdef SQ_REG_CONST_STATIC_VAR
#undef SQ_REG_CONST_STATIC_VAR
template<typename VarType>
SQClassDefBase & staticVar(const VarType * pvar,const SQChar * name,VarAccessType access=VAR_ACCESS_READ_ONLY) {
  struct CV {
    const VarType * var;
  } cv; // Cast Variable helper.
  cv.var = pvar;
  RegisterInstanceVariable(newClass,ClassType<TClassType>::type(),*(VarType **)&cv,name,VarAccessType(access|VAR_ACCESS_STATIC));
  return *this;
} // staticVar
#endif

// SqPlusConst.h
