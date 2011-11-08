
#ifndef SQPLUS_OCHARBUF_H
#define SQPLUS_OCHARBUF_H

// Conversion routines between different character encodings, to be used in
// Push/Get/Match handlers of SqPlus. It enables using both of char* and 
// wchar_t* as function arguments.
//
// When converting from wchar_t to char, the choice is between using a Latin1 
// or a UTF8 representation in 8-bit mode. This is probably most useful if 
// Squirrel is compiled in ANSI mode and the app uses some wchar_t strings 
// (UNICODE mode on Win32). 
//

// A char is either
//  - A Latin1 character (first code page, coincides with Unicode points 0..255)
//  - Part of an UTF8 character
//
// Note: SQUTF8 requires Squirrel sources that have been patched for UTF8 strings 
// internally (when converting function arguments). 

#ifndef SQUTF8
  // Default to Latin1 conversion 
  //#undef SQPLUS_USE_LATIN1
  #ifndef SQPLUS_USE_LATIN1
    #define SQPLUS_USE_LATIN1 1
  #endif
#else
  // Using Squirrel with internal UTF8 string encoding 
  //#undef SQPLUS_USE_LATIN1
  #ifndef SQPLUS_USE_LATIN1
    #define SQPLUS_USE_LATIN1 0
  #endif
#endif 
 

// The various ol_... functions sets up for a conversion scheme 
// that can be used in both wchar_t => char and  char => wchar_t.
int ol_strlen(const wchar_t* pwc);
int ol_buflen(const wchar_t* pwc);
int ol_charlen(const wchar_t* pwc);
int ol_char(const wchar_t* pwc);
bool ol_writechar(wchar_t*& pwc, int ch);

int ol_strlen(const char* pc);
int ol_buflen(const char* pc);
int ol_charlen(const char* pc);
int ol_char(const char* pc);
bool ol_writechar(char*& pc, int ch);

#ifdef SQUNICODE
  // SQChar is wchar_t, convert to/from either Latin1 or UTF8
  typedef char SQOtherChar;
#else
  // SQChar is char, convert to/from wchar_t
  typedef wchar_t SQOtherChar;
#endif


// Buffer to hold a string and release it in destructor. 
// SQT is input character type 
// SQOT is the opposite type of SQChar
// If 'try_borrow' is set, it will not allocate a new buffer when no 
// conversion is done, it will just keep the input pointer (beware).
template<class SQT, class SQOT, bool use_latin1>
struct SQCharBufImpl {

	// Allocate a buffer from string of same type
	SQCharBufImpl(const SQT *poc=NULL, bool try_borrow=true ) : m_poc(0), m_own(false) { 
		Init( poc, try_borrow );
	}

	// This does conversion from other char type
	SQCharBufImpl(const SQOT *ps, bool try_borrow=false ) : m_poc(0), m_own(false) { 
		Init( ps ); // Cannot borrow pointer when doing conversion
	}

	void Init( const SQT *poc, bool try_borrow ){
		m_own = !try_borrow;
		if( try_borrow ){ 
			m_poc = (SQT*)poc;
		}
		else {
			int sl = poc ? ol_buflen(poc) : 0;
			if( poc ){
				m_poc = poc ? new SQT[sl+1] : NULL; 
				if( m_poc ) memcpy( m_poc, poc, (sl+1)*sizeof(SQT) );
			}
			else
				m_poc = NULL;
		}
    }

	void Init( const SQOT *ps ){
		m_own = true;
        if( ps ){
            int sl = ps ? ol_strlen(ps) : 0;	// Length of string in characters (not bytes)
		    int scl = 0;                        // Length of converted string in char/wchar_t count
			// How much storage needed? 
			if( !use_latin1 && sizeof(SQT)<sizeof(SQOT) ){
				// Converting wchar_t => UTF8 
				const SQOT *ps1 = ps;   // It is wchar_t* here
				SQT tmp_buf[8];
				SQT *ptmp;
				while( *ps1 ){
				    ptmp = tmp_buf;
                    if( ol_writechar(ptmp,*ps1++) )
                        scl += ol_charlen(tmp_buf);
                    else
                        scl++;
				}
			}
			else scl = sl;  // Converting to wchar_t or Latin1, keep length 

            m_poc = new SQT[scl+1]; 
			if( !m_poc ) return;

			// Convert 
			SQT *poc = m_poc;
			while( *ps ){
				ol_writechar( poc, ol_char(ps) );
				ps += ol_charlen(ps);
			}
			*poc = 0;	// Terminating zero
        }
        else
            m_poc = NULL;
    }
    
    ~SQCharBufImpl(){ Release(); }
    
	void Release( ){
		if(m_poc && m_own ) 
			delete [] m_poc; 
		m_poc = NULL;
	}

	SQT* Set( const SQOT *ps ){
		Release( );
		Init( ps );
		return m_poc;
	}

    operator SQT*() const { return m_poc; }
    
	SQCharBufImpl<SQT,SQOT,use_latin1>& operator = (const SQT *ps){
		Release();
		Init( ps, false );
		return *this;
	}

	SQCharBufImpl<SQT,SQOT,use_latin1>& operator = (const SQOT *ps){
		Release();
		Init( ps );
		return *this;
	}

	// Move string from other here - Note: Empties the input argument (other)
	SQCharBufImpl<SQT,SQOT,use_latin1>& operator = (const SQCharBufImpl<SQT,SQOT,use_latin1>& other){
		Release();
		m_poc = other.m_poc;
		m_own = other.m_own;
		SQT **psqt = (SQT**)&other.m_poc;
		*psqt = NULL;
		return *this;
	}

protected:
    SQT *m_poc;
	bool m_own;
};

typedef SQCharBufImpl<SQOtherChar, SQChar, bool(SQPLUS_USE_LATIN1)> SQOthCharBuf;
typedef SQCharBufImpl<SQChar, SQOtherChar, bool(SQPLUS_USE_LATIN1)> SQDefCharBuf;
typedef SQCharBufImpl<wchar_t, char, bool(SQPLUS_USE_LATIN1)>       SQWCharBuf;
typedef SQCharBufImpl<char, wchar_t, bool(SQPLUS_USE_LATIN1)>       SQACharBuf;

#ifdef SQUNICODE
  // SQChar is wchar_t, convert to/from to either Latin1 or UTF8
  #define WC2SQ(s) s
  #define SQ2WC(s) s
  SQDefCharBuf CH2SQ( const char* ps );
  SQOthCharBuf SQ2CH( const wchar_t* pws );
#else
  // SQChar is char, convert to/from wchar_t
  SQDefCharBuf WC2SQ( const wchar_t* pws );
  SQOthCharBuf SQ2WC( const char* pws );
  #define CH2SQ(s) s
  #define SQ2CH(s) s
#endif

#endif // SQPLUS_CHARBUF_H

