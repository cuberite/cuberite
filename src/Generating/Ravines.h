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

// Ravines.h

// Interfaces to the cStructGenRavines class representing the ravine structure generator





#pragma once

#include "ComposableGenerator.h"
#include "../Noise.h"





class cStructGenRavines :
	public cFinishGen
{
public:
	cStructGenRavines(int a_Seed, int a_Size);
	~cStructGenRavines();
	
protected:
	class cRavine;  // fwd: Ravines.cpp
	typedef std::list<cRavine *> cRavines;
	
	cNoise   m_Noise;
	int      m_Size;  // Max size, in blocks, of the ravines generated
	cRavines m_Cache;
	
	/// Clears everything from the cache
	void ClearCache(void);
	
	/// Returns all ravines that *may* intersect the given chunk. All the ravines are valid until the next call to this function.
	void GetRavinesForChunk(int a_ChunkX, int a_ChunkZ, cRavines & a_Ravines);
	
	// cFinishGen override:
	virtual void GenFinish(cChunkDesc & a_ChunkDesc) override;
} ;




