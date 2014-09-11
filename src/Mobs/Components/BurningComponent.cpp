
#include "Globals.h"

#ifndef SELF_TEST

#include "BurningComponent.h"

#include "BurningComponent.inc"

#include "Chunk.h"
#include "Entities/Entity.h"

template class cBurningComponent<cEntity, cChunk>;
#endif
