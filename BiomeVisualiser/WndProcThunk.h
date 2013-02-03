
// WndProcThunk.h

// Interfaces to the CWndProcThunk class responsible for WNDPROC class-thunking
// For details, see http://www.hackcraft.net/cpp/windowsThunk/thiscall/
// Also available is a CDlgProcThunk class doing the same work for DIALOGPROC

// MD: Made NX-compat by allocating the code structure using VirtualAlloc(..., PAGE_EXECUTE_READWRITE)





// fwd:
template <class W> class CWndProcThunk;





#ifndef WNDPROCTHUNK_H_INCLUDED
#define WNDPROCTHUNK_H_INCLUDED




template<typename To, typename From> inline To union_cast(From fr) throw()
{
  union
  {
		From f;
		To t;
  } uc;
  uc.f = fr;
  return uc.t;
} 





#pragma warning(push)
#pragma warning(disable : 4355)

#if defined(_M_IX86)

#pragma pack(push,1)

template <class W> class CWndProcThunk
{
	typedef ::LRESULT (W::* WndProc)(::HWND, ::UINT, ::WPARAM, ::LPARAM);
	typedef CWndProcThunk ThisClass;
	
	struct SCode
	{
		BYTE m_mov;           // mov ECX, m_this
		W * m_this;           //
		BYTE m_jmp;           // jmp m_relproc
		ptrdiff_t m_relproc;  // relative jmp
	};
	
	SCode * Code;
	
public:
	ThisClass(WndProc proc, W * obj)
	{
		Code = (SCode *)VirtualAlloc(NULL, sizeof(SCode), MEM_COMMIT, PAGE_EXECUTE_READWRITE);
		Code->m_mov = 0xB9,
		Code->m_this = obj,
		Code->m_jmp = 0xE9,
		Code->m_relproc = union_cast<char *>(proc) - reinterpret_cast<char *>(Code) - sizeof(*Code);
		::FlushInstructionCache(::GetCurrentProcess(), Code, sizeof(*Code));
	}
	
	virtual ~CWndProcThunk()
	{
		VirtualFree(Code, sizeof(*Code), MEM_RELEASE);
		Code = NULL;
	}

	operator ::WNDPROC() const {return reinterpret_cast<::WNDPROC>(Code); }
	operator ::LONG_PTR() const {return reinterpret_cast<::LONG_PTR>(Code); }
} ;





template <class W> class CDlgProcThunk
{
	typedef ::BOOL (W::* DlgProc)(::HWND, ::UINT, ::WPARAM, ::LPARAM);
	typedef CDlgProcThunk ThisClass;
	
	struct SCode
	{
		BYTE m_mov;           // mov ECX, m_this
		W * m_this;           //
		BYTE m_jmp;           // jmp m_relproc
		ptrdiff_t m_relproc;  // relative jmp
	};
	
	SCode * Code;
	
public:
	CDlgProcThunk(DlgProc proc, W * obj)
	{
		Code = (SCode *)VirtualAlloc(NULL, sizeof(SCode), MEM_COMMIT, PAGE_EXECUTE_READWRITE);
		Code->m_mov = 0xB9,
		Code->m_this = obj,
		Code->m_jmp = 0xE9,
		Code->m_relproc = union_cast<char *>(proc) - reinterpret_cast<char *>(Code) - sizeof(*Code);
		::FlushInstructionCache(::GetCurrentProcess(), Code, sizeof(*Code));
	}
	
	virtual ~CDlgProcThunk()
	{
		VirtualFree(Code, sizeof(*Code), MEM_RELEASE);
		Code = NULL;
	}

	operator ::DLGPROC() const {return reinterpret_cast<::DLGPROC>(Code); }
	operator ::LONG_PTR() const {return reinterpret_cast<::LONG_PTR>(Code); }
} ;





	#pragma pack(pop)

#else  // _M_IX86
	#error Only X86 supported
#endif





#endif  // WNDPROCTHUNK_H_INCLUDED




