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

// VoronoiMap.h

// Declares the cVoronoiMap class that implements a Voronoi algorithm over a noise to produce a map





#pragma once

#include "Noise.h"





class cVoronoiMap
{
public:
	cVoronoiMap(int a_Seed, int a_CellSize = 128);
	
	/// Sets the cell size used for generating the Voronoi seeds
	void SetCellSize(int a_CellSize);
	
	/// Returns the value in the cell into which the specified point lies
	int GetValueAt(int a_X, int a_Y);
	
	/// Returns the value in the cell into which the specified point lies, and the distance to the nearest Voronoi seed
	int GetValueAt(int a_X, int a_Y, int & a_MinDistance);
	
	/// Returns the value in the cell into which the specified point lies, and the distances to the 2 nearest Voronoi seeds
	int GetValueAt(int a_X, int a_Y, int & a_MinDistance1, int & a_MinDistance2);
	
protected:
	/// The noise used for generating Voronoi seeds
	cNoise m_Noise;
	
	/// Size of the Voronoi cells (avg X/Y distance between the seeds)
	int m_CellSize;
} ;




