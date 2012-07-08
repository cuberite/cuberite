//
// SqratGlobalMethods: Global Methods
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

#if !defined(_SCRAT_GLOBAL_METHODS_H_)
#define _SCRAT_GLOBAL_METHODS_H_

#include <squirrel.h>
#include "sqratTypes.h"

namespace Sqrat {

//
// Squirrel Global Functions
//

template <class R>
class SqGlobal {
public:
    // Arg Count 0
    static SQInteger Func0(HSQUIRRELVM vm) {
        typedef R (*M)();
        M* method;
        sq_getuserdata(vm, -1, (SQUserPointer*)&method, NULL);

        R ret = (*method)();

        PushVar(vm, ret);
        return 1;
    }


    // Arg Count 1
    template <class A1, SQInteger startIdx>
    static SQInteger Func1(HSQUIRRELVM vm) {
        typedef R (*M)(A1);
        M* method;
        sq_getuserdata(vm, -1, (SQUserPointer*)&method, NULL);

        R ret = (*method)(
                    Var<A1>(vm, startIdx).value
                );

        PushVar(vm, ret);
        return 1;
    }

    // Arg Count 2
    template <class A1, class A2, SQInteger startIdx>
    static SQInteger Func2(HSQUIRRELVM vm) {
        typedef R (*M)(A1, A2);
        M* method;
        sq_getuserdata(vm, -1, (SQUserPointer*)&method, NULL);

        R ret = (*method)(
                    Var<A1>(vm, startIdx).value,
                    Var<A2>(vm, startIdx + 1).value
                );

        PushVar(vm, ret);
        return 1;
    }

    // Arg Count 3
    template <class A1, class A2, class A3, SQInteger startIdx>
    static SQInteger Func3(HSQUIRRELVM vm) {
        typedef R (*M)(A1, A2, A3);
        M* method;
        sq_getuserdata(vm, -1, (SQUserPointer*)&method, NULL);

        R ret = (*method)(
                    Var<A1>(vm, startIdx).value,
                    Var<A2>(vm, startIdx + 1).value,
                    Var<A3>(vm, startIdx + 2).value
                );

        PushVar(vm, ret);
        return 1;
    }

    // Arg Count 4
    template <class A1, class A2, class A3, class A4, SQInteger startIdx>
    static SQInteger Func4(HSQUIRRELVM vm) {
        typedef R (*M)(A1, A2, A3, A4);
        M* method;
        sq_getuserdata(vm, -1, (SQUserPointer*)&method, NULL);

        R ret = (*method)(
                    Var<A1>(vm, startIdx).value,
                    Var<A2>(vm, startIdx + 1).value,
                    Var<A3>(vm, startIdx + 2).value,
                    Var<A4>(vm, startIdx + 3).value
                );

        PushVar(vm, ret);
        return 1;
    }

    // Arg Count 5
    template <class A1, class A2, class A3, class A4, class A5, SQInteger startIdx>
    static SQInteger Func5(HSQUIRRELVM vm) {
        typedef R (*M)(A1, A2, A3, A4, A5);
        M* method;
        sq_getuserdata(vm, -1, (SQUserPointer*)&method, NULL);

        R ret = (*method)(
                    Var<A1>(vm, startIdx).value,
                    Var<A2>(vm, startIdx + 1).value,
                    Var<A3>(vm, startIdx + 2).value,
                    Var<A4>(vm, startIdx + 3).value,
                    Var<A5>(vm, startIdx + 4).value
                );

        PushVar(vm, ret);
        return 1;
    }

    // Arg Count 6
    template <class A1, class A2, class A3, class A4, class A5, class A6, SQInteger startIdx>
    static SQInteger Func6(HSQUIRRELVM vm) {
        typedef R (*M)(A1, A2, A3, A4, A5, A6);
        M* method;
        sq_getuserdata(vm, -1, (SQUserPointer*)&method, NULL);

        R ret = (*method)(
                    Var<A1>(vm, startIdx).value,
                    Var<A2>(vm, startIdx + 1).value,
                    Var<A3>(vm, startIdx + 2).value,
                    Var<A4>(vm, startIdx + 3).value,
                    Var<A5>(vm, startIdx + 4).value,
                    Var<A6>(vm, startIdx + 5).value
                );

        PushVar(vm, ret);
        return 1;
    }

