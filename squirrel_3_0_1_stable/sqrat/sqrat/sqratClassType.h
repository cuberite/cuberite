
//
// SqratClassType: Type Translators
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
//    1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
//
//    2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
//
//    3. This notice may not be removed or altered from any source
//    distribution.
//

#if !defined(_SCRAT_CLASSTYPE_H_)
#define _SCRAT_CLASSTYPE_H_

#include <squirrel.h>
#include <map>

namespace Sqrat
{

//
// ClassType
//

// Get the Copy Function for this Class
typedef SQInteger (*COPYFUNC)(HSQUIRRELVM, SQInteger, const void*);

struct ClassTypeData {
    bool        initialized;
    HSQOBJECT    classObj;
    HSQOBJECT    getTable;
    HSQOBJECT    setTable;
    COPYFUNC    copyFunc;
    ClassTypeData(): initialized(false) {}
};

template<class C>
struct ClassType {

    static std::map< HSQUIRRELVM, ClassTypeData > s_classTypeDataMap;

    static inline ClassTypeData& getClassTypeData(HSQUIRRELVM vm) {
        //TODO: use mutex to lock s_classTypeDataMap in multithreaded environment
        return s_classTypeDataMap[vm];
    }

    static inline bool hasClassTypeData(HSQUIRRELVM vm) {
        //TODO: use mutex to lock s_classTypeDataMap in multithreaded environment
        return (s_classTypeDataMap.find(vm) != s_classTypeDataMap.end());
    }

    static inline void deleteClassTypeData(HSQUIRRELVM vm) {
        //TODO: use mutex to lock s_classTypeDataMap in multithreaded environment
        std::map< HSQUIRRELVM, ClassTypeData >::iterator it = s_classTypeDataMap.find(vm);
        if(it != s_classTypeDataMap.end()) {
            s_classTypeDataMap.erase(it);
        }
    }

    // Get the Squirrel Object for this Class
    static inline HSQOBJECT& ClassObject(HSQUIRRELVM vm) {
        return getClassTypeData(vm).classObj;
    }

    // Get the Get Table for this Class
    static inline HSQOBJECT& GetTable(HSQUIRRELVM vm) {
        return getClassTypeData(vm).getTable;
    }

    // Get the Set Table for this Class
    static inline HSQOBJECT& SetTable(HSQUIRRELVM vm) {
        return getClassTypeData(vm).setTable;
    }

    static inline COPYFUNC& CopyFunc(HSQUIRRELVM vm) {
        return getClassTypeData(vm).copyFunc;
    }

    static inline bool& Initialized(HSQUIRRELVM vm) {
        return getClassTypeData(vm).initialized;
    }

    static void PushInstance(HSQUIRRELVM vm, C* ptr) {
        sq_pushobject(vm, ClassObject(vm));
        sq_createinstance(vm, -1);
        sq_remove(vm, -2);
        sq_setinstanceup(vm, -1, ptr);
    }

    static void PushInstanceCopy(HSQUIRRELVM vm, C& value) {
        sq_pushobject(vm, ClassObject(vm));
        sq_createinstance(vm, -1);
        sq_remove(vm, -2);
        CopyFunc(vm)(vm, -1, &value);
    }

    static C* GetInstance(HSQUIRRELVM vm, SQInteger idx) {
        C* ptr = NULL;
        sq_getinstanceup(vm, idx, (SQUserPointer*)&ptr, NULL);
        return ptr;
    }
};

template<class C>
std::map< HSQUIRRELVM, ClassTypeData > ClassType<C>::s_classTypeDataMap;

}

#endif
