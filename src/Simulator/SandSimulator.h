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

#pragma once

#include "Simulator.h"





/// Despite the class name, this simulator takes care of all blocks that fall when suspended in the air.
class cSandSimulator :
	public cSimulator
{
public:
	cSandSimulator(cWorld & a_World, cIniFile & a_IniFile);

	// cSimulator overrides:
	virtual void Simulate(float a_Dt) override { UNUSED(a_Dt);}  // not used
	virtual void SimulateChunk(float a_Dt, int a_ChunkX, int a_ChunkZ, cChunk * a_Chunk) override;
	virtual bool IsAllowedBlock(BLOCKTYPE a_BlockType) override;
	
	/// Returns true if a falling-able block can start falling through the specified block type
	static bool CanStartFallingThrough(BLOCKTYPE a_BlockType);
	
	/// Returns true if an already-falling block can pass through the specified block type (e. g. torch)
	static bool CanContinueFallThrough(BLOCKTYPE a_BlockType);
	
	/// Returns true if the falling block rematerializing will replace the specified block type (e. g. tall grass)
	static bool IsReplacedOnRematerialization(BLOCKTYPE a_BlockType);
	
	/// Returns true if the specified block breaks falling blocks while they fall through it (e. g. halfslabs)
	static bool DoesBreakFallingThrough(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta);
	
	/** Called when a block finishes falling at the specified coords, either by insta-fall,
	or through cFallingBlock entity.
	It either rematerializes the block (a_FallingBlockType) at the specified coords, or creates a pickup,
	based on the block currently present in the world at the dest specified coords
	*/
	static void FinishFalling(
		cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ, 
		BLOCKTYPE a_FallingBlockType, NIBBLETYPE a_FallingBlockMeta
	);

protected:
	bool m_IsInstantFall;  // If set to true, blocks don't fall using cFallingBlock entity, but instantly instead
	
	int  m_TotalBlocks;    // Total number of blocks currently in the queue for simulating
	
	virtual void AddBlock(int a_BlockX, int a_BlockY, int a_BlockZ, cChunk * a_Chunk) override;
	
	/// Performs the instant fall of the block - removes it from top, Finishes it at the bottom
	void DoInstantFall(cChunk * a_Chunk, int a_RelX, int a_RelY, int a_RelZ);
};




/// Per-chunk data for the simulator, specified individual chunks to simulate; Data is not used
typedef cCoordWithIntList cSandSimulatorChunkData;




