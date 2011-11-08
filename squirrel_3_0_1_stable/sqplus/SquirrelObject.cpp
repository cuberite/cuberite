#include "sqplus.h"

SquirrelObject::SquirrelObject(void)
{
	 sq_resetobject(&_o);
}

SquirrelObject::~SquirrelObject()
{
    Reset();
}

SquirrelObject::SquirrelObject(const SquirrelObject &o)
{
	_o = o._o;
	sq_addref(SquirrelVM::_VM,&_o);
}

SquirrelObject::SquirrelObject(HSQOBJECT o)
{
	_o = o;
	sq_addref(SquirrelVM::_VM,&_o);
}

void SquirrelObject::Reset(void) {
  if(SquirrelVM::_VM)
    sq_release(SquirrelVM::_VM,&_o);
  else if( _o._type!=OT_NULL && _o._unVal.pRefCounted )
    printf( "SquirrelObject::~SquirrelObject - Cannot release\n" ); 
  sq_resetobject(&_o);
} // SquirrelObject::Reset

SquirrelObject SquirrelObject::Clone()
{
	SquirrelObject ret;
	if(GetType() == OT_TABLE || GetType() == OT_ARRAY)
	{
		sq_pushobject(SquirrelVM::_VM,_o);
		sq_clone(SquirrelVM::_VM,-1);
		ret.AttachToStackObject(-1);
		sq_pop(SquirrelVM::_VM,2);
	}
	return ret;

}

SquirrelObject & SquirrelObject::operator =(const SquirrelObject &o)
{
	//HSQOBJECT t;
	//t = o._o;
	//sq_addref(SquirrelVM::_VM,&t);
	sq_addref(SquirrelVM::_VM, (HSQOBJECT*)&o._o);
	sq_release(SquirrelVM::_VM,&_o);
	//_o = t;
	_o = o._o;
	return *this;
}

SquirrelObject & SquirrelObject::operator =(HSQOBJECT ho)
{
	sq_addref(SquirrelVM::_VM,&ho);
	sq_release(SquirrelVM::_VM,&_o);
	_o = ho;
	return *this;
}

SquirrelObject & SquirrelObject::operator =(int n)
{
	sq_pushinteger(SquirrelVM::_VM,n);
	AttachToStackObject(-1);
	sq_pop(SquirrelVM::_VM,1);
	return *this;
}

#include <assert.h>
#include "../squirrel/sqstate.h"
#include "../squirrel/sqvm.h"

SquirrelObject & SquirrelObject::operator =(HSQUIRRELVM v)
{
    if( v && SquirrelVM::_VM ){
    	SquirrelVM::_VM->Push(v);
        AttachToStackObject(-1);
        sq_poptop(SquirrelVM::_VM);
    }
    else Reset();
	return *this;
}

bool SquirrelObject::operator == (const SquirrelObject &o)
{
    bool cmp = false;
    HSQUIRRELVM v = SquirrelVM::GetVMPtr();
    int oldtop = sq_gettop(v);
    
    sq_pushobject(v, GetObjectHandle());
    sq_pushobject(v, o.GetObjectHandle());
    if(sq_cmp(v) == 0)
        cmp = true;
   
    sq_settop(v, oldtop);
    return cmp;
}

bool SquirrelObject::CompareUserPointer( const SquirrelObject &o )
{
    if( _o._type == o.GetObjectHandle()._type )
        if( _o._unVal.pUserPointer == o.GetObjectHandle()._unVal.pUserPointer )
            return true;

    return false;
}

void SquirrelObject::ArrayAppend(const SquirrelObject &o)
{
	if(sq_isarray(_o)) {
		sq_pushobject(SquirrelVM::_VM,_o);
		sq_pushobject(SquirrelVM::_VM,o._o);
		sq_arrayappend(SquirrelVM::_VM,-2);
		sq_pop(SquirrelVM::_VM,1);
	}
}

void SquirrelObject::AttachToStackObject(int idx)
{
	HSQOBJECT t;
	sq_getstackobj(SquirrelVM::_VM,idx,&t);
	sq_addref(SquirrelVM::_VM,&t);
	sq_release(SquirrelVM::_VM,&_o);
	_o = t;
}

