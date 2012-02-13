
// LeakFinder.cpp

// Finds memory leaks rather effectively

// _X: downloaded from http://www.codeproject.com/Articles/3134/Memory-Leak-and-Exception-Trace-CRT-and-COM-Leaks - the real link is in the comments, RC11 version





/**********************************************************************
 * 
 * LEAKFINDER.CPP
 *
 *
 *
 * History:
 *  2010-04-15   RC10   - Updated to VC10 RTM
 *                        Fixed Bug: Application Verifier, thanks to handsinmypocket!
 *                        http://www.codeproject.com/KB/applications/leakfinder.aspx?msg=3439751#xx3439751xx
 *  2008-08-04   RC6    - Updated to VC9 RTM
 *                        Fixed Bug: Missing "ole32.lib" LIB
 *                        http://www.codeproject.com/KB/applications/leakfinder.aspx?msg=2253980#xx2253980xx
 *                        Fixed Bug: Compiled with "WIN32_LEAN_AND_MEAN"
 *                        http://www.codeproject.com/KB/applications/leakfinder.aspx?msg=1824718#xx1824718xx
 *                        Fixed Bug: Compiling with "/Wall"
 *                        http://www.codeproject.com/KB/threads/StackWalker.aspx?msg=2638243#xx2638243xx
 *                        Removed "#pragma init_seg (compiler)" from h-file
 *
 *  2005-12-30   RC5    - Now again VC8 RTM compatible
 *                      - Added Xml-Output (like in the old Leakfinder)
 *                        YOu need to define XML_LEAK_FINDER to activate it
 *                        So you can use the LeakAnalyseTool from
 *                        http://www.codeproject.com/tools/leakfinder.asp
 *
 *  2005-12-13   RC4    - Merged with the new "StackWalker"-project on 
 *                        http://www.codeproject.com/threads/StackWalker.asp
 *
 *  2005-08-01   RC3    - Merged with the new "StackWalker"-project on 
 *                        http://www.codeproject.com/threads/StackWalker.asp
 *
 *  2005-07-05   RC2    - First version with x86, IA64 and x64 support
 *
 *  2005-07-04   RC1    - Added "OutputOptions"
 *                      - New define "INIT_LEAK_FINDER_VERBOSE" to 
 *                        display more info (for error reporting)
 *
 *  2005-07-01   Beta3  - Workaround for a bug in the new dbghelp.dll
 *                        (version 6.5.3.7 from 2005-05-30; StakWalk64 no 
 *                        refused to produce an callstack on x86 systems
 *                        if the context is NULL or has some registers set 
 *                        to 0 (for example Esp). This is against the 
 *                        documented behaviour of StackWalk64...)
 *                      - First version with x64-support
 *
 *   2005-06-16  Beta1  First public release with the following features:
 *                      - Completely rewritten in C++ (object oriented)
 *                      - CRT-Leak-Report
 *                      - COM-Leak-Report
 *                      - Report is done via "OutputDebugString" so
 *                        the line can directly selected in the debugger
 *                        and is opening the corresponding file/line of 
 *                        the allocation
 *                      - Tried to support x64 systems, bud had some
 *                        trouble wih StackWalk64
 *                        See: http://blog.kalmbachnet.de/?postid=43
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

#include <windows.h>
#include <objidl.h>  // Needed if compiled with "WIN32_LEAN_AND_MEAN"
#include <tchar.h>
#include <crtdbg.h>
#include <stdio.h>

#include <string>
#include <vector>


#include "LeakFinder.h"

// Currently only tested with MS VC++ 5 to 10
#if (_MSC_VER < 1100) || (_MSC_VER > 1600)
#error Only MS VC++ 5/6/7/7.1/8/9 supported. Check if the '_CrtMemBlockHeader' has not changed with this compiler!
#endif


// Controlling the callstack depth
#define MAX_CALLSTACK_LEN_BUF 0x2000

#define IGNORE_CRT_ALLOC

// disable 64-bit compatibility-checks (because we explicite have here either x86 or x64!)
#pragma warning(disable:4312)  // warning C4312: 'type cast' : conversion from 'DWORD' to 'LPCVOID' of greater size
#pragma warning(disable:4826)


// secure-CRT_functions are only available starting with VC8
#if _MSC_VER < 1400
#define _snprintf_s _snprintf
#define _tcscat_s _tcscat
#endif

static std::string SimpleXMLEncode(LPCSTR szText)
{
  std::string szRet;
  for (size_t i=0; i<strlen(szText); i++)
  {
    switch(szText[i])
    {
    case '&':
      szRet.append("&amp;");
      break;
    case '<':
      szRet.append("&lt;");
      break;
    case '>':
      szRet.append("&gt;");
      break;
    case '"':
      szRet.append("&quot;");
      break;
    case '\'':
      szRet.append("&apos;");
      break;
    default:
      szRet += szText[i];
    }
  }
  return szRet;
}


LeakFinderOutput::LeakFinderOutput(int options, LPCSTR szSymPath)
  : StackWalker(options, szSymPath)
{
}
void LeakFinderOutput::OnLeakSearchStart(LPCSTR szLeakFinderName)
{
  CHAR buffer[1024];
  _snprintf_s(buffer, 1024, "######## %s ########\n", szLeakFinderName);
  this->OnOutput(buffer);
}
void LeakFinderOutput::OnLeakStartEntry(LPCSTR szKeyName, SIZE_T nDataSize)
{
  CHAR buffer[1024];
  _snprintf_s(buffer, 1024, "--------------- Key: %s, %d bytes ---------\n", szKeyName, nDataSize);
  this->OnOutput(buffer);
}
void LeakFinderOutput::OnCallstackEntry(CallstackEntryType eType, CallstackEntry &entry)
{
  if ( (eType != lastEntry) && (entry.offset != 0) )
  {
    if ( ((this->m_options & LeakFinderShowCompleteCallstack) == 0) && ( 
      (strstr(entry.lineFileName, "afxmem.cpp") != NULL) ||
      (strstr(entry.lineFileName, "dbgheap.c") != NULL) ||
      (strstr(entry.lineFileName, "new.cpp") != NULL) ||
      (strstr(entry.lineFileName, "newop.cpp") != NULL) ||
      (strstr(entry.lineFileName, "leakfinder.cpp") != NULL) ||
      (strstr(entry.lineFileName, "stackwalker.cpp") != NULL) 
      ) )
    {
      return;
    }
  }
  StackWalker::OnCallstackEntry(eType, entry);
}


// ####################################################################
// XML-Output
LeakFinderXmlOutput::LeakFinderXmlOutput()
{
  TCHAR szXMLFileName[1024];

  GetModuleFileName(NULL, szXMLFileName, sizeof(szXMLFileName) / sizeof(TCHAR));
  _tcscat_s(szXMLFileName, _T(".mem.xml-leaks"));
#if _MSC_VER < 1400
  m_fXmlFile = _tfopen(szXMLFileName, _T("w"));
#else
  m_fXmlFile = NULL;
  _tfopen_s(&m_fXmlFile, szXMLFileName, _T("w"));
#endif
  if (m_fXmlFile != NULL)
  {
    SYSTEMTIME st;
    GetLocalTime(&st);
    fprintf(m_fXmlFile, "<MEMREPORT date=\"%.2d/%.2d/%.4d\" time=\"%.2d:%.2d:%.2d\">\n",
      st.wMonth, st.wDay, st.wYear,
      st.wHour, st.wMinute, st.wSecond);
  }
  else
  {
    MessageBox(NULL, _T("Could not open xml-logfile for leakfinder!"), _T("Warning"), MB_ICONHAND);
  }
}
LeakFinderXmlOutput::LeakFinderXmlOutput(LPCTSTR szFileName)
{
#if _MSC_VER < 1400
  m_fXmlFile = _tfopen(szFileName, _T("w"));
#else
  m_fXmlFile = NULL;
  _tfopen_s(&m_fXmlFile, szFileName, _T("w"));
#endif
  if (m_fXmlFile == NULL)
  {
    MessageBox(NULL, _T("Could not open xml-logfile for leakfinder!"), _T("Warning"), MB_ICONHAND);
  }
}
LeakFinderXmlOutput::~LeakFinderXmlOutput()
{
  if (m_fXmlFile != NULL) 
  {
    // Write the ending-tags and close the file
    fprintf(m_fXmlFile, "</MEMREPORT>\n");
    fclose(m_fXmlFile);
  }
  m_fXmlFile = NULL;
}
void LeakFinderXmlOutput::OnLeakSearchStart(LPCSTR sszLeakFinderName)
{
}
void LeakFinderXmlOutput::OnLeakStartEntry(LPCSTR szKeyName, SIZE_T nDataSize)
{
  if (m_fXmlFile != NULL)
  {
    fprintf(m_fXmlFile, "  <LEAK requestID=\"%s\" size=\"%d\">\n", SimpleXMLEncode(szKeyName).c_str(), nDataSize);
  }
}
void LeakFinderXmlOutput::OnCallstackEntry(CallstackEntryType eType, CallstackEntry &entry)
{
  if (m_fXmlFile != NULL)
  {
    if (eType != lastEntry)
    {
      fprintf(m_fXmlFile, "    <STACKENTRY decl=\"%s\" decl_offset=\"%+ld\" ", SimpleXMLEncode(entry.undName).c_str(), entry.offsetFromSmybol);
      fprintf(m_fXmlFile, "srcfile=\"%s\" line=\"%d\" line_offset=\"%+ld\" ", SimpleXMLEncode(entry.lineFileName).c_str(), entry.lineNumber, entry.offsetFromLine);
      fprintf(m_fXmlFile, "module=\"%s\" base=\"%08lx\" ", SimpleXMLEncode(entry.moduleName).c_str(), entry.baseOfImage);
      fprintf(m_fXmlFile, "/>\n");
    }
    else
    {
      fprintf(m_fXmlFile, "  </LEAK>\n");
    }
  }
}

// ##########################################################################
// ##########################################################################
// ##########################################################################
// Base class for storing contexts in a hashtable
template <typename HASHTABLE_KEY> class ContextHashtableBase
{
public:
  ContextHashtableBase(SIZE_T sizeOfHastable, LPCSTR finderName)
  {
    SIZE_T s = sizeOfHastable*sizeof(AllocHashEntryType);
    m_hHeap = HeapCreate(0, 10*1024 + s, 0);
    if (m_hHeap == NULL)
      throw;
    pAllocHashTable = (AllocHashEntryType*) own_malloc(s);
    sAllocEntries = sizeOfHastable;
    m_finderName = own_strdup(finderName);
  }

protected:
  virtual ~ContextHashtableBase()
  {
    if (pAllocHashTable != NULL)
      own_free(pAllocHashTable);
    pAllocHashTable = NULL;

    own_free(m_finderName);
    m_finderName = NULL;

    if (m_hHeap != NULL)
      HeapDestroy(m_hHeap);
  }

  __inline LPVOID own_malloc(SIZE_T size)
  {
    return HeapAlloc(m_hHeap, HEAP_ZERO_MEMORY, size);
  }
  __inline VOID own_free(LPVOID memblock)
  {
    HeapFree(m_hHeap, 0, memblock);
  }
  __inline CHAR *own_strdup(const char *str)
  {
    size_t len = strlen(str)+1;
    CHAR *c = (CHAR*)own_malloc(len);
#if _MSC_VER >= 1400
    strcpy_s(c, len, str);
#else
    strcpy(c, str);
#endif
    return c;
  }

  // Disables this leak-finder
  virtual LONG Disable() = 0;
  // enables the leak-finder again...
  virtual LONG Enable() = 0;

private:
  // Entry for each allocation
  typedef struct AllocHashEntryType {
    HASHTABLE_KEY             key;
    SIZE_T                    nDataSize;     // Size of the allocated memory
    struct AllocHashEntryType *Next;
    CONTEXT c;
    PVOID                     pStackBaseAddr;
    SIZE_T                    nMaxStackSize;

    PVOID                     pCallstackOffset;
    SIZE_T                    nCallstackLen;
    char                      pcCallstackAddr[MAX_CALLSTACK_LEN_BUF];  // min of both values...
  } AllocHashEntryType;

protected:
  virtual SIZE_T HashFunction(HASHTABLE_KEY &key) = 0;
  virtual BOOL IsKeyEmpty(HASHTABLE_KEY &key) = 0;
  virtual VOID SetEmptyKey(HASHTABLE_KEY &key) = 0;
  virtual VOID GetKeyAsString(HASHTABLE_KEY &key, CHAR *szName, SIZE_T nBufferLen) = 0;
  //virtual SIZE_T GetNativeBytes(HASHTABLE_KEY &key, CHAR *szName, SIZE_T nBufferLen) { return 0; }

public:
  VOID Insert(HASHTABLE_KEY &key, CONTEXT &context, SIZE_T nDataSize)
  {
    SIZE_T HashIdx;
    AllocHashEntryType *pHashEntry;

    // generate hash-value
    HashIdx = HashFunction(key);

    pHashEntry = &pAllocHashTable[HashIdx];
    if (IsKeyEmpty(pHashEntry->key) != FALSE) {
      // Entry is empty...
    }
    else {
      // Entry is not empy! make a list of entries for this hash value...
      while(pHashEntry->Next != NULL) {
        pHashEntry = pHashEntry->Next;
      }

      pHashEntry->Next = (AllocHashEntryType*) own_malloc(sizeof(AllocHashEntryType));
      pHashEntry = pHashEntry->Next;
    }
    pHashEntry->key = key;
    pHashEntry->nDataSize = nDataSize;
    pHashEntry->Next = NULL;
#ifdef _M_IX86
    pHashEntry->pCallstackOffset = (LPVOID) min(context.Ebp, context.Esp);
#elif _M_X64
    pHashEntry->pCallstackOffset = (LPVOID) min(context.Rdi, context.Rsp);
#elif _M_IA64
    pHashEntry->pCallstackOffset = (LPVOID) min(context.IntSp, context.RsBSP);
#else
#error "Platform not supported!"
#endif
    pHashEntry->c = context;

    // Query the max. stack-area:
    MEMORY_BASIC_INFORMATION MemBuffer;
    if(VirtualQuery((LPCVOID) pHashEntry->pCallstackOffset, &MemBuffer, sizeof(MemBuffer)) > 0)
    {
      pHashEntry->pStackBaseAddr = MemBuffer.BaseAddress;
      pHashEntry->nMaxStackSize = MemBuffer.RegionSize;
    }
    else
    {
      pHashEntry->pStackBaseAddr = 0;
      pHashEntry->nMaxStackSize = 0;
    }

    SIZE_T bytesToRead = MAX_CALLSTACK_LEN_BUF;
    if (pHashEntry->nMaxStackSize > 0)
    {
      SIZE_T len = ((SIZE_T) pHashEntry->pStackBaseAddr + pHashEntry->nMaxStackSize) - (SIZE_T)pHashEntry->pCallstackOffset;
      bytesToRead = min(len, MAX_CALLSTACK_LEN_BUF);
    }
    // Now read the callstack:
    if (ReadProcessMemory(GetCurrentProcess(), (LPCVOID) pHashEntry->pCallstackOffset, &(pHashEntry->pcCallstackAddr), bytesToRead, &(pHashEntry->nCallstackLen)) == 0) 
    {
      // Could not read memory...
      pHashEntry->nCallstackLen = 0;
      pHashEntry->pCallstackOffset = 0;
    }  // read callstack
  }  // Insert

  BOOL Remove(HASHTABLE_KEY &key)
  {
    SIZE_T HashIdx;
    AllocHashEntryType *pHashEntry, *pHashEntryLast;

    // get the Hash-Value
    HashIdx = HashFunction(key);

    pHashEntryLast = NULL;
    pHashEntry = &pAllocHashTable[HashIdx];
    while(pHashEntry != NULL) {
      if (pHashEntry->key == key) {
        // release my memory
        if (pHashEntryLast == NULL) {
          // It is an entry in the table, so do not release this memory
          if (pHashEntry->Next == NULL) {
            // It was the last entry, so empty the table entry
            SetEmptyKey(pAllocHashTable[HashIdx].key);
            //memset(&pAllocHashTable[HashIdx], 0, sizeof(pAllocHashTable[HashIdx]));
          }
          else {
            // There are some more entries, so shorten the list
            AllocHashEntryType *pTmp = pHashEntry->Next;
            *pHashEntry = *(pHashEntry->Next);
            own_free(pTmp);
          }
          return TRUE;
        }
        else {
          // now, I am in an dynamic allocated entry (it was a collision)
          pHashEntryLast->Next = pHashEntry->Next;
          own_free(pHashEntry);
          return TRUE;
        }
      }
      pHashEntryLast = pHashEntry;
      pHashEntry = pHashEntry->Next;
    }

    // if we are here, we could not find the RequestID
    return FALSE;
  }

  AllocHashEntryType *Find(HASHTABLE_KEY &key)
  {
    SIZE_T HashIdx;
    AllocHashEntryType *pHashEntry;

    // get the Hash-Value
    HashIdx = HashFunction(key);

    pHashEntry = &pAllocHashTable[HashIdx];
    while(pHashEntry != NULL) {
      if (pHashEntry->key == key) {
        return pHashEntry;
      }
      pHashEntry = pHashEntry->Next;
    }

    // entry was not found!
    return NULL;
  }

  // For the followong static-var See comment in "ShowCallstack"...
  static BOOL CALLBACK ReadProcessMemoryFromHashEntry64(
    HANDLE hProcess, // hProcess must be a pointer to an hash-entry!
    DWORD64 lpBaseAddress, 
    PVOID lpBuffer, 
    DWORD nSize, 
    LPDWORD lpNumberOfBytesRead,
    LPVOID pUserData  // optional data, which was passed in "ShowCallstack"
    )
  {
    *lpNumberOfBytesRead = 0;
    AllocHashEntryType *pHashEntry = (AllocHashEntryType*) pUserData;
    if (pHashEntry == NULL)
    {
      return FALSE;
    }

    if ( ( (DWORD64)lpBaseAddress >= (DWORD64)pHashEntry->pCallstackOffset) && ((DWORD64)lpBaseAddress <= ((DWORD64)pHashEntry->pCallstackOffset+pHashEntry->nCallstackLen)) ) {
      // Memory is located in saved Callstack:
      // Calculate the offset
      DWORD dwOffset = (DWORD) ((DWORD64)lpBaseAddress - (DWORD64)pHashEntry->pCallstackOffset);
      DWORD dwSize = __min(nSize, MAX_CALLSTACK_LEN_BUF-dwOffset);
      memcpy(lpBuffer, &(pHashEntry->pcCallstackAddr[dwOffset]), dwSize);
      *lpNumberOfBytesRead = dwSize;
      if (dwSize != nSize)
      {
        return FALSE;
      }
      *lpNumberOfBytesRead = nSize;
      return TRUE;
    }

    if (*lpNumberOfBytesRead == 0)  // Memory could not be found
    {
      if ( ( (DWORD64)lpBaseAddress < (DWORD64)pHashEntry->pStackBaseAddr) || ((DWORD64)lpBaseAddress > ((DWORD64)pHashEntry->pStackBaseAddr+pHashEntry->nMaxStackSize)) )
      {
        // Stackwalking is done by reading the "real memory" (normally this happens when the StackWalk64 tries to read some code)
        SIZE_T st = 0;
        BOOL bRet = ReadProcessMemory(hProcess, (LPCVOID) lpBaseAddress, lpBuffer, nSize, &st);
        *lpNumberOfBytesRead = (DWORD) st;
        return bRet;
      }
    }

    return TRUE;
  }

  VOID ShowLeaks(LeakFinderOutput &leakFinderOutput)
  {
    SIZE_T ulTemp;
    AllocHashEntryType *pHashEntry;
    ULONG ulCount = 0;
    SIZE_T ulLeaksByte = 0;

    leakFinderOutput.OnLeakSearchStart(this->m_finderName);

    // Move throu every entry
    CHAR keyName[1024];
    for(ulTemp = 0; ulTemp < this->sAllocEntries; ulTemp++) {
      pHashEntry = &pAllocHashTable[ulTemp];
      if (IsKeyEmpty(pHashEntry->key) == FALSE) {
        while(pHashEntry != NULL) {
          ulCount++;
          CONTEXT c;
          memcpy(&c, &(pHashEntry->c), sizeof(CONTEXT));

          this->GetKeyAsString(pHashEntry->key, keyName, 1024);

          leakFinderOutput.OnLeakStartEntry(keyName, pHashEntry->nDataSize);
          leakFinderOutput.ShowCallstack(GetCurrentThread(), &c, ReadProcessMemoryFromHashEntry64, pHashEntry);

          // Count the number of leaky bytes
          ulLeaksByte += pHashEntry->nDataSize;

          pHashEntry = pHashEntry->Next;
        }  // while
      }
    }
  }

  AllocHashEntryType *pAllocHashTable;
  SIZE_T sAllocEntries;
  HANDLE m_hHeap;
  LPSTR m_finderName;
  bool m_bSupressUselessLines;
};  // template <typename HASHTABLE_KEY> class ContextHashtableBase


// ##########################################################################
// ##########################################################################
// ##########################################################################
// Specialization for CRT-Leaks:
// VC5 has excluded all types in release-builds
#ifdef _DEBUG

// The follwoing is copied from dbgint.h:
// <CRT_INTERNALS>
/*
* For diagnostic purpose, blocks are allocated with extra information and
* stored in a doubly-linked list.  This makes all blocks registered with
* how big they are, when they were allocated, and what they are used for.
*/