    // Arg Count 7
    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, SQInteger startIdx>
    static SQInteger Func7(HSQUIRRELVM vm) {
        typedef R (*M)(A1, A2, A3, A4, A5, A6, A7);
        M* method;
        sq_getuserdata(vm, -1, (SQUserPointer*)&method, NULL);

        R ret = (*method)(
                    Var<A1>(vm, startIdx).value,
                    Var<A2>(vm, startIdx + 1).value,
                    Var<A3>(vm, startIdx + 2).value,
                    Var<A4>(vm, startIdx + 3).value,
                    Var<A5>(vm, startIdx + 4).value,
                    Var<A6>(vm, startIdx + 5).value,
                    Var<A7>(vm, startIdx + 6).value
                );

        PushVar(vm, ret);
        return 1;
    }

    // Arg Count 8
    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, SQInteger startIdx>
    static SQInteger Func8(HSQUIRRELVM vm) {
        typedef R (*M)(A1, A2, A3, A4, A5, A6, A7, A8);
        M* method;
        sq_getuserdata(vm, -1, (SQUserPointer*)&method, NULL);

        R ret = (*method)(
                    Var<A1>(vm, startIdx).value,
                    Var<A2>(vm, startIdx + 1).value,
                    Var<A3>(vm, startIdx + 2).value,
                    Var<A4>(vm, startIdx + 3).value,
                    Var<A5>(vm, startIdx + 4).value,
                    Var<A6>(vm, startIdx + 5).value,
                    Var<A7>(vm, startIdx + 6).value,
                    Var<A8>(vm, startIdx + 7).value
                );

        PushVar(vm, ret);
        return 1;
    }

    // Arg Count 9
    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, SQInteger startIdx>
    static SQInteger Func9(HSQUIRRELVM vm) {
        typedef R (*M)(A1, A2, A3, A4, A5, A6, A7, A8, A9);
        M* method;
        sq_getuserdata(vm, -1, (SQUserPointer*)&method, NULL);

        R ret = (*method)(
                    Var<A1>(vm, startIdx).value,
                    Var<A2>(vm, startIdx + 1).value,
                    Var<A3>(vm, startIdx + 2).value,
                    Var<A4>(vm, startIdx + 3).value,
                    Var<A5>(vm, startIdx + 4).value,
                    Var<A6>(vm, startIdx + 5).value,
                    Var<A7>(vm, startIdx + 6).value,
                    Var<A8>(vm, startIdx + 7).value,
                    Var<A9>(vm, startIdx + 8).value
                );

        PushVar(vm, ret);
        return 1;
    }

    // Arg Count 10
    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, SQInteger startIdx>
    static SQInteger Func10(HSQUIRRELVM vm) {
        typedef R (*M)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10);
        M* method;
        sq_getuserdata(vm, -1, (SQUserPointer*)&method, NULL);

        R ret = (*method)(
                    Var<A1>(vm, startIdx).value,
                    Var<A2>(vm, startIdx + 1).value,
                    Var<A3>(vm, startIdx + 2).value,
                    Var<A4>(vm, startIdx + 3).value,
                    Var<A5>(vm, startIdx + 4).value,
                    Var<A6>(vm, startIdx + 5).value,
                    Var<A7>(vm, startIdx + 6).value,
                    Var<A8>(vm, startIdx + 7).value,
                    Var<A9>(vm, startIdx + 8).value,
                    Var<A10>(vm, startIdx + 9).value
                );

        PushVar(vm, ret);
        return 1;
    }

    // Arg Count 11
    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, SQInteger startIdx>
    static SQInteger Func11(HSQUIRRELVM vm) {
        typedef R (*M)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11);
        M* method;
        sq_getuserdata(vm, -1, (SQUserPointer*)&method, NULL);

        R ret = (*method)(
                    Var<A1>(vm, startIdx).value,
                    Var<A2>(vm, startIdx + 1).value,
                    Var<A3>(vm, startIdx + 2).value,
                    Var<A4>(vm, startIdx + 3).value,
                    Var<A5>(vm, startIdx + 4).value,
                    Var<A6>(vm, startIdx + 5).value,
                    Var<A7>(vm, startIdx + 6).value,
                    Var<A8>(vm, startIdx + 7).value,
                    Var<A9>(vm, startIdx + 8).value,
                    Var<A10>(vm, startIdx + 9).value,
                    Var<A11>(vm, startIdx + 10).value
                );

