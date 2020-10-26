
// TwoHeights.h

// Declares the function to create a new instance of the cTwoHeights terrain shape generator





#pragma once

#include "ComposableGenerator.h"




/** Creates and returns a new instance of the cTwoHeights terrain shape generator.
The instance must be Initialize()-d before it is used. */
extern cTerrainShapeGenPtr CreateShapeGenTwoHeights(int a_Seed, const cBiomeGenPtr & a_BiomeGen);