// forward declaration:
#ifndef _M_CEE_PURE
#define MyAllocHookCallingConvention __cdecl
#endif
#if _MSC_VER >= 1400
#ifdef _M_CEE
#define MyAllocHookCallingConvention __clrcall
#endif
#endif

static int MyAllocHookCallingConvention MyAllocHook(int nAllocType, void *pvData, 
  size_t nSize, int nBlockUse, long lRequest, 
#if _MSC_VER <= 1100 // Special case for VC 5 and before
  const char * szFileName, 
#else
  const unsigned char * szFileName, 
#endif
  int nLine);

static _CRT_ALLOC_HOOK s_pfnOldCrtAllocHook = NULL;
static LONG s_CrtDisableCount = 0;
static LONG s_lMallocCalled = 0;


class CRTTable : public ContextHashtableBase<LONG>
{
public:
  CRTTable() : ContextHashtableBase<LONG>(1021, "CRT-Leaks")
  {
    // save the previous alloc hook
    s_pfnOldCrtAllocHook = _CrtSetAllocHook(MyAllocHook);
  }

  virtual ~CRTTable()
  {
    _CrtSetAllocHook(s_pfnOldCrtAllocHook);
  }

  virtual LONG Disable()
  {
    return InterlockedIncrement(&s_CrtDisableCount);
  }
  virtual LONG Enable()
  {
    return InterlockedDecrement(&s_CrtDisableCount);
  }

