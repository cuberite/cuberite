/**
 * Copyright 2014 MCServer Contributors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

// tolua++.h

// Redirection file, needed because ToLua++ generates the Bindings.cpp file with >> #include "tolua++.h" <<
// Only used from Bindings.cpp




#include "tolua++/include/tolua++.h"

#ifdef _MSC_VER
	// Disable specific warnings for the generated Bindings.cpp file:
	#pragma warning(disable: 4800)  // 'int' : forcing value to bool 'true' or 'false' (performance warning)
#endif  // _MSC_VER




