
#include "Globals.h"

#include "ChunkDef.h"

// It appears that failing to have this definition causes link errors as cChunkDef::Height is not
// defined. It also appears that we can have the initalizer in the declaration so it can be inlined
// if the declaration is in a class????
const int cChunkDef::Height;