  virtual SIZE_T HashFunction(LONG &key)
  {
    // I couldn´t find any better and faster
    return key % sAllocEntries;
  }
  virtual BOOL IsKeyEmpty(LONG &key)
  {
    if (key == 0)
      return TRUE;
    return FALSE;
  }
  virtual VOID SetEmptyKey(LONG &key)
  {
    key = 0;
  }
  virtual VOID GetKeyAsString(LONG &key, CHAR *szName, SIZE_T nBufferLen)
  {
#if _MSC_VER < 1400
    _snprintf_s(szName, nBufferLen, "%d", key);
#else
    _snprintf_s(szName, nBufferLen, nBufferLen, "%d", key);
#endif
  }

protected:
  CHAR *m_pBuffer;
  SIZE_T m_maxBufferLen;
  SIZE_T m_bufferLen;
};  // class CRTTable


#define nNoMansLandSize 4

typedef struct _CrtMemBlockHeader
{
  struct _CrtMemBlockHeader * pBlockHeaderNext;
  struct _CrtMemBlockHeader * pBlockHeaderPrev;
  char *                      szFileName;
  int                         nLine;
#ifdef _WIN64
  /* These items are reversed on Win64 to eliminate gaps in the struct
  * and ensure that sizeof(struct)%16 == 0, so 16-byte alignment is
  * maintained in the debug heap.
  */
  int                         nBlockUse;
  size_t                      nDataSize;
#else  /* _WIN64 */
  size_t                      nDataSize;
  int                         nBlockUse;
#endif  /* _WIN64 */
  long                        lRequest;
  unsigned char               gap[nNoMansLandSize];
  /* followed by:
  *  unsigned char           data[nDataSize];
  *  unsigned char           anotherGap[nNoMansLandSize];
  */
} _CrtMemBlockHeader;
#define pbData(pblock) ((unsigned char *)((_CrtMemBlockHeader *)pblock + 1))
#define pHdr(pbData) (((_CrtMemBlockHeader *)pbData)-1) 
// </CRT_INTERNALS>

