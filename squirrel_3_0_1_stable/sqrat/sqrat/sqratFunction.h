//
// SqObject: Referenced Squirrel Object Wrapper
//

//
// Copyright (c) 2009 Brandon Jones
// Copyirght 2011 Li-Cheng (Andy) Tai
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

#if !defined(_SCRAT_SQFUNC_H_)
#define _SCRAT_SQFUNC_H_

#include <squirrel.h>
#include "sqratObject.h"

namespace Sqrat {

class Function  {
    friend class TableBase;
    friend class Table;
    friend class ArrayBase;
    friend struct Var<Function>;
private:
    HSQUIRRELVM vm;
    HSQOBJECT env, obj;

    Function(HSQUIRRELVM v, HSQOBJECT e, HSQOBJECT o) : vm(v), env(e), obj(o) {
        sq_addref(vm, &env);
        sq_addref(vm, &obj);
    }

public:
    Function() {
        sq_resetobject(&env);
        sq_resetobject(&obj);
    }

    Function(const Function& sf) : vm(sf.vm), env(sf.env), obj(sf.obj) {
        sq_addref(vm, &env);
        sq_addref(vm, &obj);
    }

    Function(const Object& e, const SQChar* slot) : vm(e.GetVM()), env(e.GetObject()) {
        sq_addref(vm, &env);
        Object so = e.GetSlot(slot);
        obj = so.GetObject();
        sq_addref(vm, &obj);
    }

    ~Function() {
        Release();
    }

    Function& operator=(const Function& sf) {
        Release();
        vm = sf.vm;
        env = sf.env;
        obj = sf.obj;
        sq_addref(vm, &env);
        sq_addref(vm, &obj);
        return *this;
    }

    bool IsNull() {
        return sq_isnull(obj);
    }

    HSQOBJECT& GetEnv() {
        return env;
    }

    HSQOBJECT& GetFunc() {
        return obj;
    }

    HSQUIRRELVM& GetVM() {
        return vm;
    }

    void Release() {
        if(!IsNull()) {
            sq_release(vm, &env);
            sq_release(vm, &obj);
            sq_resetobject(&env);
            sq_resetobject(&obj);
        }
    }

    template <class R>
    R Evaluate() {
        sq_pushobject(vm, obj);
        sq_pushobject(vm, env);

        sq_call(vm, 1, true, ErrorHandling::IsEnabled());
        R ret = Var<R>(vm, -1).value;
        sq_pop(vm, 2);
        return ret;
    }

    template <class R, class A1>
    R Evaluate(A1 a1) {
        sq_pushobject(vm, obj);
        sq_pushobject(vm, env);

        PushVar(vm, a1);

        sq_call(vm, 2, true, ErrorHandling::IsEnabled());
        Var<R> ret(vm, -1);
        sq_pop(vm, 2);
        return ret.value;
    }

    template <class R, class A1, class A2>
    R Evaluate(A1 a1, A2 a2) {
        sq_pushobject(vm, obj);
        sq_pushobject(vm, env);

        PushVar(vm, a1);
        PushVar(vm, a2);

        sq_call(vm, 3, true, ErrorHandling::IsEnabled());
        R ret = Var<R>(vm, -1).value;
        sq_pop(vm, 2);
        return ret;
    }

    template <class R, class A1, class A2, class A3>
    R Evaluate(A1 a1, A2 a2, A3 a3) {
        sq_pushobject(vm, obj);
        sq_pushobject(vm, env);

        PushVar(vm, a1);
        PushVar(vm, a2);
        PushVar(vm, a3);

        sq_call(vm, 4, true, ErrorHandling::IsEnabled());
        R ret = Var<R>(vm, -1).value;
        sq_pop(vm, 2);
        return ret;
    }

    template <class R, class A1, class A2, class A3, class A4>
    R Evaluate(A1 a1, A2 a2, A3 a3, A4 a4) {
        sq_pushobject(vm, obj);
        sq_pushobject(vm, env);

        PushVar(vm, a1);
        PushVar(vm, a2);
        PushVar(vm, a3);
        PushVar(vm, a4);

        sq_call(vm, 5, true, ErrorHandling::IsEnabled());
        R ret = Var<R>(vm, -1).value;
        sq_pop(vm, 2);
        return ret;
    }