        PushVar(vm, ret);
        return 1;
    }

    // Arg Count 12
    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, SQInteger startIdx>
    static SQInteger Func12(HSQUIRRELVM vm) {
        typedef R (*M)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12);
        M* method;
        sq_getuserdata(vm, -1, (SQUserPointer*)&method, NULL);

        R ret = (*method)(
                    Var<A1>(vm, startIdx).value,
                    Var<A2>(vm, startIdx + 1).value,
                    Var<A3>(vm, startIdx + 2).value,
                    Var<A4>(vm, startIdx + 3).value,
                    Var<A5>(vm, startIdx + 4).value,
                    Var<A6>(vm, startIdx + 5).value,
                    Var<A7>(vm, startIdx + 6).value,
                    Var<A8>(vm, startIdx + 7).value,
                    Var<A9>(vm, startIdx + 8).value,
                    Var<A10>(vm, startIdx + 9).value,
                    Var<A11>(vm, startIdx + 10).value,
                    Var<A12>(vm, startIdx + 11).value
                );

        PushVar(vm, ret);
        return 1;
    }

    // Arg Count 13
    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, SQInteger startIdx>
    static SQInteger Func13(HSQUIRRELVM vm) {
        typedef R (*M)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13);
        M* method;
        sq_getuserdata(vm, -1, (SQUserPointer*)&method, NULL);

        R ret = (*method)(
                    Var<A1>(vm, startIdx).value,
                    Var<A2>(vm, startIdx + 1).value,
                    Var<A3>(vm, startIdx + 2).value,
                    Var<A4>(vm, startIdx + 3).value,
                    Var<A5>(vm, startIdx + 4).value,
                    Var<A6>(vm, startIdx + 5).value,
                    Var<A7>(vm, startIdx + 6).value,
                    Var<A8>(vm, startIdx + 7).value,
                    Var<A9>(vm, startIdx + 8).value,
                    Var<A10>(vm, startIdx + 9).value,
                    Var<A11>(vm, startIdx + 10).value,
                    Var<A12>(vm, startIdx + 11).value,
                    Var<A13>(vm, startIdx + 12).value
                );

        PushVar(vm, ret);
        return 1;
    }

    // Arg Count 14
    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, SQInteger startIdx>
    static SQInteger Func14(HSQUIRRELVM vm) {
        typedef R (*M)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14);
        M* method;
        sq_getuserdata(vm, -1, (SQUserPointer*)&method, NULL);

        R ret = (*method)(
                    Var<A1>(vm, startIdx).value,
                    Var<A2>(vm, startIdx + 1).value,
                    Var<A3>(vm, startIdx + 2).value,
                    Var<A4>(vm, startIdx + 3).value,
                    Var<A5>(vm, startIdx + 4).value,
                    Var<A6>(vm, startIdx + 5).value,
                    Var<A7>(vm, startIdx + 6).value,
                    Var<A8>(vm, startIdx + 7).value,
                    Var<A9>(vm, startIdx + 8).value,
                    Var<A10>(vm, startIdx + 9).value,
                    Var<A11>(vm, startIdx + 10).value,
                    Var<A12>(vm, startIdx + 11).value,
                    Var<A13>(vm, startIdx + 12).value,
                    Var<A14>(vm, startIdx + 13).value
                );

        PushVar(vm, ret);
        return 1;
    }
};

//
// void return specialization
//

template <>
class SqGlobal<void> {
public:
    // Arg Count 0
    static SQInteger Func0(HSQUIRRELVM vm) {
        typedef void (*M)();
        M* method;
        sq_getuserdata(vm, -1, (SQUserPointer*)&method, NULL);
        (*method)();
        return 0;
    }

    // Arg Count 1
    template <class A1, SQInteger startIdx>
    static SQInteger Func1(HSQUIRRELVM vm) {
        typedef void (*M)(A1);
        M* method;
        sq_getuserdata(vm, -1, (SQUserPointer*)&method, NULL);

        (*method)(
            Var<A1>(vm, startIdx).value
        );
        return 0;
    }

