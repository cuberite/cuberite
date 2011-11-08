#ifndef SQPLUSSETUP_H
#define SQPLUSSETUP_H

// Setup file for SqPlus. 
// Comment / uncomment / define options below.


// Inheritance in Squirrel allows one class to inherit a base class's
// functions and variables.
//
// Variables are merged: if Derived has a var name 'val' and Base has
// a var of the same name, the resulting var 'val' will take Derived's
// initialization value.
//
// Functions are not merged, and can be called via Squirrel scoping rules.
//
// Define SQ_USE_CLASS_INHERITANCE to enable class inheritance support
// (requires slightly more memory and adds some CPU overhead).
//
// Can also be useful for debugging, as class type information is
// checked before dispatching instance function calls and before
// accessing instance member variables.
#define SQ_USE_CLASS_INHERITANCE


// This is a new C++ template based inheritence scheme that uses no
// additional memory per instance. To support offset between "this"
// pointer in base and derived class, use a second class in:
//    SQClassDef< MyClass, MyBaseClass >.
// NOTE: For new code SQ_USE_CLASS_INHERITANCE_SIMPLE is more
// efficient than above method.
//#define SQ_USE_CLASS_INHERITANCE_SIMPLE

// === Instance type info support ===
#define SQ_SUPPORT_INSTANCE_TYPE_INFO

// === Constant argument and constant member function support ===
// Define SQPLUS_CONST_OPT before including SqPlus.h for constant
// argument + constant member function support.
#define SQPLUS_CONST_OPT


// === Uncomment to support smart pointer ===
// Define SQPLUS_SMARTPOINTER_OPT before including SqPlus.h for
// smartpointer member function + variable support
//#define SQPLUS_SMARTPOINTER_OPT


// === Function overloading support ===
#define SQPLUS_OVERLOAD_OPT

// === Uncomment to support std::string ===
// Requires that Squirrel is compiled with SQChar == char
//#define SQPLUS_SUPPORT_STD_STRING

// === Uncomment to support typedef std::basic_string<SQChar> sq_std_string ===
#define SQPLUS_SUPPORT_SQ_STD_STRING

// === Uncomment to support NULL INSTANCE arguments ===
#define SQPLUS_SUPPORT_NULL_INSTANCES

// === Uncomment to disable copying of class instances ===
// If classes being exposed have private or protected constructors 
// one cannot do assign (=) in template functions.
//#define SQPLUS_DISABLE_COPY_INSTANCES


// === Auto generating typemasks for registered functions  ===
// This is useful when using Squirrel interactively
//#define SQPLUS_ENABLE_AUTO_TYPEMASK

// === Uncomment to generate a typeof function for each class  ===
// This is mostly for displaying function help from inside a Squirrel prompt
//#define SQPLUS_ENABLE_TYPEOF

// === Uncomment to skip sq_argassert() ===
//#define SQ_SKIP_ARG_ASSERT

// === GCC Inline template fix ===
// Can solve problems when linking if GCC has problems with inline functions
//#define GCC_INLINE_WORKAROUND


// === MSVC cdecl member functions ===
// Enable binding member functions with __cdecl calling convention under MSVC. 
// Mostly useful when linking with binaries from other compilers. 
#ifdef _MSC_VER
//#define SQPLUS_ENABLE_CDECL_MEMBER_FUNCTIONS
#endif 


// === Generic Push/Pop/Match handlers ===
// This enables using a 'fallback' set of Push/Get/Match that operates
// on class level (not on template level). This opens for handling all
// members of a certain class hierarchy using the same function
// GenPush/GenGet/GenMatch overrides (provided the hierarchy has some
// run-time type mechanism to use). They are used whenever an exact
// template match is not found.
//#define SQPLUS_USE_GENERIC_HANDLERS


// === Sandbox VM ===
// This enables giving one VM the role of 'sandbox' VM where scripts
// of unknown origin may execute and not have access to any SqPlus
// registered functions.  All normal VMs have full access to script
// functions, but the one given in SetSandboxVm() can only use std
// Squirrel functions.
//#define SQPLUS_USE_SANDBOX_VM


#if defined(SQPLUS_SUPPORT_STD_STRING) || defined(SQPLUS_SUPPORT_SQ_STD_STRING)
# include <string>
#endif


// === Conversion of arguments from opposite char type ( char <=> wchar_t ) === 
// Converts strings to opposite encoding in Push/Get handlers (UTF8 / Latin1)
#define SQPLUS_AUTOCONVERT_OTHER_CHAR

// === Whether 8 bit chars are treated as Latin1(1) or UTF8(0) ===
// (leave undefined for undefined for automatic handling)
#define SQPLUS_USE_LATIN1 1

// === Include system library bindings when creating VM:s in SquirrelVM::Init() ===
// Define to bind to system lib when VM:s are created automatically in 
// SquirrelVM::Init()
//#define SQPLUS_SQUIRRELVM_WITH_SYSTEMLIB

#endif // SQPLUSSETUP_H