static CRTTable *g_pCRTTable = NULL;


// MyAllocHook is Single-Threaded, that means the the calls are serialized in the calling function!
static int MyAllocHook(int nAllocType, void *pvData, 
  size_t nSize, int nBlockUse, long lRequest, 
#if _MSC_VER <= 1100 // Special case for VC 5
  const char * szFileName, 
#else
  const unsigned char * szFileName, 
#endif
  int nLine) 
{
  //static TCHAR *operation[] = { _T(""), _T("ALLOCATIONG"), _T("RE-ALLOCATING"), _T("FREEING") };
  //static TCHAR *blockType[] = { _T("Free"), _T("Normal"), _T("CRT"), _T("Ignore"), _T("Client") };

#ifdef IGNORE_CRT_ALLOC
  if (_BLOCK_TYPE(nBlockUse) == _CRT_BLOCK)  // Ignore internal C runtime library allocations
    return TRUE;
#endif
  extern int _crtDbgFlag;
  if  ( ((_CRTDBG_ALLOC_MEM_DF & _crtDbgFlag) == 0) && ( (nAllocType == _HOOK_ALLOC) || (nAllocType == _HOOK_REALLOC) ) )
  {
    // Someone has disabled that the runtime should log this allocation
    // so we do not log this allocation
    if (s_pfnOldCrtAllocHook != NULL)
      s_pfnOldCrtAllocHook(nAllocType, pvData, nSize, nBlockUse, lRequest, szFileName, nLine);
    return TRUE;
  }

  // Handle the Disable/Enable setting
  if (InterlockedExchangeAdd(&s_CrtDisableCount, 0) != 0)
    return TRUE;

  // Prevent from reentrat calls
  if (InterlockedIncrement(&s_lMallocCalled) > 1) { // I was already called
    InterlockedDecrement(&s_lMallocCalled);
    // call the previous alloc hook
    if (s_pfnOldCrtAllocHook != NULL)
      s_pfnOldCrtAllocHook(nAllocType, pvData, nSize, nBlockUse, lRequest, szFileName, nLine);
    return TRUE;
  }

  _ASSERT( (nAllocType == _HOOK_ALLOC) || (nAllocType == _HOOK_REALLOC) || (nAllocType == _HOOK_FREE) );
  _ASSERT( ( _BLOCK_TYPE(nBlockUse) >= 0 ) && ( _BLOCK_TYPE(nBlockUse) < 5 ) );

  if (nAllocType == _HOOK_FREE) { // freeing
    // Try to get the header information
    if (_CrtIsValidHeapPointer(pvData)) {  // it is a valid Heap-Pointer
      // get the ID
      _CrtMemBlockHeader *pHead;
      // get a pointer to memory block header
      pHead = pHdr(pvData);
      nSize = pHead->nDataSize;
      lRequest = pHead->lRequest; // This is the ID!

      if (pHead->nBlockUse == _IGNORE_BLOCK)
      {
        InterlockedDecrement(&s_lMallocCalled);
        if (s_pfnOldCrtAllocHook != NULL)
          s_pfnOldCrtAllocHook(nAllocType, pvData, nSize, nBlockUse, lRequest, szFileName, nLine);
        return TRUE;
      }
    }
    if (lRequest != 0) {  // RequestID was found
      g_pCRTTable->Remove(lRequest);
    }
  }  // freeing

  if (nAllocType == _HOOK_REALLOC) { // re-allocating
    // Try to get the header information
    if (_CrtIsValidHeapPointer(pvData)) {  // it is a valid Heap-Pointer
      BOOL bRet;
      LONG lReallocRequest;
      // get the ID
      _CrtMemBlockHeader *pHead;
      // get a pointer to memory block header
      pHead = pHdr(pvData);
      // Try to find the RequestID in the Hash-Table, mark it that it was freed
      lReallocRequest = pHead->lRequest;
      bRet = g_pCRTTable->Remove(lReallocRequest);
    }  // ValidHeapPointer
  }  // re-allocating

  //if ( (g_ulShowStackAtAlloc < 3) && (nAllocType == _HOOK_FREE) ) {
  if (nAllocType == _HOOK_FREE) {
    InterlockedDecrement(&s_lMallocCalled);
    // call the previous alloc hook
    if (s_pfnOldCrtAllocHook != NULL)
      s_pfnOldCrtAllocHook(nAllocType, pvData, nSize, nBlockUse, lRequest, szFileName, nLine);
    return TRUE;
  }

  CONTEXT c;
  GET_CURRENT_CONTEXT(c, CONTEXT_FULL);

  // Only insert in the Hash-Table if it is not a "freeing"
  if (nAllocType != _HOOK_FREE) {
    if(lRequest != 0) // Always a valid RequestID should be provided (see comments in the header)
      g_pCRTTable->Insert(lRequest, c, nSize);
  }

  InterlockedDecrement(&s_lMallocCalled);
  // call the previous alloc hook
  if (s_pfnOldCrtAllocHook != NULL)
    s_pfnOldCrtAllocHook(nAllocType, pvData, nSize, nBlockUse, lRequest, szFileName, nLine);
  return TRUE; // allow the memory operation to proceed
}  // MyAllocHook