    // Arg Count 2
    template <class A1, class A2, SQInteger startIdx>
    static SQInteger Func2(HSQUIRRELVM vm) {
        typedef void (*M)(A1, A2);
        M* method;
        sq_getuserdata(vm, -1, (SQUserPointer*)&method, NULL);

        (*method)(
            Var<A1>(vm, startIdx).value,
            Var<A2>(vm, startIdx + 1).value
        );
        return 0;
    }

    // Arg Count 3
    template <class A1, class A2, class A3, SQInteger startIdx>
    static SQInteger Func3(HSQUIRRELVM vm) {
        typedef void (*M)(A1, A2, A3);
        M* method;
        sq_getuserdata(vm, -1, (SQUserPointer*)&method, NULL);

        (*method)(
            Var<A1>(vm, startIdx).value,
            Var<A2>(vm, startIdx + 1).value,
            Var<A3>(vm, startIdx + 2).value
        );
        return 0;
    }

    // Arg Count 4
    template <class A1, class A2, class A3, class A4, SQInteger startIdx>
    static SQInteger Func4(HSQUIRRELVM vm) {
        typedef void (*M)(A1, A2, A3, A4);
        M* method;
        sq_getuserdata(vm, -1, (SQUserPointer*)&method, NULL);

        (*method)(
            Var<A1>(vm, startIdx).value,
            Var<A2>(vm, startIdx + 1).value,
            Var<A3>(vm, startIdx + 2).value,
            Var<A4>(vm, startIdx + 3).value
        );
        return 0;
    }

    // Arg Count 5
    template <class A1, class A2, class A3, class A4, class A5, SQInteger startIdx>
    static SQInteger Func5(HSQUIRRELVM vm) {
        typedef void (*M)(A1, A2, A3, A4, A5);
        M* method;
        sq_getuserdata(vm, -1, (SQUserPointer*)&method, NULL);

        (*method)(
            Var<A1>(vm, startIdx).value,
            Var<A2>(vm, startIdx + 1).value,
            Var<A3>(vm, startIdx + 2).value,
            Var<A4>(vm, startIdx + 3).value,
            Var<A5>(vm, startIdx + 4).value
        );
        return 0;
    }

    // Arg Count 6
    template <class A1, class A2, class A3, class A4, class A5, class A6, SQInteger startIdx>
    static SQInteger Func6(HSQUIRRELVM vm) {
        typedef void (*M)(A1, A2, A3, A4, A5, A6);
        M* method;
        sq_getuserdata(vm, -1, (SQUserPointer*)&method, NULL);

        (*method)(
            Var<A1>(vm, startIdx).value,
            Var<A2>(vm, startIdx + 1).value,
            Var<A3>(vm, startIdx + 2).value,
            Var<A4>(vm, startIdx + 3).value,
            Var<A5>(vm, startIdx + 4).value,
            Var<A6>(vm, startIdx + 5).value
        );
        return 0;
    }

    // Arg Count 7
    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, SQInteger startIdx>
    static SQInteger Func7(HSQUIRRELVM vm) {
        typedef void (*M)(A1, A2, A3, A4, A5, A6, A7);
        M* method;
        sq_getuserdata(vm, -1, (SQUserPointer*)&method, NULL);

        (*method)(
            Var<A1>(vm, startIdx).value,
            Var<A2>(vm, startIdx + 1).value,
            Var<A3>(vm, startIdx + 2).value,
            Var<A4>(vm, startIdx + 3).value,
            Var<A5>(vm, startIdx + 4).value,
            Var<A6>(vm, startIdx + 5).value,
            Var<A7>(vm, startIdx + 6).value
        );
        return 0;
    }

    // Arg Count 8
    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, SQInteger startIdx>
    static SQInteger Func8(HSQUIRRELVM vm) {
        typedef void (*M)(A1, A2, A3, A4, A5, A6, A7, A8);
        M* method;
        sq_getuserdata(vm, -1, (SQUserPointer*)&method, NULL);

        (*method)(
            Var<A1>(vm, startIdx).value,
            Var<A2>(vm, startIdx + 1).value,
            Var<A3>(vm, startIdx + 2).value,
            Var<A4>(vm, startIdx + 3).value,
            Var<A5>(vm, startIdx + 4).value,
            Var<A6>(vm, startIdx + 5).value,
            Var<A7>(vm, startIdx + 6).value,
            Var<A8>(vm, startIdx + 7).value
        );
        return 0;
    }

