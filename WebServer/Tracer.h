/*
   Tracer.h

   Copyright (C) 2002-2004 René Nyffenegger

   This source code is provided 'as-is', without any express or implied
   warranty. In no event will the author be held liable for any damages
   arising from the use of this software.

   Permission is granted to anyone to use this software for any purpose,
   including commercial applications, and to alter it and redistribute it
   freely, subject to the following restrictions:

   1. The origin of this source code must not be misrepresented; you must not
      claim that you wrote the original source code. If you use this source code
      in a product, an acknowledgment in the product documentation would be
      appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
      misrepresented as being the original source code.

   3. This notice may not be removed or altered from any source distribution.

   The most current version of Tracer.h can be found at
   http://www.adp-gmbh.ch/cpp/common/Tracer.html

   René Nyffenegger rene.nyffenegger@adp-gmbh.ch
*/

/*
	Note on point 2:
		THIS IS NOT THE ORIGINAL SOURCE1!!1!!!~!!~`1ONE!!`1
*/

#ifndef TRACER_H__
#define TRACER_H__

#ifdef DO_TRACE 

#include <string>
#include <sstream>

#include <windows.h>

#define StartTrace(x)    TraceFunc_::StartTrace_(x)
#define Trace(x)         dummy_____for_trace_func______.Trace_(x)
#define Trace2(x,y)      dummy_____for_trace_func______.Trace_(x,y)
#define TraceFunc(x)     TraceFunc_ dummy_____for_trace_func______(x)
#define TraceFunc2(x,y)  TraceFunc_ dummy_____for_trace_func______(x,y)

class TraceFunc_ {
  std::string func_name_;
  public:
    /*
      Calling TraceFunc_ indents the output until the enclosing scope ( {...} )
      is left.
    */
    TraceFunc_(std::string const&);
    TraceFunc_(std::string const&, std::string const& something);
   ~TraceFunc_();

   /*
      Creates the trace output file named by filename.
      Must be called before any other tracing function.
      Use the StartTrace macro for it.
   */
   static void StartTrace_(std::string const& filename);

   template <class T>
   void Trace_(T const& t) {
     DWORD d;
     std::string indent_s;
     std::stringstream s;

     s << t;
  
     for (int i=0; i< indent; i++) indent_s += " ";
  
     ::WriteFile(trace_file_,indent_s.c_str(), indent_s.size(), &d, 0);
     ::WriteFile(trace_file_, s.str().c_str(), s.str().size() ,&d, 0);
     ::WriteFile(trace_file_,"\x0a",1,&d,0);
   }

   template <class T, class U>
   void Trace_(T const& t, U const& u) {
     std::string indent_s;
     std::stringstream s;

     s << t;
     s << u;
     Trace_(s.str());
   }

   static int indent;
   /*  trace_file_ is a HANDLE for the file in which the traced output goes.
       The file is opened (that is, created) by calling StartTrace_.
       Better yet, use the StartTrace macro
       to create the file.
   */
   static HANDLE trace_file_;
};

#else

#define StartTrace(x)
#define Trace(x)
#define Trace2(x,y)
#define TraceFunc(x)
#define TraceFunc2(x,y)

#endif

#endif  // TRACER_H__
