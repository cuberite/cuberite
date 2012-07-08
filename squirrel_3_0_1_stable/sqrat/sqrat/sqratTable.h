//
// SqratTable: Table Binding
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

#if !defined(_SCRAT_TABLE_H_)
#define _SCRAT_TABLE_H_

#include <squirrel.h>
#include <string.h>

#include "sqratObject.h"
#include "sqratFunction.h"
#include "sqratGlobalMethods.h"

namespace Sqrat {

class TableBase : public Object {
public:
    TableBase(HSQUIRRELVM v = DefaultVM::Get()) : Object(v, true) {
    }

    TableBase(const Object& obj) : Object(obj) {
    }

    // Bind a Table or Class to the Table (Can be used to facilitate Namespaces)
    // Note: Bind cannot be called "inline" like other functions because it introduces order-of-initialization bugs.
    void Bind(const SQChar* name, Object& obj) {
        sq_pushobject(vm, GetObject());
        sq_pushstring(vm, name, -1);
        sq_pushobject(vm, obj.GetObject());
        sq_newslot(vm, -3, false);
        sq_pop(vm,1); // pop table
    }

    // Bind a raw Squirrel closure to the Table
    TableBase& SquirrelFunc(const SQChar* name, SQFUNCTION func) {
        sq_pushobject(vm, GetObject());
        sq_pushstring(vm, name, -1);
        sq_newclosure(vm, func, 0);
        sq_newslot(vm, -3, false);
        sq_pop(vm,1); // pop table

        return *this;
    }

    //
    // Variable Binding
    //

    template<class V>
    TableBase& SetValue(const SQChar* name, const V& val) {
        BindValue<V>(name, val, false);
        return *this;
    }
    template<class V>
    TableBase& SetValue(const SQInteger index, const V& val) {
        BindValue<V>(index, val, false);
        return *this;
    }

    template<class V>
    TableBase& SetInstance(const SQChar* name, V* val) {
        BindInstance<V>(name, val, false);
        return *this;
    }

    template<class F>
    TableBase& Func(const SQChar* name, F method) {
        BindFunc(name, &method, sizeof(method), SqGlobalFunc(method));
        return *this;
    }

    template<class F>
    TableBase& Overload(const SQChar* name, F method) {
        BindOverload(name, &method, sizeof(method), SqGlobalFunc(method), SqOverloadFunc(method), SqGetArgCount(method));
        return *this;
    }

    //
    // Function Calls
    //

    Function GetFunction(const SQChar* name) {
        HSQOBJECT funcObj;
        sq_pushobject(vm, GetObject());
        sq_pushstring(vm, name, -1);
        if(SQ_FAILED(sq_get(vm, -2))) {
            sq_pushnull(vm);
        }
        sq_getstackobj(vm, -1, &funcObj);
        Function ret(vm, GetObject(), funcObj); // must addref before the pop!

        sq_pop(vm, 2);

        return ret;
    }
    Function GetFunction(const SQInteger index) {
        HSQOBJECT funcObj;
        sq_pushobject(vm, GetObject());
        sq_pushinteger(vm, index);
        if(SQ_FAILED(sq_get(vm, -2))) {
            sq_pushnull(vm);
        }
        sq_getstackobj(vm, -1, &funcObj);
        Function ret(vm, GetObject(), funcObj);
        sq_pop(vm, 2);

        return ret;
    }
};

class Table : public TableBase {
public:
    Table(HSQUIRRELVM v = DefaultVM::Get()) : TableBase(v) {
        sq_newtable(vm);
        sq_getstackobj(vm,-1,&obj);
        sq_addref(vm, &obj);
        sq_pop(vm,1);
    }
    Table(const Object& obj) : TableBase(obj) {
    }
};

//
// Root Table
//

class RootTable : public TableBase {
public:
    RootTable(HSQUIRRELVM v = DefaultVM::Get()) : TableBase(v) {
        sq_pushroottable(vm);
        sq_getstackobj(vm,-1,&obj);
        sq_addref(vm, &obj);
        sq_pop(v,1); // pop root table
    }
};
}

#endif
