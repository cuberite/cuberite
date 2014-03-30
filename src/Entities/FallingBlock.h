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

#include "Entity.h"




class cPlayer;
class cItem;






class cFallingBlock :
	public cEntity
{
	typedef cEntity super;
	
public:
	CLASS_PROTODEF(cFallingBlock);

	/// Creates a new falling block. a_BlockPosition is expected in world coords
	cFallingBlock(const Vector3i & a_BlockPosition, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta);

	BLOCKTYPE  GetBlockType(void) const { return m_BlockType; }
	NIBBLETYPE GetBlockMeta(void) const { return m_BlockMeta; }
	
	// cEntity overrides:
	virtual void SpawnOn(cClientHandle & a_ClientHandle) override;
	virtual void Tick(float a_Dt, cChunk & a_Chunk) override;
	
private:
	BLOCKTYPE  m_BlockType;
	NIBBLETYPE m_BlockMeta;
	Vector3i   m_OriginalPosition;  // Position where the falling block has started, in world coords
} ;




