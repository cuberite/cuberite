
// CompoGenBiomal.h





#pragma once

#include "ComposableGenerator.h"





/** Returns a new instance of the Biomal composition generator. */
std::unique_ptr<cTerrainCompositionGen> CreateCompoGenBiomal(int a_Seed);
