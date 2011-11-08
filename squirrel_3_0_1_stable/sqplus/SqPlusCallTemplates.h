
// This file is included multiple times, with varying options - No header guard. 

// Include this file to generate Call templates with or without these options: 
// - SQPLUS_APPLY_CONST - const qualifier after functions signature (const func / member func)
// - SQPLUS_APPLY_CDECL - qualifier before class name (MSVC specific calling convention)

#undef CONST_QUAL
#undef CALL_QUAL 

#ifdef SQPLUS_APPLY_CONST
  #define CONST_QUAL const
#else
  #define CONST_QUAL 
#endif 

#ifdef SQPLUS_APPLY_CDECL
  #define CALL_QUAL __cdecl
#else
  #define CALL_QUAL 
#endif 


#ifdef SQPLUS_CALL_MFUNC_RET0

  // Include this file again, with __cdecl also  (Visual C++ specific)
  #if defined(SQPLUS_ENABLE_CDECL_MEMBER_FUNCTIONS) && !defined(SQPLUS_APPLY_CDECL)
    #define SQPLUS_APPLY_CDECL
    #include "SqPlusCallTemplates.h" 
    #undef CALL_QUAL
	#define CALL_QUAL
  #endif

  template <typename Callee>
  static int Call(Callee & callee,RT (CALL_QUAL Callee::*func)() CONST_QUAL,HSQUIRRELVM v,int /*index*/) {
    RT ret = (callee.*func)();
    Push(v,ret);
    return 1;
  }

  template <typename Callee,typename P1>
  static int Call(Callee & callee,RT (CALL_QUAL Callee::*func)(P1) CONST_QUAL,HSQUIRRELVM v,int index) {
    sq_argassert(1,index + 0);
    RT ret = (callee.*func)(
      Get(TypeWrapper<P1>(),v,index + 0)
    );
    Push(v,ret);
    return 1;
  }

  template<typename Callee,typename P1,typename P2>
  static int Call(Callee & callee,RT (CALL_QUAL Callee::*func)(P1,P2) CONST_QUAL,HSQUIRRELVM v,int index) {
    sq_argassert(1,index + 0);
    sq_argassert(2,index + 1);
    RT ret = (callee.*func)(
      Get(TypeWrapper<P1>(),v,index + 0),
      Get(TypeWrapper<P2>(),v,index + 1)
    );
    Push(v,ret);
    return 1;
  }

  template<typename Callee,typename P1,typename P2,typename P3>
  static int Call(Callee & callee,RT (CALL_QUAL Callee::*func)(P1,P2,P3) CONST_QUAL,HSQUIRRELVM v,int index) {
    sq_argassert(1,index + 0);
    sq_argassert(2,index + 1);
    sq_argassert(3,index + 2);
    RT ret = (callee.*func)(
      Get(TypeWrapper<P1>(),v,index + 0),
      Get(TypeWrapper<P2>(),v,index + 1),
      Get(TypeWrapper<P3>(),v,index + 2)
    );
    Push(v,ret);
    return 1;
  }

  template<typename Callee,typename P1,typename P2,typename P3,typename P4>
  static int Call(Callee & callee,RT (CALL_QUAL Callee::*func)(P1,P2,P3,P4) CONST_QUAL,HSQUIRRELVM v,int index) {
    sq_argassert(1,index + 0);
    sq_argassert(2,index + 1);
    sq_argassert(3,index + 2);
    sq_argassert(4,index + 3);
    RT ret = (callee.*func)(
      Get(TypeWrapper<P1>(),v,index + 0),
      Get(TypeWrapper<P2>(),v,index + 1),
      Get(TypeWrapper<P3>(),v,index + 2),
      Get(TypeWrapper<P4>(),v,index + 3)
    );
    Push(v,ret);
    return 1;
  }

  template<typename Callee,typename P1,typename P2,typename P3,typename P4,typename P5>
  static int Call(Callee & callee,RT (CALL_QUAL Callee::*func)(P1,P2,P3,P4,P5) CONST_QUAL,HSQUIRRELVM v,int index) {
    sq_argassert(1,index + 0);
    sq_argassert(2,index + 1);
    sq_argassert(3,index + 2);
    sq_argassert(4,index + 3);
    sq_argassert(5,index + 4);
    RT ret = (callee.*func)(
      Get(TypeWrapper<P1>(),v,index + 0),
      Get(TypeWrapper<P2>(),v,index + 1),
      Get(TypeWrapper<P3>(),v,index + 2),
      Get(TypeWrapper<P4>(),v,index + 3),
      Get(TypeWrapper<P5>(),v,index + 4)
    );
    Push(v,ret);
    return 1;
  }

  template<typename Callee,typename P1,typename P2,typename P3,typename P4,typename P5,typename P6>
  static int Call(Callee & callee,RT (CALL_QUAL Callee::*func)(P1,P2,P3,P4,P5,P6) CONST_QUAL,HSQUIRRELVM v,int index) {
    sq_argassert(1,index + 0);
    sq_argassert(2,index + 1);
    sq_argassert(3,index + 2);
    sq_argassert(4,index + 3);
    sq_argassert(5,index + 4);
    sq_argassert(6,index + 5);
    RT ret = (callee.*func)(
      Get(TypeWrapper<P1>(),v,index + 0),
      Get(TypeWrapper<P2>(),v,index + 1),
      Get(TypeWrapper<P3>(),v,index + 2),
      Get(TypeWrapper<P4>(),v,index + 3),
      Get(TypeWrapper<P5>(),v,index + 4),
      Get(TypeWrapper<P6>(),v,index + 5)
    );
    Push(v,ret);
    return 1;
  }

  template<typename Callee,typename P1,typename P2,typename P3,typename P4,typename P5,typename P6,typename P7>
  static int Call(Callee & callee,RT (CALL_QUAL Callee::*func)(P1,P2,P3,P4,P5,P6,P7) CONST_QUAL,HSQUIRRELVM v,int index) {
    sq_argassert(1,index + 0);
    sq_argassert(2,index + 1);
    sq_argassert(3,index + 2);
    sq_argassert(4,index + 3);
    sq_argassert(5,index + 4);
    sq_argassert(6,index + 5);
    sq_argassert(7,index + 6);
    RT ret = (callee.*func)(
      Get(TypeWrapper<P1>(),v,index + 0),
      Get(TypeWrapper<P2>(),v,index + 1),
      Get(TypeWrapper<P3>(),v,index + 2),
      Get(TypeWrapper<P4>(),v,index + 3),
      Get(TypeWrapper<P5>(),v,index + 4),
      Get(TypeWrapper<P6>(),v,index + 5),
      Get(TypeWrapper<P7>(),v,index + 6)
    );
    Push(v,ret);
    return 1;
  }
