//
// SqratAllocator: Custom Class Allocation/Deallocation
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

#if !defined(_SCRAT_ALLOCATOR_H_)
#define _SCRAT_ALLOCATOR_H_

#include <squirrel.h>
#include <string.h>

#include "sqratObject.h"

namespace Sqrat {

//
// DefaultAllocator
//

template<class C>
class DefaultAllocator {
public:
    static SQInteger New(HSQUIRRELVM vm) {
        C* instance = new C();
        sq_setinstanceup(vm, 1, instance);
        sq_setreleasehook(vm, 1, &Delete);
        return 0;
    }

    static SQInteger Copy(HSQUIRRELVM vm, SQInteger idx, const void* value) {
        C* instance = new C(*static_cast<const C*>(value));
        sq_setinstanceup(vm, idx, instance);
        sq_setreleasehook(vm, idx, &Delete);
        return 0;
    }

    static SQInteger Delete(SQUserPointer ptr, SQInteger size) {
        C* instance = reinterpret_cast<C*>(ptr);
        delete instance;
        return 0;
    }
};

//
// NoConstructorAllocator
//

class NoConstructor {
public:
    static SQInteger New(HSQUIRRELVM) {
        return 0;
    }
    static SQInteger Copy(HSQUIRRELVM, SQInteger, const void*) {
        return 0;
    }
    static SQInteger Delete(SQUserPointer, SQInteger) {
        return 0;
    }
};

//
// CopyOnly
//

template<class C>
class CopyOnly {
public:
    static SQInteger New(HSQUIRRELVM) {
        return 0;
    }
    static SQInteger Copy(HSQUIRRELVM vm, SQInteger idx, const void* value) {
        C* instance = new C(*static_cast<const C*>(value));
        sq_setinstanceup(vm, idx, instance);
        sq_setreleasehook(vm, idx, &Delete);
        return 0;
    }
    static SQInteger Delete(SQUserPointer ptr, SQInteger size) {
        void* instance = reinterpret_cast<C*>(ptr);
        delete instance;
        return 0;
    }
};


//
// NoCopy
//

template<class C>
class NoCopy {
public:
    static SQInteger New(HSQUIRRELVM vm) {
        C* instance = new C();
        sq_setinstanceup(vm, 1, instance);
        sq_setreleasehook(vm, 1, &Delete);
        return 0;
    }

    static SQInteger Copy(HSQUIRRELVM vm, SQInteger idx, const void* value) {
        return 0;
    }

    static SQInteger Delete(SQUserPointer ptr, SQInteger size) {
        C* instance = reinterpret_cast<C*>(ptr);
        delete instance;
        return 0;
    }
};

}

#endif