    // Arg Count 9
    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, SQInteger startIdx>
    static SQInteger Func9(HSQUIRRELVM vm) {
        typedef void (*M)(A1, A2, A3, A4, A5, A6, A7, A8, A9);
        M* method;
        sq_getuserdata(vm, -1, (SQUserPointer*)&method, NULL);

        (*method)(
            Var<A1>(vm, startIdx).value,
            Var<A2>(vm, startIdx + 1).value,
            Var<A3>(vm, startIdx + 2).value,
            Var<A4>(vm, startIdx + 3).value,
            Var<A5>(vm, startIdx + 4).value,
            Var<A6>(vm, startIdx + 5).value,
            Var<A7>(vm, startIdx + 6).value,
            Var<A8>(vm, startIdx + 7).value,
            Var<A9>(vm, startIdx + 8).value
        );
        return 0;
    }

    // Arg Count 10
    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, SQInteger startIdx>
    static SQInteger Func10(HSQUIRRELVM vm) {
        typedef void (*M)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10);
        M* method;
        sq_getuserdata(vm, -1, (SQUserPointer*)&method, NULL);

        (*method)(
            Var<A1>(vm, startIdx).value,
            Var<A2>(vm, startIdx + 1).value,
            Var<A3>(vm, startIdx + 2).value,
            Var<A4>(vm, startIdx + 3).value,
            Var<A5>(vm, startIdx + 4).value,
            Var<A6>(vm, startIdx + 5).value,
            Var<A7>(vm, startIdx + 6).value,
            Var<A8>(vm, startIdx + 7).value,
            Var<A9>(vm, startIdx + 8).value,
            Var<A10>(vm, startIdx + 9).value
        );
        return 0;
    }

    // Arg Count 11
    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, SQInteger startIdx>
    static SQInteger Func11(HSQUIRRELVM vm) {
        typedef void (*M)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11);
        M* method;
        sq_getuserdata(vm, -1, (SQUserPointer*)&method, NULL);

        (*method)(
            Var<A1>(vm, startIdx).value,
            Var<A2>(vm, startIdx + 1).value,
            Var<A3>(vm, startIdx + 2).value,
            Var<A4>(vm, startIdx + 3).value,
            Var<A5>(vm, startIdx + 4).value,
            Var<A6>(vm, startIdx + 5).value,
            Var<A7>(vm, startIdx + 6).value,
            Var<A8>(vm, startIdx + 7).value,
            Var<A9>(vm, startIdx + 8).value,
            Var<A10>(vm, startIdx + 9).value,
            Var<A11>(vm, startIdx + 10).value
        );
        return 0;
    }

    // Arg Count 12
    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, SQInteger startIdx>
    static SQInteger Func12(HSQUIRRELVM vm) {
        typedef void (*M)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12);
        M* method;
        sq_getuserdata(vm, -1, (SQUserPointer*)&method, NULL);

        (*method)(
            Var<A1>(vm, startIdx).value,
            Var<A2>(vm, startIdx + 1).value,
            Var<A3>(vm, startIdx + 2).value,
            Var<A4>(vm, startIdx + 3).value,
            Var<A5>(vm, startIdx + 4).value,
            Var<A6>(vm, startIdx + 5).value,
            Var<A7>(vm, startIdx + 6).value,
            Var<A8>(vm, startIdx + 7).value,
            Var<A9>(vm, startIdx + 8).value,
            Var<A10>(vm, startIdx + 9).value,
            Var<A11>(vm, startIdx + 10).value,
            Var<A12>(vm, startIdx + 11).value
        );
        return 0;
    }

    // Arg Count 13
    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, SQInteger startIdx>
    static SQInteger Func13(HSQUIRRELVM vm) {
        typedef void (*M)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13);
        M* method;
        sq_getuserdata(vm, -1, (SQUserPointer*)&method, NULL);

        (*method)(
            Var<A1>(vm, startIdx).value,
            Var<A2>(vm, startIdx + 1).value,
            Var<A3>(vm, startIdx + 2).value,
            Var<A4>(vm, startIdx + 3).value,
            Var<A5>(vm, startIdx + 4).value,
            Var<A6>(vm, startIdx + 5).value,
            Var<A7>(vm, startIdx + 6).value,
            Var<A8>(vm, startIdx + 7).value,
            Var<A9>(vm, startIdx + 8).value,
            Var<A10>(vm, startIdx + 9).value,
            Var<A11>(vm, startIdx + 10).value,
            Var<A12>(vm, startIdx + 11).value,
            Var<A13>(vm, startIdx + 12).value
        );
        return 0;
    }

    // Arg Count 14
    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, SQInteger startIdx>
    static SQInteger Func14(HSQUIRRELVM vm) {
        typedef void (*M)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14);
        M* method;
        sq_getuserdata(vm, -1, (SQUserPointer*)&method, NULL);

        (*method)(
            Var<A1>(vm, startIdx).value,
            Var<A2>(vm, startIdx + 1).value,
            Var<A3>(vm, startIdx + 2).value,
            Var<A4>(vm, startIdx + 3).value,
            Var<A5>(vm, startIdx + 4).value,
            Var<A6>(vm, startIdx + 5).value,
            Var<A7>(vm, startIdx + 6).value,
            Var<A8>(vm, startIdx + 7).value,
            Var<A9>(vm, startIdx + 8).value,
            Var<A10>(vm, startIdx + 9).value,
            Var<A11>(vm, startIdx + 10).value,
            Var<A12>(vm, startIdx + 11).value,
            Var<A13>(vm, startIdx + 12).value,
            Var<A14>(vm, startIdx + 13).value
        );
        return 0;
    }
};


