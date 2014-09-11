
#include "Globals.h"

#ifndef SELF_TEST

#include "IncrementalRedstoneSimulator.h"

#include "IncrementalRedstoneSimulator.inc"

#include "Chunk.h"
#include "World.h"
#include "Blocks/GetHandlerCompileTimeTemplate.h"
#include "Blocks/BlockTorch.h"
#include "Blocks/BlockLever.h"
#include "Blocks/BlockButton.h"
#include "Blocks/BlockTripwireHook.h"
#include "Blocks/BlockDoor.h"
#include "Blocks/BlockPiston.h"
#include "BlockEntities/ChestEntity.h"

#pragma clang diagnostic ignored "-Wweak-template-vtables"
template class cIncrementalRedstoneSimulator<cChunk, cWorld, GetHandlerCompileTime, cChestEntity>;
#endif