BOOL SquirrelObject::SetDelegate(SquirrelObject &obj)
{
	if(obj.GetType() == OT_TABLE ||
		obj.GetType() == OT_NULL) {
			switch(_o._type) {
				case OT_USERDATA:
				case OT_TABLE:
					sq_pushobject(SquirrelVM::_VM,_o);
					sq_pushobject(SquirrelVM::_VM,obj._o);
					if(SQ_SUCCEEDED(sq_setdelegate(SquirrelVM::_VM,-2))) {
					  sq_pop(SquirrelVM::_VM,1);
					  return TRUE;
					}
					sq_pop(SquirrelVM::_VM,1);
					break;
			}
	}
	return FALSE;
}

SquirrelObject SquirrelObject::GetDelegate()
{
	SquirrelObject ret;
	if(_o._type == OT_TABLE || _o._type == OT_USERDATA)
	{
    int top = sq_gettop(SquirrelVM::_VM);
		sq_pushobject(SquirrelVM::_VM,_o);
		sq_getdelegate(SquirrelVM::_VM,-1);
		ret.AttachToStackObject(-1);
    sq_settop(SquirrelVM::_VM,top);
//		sq_pop(SquirrelVM::_VM,2);
	}
	return ret;
}

BOOL SquirrelObject::IsNull() const
{
	return sq_isnull(_o);
}

BOOL SquirrelObject::IsNumeric() const
{
	return sq_isnumeric(_o);
}

int SquirrelObject::Len() const
{
	int ret = 0;
	if(sq_isarray(_o) || sq_istable(_o) || sq_isstring(_o)) {
		sq_pushobject(SquirrelVM::_VM,_o);
		ret = sq_getsize(SquirrelVM::_VM,-1);
		sq_pop(SquirrelVM::_VM,1);
	}
	return ret;
}

#define _SETVALUE_INT_BEGIN \
	BOOL ret = FALSE; \
	int top = sq_gettop(SquirrelVM::_VM); \
	sq_pushobject(SquirrelVM::_VM,_o); \
	sq_pushinteger(SquirrelVM::_VM,key);

#define _SETVALUE_INT_END \
	if(SQ_SUCCEEDED(sq_rawset(SquirrelVM::_VM,-3))) { \
		ret = TRUE; \
	} \
	sq_settop(SquirrelVM::_VM,top); \
	return ret;

BOOL SquirrelObject::SetValue(INT key,const SquirrelObject &val)
{
	_SETVALUE_INT_BEGIN
	sq_pushobject(SquirrelVM::_VM,val._o);
	_SETVALUE_INT_END
}

BOOL SquirrelObject::SetValue(INT key,INT n)
{
	_SETVALUE_INT_BEGIN
	sq_pushinteger(SquirrelVM::_VM,n);
	_SETVALUE_INT_END
}

BOOL SquirrelObject::SetValue(INT key,FLOAT f)
{
	_SETVALUE_INT_BEGIN
	sq_pushfloat(SquirrelVM::_VM,f);
	_SETVALUE_INT_END
}

BOOL SquirrelObject::SetValue(INT key,const SQChar *s)
{
	_SETVALUE_INT_BEGIN
	sq_pushstring(SquirrelVM::_VM,s,-1);
	_SETVALUE_INT_END
}

BOOL SquirrelObject::SetValue(INT key,bool b)
{
	_SETVALUE_INT_BEGIN
	sq_pushbool(SquirrelVM::_VM,b);
	_SETVALUE_INT_END
}

BOOL SquirrelObject::SetValue(const SquirrelObject &key,const SquirrelObject &val)
{
	BOOL ret = FALSE;
	int top = sq_gettop(SquirrelVM::_VM);
	sq_pushobject(SquirrelVM::_VM,_o);
	sq_pushobject(SquirrelVM::_VM,key._o);
	sq_pushobject(SquirrelVM::_VM,val._o);
	if(SQ_SUCCEEDED(sq_rawset(SquirrelVM::_VM,-3))) {
		ret = TRUE;
	}
	sq_settop(SquirrelVM::_VM,top);
	return ret;
}