#undef SQPLUS_CALL_MFUNC_RET0
#endif // SQPLUS_CALL_MFUNC_RET0


#ifdef SQPLUS_CALL_MFUNC_NORET

  // Include this very same thing with __cdecl also 
  #if defined(SQPLUS_ENABLE_CDECL_MEMBER_FUNCTIONS) && !defined(SQPLUS_APPLY_CDECL)
    #define SQPLUS_APPLY_CDECL
    #include "SqPlusCallTemplates.h" 
    #undef CALL_QUAL
	#define CALL_QUAL
  #endif
  
  // === Member function calls ===

	template<typename Callee>
	static int Call(Callee & callee,void (CALL_QUAL Callee::*func)() CONST_QUAL, HSQUIRRELVM,int /*index*/) {
		(callee.*func)();
		return 0;
	}

	template<typename Callee,typename P1>
	static int Call(Callee & callee,void (CALL_QUAL Callee::*func)(P1) CONST_QUAL, HSQUIRRELVM v,int index) {
    sq_argassert(1,index + 0);
		(callee.*func)(
			Get(TypeWrapper<P1>(),v,index + 0)
		);
		return 0;
	}

	template<typename Callee,typename P1,typename P2>
	static int Call(Callee & callee,void (CALL_QUAL Callee::*func)(P1,P2) CONST_QUAL, HSQUIRRELVM v,int index) {
    sq_argassert(1,index + 0);
    sq_argassert(2,index + 1);
		(callee.*func)(
			Get(TypeWrapper<P1>(),v,index + 0),
			Get(TypeWrapper<P2>(),v,index + 1)
		);
		return 0;
	}

	template<typename Callee,typename P1,typename P2,typename P3>
	static int Call(Callee & callee,void (CALL_QUAL Callee::*func)(P1,P2,P3) CONST_QUAL, HSQUIRRELVM v,int index) {
    sq_argassert(1,index + 0);
    sq_argassert(2,index + 1);
    sq_argassert(3,index + 2);
		(callee.*func)(
			Get(TypeWrapper<P1>(),v,index + 0),
			Get(TypeWrapper<P2>(),v,index + 1),
      Get(TypeWrapper<P3>(),v,index + 2)
		);
		return 0;
	}

	template<typename Callee,typename P1,typename P2,typename P3,typename P4>
	static int Call(Callee & callee,void (CALL_QUAL Callee::*func)(P1,P2,P3,P4) CONST_QUAL, HSQUIRRELVM v,int index) {
    sq_argassert(1,index + 0);
    sq_argassert(2,index + 1);
    sq_argassert(3,index + 2);
    sq_argassert(4,index + 3);
		(callee.*func)(
			Get(TypeWrapper<P1>(),v,index + 0),
			Get(TypeWrapper<P2>(),v,index + 1),
      Get(TypeWrapper<P3>(),v,index + 2),
      Get(TypeWrapper<P4>(),v,index + 3)
		);
		return 0;
	}

	template<typename Callee,typename P1,typename P2,typename P3,typename P4,typename P5>
	static int Call(Callee & callee,void (CALL_QUAL Callee::*func)(P1,P2,P3,P4,P5) CONST_QUAL, HSQUIRRELVM v,int index) {
    sq_argassert(1,index + 0);
    sq_argassert(2,index + 1);
    sq_argassert(3,index + 2);
    sq_argassert(4,index + 3);
    sq_argassert(5,index + 4);
		(callee.*func)(
			Get(TypeWrapper<P1>(),v,index + 0),
			Get(TypeWrapper<P2>(),v,index + 1),
      Get(TypeWrapper<P3>(),v,index + 2),
      Get(TypeWrapper<P4>(),v,index + 3),
      Get(TypeWrapper<P5>(),v,index + 4)
		);
		return 0;
	}

	template<typename Callee,typename P1,typename P2,typename P3,typename P4,typename P5,typename P6>
	static int Call(Callee & callee,void (CALL_QUAL Callee::*func)(P1,P2,P3,P4,P5,P6) CONST_QUAL, HSQUIRRELVM v,int index) {
    sq_argassert(1,index + 0);
    sq_argassert(2,index + 1);
    sq_argassert(3,index + 2);
    sq_argassert(4,index + 3);
    sq_argassert(5,index + 4);
    sq_argassert(6,index + 5);
		(callee.*func)(
			Get(TypeWrapper<P1>(),v,index + 0),
			Get(TypeWrapper<P2>(),v,index + 1),
      Get(TypeWrapper<P3>(),v,index + 2),
      Get(TypeWrapper<P4>(),v,index + 3),
      Get(TypeWrapper<P5>(),v,index + 4),
      Get(TypeWrapper<P6>(),v,index + 5)
		);
		return 0;
	}

	template<typename Callee,typename P1,typename P2,typename P3,typename P4,typename P5,typename P6,typename P7>
	static int Call(Callee & callee,void (CALL_QUAL Callee::*func)(P1,P2,P3,P4,P5,P6,P7) CONST_QUAL, HSQUIRRELVM v,int index) {
    sq_argassert(1,index + 0);
    sq_argassert(2,index + 1);
    sq_argassert(3,index + 2);
    sq_argassert(4,index + 3);
    sq_argassert(5,index + 4);
    sq_argassert(6,index + 5);
    sq_argassert(7,index + 6);
		(callee.*func)(
			Get(TypeWrapper<P1>(),v,index + 0),
			Get(TypeWrapper<P2>(),v,index + 1),
      Get(TypeWrapper<P3>(),v,index + 2),
      Get(TypeWrapper<P4>(),v,index + 3),
      Get(TypeWrapper<P5>(),v,index + 4),
      Get(TypeWrapper<P6>(),v,index + 5),
      Get(TypeWrapper<P7>(),v,index + 6)
		);
		return 0;
	}
