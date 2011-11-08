#include <stdio.h>
#include <stdarg.h>
#include <assert.h>

#define _DEBUG_DUMP

#include "sqplus.h"

#include <sqstdio.h>
#include <sqstdmath.h>
#include <sqstdstring.h>
#include <sqstdaux.h>
#include <sqstdblob.h>
#include <sqstdsystem.h>


HSQUIRRELVM     SquirrelVM::_VM;
bool            SquirrelVM::_no_vm_ref;
int             SquirrelVM::_CallState = -1;
SquirrelObject* SquirrelVM::_root;
HSQUIRRELVM     SquirrelVM::_sandboxVM;
SquirrelObject  SquirrelVM::_vm;


// Helper struct to keep track of all SQSharedState:s created by SquirrelVM.
#include "../squirrel/sqpcheader.h"
#include "../squirrel/sqvm.h"
struct SQSharedStateNode { 
    SQSharedStateNode( SQSharedState* ps );
    ~SQSharedStateNode( );
    SQSharedState*     m_ps;
    SQSharedStateNode* m_nxt;
};

// Linked list of shared states
static SQSharedStateNode* g_sqss_fst;

SQSharedStateNode::SQSharedStateNode( SQSharedState* ps ) : m_ps(ps), m_nxt(g_sqss_fst) { 
    g_sqss_fst = this;
}

SQSharedStateNode::~SQSharedStateNode() {
    if(m_ps) sq_delete(m_ps,SQSharedState); 
    delete m_nxt;
}

static struct SquirrelVM_ModConstr { 
    ~SquirrelVM_ModConstr(){ 
        // Delete any shared states we created
        delete g_sqss_fst;
        g_sqss_fst = NULL;
    }
} g_squirrelvm_mod_constr;



SquirrelError::SquirrelError() 
{
    const SQChar *s;
    sq_getlasterror(SquirrelVM::_VM);
    sq_getstring(SquirrelVM::_VM,-1,&s);
    if(s) {
        desc = s;
    }
    else {
        desc = _SC("unknown error");
    }
}


SquirrelVMSys::~SquirrelVMSys() {
    // Must take care to release object with the 'ref' VM
    PushRefVM( _vm.GetObjectHandle()._unVal.pThread );
    _vm.Reset();
    PopRefVM();
}

void SquirrelVMSys::Set( HSQUIRRELVM v ){
    // Must take care to release object with the 'ref' VM
    PushRefVM( v );
    _vm = v;
    PopRefVM( );
}

void SquirrelVMSys::Set( const SquirrelObject& ov ){
    assert( ov.GetObjectHandle()._type==OT_THREAD );
    // Must take care to release object with the 'ref' VM
    PushRefVM( ov.GetObjectHandle()._unVal.pThread );
    _vm = ov;
    PopRefVM( );
}

SquirrelVMSys::operator HSQUIRRELVM () const { 
    // Avoid const madness
    SquirrelObject *pvm = (SquirrelObject*)&_vm;
    assert( pvm->GetObjectHandle()._type==OT_THREAD );
    return pvm->GetObjectHandle()._unVal.pThread; 
}


// When doing a SquirrelObject assignment, a reference using the current
// VM is done. 
HSQUIRRELVM g_VM_pushed;
void SquirrelVMSys::PushRefVM( HSQUIRRELVM v ){
    assert( !g_VM_pushed );
    g_VM_pushed = SquirrelVM::_VM;
    SquirrelVM::_VM = v;
}

void SquirrelVMSys::PopRefVM( ){
    SquirrelVM::_VM = g_VM_pushed;
    g_VM_pushed = NULL;
}