    template <class R, class A1, class A2, class A3, class A4, class A5>
    R Evaluate(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5) {
        sq_pushobject(vm, obj);
        sq_pushobject(vm, env);

        PushVar(vm, a1);
        PushVar(vm, a2);
        PushVar(vm, a3);
        PushVar(vm, a4);
        PushVar(vm, a5);

        sq_call(vm, 6, true, ErrorHandling::IsEnabled());
        R ret = Var<R>(vm, -1).value;
        sq_pop(vm, 2);
        return ret;
    }

    template <class R, class A1, class A2, class A3, class A4, class A5, class A6>
    R Evaluate(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6) {
        sq_pushobject(vm, obj);
        sq_pushobject(vm, env);

        PushVar(vm, a1);
        PushVar(vm, a2);
        PushVar(vm, a3);
        PushVar(vm, a4);
        PushVar(vm, a5);
        PushVar(vm, a6);

        sq_call(vm, 7, true, ErrorHandling::IsEnabled());
        R ret = Var<R>(vm, -1).value;
        sq_pop(vm, 2);
        return ret;
    }

    template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7>
    R Evaluate(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7) {
        sq_pushobject(vm, obj);
        sq_pushobject(vm, env);

        PushVar(vm, a1);
        PushVar(vm, a2);
        PushVar(vm, a3);
        PushVar(vm, a4);
        PushVar(vm, a5);
        PushVar(vm, a6);
        PushVar(vm, a7);

        sq_call(vm, 8, true, ErrorHandling::IsEnabled());
        R ret = Var<R>(vm, -1).value;
        sq_pop(vm, 2);
        return ret;
    }

    template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8>
    R Evaluate(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8) {
        sq_pushobject(vm, obj);
        sq_pushobject(vm, env);

        PushVar(vm, a1);
        PushVar(vm, a2);
        PushVar(vm, a3);
        PushVar(vm, a4);
        PushVar(vm, a5);
        PushVar(vm, a6);
        PushVar(vm, a7);
        PushVar(vm, a8);

        sq_call(vm, 9, true, ErrorHandling::IsEnabled());
        R ret = Var<R>(vm, -1).value;
        sq_pop(vm, 2);
        return ret;
    }

    template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9>
    R Evaluate(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9) {
        sq_pushobject(vm, obj);
        sq_pushobject(vm, env);

        PushVar(vm, a1);
        PushVar(vm, a2);
        PushVar(vm, a3);
        PushVar(vm, a4);
        PushVar(vm, a5);
        PushVar(vm, a6);
        PushVar(vm, a7);
        PushVar(vm, a8);
        PushVar(vm, a9);

        sq_call(vm, 10, true, ErrorHandling::IsEnabled());
        R ret = Var<R>(vm, -1).value;
        sq_pop(vm, 2);
        return ret;
    }

    template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10>
    R Evaluate(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10) {
        sq_pushobject(vm, obj);
        sq_pushobject(vm, env);

        PushVar(vm, a1);
        PushVar(vm, a2);
        PushVar(vm, a3);
        PushVar(vm, a4);
        PushVar(vm, a5);
        PushVar(vm, a6);
        PushVar(vm, a7);
        PushVar(vm, a8);
        PushVar(vm, a9);
        PushVar(vm, a10);

        sq_call(vm, 11, true, ErrorHandling::IsEnabled());
        R ret = Var<R>(vm, -1).value;
        sq_pop(vm, 2);
        return ret;
    }

    template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11>
    R Evaluate(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11) {
        sq_pushobject(vm, obj);
        sq_pushobject(vm, env);

        PushVar(vm, a1);
        PushVar(vm, a2);
        PushVar(vm, a3);
        PushVar(vm, a4);
        PushVar(vm, a5);
        PushVar(vm, a6);
        PushVar(vm, a7);
        PushVar(vm, a8);
        PushVar(vm, a9);
        PushVar(vm, a10);
        PushVar(vm, a11);

        sq_call(vm, 12, true, ErrorHandling::IsEnabled());
        R ret = Var<R>(vm, -1).value;
        sq_pop(vm, 2);
        return ret;
    }

