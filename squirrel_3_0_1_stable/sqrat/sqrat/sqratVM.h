// wrapper for the Squirrel VM under Sqrat
//
// Copyright (c) 2011 Alston Chen
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
//	1. The origin of this software must not be misrepresented; you must not
//	claim that you wrote the original software. If you use this software
//	in a product, an acknowledgment in the product documentation would be
//	appreciated but is not required.
//
//	2. Altered source versions must be plainly marked as such, and must not be
//	misrepresented as being the original software.
//
//	3. This notice may not be removed or altered from any source
//	distribution.
//
//

#if !defined(_SCRAT_VM_H_)
#define _SCRAT_VM_H_

#include <squirrel.h>
#include <sqrat.h>
#include <map>

#include <iostream>
#include <stdarg.h>

#include <sqstdio.h>
#include <sqstdblob.h>
#include <sqstdmath.h>
#include <sqstdsystem.h>
#include <sqstdstring.h>

namespace Sqrat {

class SqratVM
{
private:
    static std::map<HSQUIRRELVM, SqratVM*> ms_sqratVMs;
    static void s_addVM(HSQUIRRELVM vm, SqratVM* sqratvm);
    static void s_deleteVM(HSQUIRRELVM vm);
    static SqratVM* s_getVM(HSQUIRRELVM vm);

    HSQUIRRELVM m_vm;
    Sqrat::RootTable* m_rootTable;
    Sqrat::Script* m_script;
    Sqrat::string m_lastErrorMsg;

private:
    static void printFunc(HSQUIRRELVM v, const SQChar *s, ...);
    static SQInteger runtimeErrorHandler(HSQUIRRELVM v);
    static void compilerErrorHandler(HSQUIRRELVM v,
        const SQChar* desc,
        const SQChar* source,
        SQInteger line,
        SQInteger column);
public:
    enum ERROR_STATE
    {
        NO_ERROR, COMPILE_ERROR, RUNTIME_ERROR
    };

    SqratVM(int initialStackSize = 1024);
    ~SqratVM();

    HSQUIRRELVM getVM() { return m_vm; }
    Sqrat::RootTable& getRootTable() { return *m_rootTable; }
    Sqrat::Script& getScript() { return *m_script; }

    Sqrat::string getLastErrorMsg() { return m_lastErrorMsg; }
    void setLastErrorMsg(const Sqrat::string& str) { m_lastErrorMsg = str; }

    void setPrintFunc(SQPRINTFUNCTION printFunc, SQPRINTFUNCTION errFunc);
    void setErrorHandler(SQFUNCTION runErr, SQCOMPILERERROR comErr);

    ERROR_STATE doString(const Sqrat::string& str);
    ERROR_STATE doFile(const Sqrat::string& file);
};




#ifdef SQUNICODE
    #define scvprintf vwprintf
#else
    #define scvprintf vprintf
#endif


std::map<HSQUIRRELVM, SqratVM*> SqratVM::ms_sqratVMs;

void SqratVM::s_addVM(HSQUIRRELVM vm, SqratVM* sqratvm)
{
    //TODO: use mutex to lock ms_sqratVMs
    ms_sqratVMs.insert(std::make_pair(vm, sqratvm));
}
void SqratVM::s_deleteVM(HSQUIRRELVM vm)
{
    //TODO: use mutex to lock ms_sqratVMs
    ms_sqratVMs.erase(vm);
}
SqratVM* SqratVM::s_getVM(HSQUIRRELVM vm)
{
    //TODO: use mutex to lock ms_sqratVMs
    return ms_sqratVMs[vm];
}

void SqratVM::printFunc(HSQUIRRELVM v, const SQChar *s, ...)
{
    va_list vl;
    va_start(vl, s);
    scvprintf(s, vl);
    va_end(vl);
}

SQInteger SqratVM::runtimeErrorHandler(HSQUIRRELVM v)
{
    const SQChar *sErr = 0;
    if(sq_gettop(v) >= 1)
    {
        Sqrat::string& errStr = s_getVM(v)->m_lastErrorMsg;

        if(SQ_SUCCEEDED(sq_getstring(v, 2, &sErr)))
        {
            //scprintf(_SC("RuntimeError: %s\n"), sErr);
            //errStr = _SC("RuntimeError: ") + sErr;
            errStr = sErr;
        }
        else
        {
            //scprintf(_SC("An Unknown RuntimeError Occured.\n"));
            errStr = _SC("An Unknown RuntimeError Occured.");
        }
    }
    return 0;
}

void SqratVM::compilerErrorHandler(HSQUIRRELVM v,
    const SQChar* desc,
    const SQChar* source,
    SQInteger line,
    SQInteger column)
{
    //scprintf(_SC("%s(%d:%d): %s\n"), source, line, column, desc);

    SQChar buf[512];
    scsprintf(buf, _SC("%s(%d:%d): %s"), source, line, column, desc);
    s_getVM(v)->m_lastErrorMsg = buf;
}

SqratVM::SqratVM(int initialStackSize /* = 1024 */)
    : m_vm(sq_open(initialStackSize))
    , m_rootTable(new Sqrat::RootTable(m_vm))
    , m_script(new Sqrat::Script(m_vm))
    , m_lastErrorMsg()
{
    s_addVM(m_vm, this);

    //register std libs
    sq_pushroottable(m_vm);
    sqstd_register_iolib(m_vm);
    sqstd_register_bloblib(m_vm);
    sqstd_register_mathlib(m_vm);
    sqstd_register_systemlib(m_vm);
    sqstd_register_stringlib(m_vm);
    sq_pop(m_vm, 1);

    setPrintFunc(printFunc, printFunc);
    setErrorHandler(runtimeErrorHandler, compilerErrorHandler);
}

SqratVM::~SqratVM()
{
    s_deleteVM(m_vm);

    delete m_script;
    delete m_rootTable;
    sq_close(m_vm);
}

void SqratVM::setPrintFunc(SQPRINTFUNCTION printFunc, SQPRINTFUNCTION errFunc)
{
    sq_setprintfunc(m_vm, printFunc, errFunc);
}

void SqratVM::setErrorHandler(SQFUNCTION runErr, SQCOMPILERERROR comErr)
{
    sq_newclosure(m_vm, runErr, 0);
    sq_seterrorhandler(m_vm);

    sq_setcompilererrorhandler(m_vm, comErr);
}

SqratVM::ERROR_STATE SqratVM::doString(const Sqrat::string& str)
{
    Sqrat::string msg;
    m_lastErrorMsg.clear();

    if(!m_script->CompileString(str, msg))
    {
        if(m_lastErrorMsg.empty())
        {
            m_lastErrorMsg = msg;
        }
        return COMPILE_ERROR;
    }

    if(!m_script->Run(msg))
    {
        if(m_lastErrorMsg.empty())
        {
            m_lastErrorMsg = msg;
        }
        return RUNTIME_ERROR;
    }

    return NO_ERROR;
}

SqratVM::ERROR_STATE SqratVM::doFile(const Sqrat::string& file)
{
    Sqrat::string msg;
    m_lastErrorMsg.clear();

    if(!m_script->CompileFile(file, msg))
    {
        if(m_lastErrorMsg.empty())
        {
            m_lastErrorMsg = msg;
        }
        return COMPILE_ERROR;
    }

    if(!m_script->Run(msg))
    {
        if(m_lastErrorMsg.empty())
        {
            m_lastErrorMsg = msg;
        }
        return RUNTIME_ERROR;
    }

    return NO_ERROR;
}
}

#endif