#endif  // _DEBUG


// ##########################################################################
// ##########################################################################
// ##########################################################################
// Specialization for COM-Leaks:

// forwards:
class COMTable;
class CMallocSpy : public IMallocSpy
{
public: 
  CMallocSpy() { m_cbRequest = 0; m_cRef = 0; m_disableCount = 0; }
  virtual ~CMallocSpy() {}
  // IUnknown methods
  STDMETHOD(QueryInterface) (REFIID riid, LPVOID *ppUnk);
  STDMETHOD_(ULONG, AddRef) ();
  STDMETHOD_(ULONG, Release) ();
  // IMallocSpy methods
  STDMETHOD_(SIZE_T, PreAlloc) (SIZE_T cbRequest);
  STDMETHOD_(void *, PostAlloc) (void *pActual);
  STDMETHOD_(void *, PreFree) (void *pRequest, BOOL fSpyed);
  STDMETHOD_(void, PostFree) (BOOL fSpyed) { return; };
  STDMETHOD_(SIZE_T, PreRealloc) (void *pRequest, SIZE_T cbRequest, void **ppNewRequest, BOOL fSpyed);
  STDMETHOD_(void *, PostRealloc) (void *pActual, BOOL fSpyed);
  STDMETHOD_(void *, PreGetSize) (void *pRequest, BOOL fSpyed) { return pRequest; }
  STDMETHOD_(SIZE_T, PostGetSize) (SIZE_T cbActual, BOOL fSpyed) { return cbActual; }
  STDMETHOD_(void *, PreDidAlloc) (void *pRequest, BOOL fSpyed) { return pRequest; }
  STDMETHOD_(BOOL, PostDidAlloc) (void *pRequest, BOOL fSpyed, BOOL fActual) { return fActual; }
  STDMETHOD_(void, PreHeapMinimize) (void) { return; }
  STDMETHOD_(void, PostHeapMinimize) (void) { return; }
private:
  LONG m_cRef;
  SIZE_T m_cbRequest;
protected:
  COMTable *m_pComTable;
  LONG m_disableCount;
  friend COMTable;
};

