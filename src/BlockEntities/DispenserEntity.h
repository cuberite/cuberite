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

#include "DropSpenserEntity.h"





// tolua_begin
class cDispenserEntity :
	public cDropSpenserEntity
{
	typedef cDropSpenserEntity super;
	
public:

	// tolua_end
	
	/// Constructor used for normal operation
	cDispenserEntity(int a_BlockX, int a_BlockY, int a_BlockZ, cWorld * a_World);

	static const char * GetClassStatic(void) { return "cDispenserEntity"; }

private:
	// cDropSpenser overrides:
	virtual void DropSpenseFromSlot(cChunk & a_Chunk, int a_SlotNum) override;
	
	/// If such a bucket can fit, adds it to m_Contents and returns true
	bool ScoopUpLiquid(int a_SlotNum, short a_BucketItemType);
	
	/// If the a_BlockInFront is liquidable and the empty bucket can fit, does the m_Contents processing and returns true
	bool EmptyLiquidBucket(BLOCKTYPE a_BlockInFront, int a_SlotNum);
} ;  // tolua_export