    template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12>
    R Evaluate(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12) {
        sq_pushobject(vm, obj);
        sq_pushobject(vm, env);

        PushVar(vm, a1);
        PushVar(vm, a2);
        PushVar(vm, a3);
        PushVar(vm, a4);
        PushVar(vm, a5);
        PushVar(vm, a6);
        PushVar(vm, a7);
        PushVar(vm, a8);
        PushVar(vm, a9);
        PushVar(vm, a10);
        PushVar(vm, a11);
        PushVar(vm, a12);

        sq_call(vm, 13, true, ErrorHandling::IsEnabled());
        R ret = Var<R>(vm, -1).value;
        sq_pop(vm, 2);
        return ret;
    }

    template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13>
    R Evaluate(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13) {
        sq_pushobject(vm, obj);
        sq_pushobject(vm, env);

        PushVar(vm, a1);
        PushVar(vm, a2);
        PushVar(vm, a3);
        PushVar(vm, a4);
        PushVar(vm, a5);
        PushVar(vm, a6);
        PushVar(vm, a7);
        PushVar(vm, a8);
        PushVar(vm, a9);
        PushVar(vm, a10);
        PushVar(vm, a11);
        PushVar(vm, a12);
        PushVar(vm, a13);

        sq_call(vm, 14, true, ErrorHandling::IsEnabled());
        R ret = Var<R>(vm, -1).value;
        sq_pop(vm, 2);
        return ret;
    }

    template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14>
    R Evaluate(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14) {
        sq_pushobject(vm, obj);
        sq_pushobject(vm, env);

        PushVar(vm, a1);
        PushVar(vm, a2);
        PushVar(vm, a3);
        PushVar(vm, a4);
        PushVar(vm, a5);
        PushVar(vm, a6);
        PushVar(vm, a7);
        PushVar(vm, a8);
        PushVar(vm, a9);
        PushVar(vm, a10);
        PushVar(vm, a11);
        PushVar(vm, a12);
        PushVar(vm, a13);
        PushVar(vm, a14);

        sq_call(vm, 15, true, ErrorHandling::IsEnabled());
        R ret = Var<R>(vm, -1).value;
        sq_pop(vm, 2);
        return ret;
    }

    //
    // void returns
    //

    void Execute() {
        sq_pushobject(vm, obj);
        sq_pushobject(vm, env);

        sq_call(vm, 1, false, ErrorHandling::IsEnabled());
        sq_pop(vm, 1);
    }

    template <class A1>
    void Execute(A1 a1) {
        sq_pushobject(vm, obj);
        sq_pushobject(vm, env);

        PushVar(vm, a1);

        sq_call(vm, 2, false, ErrorHandling::IsEnabled());
        sq_pop(vm, 1);
    }

    template <class A1, class A2>
    void Execute(A1 a1, A2 a2) {
        sq_pushobject(vm, obj);
        sq_pushobject(vm, env);

        PushVar(vm, a1);
        PushVar(vm, a2);

        sq_call(vm, 3, false, ErrorHandling::IsEnabled());
        sq_pop(vm, 1);
    }

    template <class A1, class A2, class A3>
    void Execute(A1 a1, A2 a2, A3 a3) {
        sq_pushobject(vm, obj);
        sq_pushobject(vm, env);

        PushVar(vm, a1);
        PushVar(vm, a2);
        PushVar(vm, a3);

        sq_call(vm, 4, false, ErrorHandling::IsEnabled());
        sq_pop(vm, 1);
    }