//
// Global Function Resolvers
//

// Arg Count 0
template <class R>
SQFUNCTION SqGlobalFunc(R (*method)()) {
    return &SqGlobal<R>::Func0;
}

// Arg Count 1
template <class R, class A1>
SQFUNCTION SqGlobalFunc(R (*method)(A1)) {
    return &SqGlobal<R>::template Func1<A1, 2>;
}

// Arg Count 2
template <class R, class A1, class A2>
SQFUNCTION SqGlobalFunc(R (*method)(A1, A2)) {
    return &SqGlobal<R>::template Func2<A1, A2, 2>;
}

// Arg Count 3
template <class R, class A1, class A2, class A3>
SQFUNCTION SqGlobalFunc(R (*method)(A1, A2, A3)) {
    return &SqGlobal<R>::template Func3<A1, A2, A3, 2>;
}

// Arg Count 4
template <class R, class A1, class A2, class A3, class A4>
SQFUNCTION SqGlobalFunc(R (*method)(A1, A2, A3, A4)) {
    return &SqGlobal<R>::template Func4<A1, A2, A3, A4, 2>;
}

// Arg Count 5
template <class R, class A1, class A2, class A3, class A4, class A5>
SQFUNCTION SqGlobalFunc(R (*method)(A1, A2, A3, A4, A5)) {
    return &SqGlobal<R>::template Func5<A1, A2, A3, A4, A5, 2>;
}

// Arg Count 6
template <class R, class A1, class A2, class A3, class A4, class A5, class A6>
SQFUNCTION SqGlobalFunc(R (*method)(A1, A2, A3, A4, A5, A6)) {
    return &SqGlobal<R>::template Func6<A1, A2, A3, A4, A5, A6, 2>;
}

// Arg Count 7
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7>
SQFUNCTION SqGlobalFunc(R (*method)(A1, A2, A3, A4, A5, A6, A7)) {
    return &SqGlobal<R>::template Func7<A1, A2, A3, A4, A5, A6, A7, 2>;
}

// Arg Count 8
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8>
SQFUNCTION SqGlobalFunc(R (*method)(A1, A2, A3, A4, A5, A6, A7, A8)) {
    return &SqGlobal<R>::template Func8<A1, A2, A3, A4, A5, A6, A7, A8, 2>;
}

// Arg Count 9
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9>
SQFUNCTION SqGlobalFunc(R (*method)(A1, A2, A3, A4, A5, A6, A7, A8, A9)) {
    return &SqGlobal<R>::template Func9<A1, A2, A3, A4, A5, A6, A7, A8, A9, 2>;
}