#define _SETVALUE_STR_BEGIN \
	BOOL ret = FALSE; \
	int top = sq_gettop(SquirrelVM::_VM); \
	sq_pushobject(SquirrelVM::_VM,_o); \
	sq_pushstring(SquirrelVM::_VM,key,-1);

#define _SETVALUE_STR_END \
	if(SQ_SUCCEEDED(sq_rawset(SquirrelVM::_VM,-3))) { \
		ret = TRUE; \
	} \
	sq_settop(SquirrelVM::_VM,top); \
	return ret;

BOOL SquirrelObject::SetValue(const SQChar *key,const SquirrelObject &val)
{
	_SETVALUE_STR_BEGIN
	sq_pushobject(SquirrelVM::_VM,val._o);
	_SETVALUE_STR_END
}

BOOL SquirrelObject::SetValue(const SQChar *key,INT n)
{
	_SETVALUE_STR_BEGIN
	sq_pushinteger(SquirrelVM::_VM,n);
	_SETVALUE_STR_END
}

BOOL SquirrelObject::SetValue(const SQChar *key,FLOAT f)
{
	_SETVALUE_STR_BEGIN
	sq_pushfloat(SquirrelVM::_VM,f);
	_SETVALUE_STR_END
}

BOOL SquirrelObject::SetValue(const SQChar *key,const SQChar *s)
{
	_SETVALUE_STR_BEGIN
	sq_pushstring(SquirrelVM::_VM,s,-1);
	_SETVALUE_STR_END
}

BOOL SquirrelObject::SetValue(const SQChar *key,bool b)
{
	_SETVALUE_STR_BEGIN
	sq_pushbool(SquirrelVM::_VM,b);
	_SETVALUE_STR_END
}

// === BEGIN User Pointer, User Data ===

BOOL SquirrelObject::SetUserPointer(const SQChar * key,SQUserPointer up) {
  _SETVALUE_STR_BEGIN
  sq_pushuserpointer(SquirrelVM::_VM,up);
  _SETVALUE_STR_END
} // SquirrelObject::SetUserPointer

SQUserPointer SquirrelObject::GetUserPointer(const SQChar * key) {
  SQUserPointer ret = NULL;
  if (GetSlot(key)) {
    sq_getuserpointer(SquirrelVM::_VM,-1,&ret);
    sq_pop(SquirrelVM::_VM,1);
  } // if
  sq_pop(SquirrelVM::_VM,1);
  return ret;
} // SquirrelObject::GetUserPointer

BOOL SquirrelObject::SetUserPointer(INT key,SQUserPointer up) {
  _SETVALUE_INT_BEGIN
  sq_pushuserpointer(SquirrelVM::_VM,up);
  _SETVALUE_INT_END
} // SquirrelObject::SetUserPointer

SQUserPointer SquirrelObject::GetUserPointer(INT key) {
  SQUserPointer ret = NULL;
  if (GetSlot(key)) {
    sq_getuserpointer(SquirrelVM::_VM,-1,&ret);
    sq_pop(SquirrelVM::_VM,1);
  } // if
  sq_pop(SquirrelVM::_VM,1);
  return ret;
} // SquirrelObject::GetUserPointer

// === User Data ===

BOOL SquirrelObject::NewUserData(const SQChar * key,INT size,SQUserPointer * typetag) {
  _SETVALUE_STR_BEGIN
  sq_newuserdata(SquirrelVM::_VM,size);
  if (typetag) {
    sq_settypetag(SquirrelVM::_VM,-1,typetag);
  } // if
  _SETVALUE_STR_END
} // SquirrelObject::NewUserData

BOOL SquirrelObject::GetUserData(const SQChar * key,SQUserPointer * data,SQUserPointer * typetag) {
  BOOL ret = false;
  if (GetSlot(key)) {
    sq_getuserdata(SquirrelVM::_VM,-1,data,typetag);
    sq_pop(SquirrelVM::_VM,1);
    ret = true;
  } // if
  sq_pop(SquirrelVM::_VM,1);
  return ret;
} // SquirrelObject::GetUserData