class COMTable : public ContextHashtableBase<LPVOID>
{
public:
  COMTable() : ContextHashtableBase<LPVOID>(1021, "COM-Leaks")
  {
    m_pMallocSpy = new CMallocSpy(); // wird später durch Release freigegeben
    if (m_pMallocSpy != NULL)
    {
      m_pMallocSpy->m_pComTable = this;
      // CoInitilize(); // ??? Is this necessary ?
      HRESULT hr = CoRegisterMallocSpy(m_pMallocSpy);
      if FAILED(hr)
      {
        _tprintf(_T("\nCoRegisterMallocSpay failed with %.8x"), hr);
      }
    }
  }

  virtual ~COMTable()
  {
    if (m_pMallocSpy != NULL)
      m_pMallocSpy->m_pComTable = NULL;
    CoRevokeMallocSpy();
  }

  virtual LONG Disable()
  {
    return InterlockedIncrement(&(m_pMallocSpy->m_disableCount));
  }
  virtual LONG Enable()
  {
    return InterlockedDecrement(&(m_pMallocSpy->m_disableCount));
  }

  virtual SIZE_T HashFunction(LPVOID &key)
  {
    // I couldn´t find any better and faster
#ifdef _M_IX86
#if _MSC_VER > 1100
#pragma warning (push)
#endif
#pragma warning (disable: 4311)
    DWORD llP = (DWORD) key;
#if _MSC_VER > 1100
#pragma warning (pop)
#endif
#else
    ULONGLONG llP = (ULONGLONG) key;
#endif
    return (SIZE_T) llP % sAllocEntries;
  }
  virtual BOOL IsKeyEmpty(LPVOID &key)
  {
    if (key == 0)
      return TRUE;
    return FALSE;
  }
  virtual VOID SetEmptyKey(LPVOID &key)
  {
    key = 0;
  }
  virtual VOID GetKeyAsString(LPVOID &key, CHAR *szName, SIZE_T nBufferLen)
  {
#if _MSC_VER < 1400
    _snprintf_s(szName, nBufferLen, "%p", key);
#else
    _snprintf_s(szName, nBufferLen, nBufferLen, "%p", key);
#endif
  }

