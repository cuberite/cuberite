
#pragma once

#include "BlockHandler.h"





class cBlockPistonHandler :
	public cBlockHandler
{
public:
	cBlockPistonHandler(BLOCKTYPE a_BlockType);
	
	virtual void OnDestroyed(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, int a_BlockX, int a_BlockY, int a_BlockZ) override;

	virtual bool GetPlacementBlockTypeMeta(
		cChunkInterface & a_ChunkInterface, cPlayer * a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace,
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) override;

	static NIBBLETYPE RotationPitchToMetaData(double a_Rotation, double a_Pitch)
	{
		if (a_Pitch >= 50)
		{
			return 0x1;
		}
		else if (a_Pitch <= -50)
		{
			return 0x0;
		}
		else
		{
			a_Rotation += 90 + 45;  // So its not aligned with axis

			if (a_Rotation > 360)
			{
				a_Rotation -= 360;
			}
			if ((a_Rotation >= 0) && (a_Rotation < 90))
			{
				return 0x4;
			}
			else if ((a_Rotation >= 180) && (a_Rotation < 270))
			{
				return 0x5;
			}
			else if ((a_Rotation >= 90) && (a_Rotation < 180))
			{
				return 0x2;
			}
			else
			{
				return 0x3;
			}
		}
	}

	static eBlockFace MetaDataToDirection(NIBBLETYPE a_MetaData)
	{
		switch (a_MetaData)
		{
			case 0x0: return BLOCK_FACE_YM;
			case 0x1: return BLOCK_FACE_YP;
			case 0x2: return BLOCK_FACE_ZM;
			case 0x3: return BLOCK_FACE_ZP;
			case 0x4: return BLOCK_FACE_XM;
			case 0x5: return BLOCK_FACE_XP;
			default:
			{
				ASSERT(!"Invalid Metadata");
				return BLOCK_FACE_NONE;
			}
		}
	}

	static void ExtendPiston(int a_BlockX, int a_BlockY, int a_BlockZ, cWorld * a_World);
	static void RetractPiston(int a_BlockX, int a_BlockY, int a_BlockZ, cWorld * a_World);

private:

	/// Returns true if the piston (specified by blocktype) is a sticky piston
	static inline bool IsSticky(BLOCKTYPE a_BlockType) { return (a_BlockType == E_BLOCK_STICKY_PISTON); }

	/// Returns true if the piston (with the specified meta) is extended
	static inline bool IsExtended(NIBBLETYPE a_PistonMeta) { return ((a_PistonMeta & 0x8) != 0x0); }

	/// Returns true if the specified block can be pushed by a piston (and left intact)
	static inline bool CanPush(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
	{
		switch (a_BlockType)
		{
			case E_BLOCK_ANVIL:
			case E_BLOCK_BEDROCK:
			case E_BLOCK_BREWING_STAND:
			case E_BLOCK_CHEST:
			case E_BLOCK_COMMAND_BLOCK:
			case E_BLOCK_DISPENSER:
			case E_BLOCK_DROPPER:
			case E_BLOCK_ENCHANTMENT_TABLE:
			case E_BLOCK_END_PORTAL:
			case E_BLOCK_END_PORTAL_FRAME:
			// Notice the lack of an E_BLOCK_ENDER_CHEST here; its because ender chests can totally be pushed/pulled in MCS :)
			case E_BLOCK_FURNACE:
			case E_BLOCK_LIT_FURNACE:
			case E_BLOCK_HOPPER:
			case E_BLOCK_JUKEBOX:
			case E_BLOCK_MOB_SPAWNER:
			case E_BLOCK_NETHER_PORTAL:
			case E_BLOCK_NOTE_BLOCK:
			case E_BLOCK_OBSIDIAN:
			case E_BLOCK_PISTON_EXTENSION:
			case E_BLOCK_TRAPPED_CHEST:
			{
				return false;
			}
			case E_BLOCK_STICKY_PISTON:
			case E_BLOCK_PISTON:
			{
				// A piston can only be pushed if retracted:
				return !IsExtended(a_BlockMeta);
			}
		}
		return true;
	}

	/// Returns true if the specified block can be pulled by a sticky piston
	static inline bool CanPull(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
	{
		if (cBlockInfo::IsPistonBreakable(a_BlockType))
		{
			return false;  // CanBreakPush returns true, but we need false to prevent pulling
		}
		
		return CanPush(a_BlockType, a_BlockMeta);
	}

	/// Returns how many blocks the piston has to push (where the first free space is); < 0 when unpushable
	static int FirstPassthroughBlock(int a_PistonX, int a_PistonY, int a_PistonZ, NIBBLETYPE a_PistonMeta, cWorld * a_World);
} ;





class cBlockPistonHeadHandler :
	public cBlockHandler
{
	typedef cBlockHandler super;
	
public:
	cBlockPistonHeadHandler(void);
	
	virtual void OnDestroyedByPlayer(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ) override;

	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		// No pickups
		// Also with 1.7, the item forms of these technical blocks have been removed, so giving someone this will crash their client...
	}
} ;




