
#include "Globals.h"

#include "../World.h"
#include "../BlockID.h"
#include "../Defines.h"
#include "../Chunk.h"

#include "Simulator.inc"

#ifdef __clang__
	#pragma clang diagnostic ignored "-Wweak-template-vtables"
#endif  // __clang__

template class cSimulator<cChunk, cWorld>;



