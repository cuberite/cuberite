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

// DropperEntity.h

// Declares the cDropperEntity class representing a dropper block entity





#pragma once

#include "DropSpenserEntity.h"





// tolua_begin
class cDropperEntity :
	public cDropSpenserEntity
{
	typedef cDropSpenserEntity super;
	
public:

	// tolua_end
	
	/// Constructor used for normal operation
	cDropperEntity(int a_BlockX, int a_BlockY, int a_BlockZ, cWorld * a_World);

	static const char * GetClassStatic(void) { return "cDropperEntity"; }

protected:
	// cDropSpenserEntity overrides:
	virtual void DropSpenseFromSlot(cChunk & a_Chunk, int a_SlotNum) override;
	
	/** Takes an item from slot a_SlotNum and puts it into the container in front of the dropper.
	Called when there's a container directly in front of the dropper,
	so the dropper should store items there, rather than dropping.
	*/
	void PutIntoContainer(cChunk & a_Chunk, int a_SlotNum, BLOCKTYPE a_ContainerBlock, int a_ContainerX, int a_ContainerY, int a_ContainerZ);
} ;  // tolua_export




