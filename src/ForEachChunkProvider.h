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

// ForEachChunkProvider.h

// Declares the cForEachChunkProvider class which acts as an interface for classes that provide a ForEachChunkInRect() function
// Primarily serves as a decoupling between cBlockArea and cWorld





#pragma once




// fwd:
class cChunkDataCallback;
class cBlockArea;





class cForEachChunkProvider
{
public:
	/** Calls the callback for each chunk in the specified range. */
	virtual bool ForEachChunkInRect(int a_MinChunkX, int a_MaxChunkX, int a_MinChunkZ, int a_MaxChunkZ, cChunkDataCallback & a_Callback) = 0;
	
	/** Writes the block area into the specified coords.
	Returns true if all chunks have been processed.
	a_DataTypes is a bitmask of cBlockArea::baXXX constants ORed together.
	*/
	virtual bool WriteBlockArea(cBlockArea & a_Area, int a_MinBlockX, int a_MinBlockY, int a_MinBlockZ, int a_DataTypes) = 0;
};




