
// tolua++.h

// Redirection file, needed because ToLua++ generates the Bindings.cpp file with >> #include "tolua++.h" <<
// Only used from Bindings.cpp




#include "tolua++/include/tolua++.h"

#ifdef _MSC_VER
	// Disable specific warnings for the generated Bindings.cpp file:
	#pragma warning(disable: 4800)  // 'int' : forcing value to bool 'true' or 'false' (performance warning)
#endif  // _MSC_VER




