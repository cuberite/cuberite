// Mixins.h

// Provides various mixins for easier cBlockHandler descendant implementations

/* The general use case is to derive a handler from these mixins, providing a suitable base to them:
class cBlockAir: public cBlockWithNoDrops<cBlockHandler>;
class cBlockLadder: public cMetaRotator<cClearMetaOnDrop, ...>
*/

#pragma once

#include "../../Item.h"





// MSVC generates warnings for the templated AssertIfNotMatched parameter conditions, so disable it:
#ifdef _MSC_VER
	#pragma warning(disable: 4127)  // Conditional expression is constant
#endif