bool SquirrelVM::Init( HSQUIRRELVM v ){
    if( v && v==_VM )
        return true; 
    
    // Do we have a previous state?
    Release( );
        
    bool created_new = false;
    if( !v ){
        // Create a new VM - a root VM with new SharedState.
        v = sq_open(1024);
        if( !v ) return false;
        // Store the associated shared state in a linked list. The state will only 
        // be destroyed at app shutdown. Often that is fine, but if using many 
        // VM:s briefly, this allocation is not optimal.
        new SQSharedStateNode( _ss(v) );
        created_new = true;
        sq_setprintfunc(v,SquirrelVM::PrintFunc, SquirrelVM::PrintFunc);
        sq_pushroottable(v);
        sqstd_register_iolib(v);
        sqstd_register_bloblib(v);
        sqstd_register_mathlib(v);
        sqstd_register_stringlib(v);
#ifdef SQPLUS_SQUIRRELVM_WITH_SYSTEMLIB        
        sqstd_register_systemlib(v);
#endif        
        sqstd_seterrorhandlers(v);
        //TODO error handler, compiler error handler
        sq_pop(v,1);
    }
    
    // After this we hold a ref
    _no_vm_ref = false;
    _VM = v;
    _vm = v;
   
    // In the case where Squirrel is ref counted we currently
    // hold two references to the VM (since it is created with 
    // a ref count of 1). In the GC case, it is outside of the 
    // chain of valid objects, so it is not referenced. Compensate 
    // in ref counted case.
    if( created_new )
        DropVMRefIfRefCounted( v );
    
    return true;
}

bool SquirrelVM::InitNoRef( HSQUIRRELVM v ){
    if( v && v==_VM )
        return true; 
    
    // Do we have a previous state?
    Release( );
        
    // Set pointer to this VM, without referencing it
    _no_vm_ref = true;
    _VM = v;
    
    return true;
}

/*
void SquirrelVM::Init( HSQUIRRELVM v )
{
    if( v && v==_VM ) {
      return;
    }
    
    // Do we have a previous state?
    Release();
    
    if( !v ){
        // Create a new VM and own it
        _VM = sq_open(1024);
        sq_setprintfunc(_VM,SquirrelVM::PrintFunc);
        sq_pushroottable(_VM);
        sqstd_register_iolib(_VM);
        sqstd_register_bloblib(_VM);
        sqstd_register_mathlib(_VM);
        sqstd_register_stringlib(_VM);
        sqstd_seterrorhandlers(_VM);
        //TODO error handler, compiler error handler
        sq_pop(_VM,1);
    }
    else {
        _VM = v;
    }
    // After this we hold a ref
    _vm = _VM;
}
*/

void SquirrelVM::Release() {
    // Release root table object if we have one
    if( _root ){ 
        delete _root;
        _root = NULL;
    }
    
    // Release our ref on VM - if we should
    if( !_no_vm_ref )
        _vm.Reset();
        
    _VM = NULL;
}

void SquirrelVM::DropVMRefIfRefCounted( HSQUIRRELVM v ){ 
#ifdef NO_GARBAGE_COLLECTOR
    if( v ){
        SQObject t;
        t._unVal.pThread = v;
        t._type = OT_THREAD;
        sq_release( v, &t );
    }
#endif     
}

BOOL SquirrelVM::Update()
{
    //update remote debugger
    return TRUE;
}

void SquirrelVM::PrintFunc(HSQUIRRELVM v,const SQChar* s,...)
{
    static SQChar temp[2048];
    va_list vl;
    va_start(vl, s);
    scvsprintf( temp,s, vl);
    SCPUTS(temp);
    va_end(vl);
}

SquirrelObject SquirrelVM::CompileScript(const SQChar *s)
{
    SquirrelObject ret;
    if(SQ_SUCCEEDED(sqstd_loadfile(_VM,s,1))) {
        ret.AttachToStackObject(-1);
        sq_pop(_VM,1);
        return ret;
    }
    throw SquirrelError();
}

SquirrelObject SquirrelVM::CompileBuffer(const SQChar *s,const SQChar * debugInfo)
{
    SquirrelObject ret;
    if(SQ_SUCCEEDED(sq_compilebuffer(_VM,s,(int)scstrlen(s)*sizeof(SQChar),debugInfo,1))) {
        ret.AttachToStackObject(-1);
        sq_pop(_VM,1);
        return ret;
    }
    throw SquirrelError();
}

