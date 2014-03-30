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

// DropperEntity.cpp

// Implements the cRtopperEntity class representing a Dropper block entity

#include "Globals.h"
#include "DropperEntity.h"
#include "../Entities/Player.h"
#include "../Simulator/FluidSimulator.h"





cDropperEntity::cDropperEntity(int a_BlockX, int a_BlockY, int a_BlockZ, cWorld * a_World) :
	super(E_BLOCK_DROPPER, a_BlockX, a_BlockY, a_BlockZ, a_World)
{
	SetBlockEntity(this);  // cBlockEntityWindowOwner
}





void cDropperEntity::DropSpenseFromSlot(cChunk & a_Chunk, int a_SlotNum)
{
	DropFromSlot(a_Chunk, a_SlotNum);
}