BOOL SquirrelObject::RawGetUserData(const SQChar * key,SQUserPointer * data,SQUserPointer * typetag) {
  BOOL ret = false;
  if (RawGetSlot(key)) {
    sq_getuserdata(SquirrelVM::_VM,-1,data,typetag);
    sq_pop(SquirrelVM::_VM,1);
    ret = true;
  } // if
  sq_pop(SquirrelVM::_VM,1);
  return ret;
} // SquirrelObject::RawGetUserData

// === END User Pointer ===

// === BEGIN Arrays ===

BOOL SquirrelObject::ArrayResize(INT newSize) {
//  int top = sq_gettop(SquirrelVM::_VM);
  sq_pushobject(SquirrelVM::_VM,GetObjectHandle());
  BOOL res = sq_arrayresize(SquirrelVM::_VM,-1,newSize) == SQ_OK;
  sq_pop(SquirrelVM::_VM,1);
//  sq_settop(SquirrelVM::_VM,top);
  return res;
} // SquirrelObject::ArrayResize

BOOL SquirrelObject::ArrayExtend(INT amount) {
  int newLen = Len()+amount;
  return ArrayResize(newLen);
} // SquirrelObject::ArrayExtend

BOOL SquirrelObject::ArrayReverse(void) {
  sq_pushobject(SquirrelVM::_VM,GetObjectHandle());
  BOOL res = sq_arrayreverse(SquirrelVM::_VM,-1) == SQ_OK;
  sq_pop(SquirrelVM::_VM,1);
  return res;
} // SquirrelObject::ArrayReverse

SquirrelObject SquirrelObject::ArrayPop(SQBool returnPoppedVal) {
  SquirrelObject ret;
  int top = sq_gettop(SquirrelVM::_VM);
  sq_pushobject(SquirrelVM::_VM,GetObjectHandle());
  if (sq_arraypop(SquirrelVM::_VM,-1,returnPoppedVal) == SQ_OK) {
    if (returnPoppedVal) {
      ret.AttachToStackObject(-1);
    } // if
  } // if
  sq_settop(SquirrelVM::_VM,top);
  return ret;
} // SquirrelObject::ArrayPop

// === END Arrays ===

SQObjectType SquirrelObject::GetType()
{
	return _o._type;
}

BOOL SquirrelObject::GetSlot(INT key) const
{
	sq_pushobject(SquirrelVM::_VM,_o);
	sq_pushinteger(SquirrelVM::_VM,key);
	if(SQ_SUCCEEDED(sq_get(SquirrelVM::_VM,-2))) {
		return TRUE;
	}
	
	return FALSE;
}


SquirrelObject SquirrelObject::GetValue(INT key)const
{
	SquirrelObject ret;
	if(GetSlot(key)) {
		ret.AttachToStackObject(-1);
		sq_pop(SquirrelVM::_VM,1);
	}
	sq_pop(SquirrelVM::_VM,1);
	return ret;
}

FLOAT SquirrelObject::GetFloat(INT key) const
{
	FLOAT ret = 0.0f;
	if(GetSlot(key)) {
		sq_getfloat(SquirrelVM::_VM,-1,&ret);
		sq_pop(SquirrelVM::_VM,1);
	}
	sq_pop(SquirrelVM::_VM,1);
	return ret;
}

INT SquirrelObject::GetInt(INT key) const
{
	INT ret = 0;
	if(GetSlot(key)) {
		sq_getinteger(SquirrelVM::_VM,-1,&ret);
		sq_pop(SquirrelVM::_VM,1);
	}
	sq_pop(SquirrelVM::_VM,1);
	return ret;
}

const SQChar *SquirrelObject::GetString(INT key) const
{
	const SQChar *ret = NULL;
	if(GetSlot(key)) {
		sq_getstring(SquirrelVM::_VM,-1,&ret);
		sq_pop(SquirrelVM::_VM,1);
	}
	sq_pop(SquirrelVM::_VM,1);
	return ret;
}

