#ifndef _SQUIRREL_VM_H_
#define _SQUIRREL_VM_H_

#include "SquirrelObject.h"

struct SquirrelError {
    SquirrelError();
    SquirrelError(const SQChar* s):desc(s){}
    const SQChar *desc;
};

// This class can hold a reference to a SquirrelVM. It keeps a Squirrel ref
// to the VM to protect it from being deleted while held.
struct SquirrelVMSys { 
    SquirrelVMSys() { }
    ~SquirrelVMSys();
    
    void Set( HSQUIRRELVM v );
    void Set( const SquirrelObject& ov );
    void Reset( ){ _vm.Reset(); } 
    
    SquirrelVMSys& operator = (HSQUIRRELVM v){ Set(v); return *this; }
    operator HSQUIRRELVM () const;
    
protected:    
    void PushRefVM(HSQUIRRELVM v);
    void PopRefVM();
    SquirrelObject _vm;
    friend class SquirrelVM;
};

// Notes on creating / destroying SquirrelVM:s:
//
// VM:s created through sq_open are special since they create a new 
// SQSharedState. That shared state is later shared by any new thread
// or friend VM. sq_close can be used for closing VM:s created through
// sq_open (but not for friend VMs). 
//
// Using squirrel references in SquirrelVMSys and SquirrelVM, one must
// make sure that these are all reset if one calls sq_close manually.
//
// When there are no more references to a VM, it is destroyed automatically,
// but the shared state is not! For VM:s created by SquirrelVM, it keeps 
// a list of shared states it has created and will destroy them all on 
// app shutdown. 

class SquirrelVM {
    friend class SquirrelObject;
    friend struct SquirrelError;
    friend struct SquirrelVMSys;
    
public:
    // If a VM is passed as arg here, Init will not alter it. Otherwise 
    // a new VM is created and initialized. A squirrel reference is kept 
    // while it is the current VM.
    static bool Init( HSQUIRRELVM v=NULL );
    
    // Initialize with an externally created VM, without adding a ref 
    // on it. NOTE: This may not be compatible with Set/GetVMSys as 
    // we're just working with raw pointers here.
    static bool InitNoRef( HSQUIRRELVM v );
    static BOOL IsInitialized(){return _VM == NULL?FALSE:TRUE;}
    
    static void Release();   // Release ref on VM and reset VM pointer
    static void Shutdown(){ Release(); }
    static void AppFinalShutdown();  // Call when finally shutting down app
    
    static BOOL Update(); //debugger and maybe GC later
    
    static SquirrelObject CompileScript(const SQChar *s);
    static SquirrelObject CompileBuffer(const SQChar *s,const SQChar * debugInfo=_SC("console_buffer"));
    static SquirrelObject RunScript(const SquirrelObject &o,SquirrelObject *_this = NULL);
    
    static void PrintFunc(HSQUIRRELVM v,const SQChar* s,...);
    
    static BOOL BeginCall(const SquirrelObject &func);
    static BOOL BeginCall(const SquirrelObject &func,SquirrelObject &_this);
    
    static void PushParam(const SquirrelObject &o);
    static void PushParam(const SQChar *s);
    static void PushParam(SQInteger n);
    static void PushParam(SQFloat f);
    static void PushParam(SQUserPointer up);
    static void PushParamNull();
    
    static SquirrelObject EndCall();
    static SquirrelObject CreateString(const SQChar *s);
    static SquirrelObject CreateTable();    
    static SquirrelObject CreateArray(int size);
    static SquirrelObject CreateInstance(SquirrelObject &oclass); // oclass is an existing class. Create an 'instance' (OT_INSTANCE) of oclass.
    static SquirrelObject CreateFunction(SQFUNCTION func);
    static SquirrelObject CreateUserData(int size);

    static const SquirrelObject &GetRootTable();
    static HSQUIRRELVM GetVMPtr() { return _VM; }

    // The sandbox VM ptr is one which cannot access functions bound with 
    // SqPlus. It is suitable for running non-trusted scripts that can only
    // access basic functionality.
    static void SetSandboxVMPtr(HSQUIRRELVM v) {
      _sandboxVM = v;
    } // SetSandboxVMPtr
  
    static HSQUIRRELVM GetSandboxVMPtr() {
      return _sandboxVM;
    } // GetSandboxVMPtr
  
    static void GetVMSys(SquirrelVMSys & vmSys) {
      vmSys.Set( _vm );
    } // GetVMSys

    static void SetVMSys(const SquirrelVMSys & vmSys) {
      Release();
      HSQUIRRELVM v = (HSQUIRRELVM)vmSys;
      if( v ) 
          Init( v );
    } // SetVMSys

    static void PushValue(INT val) {
        sq_pushinteger(_VM,val);
    } // PushValue
    static void PushValue(FLOAT val) {
        sq_pushfloat(_VM,val);
    } // PushValue
    static void PushValue(bool val) { // Compiler treats SQBool as INT.
        sq_pushbool(_VM,val);
    } // PushValue
    static void PushValue(SQChar * val) {
        sq_pushstring(_VM,val,-1);
    } // PushValue
    static void PushValue(SQUserPointer val) {
        sq_pushuserpointer(_VM,val);
    } // PushValue
    static void PushValue(const SQChar * val) {
        sq_pushstring(_VM,val,-1);
    } // PushValue
    static void PushObject(SquirrelObject & so) {
        sq_pushobject(_VM,so._o);
    } // PushObject
    static void Pop(SQInteger nelemstopop) {
        sq_pop(_VM,nelemstopop);
    } // Pop
    
    static void PushRootTable(void);
    
    // Create/bind a function on the table currently on the stack.
    static SquirrelObject CreateFunction(SQFUNCTION func,const SQChar * scriptFuncName,const SQChar * typeMask=0);
    // Create/bind a function on the table so. typeMask: standard Squirrel types plus: no typemask means no args, "*" means any type of args.
    static SquirrelObject CreateFunction(SquirrelObject & so,SQFUNCTION func,const SQChar * scriptFuncName,const SQChar * typeMask=0);
    // Create/bind a function to the root table. typeMask: standard Squirrel types plus: no typemask means no args, "*" means any type of args.
    static SquirrelObject CreateFunctionGlobal(SQFUNCTION func,const SQChar * scriptFuncName,const SQChar * typeMask=0);

    // This is a helper to correct a difference in referncing new VM:s in 
    // ref counted versus garbage collected modes. NOTE: Only use after creating
    // a VM with:  1 - sq_open()  2 - Creating a ref to the VM (SquirrelObject)
    static void DropVMRefIfRefCounted( HSQUIRRELVM v );
  

private:
    static SquirrelObject _vm;        // This is a Squirrel reference to the VM
    static HSQUIRRELVM    _VM;        // The raw C++ pointer
    static bool           _no_vm_ref; // Set if we only keep the raw C++ pointer and no ref
    static int _CallState;
    static SquirrelObject * _root;    // Cached root table if non NULL
    static HSQUIRRELVM _sandboxVM;    // The sandbox VM (that cannot use bound functions)
};


template<typename T>
inline BOOL SquirrelObject::ArrayAppend(T item) {
    sq_pushobject(SquirrelVM::_VM,GetObjectHandle());
    SquirrelVM::PushValue(item);
    BOOL res = sq_arrayappend(SquirrelVM::_VM,-2) == SQ_OK;
    sq_pop(SquirrelVM::_VM,1);
    return res;
} // ArrayAppend

#endif //_SQUIRREL_VM_H_

