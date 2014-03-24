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

// LineBlockTracer.h

// Declares the cLineBlockTracer class representing a cBlockTracer that traces along a straight line between two points





#pragma once

#include "BlockTracer.h"





// fwd: Chunk.h
class cChunk;

// fwd: cChunkMap.h
typedef cItemCallback<cChunk> cChunkCallback;






class cLineBlockTracer :
	public cBlockTracer,
	public cChunkCallback
{
	typedef cBlockTracer super;
	
public:
	cLineBlockTracer(cWorld & a_World, cCallbacks & a_Callbacks);
	
	/// Traces one line between Start and End; returns true if the entire line was traced (until OnNoMoreHits())
	bool Trace(double a_StartX, double a_StartY, double a_StartZ, double a_EndX, double a_EndY, double a_EndZ);

	// Utility functions for simple one-line usage:
	/// Traces one line between Start and End; returns true if the entire line was traced (until OnNoMoreHits())
	static bool Trace(cWorld & a_World, cCallbacks & a_Callbacks, double a_StartX, double a_StartY, double a_StartZ, double a_EndX, double a_EndY, double a_EndZ);

	/// Traces one line between Start and End; returns true if the entire line was traced (until OnNoMoreHits())
	static bool Trace(cWorld & a_World, cCallbacks & a_Callbacks, const Vector3d & a_Start, const Vector3d & a_End);
	
protected:
	// The start point of the trace
	double m_StartX, m_StartY, m_StartZ;
	
	// The end point of the trace
	double m_EndX, m_EndY, m_EndZ;
	
	// The difference in coords, End - Start
	double m_DiffX, m_DiffY, m_DiffZ;
	
	// The increment at which the block coords are going from Start to End; either +1 or -1
	int m_DirX, m_DirY, m_DirZ;
	
	// The current block
	int m_CurrentX, m_CurrentY, m_CurrentZ;
	
	// The face through which the current block has been entered
	char m_CurrentFace;

	
	/// Adjusts the start point above the world to just at the world's top
	void FixStartAboveWorld(void);
	
	/// Adjusts the start point below the world to just at the world's bottom
	void FixStartBelowWorld(void);
	
	/// Calculates the XZ coords of an intersection with the specified Yconst plane; assumes that such an intersection exists
	void CalcXZIntersection(double a_Y, double & a_IntersectX, double & a_IntersectZ);
	
	/// Moves m_Current to the next block on the line; returns false if no move is possible (reached the end)
	bool MoveToNextBlock(void);
	
	// cChunkCallback overrides:
	virtual bool Item(cChunk * a_Chunk) override;
} ;