bool SquirrelObject::GetBool(INT key) const
{
	SQBool ret = FALSE;
	if(GetSlot(key)) {
		sq_getbool(SquirrelVM::_VM,-1,&ret);
		sq_pop(SquirrelVM::_VM,1);
	}
	sq_pop(SquirrelVM::_VM,1);
	return ret?true:false;
}

BOOL SquirrelObject::Exists(const SQChar *key) const
{
	if(GetSlot(key)) {
  	sq_pop(SquirrelVM::_VM,2);
		return TRUE;
	} else {
  	sq_pop(SquirrelVM::_VM,1);
		return FALSE;
	}
}
////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

BOOL SquirrelObject::GetSlot(const SQChar *name) const
{
	sq_pushobject(SquirrelVM::_VM,_o);
	sq_pushstring(SquirrelVM::_VM,name,-1);
	if(SQ_SUCCEEDED(sq_get(SquirrelVM::_VM,-2))) {
		return TRUE;
	}
	
	return FALSE;
}

BOOL SquirrelObject::RawGetSlot(const SQChar *name) const {
  sq_pushobject(SquirrelVM::_VM,_o);
  sq_pushstring(SquirrelVM::_VM,name,-1);
  if(SQ_SUCCEEDED(sq_rawget(SquirrelVM::_VM,-2))) {
    return TRUE;
  }
  return FALSE;
} // SquirrelObject::RawGetSlot

SquirrelObject SquirrelObject::GetValue(const SQChar *key)const
{
	SquirrelObject ret;
	if(GetSlot(key)) {
		ret.AttachToStackObject(-1);
		sq_pop(SquirrelVM::_VM,1);
	}
	sq_pop(SquirrelVM::_VM,1);
	return ret;
}

FLOAT SquirrelObject::GetFloat(const SQChar *key) const
{
	FLOAT ret = 0.0f;
	if(GetSlot(key)) {
		sq_getfloat(SquirrelVM::_VM,-1,&ret);
		sq_pop(SquirrelVM::_VM,1);
	}
	sq_pop(SquirrelVM::_VM,1);
	return ret;
}

INT SquirrelObject::GetInt(const SQChar *key) const
{
	INT ret = 0;
	if(GetSlot(key)) {
		sq_getinteger(SquirrelVM::_VM,-1,&ret);
		sq_pop(SquirrelVM::_VM,1);
	}
	sq_pop(SquirrelVM::_VM,1);
	return ret;
}

const SQChar *SquirrelObject::GetString(const SQChar *key) const
{
	const SQChar *ret = NULL;
	if(GetSlot(key)) {
		sq_getstring(SquirrelVM::_VM,-1,&ret);
		sq_pop(SquirrelVM::_VM,1);
	}
	sq_pop(SquirrelVM::_VM,1);
	return ret;
}

bool SquirrelObject::GetBool(const SQChar *key) const
{
	SQBool ret = FALSE;
	if(GetSlot(key)) {
		sq_getbool(SquirrelVM::_VM,-1,&ret);
		sq_pop(SquirrelVM::_VM,1);
	}
	sq_pop(SquirrelVM::_VM,1);
	return ret?true:false;
}

SQUserPointer SquirrelObject::GetInstanceUP(SQUserPointer tag) const
{
    SQUserPointer up;
    sq_pushobject(SquirrelVM::_VM,_o);
    if (SQ_FAILED(sq_getinstanceup(SquirrelVM::_VM,-1,(SQUserPointer*)&up,tag))) {
        sq_reseterror(SquirrelVM::_VM);
        up = NULL;
    } // if
    sq_pop(SquirrelVM::_VM,1);
    return up;
}

BOOL SquirrelObject::SetInstanceUP(SQUserPointer up)
{
	if(!sq_isinstance(_o)) return FALSE;
	sq_pushobject(SquirrelVM::_VM,_o);
	sq_setinstanceup(SquirrelVM::_VM,-1,up);
	sq_pop(SquirrelVM::_VM,1);
	return TRUE;
}

