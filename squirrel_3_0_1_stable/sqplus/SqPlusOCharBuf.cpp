
#include "sqplus.h"
#include "SqPlusOCharBuf.h" 

#include <wchar.h>

// Conversion functions, between char* string and wchar_t* strings.

int ol_strlen(const wchar_t* pwc){ return (int)wcslen(pwc); }
int ol_buflen(const wchar_t* pwc){ return (int)wcslen(pwc); }
int ol_charlen(const wchar_t* pwc){ return 1; }
//int ol_charlen(const wchar_t pwc){ return 1; }
int ol_char(const wchar_t* pwc){ return (int)*pwc; }
bool ol_writechar(wchar_t*& pwc, int ch){ *pwc=(wchar_t)ch; pwc++; return true; }

int ol_buflen(const char* pc){ return (int)strlen(pc); }

#if SQPLUS_USE_LATIN1==1
  // Convert to 8-bit LATIN1 char set. The only thing to do is convert chars out of range to '?'
  int ol_strlen(const char* pc){ return (int)strlen(pc); }
  int ol_charlen(const char* pc){ return 1; }
  //int ol_charlen(int pc){ return 1; }
  int ol_char(const char* pc){ return (int)*(unsigned char*)pc; }
  bool ol_writechar(char*& pc, int ch){ *pc=(char)(ch>255?'?':ch); pc++; return true; }
#else
  #include "SqPlusUtf8.h" 
  // Convert to 8-Bit UTF8 encoding. Some more work here. 
  int ol_strlen(const char* pc){ return sqplus_utf8_strlen(pc); }
  int ol_charlen(const char* pc){ return sqplus_utf8_len_first(pc); }
  //int ol_charlen(int ch){ char buf[8]; return sqplus_wchar_to_utf8(buf,ch,8); }  
  int ol_char(const char* pc){ int chr=-1; sqplus_utf8_to_wchar(&chr,pc); return chr; }
  bool ol_writechar(char*& pc, int ch) { 
	  int l=sqplus_wchar_to_utf8(pc,ch,8); 
	  if(l>0){ 
		  pc += l; 
		  return true;  
	  }
	  else return false;
  }
#endif


#ifdef SQUNICODE
  // SQChar is wchar_t, convert to/from to either Latin1 or UTF8
  SQDefCharBuf CH2SQ( const char* ps ){
    return SQDefCharBuf(ps);
  }
  SQOthCharBuf SQ2CH( const wchar_t* pws ){
    return SQOthCharBuf(pws);
  }
#else
  // SQChar is char, convert to/from wchar_t
  SQDefCharBuf WC2SQ( const wchar_t* pws ){
    return SQDefCharBuf(pws);
  }
  SQOthCharBuf SQ2WC( const char* ps ){
    return SQOthCharBuf(ps);
  }
#endif


