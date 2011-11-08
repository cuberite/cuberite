
// File to automatically generate a typemask for a function 

// Will generate type masks that accepts Instance pointer or null 
#ifndef SQPLUS_ACCEPT_NULL_INSTANCE
    #define SQPLUS_ACCEPT_NULL_INSTANCE
#endif 

// Macros to reduce typing: (P1), (P1,P2), (P1,P2,P3), ...
#define P_0 
#define P_1 P1
#define P_2 P_1,P2
#define P_3 P_2,P3
#define P_4 P_3,P4
#define P_5 P_4,P5
#define P_6 P_5,P6
#define P_7 P_6,P7

// Macros to reduce typing: (typename P1), (typename P1,typename P2), ...
#define PTN_0 
#define PTN_1 typename P1
#define PTN_2 PTN_1,typename P2
#define PTN_3 PTN_2,typename P3
#define PTN_4 PTN_3,typename P4
#define PTN_5 PTN_4,typename P5
#define PTN_6 PTN_5,typename P6
#define PTN_7 PTN_6,typename P7

// Include a comma first in list: ,typename P1,typename P2
#define PTNC_0 
#define PTNC_1 ,PTN_1
#define PTNC_2 ,PTN_2
#define PTNC_3 ,PTN_3
#define PTNC_4 ,PTN_4
#define PTNC_5 ,PTN_5
#define PTNC_6 ,PTN_6
#define PTNC_7 ,PTN_7

#ifdef SQUNICODE
#define scstrcpy wcscpy
#else
#define scstrcpy strcpy
#endif

inline const SQChar* strappchar(SQChar *buf, SQChar *in, SQChar ch){ 
    int l=scstrlen(in); 
    scstrcpy(buf,in);
    buf[l] = ch;
#ifdef SQPLUS_ACCEPT_NULL_INSTANCE
    if( ch=='x' ){
        buf[++l] = '|'; // Also accept NULL pointers
        buf[++l] = 'o';
    }
#endif
    buf[l+1] = 0;
    return buf; 
}

template<class T>
struct sqTypeMask { };


// Return type not included in type mask, delegate to void case

// Base case, no arguments
template<typename RT> 
struct sqTypeMask<RT(*)()> { 
    static const SQChar *Get(){
    	static SQChar buf[64];
    	//buf[0] = _SC('t');
    	//buf[1] = 0;
        //strcpy(buf,"t|x");  // Accept both instance and table, we don't use param anyway
    	buf[0] = _SC('.');  // Accept anything (not used)
    	buf[1] = 0;
    	return buf; 
    }
};

// Recursive case, peel of one arg at each level
#define DECLARE_RT_SQTYPEMASK(N,M) \
template<typename RT PTNC_##N> \
struct sqTypeMask<RT(*)(P_##N)> { \
    static const SQChar *Get(){ \
        static SQChar buf[10]; \
        return strappchar(buf, (SQChar*)sqTypeMask<RT(*)(P_##M)>::Get(), \
                               (SQChar)TypeInfo<P##N>::TypeMask); \
    } \
};

DECLARE_RT_SQTYPEMASK(1,0)
DECLARE_RT_SQTYPEMASK(2,1)
DECLARE_RT_SQTYPEMASK(3,2)
DECLARE_RT_SQTYPEMASK(4,3)
DECLARE_RT_SQTYPEMASK(5,4)
DECLARE_RT_SQTYPEMASK(6,5)
DECLARE_RT_SQTYPEMASK(7,6)


// Difference to above is that 1st param is instance instead of table

#define DECLARE_RT_MEMBER_SQTYPEMASK(N,QUAL) \
template<typename Callee, typename RT PTNC_##N> \
struct sqTypeMask<RT(Callee::*)(P_##N) QUAL> { \
    static const SQChar *Get(){ \
        SQChar *buf = (SQChar*)sqTypeMask<RT(*)(P_##N)>::Get(); \
        buf[0] = _SC('x'); \
        return buf; \
    } \
};

//    	buf[1] = 0; \ 

DECLARE_RT_MEMBER_SQTYPEMASK(0,)
DECLARE_RT_MEMBER_SQTYPEMASK(1,)
DECLARE_RT_MEMBER_SQTYPEMASK(2,)
DECLARE_RT_MEMBER_SQTYPEMASK(3,)
DECLARE_RT_MEMBER_SQTYPEMASK(4,)
DECLARE_RT_MEMBER_SQTYPEMASK(5,)
DECLARE_RT_MEMBER_SQTYPEMASK(6,)
DECLARE_RT_MEMBER_SQTYPEMASK(7,)

DECLARE_RT_MEMBER_SQTYPEMASK(0,const)
DECLARE_RT_MEMBER_SQTYPEMASK(1,const)
DECLARE_RT_MEMBER_SQTYPEMASK(2,const)
DECLARE_RT_MEMBER_SQTYPEMASK(3,const)
DECLARE_RT_MEMBER_SQTYPEMASK(4,const)
DECLARE_RT_MEMBER_SQTYPEMASK(5,const)
DECLARE_RT_MEMBER_SQTYPEMASK(6,const)
DECLARE_RT_MEMBER_SQTYPEMASK(7,const)


#ifdef _MSC_VER

// Difference to above is we're using __cdecl calling convention here
// Only makes sense for MSVC where member functions can have different 
// calling conventions (__cdecl or __thiscall)

#define DECLARE_RT_MEMBER_SQTYPEMASK_CDECL(N,QUAL) \
template<typename Callee, typename RT PTNC_##N> \
struct sqTypeMask<RT(__cdecl Callee::*)(P_##N) QUAL> { \
    static const SQChar *Get(){ \
        SQChar *buf = (SQChar*)sqTypeMask<RT(*)(P_##N)>::Get(); \
        buf[0] = _SC('x'); \
        return buf; \
    } \
};

//    	buf[1] = 0; \

DECLARE_RT_MEMBER_SQTYPEMASK_CDECL(0,)
DECLARE_RT_MEMBER_SQTYPEMASK_CDECL(1,)
DECLARE_RT_MEMBER_SQTYPEMASK_CDECL(2,)
DECLARE_RT_MEMBER_SQTYPEMASK_CDECL(3,)
DECLARE_RT_MEMBER_SQTYPEMASK_CDECL(4,)
DECLARE_RT_MEMBER_SQTYPEMASK_CDECL(5,)
DECLARE_RT_MEMBER_SQTYPEMASK_CDECL(6,)
DECLARE_RT_MEMBER_SQTYPEMASK_CDECL(7,)

DECLARE_RT_MEMBER_SQTYPEMASK_CDECL(0,const)
DECLARE_RT_MEMBER_SQTYPEMASK_CDECL(1,const)
DECLARE_RT_MEMBER_SQTYPEMASK_CDECL(2,const)
DECLARE_RT_MEMBER_SQTYPEMASK_CDECL(3,const)
DECLARE_RT_MEMBER_SQTYPEMASK_CDECL(4,const)
DECLARE_RT_MEMBER_SQTYPEMASK_CDECL(5,const)
DECLARE_RT_MEMBER_SQTYPEMASK_CDECL(6,const)
DECLARE_RT_MEMBER_SQTYPEMASK_CDECL(7,const)

#endif // _MSC_VER