    template <class A1, class A2, class A3, class A4>
    void Execute(A1 a1, A2 a2, A3 a3, A4 a4) {
        sq_pushobject(vm, obj);
        sq_pushobject(vm, env);

        PushVar(vm, a1);
        PushVar(vm, a2);
        PushVar(vm, a3);
        PushVar(vm, a4);

        sq_call(vm, 5, false, ErrorHandling::IsEnabled());
        sq_pop(vm, 1);
    }


    template <class A1, class A2, class A3, class A4, class A5>
    void Execute(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5) {
        sq_pushobject(vm, obj);
        sq_pushobject(vm, env);

        PushVar(vm, a1);
        PushVar(vm, a2);
        PushVar(vm, a3);
        PushVar(vm, a4);
        PushVar(vm, a5);

        sq_call(vm, 6, false, ErrorHandling::IsEnabled());
        sq_pop(vm, 1);
    }

    template <class A1, class A2, class A3, class A4, class A5, class A6>
    void Execute(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6) {
        sq_pushobject(vm, obj);
        sq_pushobject(vm, env);

        PushVar(vm, a1);
        PushVar(vm, a2);
        PushVar(vm, a3);
        PushVar(vm, a4);
        PushVar(vm, a5);
        PushVar(vm, a6);

        sq_call(vm, 7, false, ErrorHandling::IsEnabled());
        sq_pop(vm, 1);
    }

    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7>
    void Execute(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7) {
        sq_pushobject(vm, obj);
        sq_pushobject(vm, env);

        PushVar(vm, a1);
        PushVar(vm, a2);
        PushVar(vm, a3);
        PushVar(vm, a4);
        PushVar(vm, a5);
        PushVar(vm, a6);
        PushVar(vm, a7);

        sq_call(vm, 8, false, ErrorHandling::IsEnabled());
        sq_pop(vm, 1);
    }

    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8>
    void Execute(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8) {
        sq_pushobject(vm, obj);
        sq_pushobject(vm, env);

        PushVar(vm, a1);
        PushVar(vm, a2);
        PushVar(vm, a3);
        PushVar(vm, a4);
        PushVar(vm, a5);
        PushVar(vm, a6);
        PushVar(vm, a7);
        PushVar(vm, a8);

        sq_call(vm, 9, false, ErrorHandling::IsEnabled());
        sq_pop(vm, 1);
    }

    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9>
    void Execute(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9) {
        sq_pushobject(vm, obj);
        sq_pushobject(vm, env);

        PushVar(vm, a1);
        PushVar(vm, a2);
        PushVar(vm, a3);
        PushVar(vm, a4);
        PushVar(vm, a5);
        PushVar(vm, a6);
        PushVar(vm, a7);
        PushVar(vm, a8);
        PushVar(vm, a9);

        sq_call(vm, 10, false, ErrorHandling::IsEnabled());
        sq_pop(vm, 1);
    }

    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10>
    void Execute(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10) {
        sq_pushobject(vm, obj);
        sq_pushobject(vm, env);

        PushVar(vm, a1);
        PushVar(vm, a2);
        PushVar(vm, a3);
        PushVar(vm, a4);
        PushVar(vm, a5);
        PushVar(vm, a6);
        PushVar(vm, a7);
        PushVar(vm, a8);
        PushVar(vm, a9);
        PushVar(vm, a10);

        sq_call(vm, 11, false, ErrorHandling::IsEnabled());
        sq_pop(vm, 1);
    }

    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11>
    void Execute(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11) {
        sq_pushobject(vm, obj);
        sq_pushobject(vm, env);

        PushVar(vm, a1);
        PushVar(vm, a2);
        PushVar(vm, a3);
        PushVar(vm, a4);
        PushVar(vm, a5);
        PushVar(vm, a6);
        PushVar(vm, a7);
        PushVar(vm, a8);
        PushVar(vm, a9);
        PushVar(vm, a10);
        PushVar(vm, a11);

        sq_call(vm, 12, false, ErrorHandling::IsEnabled());
        sq_pop(vm, 1);
    }

    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12>
    void Execute(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12) {
        sq_pushobject(vm, obj);
        sq_pushobject(vm, env);

        PushVar(vm, a1);
        PushVar(vm, a2);
        PushVar(vm, a3);
        PushVar(vm, a4);
        PushVar(vm, a5);
        PushVar(vm, a6);
        PushVar(vm, a7);
        PushVar(vm, a8);
        PushVar(vm, a9);
        PushVar(vm, a10);
        PushVar(vm, a11);
        PushVar(vm, a12);

        sq_call(vm, 13, false, ErrorHandling::IsEnabled());
        sq_pop(vm, 1);
    }

    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13>
    void Execute(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13) {
        sq_pushobject(vm, obj);
        sq_pushobject(vm, env);

        PushVar(vm, a1);
        PushVar(vm, a2);
        PushVar(vm, a3);
        PushVar(vm, a4);
        PushVar(vm, a5);
        PushVar(vm, a6);
        PushVar(vm, a7);
        PushVar(vm, a8);
        PushVar(vm, a9);
        PushVar(vm, a10);
        PushVar(vm, a11);
        PushVar(vm, a12);
        PushVar(vm, a13);

        sq_call(vm, 14, false, ErrorHandling::IsEnabled());
        sq_pop(vm, 1);
    }

    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14>
    void Execute(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14) {
        sq_pushobject(vm, obj);
        sq_pushobject(vm, env);

        PushVar(vm, a1);
        PushVar(vm, a2);
        PushVar(vm, a3);
        PushVar(vm, a4);
        PushVar(vm, a5);
        PushVar(vm, a6);
        PushVar(vm, a7);
        PushVar(vm, a8);
        PushVar(vm, a9);
        PushVar(vm, a10);
        PushVar(vm, a11);
        PushVar(vm, a12);
        PushVar(vm, a13);
        PushVar(vm, a14);

        sq_call(vm, 15, false, ErrorHandling::IsEnabled());
        sq_pop(vm, 1);
    }

