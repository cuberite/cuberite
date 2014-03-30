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

// BlockHopper.h

// Declares the cBlockHopperHandler class representing the handler for the Hopper block





class cBlockHopperHandler :
	public cBlockEntityHandler
{
public:
	cBlockHopperHandler(BLOCKTYPE a_BlockType)
		: cBlockEntityHandler(a_BlockType)
	{
	}


	virtual bool GetPlacementBlockTypeMeta(
		cChunkInterface & a_ChunkInterface, cPlayer * a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, 
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) override
	{
		a_BlockType = m_BlockType;
		
		// Convert the blockface into meta:
		switch (a_BlockFace)
		{
			case BLOCK_FACE_BOTTOM: a_BlockMeta = E_META_HOPPER_FACING_YM;  break;
			case BLOCK_FACE_TOP:    a_BlockMeta = E_META_HOPPER_FACING_YM;  break;
			case BLOCK_FACE_EAST:   a_BlockMeta = E_META_HOPPER_FACING_XM;  break;
			case BLOCK_FACE_NORTH:  a_BlockMeta = E_META_HOPPER_FACING_ZP;  break;
			case BLOCK_FACE_SOUTH:  a_BlockMeta = E_META_HOPPER_FACING_ZM;  break;
			case BLOCK_FACE_WEST:   a_BlockMeta = E_META_HOPPER_FACING_XP;  break;
			default:                a_BlockMeta = E_META_HOPPER_UNATTACHED; break;
		}
		return true;
	}
} ;




