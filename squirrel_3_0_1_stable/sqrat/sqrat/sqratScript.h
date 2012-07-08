//
// SqratScript: Script Compilation and Execution
//

//
// Copyright (c) 2009 Brandon Jones
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
//  1. The origin of this software must not be misrepresented; you must not
//  claim that you wrote the original software. If you use this software
//  in a product, an acknowledgment in the product documentation would be
//  appreciated but is not required.
//
//  2. Altered source versions must be plainly marked as such, and must not be
//  misrepresented as being the original software.
//
//  3. This notice may not be removed or altered from any source
//  distribution.
//

#if !defined(_SCRAT_SCRIPT_H_)
#define _SCRAT_SCRIPT_H_

#include <squirrel.h>
#include <sqstdio.h>
#include <string.h>

#include "sqratObject.h"

namespace Sqrat {

class Script : public Object {
public:
    Script(HSQUIRRELVM v = DefaultVM::Get()) : Object(v, false) {
    }
 
    ~Script()
    {
        if(!sq_isnull(obj)) {
            sq_release(vm, &obj);
        }
    }
    void CompileString(const string& script) {
        if(!sq_isnull(obj)) {
            sq_release(vm, &obj);
        }
        if(SQ_FAILED(sq_compilebuffer(vm, script.c_str(), static_cast<SQInteger>(script.size() * sizeof(SQChar)), _SC(""), true))) {
            throw Exception(LastErrorString(vm));
        }
        sq_getstackobj(vm,-1,&obj);
        sq_addref(vm, &obj);
        sq_pop(vm, 1);
    }
    
    bool CompileString(const string& script, string& errMsg) {
        if(!sq_isnull(obj)) {
            sq_release(vm, &obj);
        }
        if(SQ_FAILED(sq_compilebuffer(vm, script.c_str(), static_cast<SQInteger>(script.size() * sizeof(SQChar)), _SC(""), true))) {
            errMsg = LastErrorString(vm);
            return false;
        }
        sq_getstackobj(vm,-1,&obj);
        sq_addref(vm, &obj);
        sq_pop(vm, 1);
        return true;
    }

    void CompileFile(const string& path) {
        if(!sq_isnull(obj)) {
            sq_release(vm, &obj);
        }
        if(SQ_FAILED(sqstd_loadfile(vm, path.c_str(), true))) {
            throw Exception(LastErrorString(vm));
        }
        sq_getstackobj(vm,-1,&obj);
        sq_addref(vm, &obj);
        sq_pop(vm, 1);
    }

    bool CompileFile(const string& path, string& errMsg) {
        if(!sq_isnull(obj)) {
            sq_release(vm, &obj);
        }
        if(SQ_FAILED(sqstd_loadfile(vm, path.c_str(), true))) {
            errMsg = LastErrorString(vm);
            return false;
        }
        sq_getstackobj(vm,-1,&obj);
        sq_addref(vm, &obj);
        sq_pop(vm, 1);
        return true;
    }

    void Run() {
        if(!sq_isnull(obj)) {
            SQRESULT result;
            sq_pushobject(vm, obj);
            sq_pushroottable(vm);
            result = sq_call(vm, 1, false, true);
            sq_pop(vm, 1);
            if(SQ_FAILED(result)) {
                throw Exception(LastErrorString(vm));
            }
        }
    }

    bool Run(string& errMsg) {
        if(!sq_isnull(obj)) {
            SQRESULT result;
            sq_pushobject(vm, obj);
            sq_pushroottable(vm);
            result = sq_call(vm, 1, false, true);
            sq_pop(vm, 1);
            if(SQ_FAILED(result)) {
                errMsg = LastErrorString(vm);
                return false;
            }
        }
        return true;
    }


    void WriteCompiledFile(const string& path) {
        if(!sq_isnull(obj)) {
            sq_pushobject(vm, obj);
            sqstd_writeclosuretofile(vm, path.c_str());
            //sq_pop(vm, 1);  // needed?
        }
    }
};
}

#endif
