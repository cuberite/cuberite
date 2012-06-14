/**********************************************************************
 * 
 * LEAKFINDER.H
 *
 *
 *
 * LICENSE (http://www.opensource.org/licenses/bsd-license.php)
 *
 *   Copyright (c) 2005-2010, Jochen Kalmbach
 *   All rights reserved.
 *
 *   Redistribution and use in source and binary forms, with or without modification, 
 *   are permitted provided that the following conditions are met:
 *
 *   Redistributions of source code must retain the above copyright notice, 
 *   this list of conditions and the following disclaimer. 
 *   Redistributions in binary form must reproduce the above copyright notice, 
 *   this list of conditions and the following disclaimer in the documentation 
 *   and/or other materials provided with the distribution. 
 *   Neither the name of Jochen Kalmbach nor the names of its contributors may be 
 *   used to endorse or promote products derived from this software without 
 *   specific prior written permission. 
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 *   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
 *   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 *   ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE 
 *   FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
 *   (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
 *   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND 
 *   ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
 *   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
 *   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 **********************************************************************/

// #pragma once is supported starting with _MCS_VER 1000, 
// so we need not to check the version (because we only support _MSC_VER >= 1100)!
#pragma once

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

HRESULT InitLeakFinder();
void DeinitLeakFinder();

#ifdef __cplusplus
}
#endif


// The following is only available if the file is CPP
#ifdef __cplusplus

#include "StackWalker.h"

// Interface for output...
class LeakFinderOutput : public StackWalker
{
public:
  typedef enum LeakFinderOptions
  {
    // No addition info will be retrived 
    // (only the address is available)
    LeakFinderNone = 0,
    LeakFinderShowCompleteCallstack = 0x1000
  } LeakFinderOptions;

  LeakFinderOutput(int options = OptionsAll,  LPCSTR szSymPath = NULL);
  virtual void OnLeakSearchStart(LPCSTR sszLeakFinderName);
  virtual void OnLeakStartEntry(LPCSTR szKeyName, SIZE_T nDataSize);
protected:
  virtual void OnCallstackEntry(CallstackEntryType eType, CallstackEntry &entry);
  virtual void OnOutput(LPCSTR szText) 
  { 
    printf(szText); 
    StackWalker::OnOutput(szText); 
  }
  virtual void OnDbgHelpErr(LPCSTR szFuncName, DWORD gle, DWORD64 addr)
  {
    if (strcmp(szFuncName, "SymGetLineFromAddr64") == 0) return;
    StackWalker::OnDbgHelpErr(szFuncName, gle, addr);
  }
};

class LeakFinderXmlOutput : public LeakFinderOutput
{
public:
  LeakFinderXmlOutput();
  virtual ~LeakFinderXmlOutput();
  LeakFinderXmlOutput(LPCTSTR szFileName);
  virtual void OnLeakSearchStart(LPCSTR sszLeakFinderName);
  virtual void OnLeakStartEntry(LPCSTR szKeyName, SIZE_T nDataSize);
protected:
  virtual void OnCallstackEntry(CallstackEntryType eType, CallstackEntry &entry);
  virtual void OnOutput(LPCSTR szText) { }
  virtual void OnDbgHelpErr(LPCSTR szFuncName, DWORD gle, DWORD64 addr) { }

  FILE *m_fXmlFile;
};

// C++ interface:
void DeinitLeakFinder(LeakFinderOutput *output);

class ZZZ_LeakFinder
{
public:
  ZZZ_LeakFinder()
  {
    m_pXml = NULL;
#ifdef XML_LEAK_FINDER
    m_pXml = new LeakFinderXmlOutput();
#endif
    InitLeakFinder();
  }
  ~ZZZ_LeakFinder()
  {
    DeinitLeakFinder(m_pXml);
    if (m_pXml != NULL) delete m_pXml;
  }
protected:
  LeakFinderXmlOutput *m_pXml;
};

#if defined(INIT_LEAK_FINDER)
#if _MSC_VER >= 1200
#pragma warning(push)
#endif
#pragma warning (disable:4074)

// WARNING: If you enable this option, the code might run without the CRT being initialized or after the CRT was deinitialized!!!
//          Currently the code is not designed to bypass the CRT...
//#pragma init_seg (compiler)
ZZZ_LeakFinder zzz_LeakFinder;

#if _MSC_VER >= 1200
#pragma warning(pop)
#else
#pragma warning(default:4074)
#endif
#endif

#endif  // __cplusplus