    //
    // Operator overloads for ease of use (calls Execute)
    //

    void operator()() {
        Execute();
    }

    template <class A1>
    void operator()(A1 a1) {
        Execute(a1);
    }

    template <class A1, class A2>
    void operator()(A1 a1, A2 a2) {
        Execute(a1, a2);
    }

    template <class A1, class A2, class A3>
    void operator()(A1 a1, A2 a2, A3 a3) {
        Execute(a1, a2, a3);
    }

    template <class A1, class A2, class A3, class A4>
    void operator()(A1 a1, A2 a2, A3 a3, A4 a4) {
        Execute(a1, a2, a3, a4);
    }


    template <class A1, class A2, class A3, class A4, class A5>
    void operator()(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5) {
        Execute(a1, a2, a3, a4, a5);
    }

    template <class A1, class A2, class A3, class A4, class A5, class A6>
    void operator()(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6) {
        Execute(a1, a2, a3, a4, a5, a6);
    }

    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7>
    void operator()(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7) {
        Execute(a1, a2, a3, a4, a5, a6, a7);
    }

    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8>
    void operator()(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8) {
        Execute(a1, a2, a3, a4, a5, a6, a7, a8);
    }

    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9>
    void operator()(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9) {
        Execute(a1, a2, a3, a4, a5, a6, a7, a8, a9);
    }

    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10>
    void operator()(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10) {
        Execute(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);
    }

    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11>
    void operator()(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11) {
        Execute(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11);
    }

    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12>
    void operator()(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12) {
        Execute(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12);
    }

    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13>
    void operator()(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13) {
        Execute(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13);
    }

    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14>
    void operator()(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14) {
        Execute(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14);
    }
};

//
// Overridden Getter/Setter
//

template<>
struct Var<Function> {
    Function value;
    Var(HSQUIRRELVM vm, SQInteger idx) {
        HSQOBJECT sqEnv;
        HSQOBJECT sqValue;
        sq_getstackobj(vm, 1, &sqEnv);
        sq_getstackobj(vm, idx, &sqValue);
        value = Function(vm, sqEnv, sqValue);
    }
    static void push(HSQUIRRELVM vm, Function& value) {
        sq_pushobject(vm, value.GetFunc());
    }
};
}

#endif