SquirrelObject SquirrelVM::RunScript(const SquirrelObject &o,SquirrelObject *_this)
{
    SquirrelObject ret;
    sq_pushobject(_VM,o._o);
    if(_this) {
        sq_pushobject(_VM,_this->_o);
    }
    else {
        sq_pushroottable(_VM);
    }
    if(SQ_SUCCEEDED(sq_call(_VM,1,SQTrue,SQ_CALL_RAISE_ERROR))) {
        ret.AttachToStackObject(-1);
        sq_pop(_VM,2);
        return ret;
    }
    sq_pop(_VM,1);
    throw SquirrelError();
    
}


BOOL SquirrelVM::BeginCall(const SquirrelObject &func)
{
    if(_CallState != -1)
        return FALSE;
    _CallState = 1;
    sq_pushobject(_VM,func._o);
    sq_pushroottable(_VM);
    return TRUE;
}

BOOL SquirrelVM::BeginCall(const SquirrelObject &func,SquirrelObject &_this)
{
    if(_CallState != -1)
        throw SquirrelError(_SC("call already initialized"));
    _CallState = 1;
    sq_pushobject(_VM,func._o);
    sq_pushobject(_VM,_this._o);
    return TRUE;
}

#define _CHECK_CALL_STATE \
    if(_CallState == -1) \
        throw SquirrelError(_SC("call not initialized"));

void SquirrelVM::PushParam(const SquirrelObject &o)
{
    _CHECK_CALL_STATE
    sq_pushobject(_VM,o._o);
    _CallState++;
}

void SquirrelVM::PushParam(const SQChar *s)
{
    _CHECK_CALL_STATE
    sq_pushstring(_VM,s,-1);
    _CallState++;
}

void SquirrelVM::PushParam(SQInteger n)
{
    _CHECK_CALL_STATE
    sq_pushinteger(_VM,n);
    _CallState++;
}

void SquirrelVM::PushParam(SQFloat f)
{
    _CHECK_CALL_STATE
    sq_pushfloat(_VM,f);
    _CallState++;
}

void SquirrelVM::PushParamNull()
{
    _CHECK_CALL_STATE
    sq_pushnull(_VM);
    _CallState++;
}

void SquirrelVM::PushParam(SQUserPointer up)
{
    _CHECK_CALL_STATE
    sq_pushuserpointer(_VM,up);
    _CallState++;
}

SquirrelObject SquirrelVM::EndCall()
{
    SquirrelObject ret;
    if(_CallState >= 0) { 
        int oldtop = sq_gettop(_VM);
        int nparams = _CallState;
        _CallState = -1;
        if(SQ_SUCCEEDED(sq_call(_VM,nparams,SQTrue,SQ_CALL_RAISE_ERROR))) {
            ret.AttachToStackObject(-1);
            sq_pop(_VM,2);
        }else {
            sq_settop(_VM,oldtop-(nparams+1));
            throw SquirrelError();
        }
        
    }
    return ret;
}

SquirrelObject SquirrelVM::CreateInstance(SquirrelObject &oclass)
{
    SquirrelObject ret;
    int oldtop = sq_gettop(_VM);
    sq_pushobject(_VM,oclass._o);
    if(SQ_FAILED(sq_createinstance(_VM,-1))) {
        sq_settop(_VM,oldtop);
        throw SquirrelError();
    }
    ret.AttachToStackObject(-1);
    sq_pop(_VM,2);
    return ret;
}

SquirrelObject SquirrelVM::CreateTable()
{
    SquirrelObject ret;
    sq_newtable(_VM);
    ret.AttachToStackObject(-1);
    sq_pop(_VM,1);
    return ret;
}

SquirrelObject SquirrelVM::CreateString(const SQChar *s)
{
    SquirrelObject ret;
    sq_pushstring(_VM,s,-1);
    ret.AttachToStackObject(-1);
    sq_pop(_VM,1);
    return ret;
}


SquirrelObject SquirrelVM::CreateArray(int size)
{
    SquirrelObject ret;
    sq_newarray(_VM,size);
    ret.AttachToStackObject(-1);
    sq_pop(_VM,1);
    return ret;
}