// Arg Count 10
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10>
SQFUNCTION SqGlobalFunc(R (*method)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10)) {
    return &SqGlobal<R>::template Func10<A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, 2>;
}

// Arg Count 11
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11>
SQFUNCTION SqGlobalFunc(R (*method)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11)) {
    return &SqGlobal<R>::template Func11<A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, 2>;
}

// Arg Count 12
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12>
SQFUNCTION SqGlobalFunc(R (*method)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12)) {
    return &SqGlobal<R>::template Func12<A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, 2>;
}

// Arg Count 13
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13>
SQFUNCTION SqGlobalFunc(R (*method)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13)) {
    return &SqGlobal<R>::template Func13<A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, 2>;
}

// Arg Count 14
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14>
SQFUNCTION SqGlobalFunc(R (*method)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14)) {
    return &SqGlobal<R>::template Func14<A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, 2>;
}

//
// Member Global Function Resolvers
//

// Arg Count 1
template <class R, class A1>
SQFUNCTION SqMemberGlobalFunc(R (*method)(A1)) {
    return &SqGlobal<R>::template Func1<A1, 1>;
}

// Arg Count 2
template <class R, class A1, class A2>
SQFUNCTION SqMemberGlobalFunc(R (*method)(A1, A2)) {
    return &SqGlobal<R>::template Func2<A1, A2, 1>;
}

// Arg Count 3
template <class R, class A1, class A2, class A3>
SQFUNCTION SqMemberGlobalFunc(R (*method)(A1, A2, A3)) {
    return &SqGlobal<R>::template Func3<A1, A2, A3, 1>;
}

// Arg Count 4
template <class R, class A1, class A2, class A3, class A4>
SQFUNCTION SqMemberGlobalFunc(R (*method)(A1, A2, A3, A4)) {
    return &SqGlobal<R>::template Func4<A1, A2, A3, A4, 1>;
}

// Arg Count 5
template <class R, class A1, class A2, class A3, class A4, class A5>
SQFUNCTION SqMemberGlobalFunc(R (*method)(A1, A2, A3, A4, A5)) {
    return &SqGlobal<R>::template Func5<A1, A2, A3, A4, A5, 1>;
}

// Arg Count 6
template <class R, class A1, class A2, class A3, class A4, class A5, class A6>
SQFUNCTION SqMemberGlobalFunc(R (*method)(A1, A2, A3, A4, A5, A6)) {
    return &SqGlobal<R>::template Func6<A1, A2, A3, A4, A5, A6, 1>;
}

// Arg Count 7
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7>
SQFUNCTION SqMemberGlobalFunc(R (*method)(A1, A2, A3, A4, A5, A6, A7)) {
    return &SqGlobal<R>::template Func7<A1, A2, A3, A4, A5, A6, A7, 1>;
}

// Arg Count 8
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8>
SQFUNCTION SqMemberGlobalFunc(R (*method)(A1, A2, A3, A4, A5, A6, A7, A8)) {
    return &SqGlobal<R>::template Func8<A1, A2, A3, A4, A5, A6, A7, A8, 1>;
}

// Arg Count 9
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9>
SQFUNCTION SqMemberGlobalFunc(R (*method)(A1, A2, A3, A4, A5, A6, A7, A8, A9)) {
    return &SqGlobal<R>::template Func9<A1, A2, A3, A4, A5, A6, A7, A8, A9, 1>;
}

// Arg Count 10
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10>
SQFUNCTION SqMemberGlobalFunc(R (*method)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10)) {
    return &SqGlobal<R>::template Func10<A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, 1>;
}

// Arg Count 11
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11>
SQFUNCTION SqMemberGlobalFunc(R (*method)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11)) {
    return &SqGlobal<R>::template Func11<A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, 1>;
}

// Arg Count 12
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12>
SQFUNCTION SqMemberGlobalFunc(R (*method)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12)) {
    return &SqGlobal<R>::template Func12<A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, 1>;
}

// Arg Count 13
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13>
SQFUNCTION SqMemberGlobalFunc(R (*method)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13)) {
    return &SqGlobal<R>::template Func13<A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, 1>;
}

// Arg Count 14
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14>
SQFUNCTION SqMemberGlobalFunc(R (*method)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14)) {
    return &SqGlobal<R>::template Func14<A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, 1>;
}


}

#endif