#undef SQPLUS_CALL_MFUNC_NORET
#endif // SQPLUS_CALL_MFUNC_NORET


#ifdef SQPLUS_CALL_MFUNC_RET1

	// Include this very same thing with __cdecl also 
	#if defined(SQPLUS_ENABLE_CDECL_MEMBER_FUNCTIONS) && !defined(SQPLUS_APPLY_CDECL)
		#define SQPLUS_APPLY_CDECL
		#include "SqPlusCallTemplates.h" 
		#undef CALL_QUAL
		#define CALL_QUAL
	#endif

	template<typename Callee,typename RT>
	int Call(Callee & callee, RT (CALL_QUAL Callee::*func)() CONST_QUAL, HSQUIRRELVM v,int index) {
	  return ReturnSpecialization<RT>::Call(callee,func,v,index);
	}

	template<typename Callee,typename RT,typename P1>
	int Call(Callee & callee,RT (CALL_QUAL Callee::*func)(P1) CONST_QUAL, HSQUIRRELVM v,int index) {
	  return ReturnSpecialization<RT>::Call(callee,func,v,index);
	}

	template<typename Callee,typename RT,typename P1,typename P2>
	int Call(Callee & callee,RT (CALL_QUAL Callee::*func)(P1,P2) CONST_QUAL, HSQUIRRELVM v,int index) {
	  return ReturnSpecialization<RT>::Call(callee,func,v,index);
	}

	template<typename Callee,typename RT,typename P1,typename P2,typename P3>
	int Call(Callee & callee,RT (CALL_QUAL Callee::*func)(P1,P2,P3) CONST_QUAL, HSQUIRRELVM v,int index) {
	  return ReturnSpecialization<RT>::Call(callee,func,v,index);
	}

	template<typename Callee,typename RT,typename P1,typename P2,typename P3,typename P4>
	int Call(Callee & callee,RT (CALL_QUAL Callee::*func)(P1,P2,P3,P4) CONST_QUAL, HSQUIRRELVM v,int index) {
	  return ReturnSpecialization<RT>::Call(callee,func,v,index);
	}

	template<typename Callee,typename RT,typename P1,typename P2,typename P3,typename P4,typename P5>
	int Call(Callee & callee,RT (CALL_QUAL Callee::*func)(P1,P2,P3,P4,P5) CONST_QUAL, HSQUIRRELVM v,int index) {
	  return ReturnSpecialization<RT>::Call(callee,func,v,index);
	}

	template<typename Callee,typename RT,typename P1,typename P2,typename P3,typename P4,typename P5,typename P6>
	int Call(Callee & callee,RT (CALL_QUAL Callee::*func)(P1,P2,P3,P4,P5,P6) CONST_QUAL, HSQUIRRELVM v,int index) {
	  return ReturnSpecialization<RT>::Call(callee,func,v,index);
	}

	template<typename Callee,typename RT,typename P1,typename P2,typename P3,typename P4,typename P5,typename P6,typename P7>
	int Call(Callee & callee,RT (CALL_QUAL Callee::*func)(P1,P2,P3,P4,P5,P6,P7) CONST_QUAL, HSQUIRRELVM v,int index) {
	  return ReturnSpecialization<RT>::Call(callee,func,v,index);
	}

#undef SQPLUS_CALL_MFUNC_RET1
#endif // SQPLUS_CALL_MFUNC_RET1

// We will be reusing these symbols later
#undef SQPLUS_APPLY_CDECL
#undef SQPLUS_APPLY_CONST 
