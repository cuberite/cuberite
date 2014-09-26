
#include "Globals.h"

#include "../World.h"
#include "../BlockID.h"
#include "../Defines.h"
#include "../Chunk.h"

#include "Simulator.inc"

#pragma clang diagnostic ignored "-Wweak-template-vtables"
template class cSimulator<cChunk, cWorld>;

