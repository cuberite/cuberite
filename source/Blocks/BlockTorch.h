#pragma once

#include "BlockHandler.h"
#include "../World.h"





class cBlockTorchHandler :
	public cBlockHandler
{
public:
	cBlockTorchHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}
	
	
	virtual bool GetPlacementBlockTypeMeta(
		cWorld * a_World, cPlayer * a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, 
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) override
	{
		// Find proper placement. Use the player-supplied one as the default, but fix if not okay:
		if (!TorchCanBePlacedAt(a_World, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace))
		{
			a_BlockFace = FindSuitableFace(a_World, a_BlockX, a_BlockY, a_BlockZ);
			
			if (a_BlockFace == BLOCK_FACE_BOTTOM)
			{
				return false;
			}
		}
		a_BlockType = m_BlockType;
		a_BlockMeta = DirectionToMetaData(a_BlockFace);
		return true;
	}
	

	static NIBBLETYPE DirectionToMetaData(char a_Direction)  // tolua_export
	{  // tolua_export
		switch (a_Direction)
		{
			case BLOCK_FACE_BOTTOM: ASSERT(!"Shouldn't be getting this face"); return 0;
			case BLOCK_FACE_TOP:    return E_META_TORCH_FLOOR;
			case BLOCK_FACE_EAST:   return E_META_TORCH_EAST;
			case BLOCK_FACE_WEST:   return E_META_TORCH_WEST;
			case BLOCK_FACE_NORTH:  return E_META_TORCH_NORTH;
			case BLOCK_FACE_SOUTH:  return E_META_TORCH_SOUTH;
			default:
			{
				ASSERT(!"Unhandled torch direction!");
				break;
			}
		};
		return 0x0;
	}  // tolua_export
	

	static char MetaDataToDirection(NIBBLETYPE a_MetaData)  // tolua_export
	{  // tolua_export
		switch (a_MetaData)
		{
			case 0:                  return BLOCK_FACE_TOP;  // by default, the torches stand on the ground
			case E_META_TORCH_FLOOR: return BLOCK_FACE_TOP;
			case E_META_TORCH_EAST:  return BLOCK_FACE_EAST;
			case E_META_TORCH_WEST:  return BLOCK_FACE_WEST;
			case E_META_TORCH_NORTH: return BLOCK_FACE_NORTH;
			case E_META_TORCH_SOUTH: return BLOCK_FACE_SOUTH;
			default:
			{
				ASSERT(!"Unhandled torch metadata");
				break;
			}
		}
		return 0;
	}  // tolua_export


	static bool IsAttachedTo(const Vector3i & a_TorchPos, char a_TorchMeta, const Vector3i & a_BlockPos)
	{
		switch (a_TorchMeta)
		{
			case 0x0:
			case E_META_TORCH_FLOOR: return ((a_TorchPos - a_BlockPos).Equals(Vector3i(0, 1, 0)));
			case E_META_TORCH_EAST:  return ((a_TorchPos - a_BlockPos).Equals(Vector3i(0, 0, -1)));
			case E_META_TORCH_WEST:  return ((a_TorchPos - a_BlockPos).Equals(Vector3i(0, 0, 1)));
			case E_META_TORCH_NORTH: return ((a_TorchPos - a_BlockPos).Equals(Vector3i(-1, 0, 0)));
			case E_META_TORCH_SOUTH: return ((a_TorchPos - a_BlockPos).Equals(Vector3i(1, 0, 0)));
			default:
			{
				ASSERT(!"Unhandled torch meta!");
				break;
			}
		}
		return false;
	}


	virtual bool DoesAllowBlockOnTop(void) override
	{
		return true;
		//was false
	}


	static bool CanBePlacedOn(BLOCKTYPE a_BlockType, char a_Direction)
	{
		switch (a_BlockType)
		{
			case E_BLOCK_GLASS:
			case E_BLOCK_FENCE:
			case E_BLOCK_NETHER_BRICK_FENCE:
			case E_BLOCK_PISTON:
			case E_BLOCK_WORKBENCH:
			{
				return (a_Direction == 0x1);  // allow only direction "standing on floor"
			}
			
			default:
			{
				return g_BlockIsSolid[a_BlockType];
			}
		}
	}
	
	
	static bool TorchCanBePlacedAt(cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace)
	{
		// TODO: If placing a torch from below, check all 4 XZ neighbors, place it on that neighbor instead
		// How to propagate that change up?
		// Simon: The easiest way is to calculate the position two times, shouldn�t cost much cpu power :)

		if (a_BlockFace == BLOCK_FACE_BOTTOM)
		{
			return false;
		}

		AddFaceDirection(a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, true);

		return CanBePlacedOn(a_World->GetBlock(a_BlockX, a_BlockY, a_BlockZ), a_BlockFace);
	}
	
	
	/// Finds a suitable Face for the Torch. Returns BLOCK_FACE_BOTTOM on failure
	static char FindSuitableFace(cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ)
	{
		for (int i = 1; i <= 5; i++)
		{
			if (TorchCanBePlacedAt(a_World, a_BlockX, a_BlockY, a_BlockZ, i))
			{
				return i;
			}
		}
		return BLOCK_FACE_BOTTOM;
	}


	/*
	virtual bool CanBePlacedAt(cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace) override
	{
		if (TorchCanBePlacedAt(a_World, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace))
		{
			return true;
		}

		return (FindSuitableFace(a_World, a_BlockX, a_BlockY, a_BlockZ) != BLOCK_FACE_BOTTOM);
	}
	*/


	virtual bool CanBeAt(int a_RelX, int a_RelY, int a_RelZ, const cChunk & a_Chunk) override
	{
		// TODO: Use AdjustCoordsByMeta(), then cChunk::UnboundedRelGetBlock() and finally some comparison
		char Face = MetaDataToDirection(a_Chunk.GetMeta(a_RelX, a_RelY, a_RelZ));
		int BlockX = a_RelX + a_Chunk.GetPosX() * cChunkDef::Width;
		int BlockZ = a_RelZ + a_Chunk.GetPosZ() * cChunkDef::Width;
		return TorchCanBePlacedAt(a_Chunk.GetWorld(), BlockX, a_RelY, BlockZ, Face);
	}


	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		// Always drop meta = 0
		a_Pickups.push_back(cItem(m_BlockType, 1, 0));
	}


	virtual const char * GetStepSound(void) override
	{
		return "step.wood";
	}
	
	
	virtual NIBBLETYPE MetaRotateCCW(NIBBLETYPE a_Meta) override
	{
		// Bit 4 stays, the rest is swapped around according to a table:
		NIBBLETYPE TopBits = (a_Meta & 0x08);
		switch (a_Meta & 0x07)
		{
			case 0x01: return TopBits | 0x04;  // East  -> North
			case 0x02: return TopBits | 0x03;  // West  -> South
			case 0x03: return TopBits | 0x01;  // South -> East
			case 0x04: return TopBits | 0x02;  // North -> West
			default:   return a_Meta;          // Floor -> Floor
		}
	}


	virtual NIBBLETYPE MetaRotateCW(NIBBLETYPE a_Meta) override
	{
		// Bit 4 stays, the rest is swapped around according to a table:
		NIBBLETYPE TopBits = (a_Meta & 0x08);
		switch (a_Meta & 0x07)
		{
			case 0x01: return TopBits | 0x03;  // East  -> South
			case 0x02: return TopBits | 0x04;  // West  -> North
			case 0x03: return TopBits | 0x02;  // South -> West
			case 0x04: return TopBits | 0x01;  // North -> East
			default:   return a_Meta;          // Floor -> Floor
		}
	}


	virtual NIBBLETYPE MetaMirrorXY(NIBBLETYPE a_Meta) override
	{
		// Bit 4 stays, the rest is swapped around according to a table:
		NIBBLETYPE TopBits = (a_Meta & 0x08);
		switch (a_Meta & 0x07)
		{
			case 0x03: return TopBits | 0x04;  // South -> North
			case 0x04: return TopBits | 0x03;  // North -> South
			default:   return a_Meta;          // Keep the rest
		}
	}


	// Mirroring around the XZ plane doesn't make sense for floor torches,
	// the others stay the same, so let's keep all the metas the same.
	// The base class does tht for us, no need to override MetaMirrorXZ()

	
	virtual NIBBLETYPE MetaMirrorYZ(NIBBLETYPE a_Meta) override
	{
		// Bit 4 stays, the rest is swapped around according to a table:
		NIBBLETYPE TopBits = (a_Meta & 0x08);
		switch (a_Meta & 0x07)
		{
			case 0x01: return TopBits | 0x02;  // East -> West
			case 0x02: return TopBits | 0x01;  // West -> East
			default:   return a_Meta;          // Keep the rest
		}
	}
} ;




