#include "sqplus.h"
#include <stdio.h>

#ifdef SQPLUS_SMARTPOINTER_OPT
#define SQPLUS_SMARTPOINTER_CPP_DECLARATION
#include "SqPlusSmartPointer.h"
#endif

namespace SqPlus {

static int getVarInfo(StackHandler & sa,VarRefPtr & vr) {
  HSQOBJECT htable = sa.GetObjectHandle(1);
  SquirrelObject table(htable);
  const SQChar * el = sa.GetString(2);
  ScriptStringVar256 varNameTag;
  getVarNameTag(varNameTag,sizeof(varNameTag),el);
  SQUserPointer data=0;
  if (!table.RawGetUserData(varNameTag,&data)) {
    return sa.ThrowError(_SC("getVarInfo: Could not retrieve UserData")); // Results in variable not being found error.
  }
  vr = (VarRefPtr)data;
  return SQ_OK;
} // getVarInfo

static int getInstanceVarInfo(StackHandler & sa,VarRefPtr & vr,SQUserPointer & data) {
  HSQOBJECT ho = sa.GetObjectHandle(1);
  SquirrelObject instance(ho);
  const SQChar * el = sa.GetString(2);
  ScriptStringVar256 varNameTag;
  getVarNameTag(varNameTag,sizeof(varNameTag),el);
  SQUserPointer ivrData=0;
  if (!instance.RawGetUserData(varNameTag,&ivrData)) {
    return sa.ThrowError(_SC("getInstanceVarInfo: Could not retrieve UserData")); // Results in variable not being found error.
  }
  vr = (VarRefPtr)ivrData;

  char * up;
  if (!(vr->m_access & (VAR_ACCESS_STATIC|VAR_ACCESS_CONSTANT))) {
    SQUserPointer typetag; 
    instance.GetTypeTag(&typetag);

#if defined(SQ_USE_CLASS_INHERITANCE) 
    if (typetag != vr->instanceType) {
      SquirrelObject typeTable = instance.GetValue(SQ_CLASS_OBJECT_TABLE_NAME);
      up = (char *)typeTable.GetUserPointer(INT((size_t)vr->instanceType)); // <TODO> 64-bit compatible version.
      if (!up) {
        throw SquirrelError(_SC("Invalid Instance Type"));
      }
    } else {
      up = (char *)instance.GetInstanceUP(0);
    } // if

#elif defined(SQ_USE_CLASS_INHERITANCE_SIMPLE)
    ClassTypeBase *ctb = (ClassTypeBase*)vr->instanceType;
    up = (char *)instance.GetInstanceUP(0);
    // Walk base classes until type tag match, adjust for inheritence offset
    while(ctb && typetag!=ctb) {
      up = (char*)up - ctb->m_offset;
      ctb = ctb->m_pbase;
    }
    if (!ctb) {
      throw SquirrelError(_SC("Invalid Instance Type"));
    }
#else
    up = (char *)instance.GetInstanceUP(0);
#endif

#ifdef SQPLUS_SMARTPOINTER_OPT
#define SQPLUS_SMARTPOINTER_INSTANCE_VARINFO
#include "SqPlusSmartPointer.h"
#endif

    up += (size_t)vr->offsetOrAddrOrConst;         // Offset
  } else {
    up = (char *)vr->offsetOrAddrOrConst; // Address
  } // if
  data = up;
  return SQ_OK;
} // getInstanceVarInfo


// If not static/global, message can (and will) disappear before arriving at catch (G++)
static ScriptStringVar256 g_msg_throw;

static int setVar(StackHandler & sa,VarRef * vr,void * data) {
  if (vr->m_access & (VAR_ACCESS_READ_ONLY|VAR_ACCESS_CONSTANT)) {
    const SQChar * el = sa.GetString(2);
    SCSNPRINTF(g_msg_throw.s,sizeof(g_msg_throw),_SC("setVar(): Cannot write to constant: %s"),el);
    throw SquirrelError(g_msg_throw.s);
  } // if
  switch (vr->m_type) {
  case TypeInfo<INT>::TypeID: {
    INT * val = (INT *)data; // Address
    if (val) {
        INT v = sa.GetInt(3);
        // Support for different int sizes
        switch( vr->m_size ) {
          case 1: v = (*(char*)val = (char)v); break;  
          case 2: v = (*(short*)val = (short)v); break;  
#ifdef _SQ64
          case 4: v = (*(int*)val = (int)v); break;
#endif            
          default: *val = v;
       }
       return sa.Return(v);
    } // if
    break;
  } // case
  case TypeInfo<unsigned>::TypeID: {
	  unsigned * val = (unsigned *)data; // Address
	  if (val) {
		  *val = sa.GetInt(3);
		  return sa.Return(static_cast<INT>(*val));
	  } // if
	  break;
  } // case
  case TypeInfo<FLOAT>::TypeID: {
    FLOAT * val = (FLOAT *)data; // Address
    if (val) {
      *val = sa.GetFloat(3);
      return sa.Return(*val);
    } // if
    break;
  } // case
  case TypeInfo<bool>::TypeID: {
    bool * val = (bool *)data; // Address
    if (val) {
      *val = sa.GetBool(3) ? true : false;
      return sa.Return(*val);
    } // if
    break;
  } // case
  case VAR_TYPE_INSTANCE: {
    HSQUIRRELVM v = sa.GetVMPtr();
    SQUserPointer src = sa.GetInstanceUp(3,(SQUserPointer)vr->varType); // Effectively performs: ClassType<>::type() == ClassType<>().
    if (!src) {
      throw SquirrelError(_SC("INSTANCE type assignment mismatch"));
    }
    vr->varType->vgetCopyFunc()(data,src);
    return 0;
  }
  case TypeInfo<SQUserPointer>::TypeID: {
    const SQChar * el = sa.GetString(2);
    SCSNPRINTF(g_msg_throw.s,sizeof(g_msg_throw),_SC("setVar(): Cannot write to an SQUserPointer: %s"),el);
    throw SquirrelError(g_msg_throw.s);
  } // case
  case TypeInfo<ScriptStringVarBase>::TypeID: {
    ScriptStringVarBase * val = (ScriptStringVarBase *)data; // Address
    if (val) {
      const SQChar * strVal = sa.GetString(3);
      if (strVal) {
        *val = strVal;
        return sa.Return(val->s);
      } // if
    } // if
    break;
  } // case
#if defined(SQPLUS_SUPPORT_STD_STRING) && !defined(SQUNICODE)
  case TypeInfo<std::string>::TypeID: {
    std::string *val = (std::string*)data; // Address
    if (val) {
      const SQChar *strVal = sa.GetString(3);
      if (strVal) {
        *val = strVal;
        return sa.Return(val->c_str());
      } // if
   } // if
   break;
  } // case
#endif      
  } // switch
  return SQ_ERROR;
} // setVar

static int getVar(StackHandler & sa,VarRef * vr,void * data) {
  switch (vr->m_type) {
  case TypeInfo<INT>::TypeID: {
    if (!(vr->m_access & VAR_ACCESS_CONSTANT)) {
      if (data) {
          INT v;
          // Support for different int sizes
          switch( vr->m_size ){
            case 1: v = *(char*)data; break;  
            case 2: v = *(short*)data; break;  
#ifdef _SQ64
            case 4: v = *(int*)data; break;
#endif            
            default: v = *(INT*)data;
          }
          return sa.Return(v);
      } // if
    } else {
      INT * val = (INT *)&data; // Constant value
      return sa.Return(*val);
    } // if
    break;
  } // case
  case TypeInfo<unsigned>::TypeID: {
	  if (!(vr->m_access & VAR_ACCESS_CONSTANT)) {
		  unsigned * val = (unsigned *)data; // Address
		  if (val){
            return sa.Return(static_cast<INT>(*val));
          }
	  } else {
		  unsigned * val = (unsigned *)&data; // Constant value
		  return sa.Return(static_cast<INT>(*val));
	  } // if
	  break;
  } // case
  case TypeInfo<FLOAT>::TypeID: {
    if (!(vr->m_access & VAR_ACCESS_CONSTANT)) {
      FLOAT * val = (FLOAT *)data; // Address
      if (val) {
        return sa.Return(*val);
      } // if
    } else {
      FLOAT * val = (FLOAT *)&data; // Constant value
      return sa.Return(*val);
    } // if
    break;
  } // case
  case TypeInfo<bool>::TypeID: {
    if (!(vr->m_access & VAR_ACCESS_CONSTANT)) {
      bool * val = (bool *)data; // Address
      if (val) {
        return sa.Return(*val);
      } // if
    } else {
      bool * val = (bool *)&data; // Constant value
      return sa.Return(*val);
    } // if
    break;
  } // case
  case VAR_TYPE_INSTANCE:
    if (!CreateNativeClassInstance(sa.GetVMPtr(),vr->varType->GetTypeName(),data,0)) { // data = address. Allocates memory.
      SCSNPRINTF(g_msg_throw.s,sizeof(g_msg_throw),_SC("getVar(): Could not create instance: %s"),vr->varType->GetTypeName());
      throw SquirrelError(g_msg_throw.s);
    } // if
    return 1;
  case TypeInfo<SQUserPointer>::TypeID: 
    return sa.Return(data); // The address of member variable, not the variable itself.
  case TypeInfo<ScriptStringVarBase>::TypeID: {
    if (!(vr->m_access & VAR_ACCESS_CONSTANT)) {
      ScriptStringVarBase * val = (ScriptStringVarBase *)data; // Address
      if (val) {
        return sa.Return(val->s);
      } // if
    } else {
      throw SquirrelError(_SC("getVar(): Invalid type+access: 'ScriptStringVarBase' with VAR_ACCESS_CONSTANT (use VAR_ACCESS_READ_ONLY instead)"));
    }
    break;
  } // case
  case TypeInfo<const SQChar *>::TypeID: {
    if (!(vr->m_access & VAR_ACCESS_CONSTANT)) {
      if( vr->m_access==VAR_ACCESS_READ_WRITE )
        throw SquirrelError(_SC("getVar(): Invalid type+access: 'const SQChar *' without VAR_ACCESS_CONSTANT"));
      // It is OK to read from a SQChar* if requested
      return sa.Return(*(const SQChar **)data); // Address
    } else {
      return sa.Return((const SQChar *)data); // Address
    }
    break;
  } // case
#ifdef SQPLUS_SUPPORT_STD_STRING
  case TypeInfo<std::string>::TypeID: {
    if (!(vr->m_access & VAR_ACCESS_CONSTANT)) {
      std::string *val = (std::string *)data; // Address
      if (val) {
        return sa.Return(val->c_str());
      }
    } else {
      throw SquirrelError(_SC("getVar(): Invalid type+access: 'std::string' with VAR_ACCESS_CONSTANT (use VAR_ACCESS_READ_ONLY instead)"));
    }
    break;
  } // case
#endif
  } // switch
  return SQ_ERROR;
} // getVar

// === Global Vars ===

int setVarFunc(HSQUIRRELVM v) {
  SquirrelVM::Init(v);  // For handling multi-VM setting right
  StackHandler sa(v);
  if (sa.GetType(1) == OT_TABLE) {
    VarRefPtr vr;
    int res = getVarInfo(sa,vr);
    if (res != SQ_OK) return res;
    return setVar(sa,vr,vr->offsetOrAddrOrConst);
  } // if
  return SQ_ERROR;
} // setVarFunc

int getVarFunc(HSQUIRRELVM v) {
  SquirrelVM::Init(v);  // For handling multi-VM setting right
  StackHandler sa(v);
  if (sa.GetType(1) == OT_TABLE) {
    VarRefPtr vr;
    int res = getVarInfo(sa,vr);
    if (res != SQ_OK) return res;
    return getVar(sa,vr,vr->offsetOrAddrOrConst);
  } // if
  return SQ_ERROR;
} // getVarFunc

// === Instance Vars ===

int setInstanceVarFunc(HSQUIRRELVM v) {
  SquirrelVM::Init(v);  // For handling multi-VM setting right
  StackHandler sa(v);
  if (sa.GetType(1) == OT_INSTANCE) {
    VarRefPtr vr;
    void * data;
    int res = getInstanceVarInfo(sa,vr,data);
    if (res != SQ_OK) return res;
    return setVar(sa,vr,data);
  } // if
  return SQ_ERROR;
} // setInstanceVarFunc

int getInstanceVarFunc(HSQUIRRELVM v) {
  SquirrelVM::Init(v);  // For handling multi-VM setting right
  StackHandler sa(v);
  if (sa.GetType(1) == OT_INSTANCE) {
    VarRefPtr vr;
    void * data;
    int res = getInstanceVarInfo(sa,vr,data);
    if (res != SQ_OK) return res;
    return getVar(sa,vr,data);
  } // if
  return SQ_ERROR;
} // getInstanceVarFunc

// === Classes ===

BOOL CreateClass(HSQUIRRELVM v,SquirrelObject & newClass,SQUserPointer classType,const SQChar * name,const SQChar * baseName) {
  int n = 0;
  int oldtop = sq_gettop(v);
  sq_pushroottable(v);
  sq_pushstring(v,name,-1);
  if (baseName) {
    sq_pushstring(v,baseName,-1);
    if (SQ_FAILED(sq_get(v,-3))) { // Make sure the base exists if specified by baseName.
      sq_settop(v,oldtop);
      return FALSE;
    } // if
  } // if
  if (SQ_FAILED(sq_newclass(v,baseName ? 1 : 0))) { // Will inherit from base class on stack from sq_get() above.
    sq_settop(v,oldtop);
    return FALSE;
  } // if
  newClass.AttachToStackObject(-1);
  sq_settypetag(v,-1,classType);
  sq_createslot(v,-3);
  sq_pop(v,1);
  return TRUE;
} // CreateClass

SquirrelObject RegisterClassType(HSQUIRRELVM v,const SQChar * scriptClassName,SQUserPointer classType,SQFUNCTION constructor) {
  SquirrelVM::Init(v);  // For handling multi-VM setting right
  int top = sq_gettop(v);
  SquirrelObject newClass;
  if (CreateClass(v,newClass,classType,scriptClassName)) {
    SquirrelVM::CreateFunction(newClass,constructor,_SC("constructor"));
  } // if
  sq_settop(v,top);
  return newClass;
} // RegisterClassType


///////////////////////////////////////////////////////////////////////////
// GCC sometimes has problems with finding inline functions at link time
// (that also have a template definition). To solve the problem,
// non-inlines goes here.
#ifdef GCC_INLINE_WORKAROUND
# include "SqPlusFunctionCallImpl.h"
#endif  // GCC_INLINE_WORKAROUND 
///////////////////////////////////////////////////////////////////////////

} // namespace SqPlus