  CMallocSpy *m_pMallocSpy;
  friend CMallocSpy;
};  // class COMTable


STDMETHODIMP CMallocSpy::QueryInterface(REFIID riid, LPVOID *ppUnk) {
  HRESULT hr = S_OK;
  if (IsEqualIID(riid, IID_IUnknown)) {
      *ppUnk = (IUnknown *) this;
  }
  else if (IsEqualIID(riid, IID_IMallocSpy)) {
      *ppUnk =  (IMalloc *) this;
  }
  else {
      *ppUnk = NULL;
      hr =  E_NOINTERFACE;
  }
  AddRef();
  return hr;
}
STDMETHODIMP_(ULONG) CMallocSpy::AddRef(void) {
  return (ULONG) InterlockedIncrement(&m_cRef);
}
STDMETHODIMP_(ULONG) CMallocSpy::Release(void) {
  LONG cRef;
  cRef = InterlockedDecrement(&m_cRef);
  if (cRef == 0)
  {
    delete this;
  }
  return (ULONG) cRef;
} 
// IMallocSpy methods
STDMETHODIMP_(SIZE_T) CMallocSpy::PreAlloc(SIZE_T cbRequest) {
  m_cbRequest = cbRequest;
  return cbRequest;
}
STDMETHODIMP_(void *) CMallocSpy::PostAlloc(void *pActual) {
  if (m_pComTable != NULL)
  {
    CONTEXT c;
    GET_CURRENT_CONTEXT(c, CONTEXT_FULL);
    m_pComTable->Insert(pActual, c, m_cbRequest);
  }
  return pActual;
}
STDMETHODIMP_(void *) CMallocSpy::PreFree(void *pRequest, BOOL fSpyed) {
  if (m_pComTable != NULL)
  {
    m_pComTable->Remove(pRequest);
  }
  return pRequest;
}
STDMETHODIMP_(SIZE_T) CMallocSpy::PreRealloc(void *pRequest, SIZE_T cbRequest,
  void **ppNewRequest, BOOL fSpyed) {
  if (m_pComTable != NULL)
  {
    m_pComTable->Remove(pRequest);
  }
  *ppNewRequest = pRequest;  // Bug fixed. Thanx to Christoph Weber
  return cbRequest;
}
STDMETHODIMP_(void *) CMallocSpy::PostRealloc(void *pActual, BOOL fSpyed) {
  if (m_pComTable != NULL)
  {
    CONTEXT c;
    GET_CURRENT_CONTEXT(c, CONTEXT_FULL);
    m_pComTable->Insert(pActual, c, m_cbRequest);
  }
  return pActual;
}