SquirrelObject SquirrelVM::CreateFunction(SQFUNCTION func)
{
    SquirrelObject ret;
    sq_newclosure(_VM,func,0);
    ret.AttachToStackObject(-1);
    sq_pop(_VM,1);
    return ret;
}

SquirrelObject SquirrelVM::CreateUserData(int size) {
  SquirrelObject ret;
  sq_newuserdata(_VM,size);
  ret.AttachToStackObject(-1);
  sq_pop(_VM,1);
  return ret;
}

const SquirrelObject &SquirrelVM::GetRootTable()
{
    if( !_root ){
        sq_pushroottable(_VM);
        _root = new SquirrelObject();
        _root->AttachToStackObject(-1);
        sq_pop(_VM,1);
    }
    return *_root;
}

void SquirrelVM::PushRootTable(void) {
  sq_pushroottable(_VM);
} // SquirrelVM::PushRootTable

// Creates a function in the table or class currently on the stack.
//void CreateFunction(HSQUIRRELVM v,const SQChar * scriptFuncName,SQFUNCTION func,int numParams=0,const SQChar * typeMask=0) {
SquirrelObject SquirrelVM::CreateFunction(SQFUNCTION func,const SQChar * scriptFuncName,const SQChar * typeMask) {
  sq_pushstring(_VM,scriptFuncName,-1);
  sq_newclosure(_VM,func,0);
  SquirrelObject ret;
  ret.AttachToStackObject(-1);
  SQChar tm[64];
  SQChar * ptm = tm;
  int numParams = SQ_MATCHTYPEMASKSTRING;
  if (typeMask) {
    if (typeMask[0] == '*') {
      ptm       = 0; // Variable args: don't check parameters.
      numParams = 0; // Clear SQ_MATCHTYPEMASKSTRING (does not mean match 0 params. See sq_setparamscheck()).
    } else {
      if (SCSNPRINTF(tm,sizeof(tm),_SC("t|y|x%s"),typeMask) < 0) {
//        sq_throwerror(_VM,_SC("CreateFunction: typeMask string too long."));
        throw SquirrelError(_SC("CreateFunction: typeMask string too long."));
      } // if
    } // if
  } else { // <TODO> Need to check object type on stack: table, class, instance, etc.
    SCSNPRINTF(tm,sizeof(tm),_SC("%s"),_SC("t|y|x")); // table, class, instance.
//    tm[0] = 't';
//    tm[1] = 0;
  } // if
#if 0
  sq_setparamscheck(_VM,numParams+1,ptm); // Parameters are table+args (thus, the +1).
#else
  if (ptm) {
    sq_setparamscheck(_VM,numParams,ptm); // Determine arg count from type string.
  } // if
#endif
#ifdef _DEBUG
  sq_setnativeclosurename(_VM,-1,scriptFuncName); // For debugging only.
#endif
  sq_createslot(_VM,-3); // Create slot in table or class (assigning function to slot at scriptNameFunc).
  return ret;
} // SquirrelVM::CreateFunction

SquirrelObject SquirrelVM::CreateFunction(SquirrelObject & so,SQFUNCTION func,const SQChar * scriptFuncName,const SQChar * typeMask) {
  PushObject(so);
  SquirrelObject ret = CreateFunction(func,scriptFuncName,typeMask);
  Pop(1);
  return ret;
} // SquirrelVM::CreateFunction

// Create a Global function on the root table.
//void CreateFunctionGlobal(HSQUIRRELVM v,const SQChar * scriptFuncName,SQFUNCTION func,int numParams=0,const SQChar * typeMask=0) {
SquirrelObject SquirrelVM::CreateFunctionGlobal(SQFUNCTION func,const SQChar * scriptFuncName,const SQChar * typeMask) {
  PushRootTable(); // Push root table.
  //  CreateFunction(scriptFuncName,func,numParams,typeMask);
  SquirrelObject ret = CreateFunction(func,scriptFuncName,typeMask);
  Pop(1);         // Pop root table.
  return ret;
} // SquirrelVM::CreateFunctionGlobal
