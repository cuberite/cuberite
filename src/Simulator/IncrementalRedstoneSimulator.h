
#pragma once

#include "RedstoneSimulator.h"

class cWorld;
class cChunk;

cRedstoneSimulator<cChunk, cWorld> * MakeIncrementalRedstoneSimulator(cWorld & a_World);