SquirrelObject SquirrelObject::GetAttributes(const SQChar *key)
{
	SquirrelObject ret;
	int top = sq_gettop(SquirrelVM::_VM);
	sq_pushobject(SquirrelVM::_VM,_o);
	if(key)
		sq_pushstring(SquirrelVM::_VM,key,-1);
	else
		sq_pushnull(SquirrelVM::_VM);
	if(SQ_SUCCEEDED(sq_getattributes(SquirrelVM::_VM,-2))) {
		ret.AttachToStackObject(-1);
	}
	sq_settop(SquirrelVM::_VM,top);
	return ret;
}

BOOL SquirrelObject::BeginIteration()
{
	if(!sq_istable(_o) && !sq_isarray(_o) && !sq_isclass(_o))
		return FALSE;
	sq_pushobject(SquirrelVM::_VM,_o);
	sq_pushnull(SquirrelVM::_VM);
	return TRUE;
}

BOOL SquirrelObject::Next(SquirrelObject &key,SquirrelObject &val)
{
	if(SQ_SUCCEEDED(sq_next(SquirrelVM::_VM,-2))) {
		key.AttachToStackObject(-2);
		val.AttachToStackObject(-1);
		sq_pop(SquirrelVM::_VM,2);
		return TRUE;
	}
	return FALSE;
}

BOOL SquirrelObject::GetTypeTag(SQUserPointer * typeTag) {
  if (SQ_SUCCEEDED(sq_getobjtypetag(&_o,typeTag))) {
    return TRUE;
  } // if
  return FALSE;
} // SquirrelObject::GetTypeTag

const SQChar * SquirrelObject::GetTypeName(const SQChar * key) {
#if 1
  // This version will work even if SQ_SUPPORT_INSTANCE_TYPE_INFO is not enabled.
  SqPlus::ScriptStringVar256 varNameTag;
  SqPlus::getVarNameTag(varNameTag,sizeof(varNameTag),key);
  SQUserPointer data=0;
  if (!RawGetUserData(varNameTag,&data)) {
    return NULL;
  } // if
  SqPlus::VarRefPtr vr = (SqPlus::VarRefPtr)data;
  return vr->varType->GetTypeName();
#else // This version will only work if SQ_SUPPORT_INSTANCE_TYPE_INFO is enabled.
  SquirrelObject so = GetValue(key);
  if (so.IsNull()) return NULL;
  return so.GetTypeName();
#endif
} // SquirrelObject::GetTypeName

const SQChar * SquirrelObject::GetTypeName(INT key) {
  SquirrelObject so = GetValue(key);
  if (so.IsNull()) return NULL;
  return so.GetTypeName();
} // SquirrelObject::GetTypeName

const SQChar * SquirrelObject::GetTypeName(void) {
  SQUserPointer typeTag=NULL;
  if (SQ_SUCCEEDED(sq_getobjtypetag(&_o,&typeTag))) {
    SquirrelObject typeTable = SquirrelVM::GetRootTable().GetValue(SQ_PLUS_TYPE_TABLE);
    if (typeTable.IsNull()) {
      return NULL; // Not compiled with SQ_SUPPORT_INSTANCE_TYPE_INFO enabled.
    } // if
    return typeTable.GetString(INT((size_t)typeTag));
  } // if
  return NULL;
} // SquirrelObject::GetTypeName

SquirrelObject SquirrelObject::GetBase(void)
{
   SquirrelObject ret;
   sq_pushobject(SquirrelVM::_VM,_o);
   sq_getbase(SquirrelVM::_VM,-1);
   ret.AttachToStackObject(-1);
   sq_pop(SquirrelVM::_VM,2);

   return ret;
}

const SQChar* SquirrelObject::ToString()
{
	return sq_objtostring(&_o);
}

SQInteger SquirrelObject::ToInteger()
{
	return sq_objtointeger(&_o);
}

SQFloat SquirrelObject::ToFloat()
{
	return sq_objtofloat(&_o);
}

bool SquirrelObject::ToBool()
{
	//<<FIXME>>
	return _o._unVal.nInteger?true:false;
}

void SquirrelObject::EndIteration()
{
	sq_pop(SquirrelVM::_VM,2);
}