// ##########################################################################
// ##########################################################################
// ##########################################################################
// Init/Deinit functions


static COMTable *g_pCOMTable;
HRESULT InitLeakFinder()
{
  // _X: Disabled COM monitoring: g_pCOMTable = new COMTable();
#ifdef _DEBUG
  g_pCRTTable = new CRTTable();
#endif
  return S_OK;
}

void DeinitLeakFinder(LeakFinderOutput *output)
{
  LeakFinderOutput *pLeakFinderOutput = output;

#ifdef _DEBUG
  g_pCRTTable->Disable();
#endif
  // _X: Disabled COM monitoring: g_pCOMTable->Disable();

  if (pLeakFinderOutput == NULL)
    pLeakFinderOutput = new LeakFinderOutput();

  // explicite load the modules:
  pLeakFinderOutput->LoadModules();

#ifdef _DEBUG
  g_pCRTTable->ShowLeaks(*pLeakFinderOutput);
  if (g_pCRTTable != NULL)
    delete g_pCRTTable;
  g_pCRTTable = NULL;
#endif

  /*
  // _X: Disabled COM monitoring:
  g_pCOMTable->ShowLeaks(*pLeakFinderOutput);
  if (g_pCOMTable != NULL)
    delete g_pCOMTable;
  g_pCOMTable = NULL;
  */

  if (output == NULL)
    delete pLeakFinderOutput;
}
void DeinitLeakFinder()
{
  DeinitLeakFinder(NULL);
}
