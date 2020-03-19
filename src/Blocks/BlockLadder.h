
#pragma once

#include "BlockHandler.h"
#include "Mixins.h"





class cBlockLadderHandler :
	public cClearMetaOnDrop<cMetaRotator<cBlockHandler, 0x07, 0x02, 0x05, 0x03, 0x04> >
{
	using super = cClearMetaOnDrop<cMetaRotator<cBlockHandler, 0x07, 0x02, 0x05, 0x03, 0x04>>;

public:

	cBlockLadderHandler(BLOCKTYPE a_BlockType):
		super(a_BlockType)
	{
	}





	virtual bool GetPlacementBlockTypeMeta(
		cChunkInterface & a_ChunkInterface, cPlayer & a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace,
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) override
	{
		if (!IsValidDirection(a_BlockFace))
		{
			return false;
		}

		Vector3i Pos{ a_BlockX, a_BlockY, a_BlockZ };
		a_BlockType = m_BlockType;
		a_BlockMeta = DirectionToMetaData(a_BlockFace);

		bool CheckResult = a_Player.GetWorld()->DoWithChunkAt(Pos, [&](cChunk & a_Chunk)
		{
			auto RelPos = cChunkDef::AbsoluteToRelative(Pos);
			return CanBeAt(a_ChunkInterface, RelPos.x, RelPos.y, RelPos.z, a_Chunk, a_BlockMeta);
		});

		if (CheckResult)
		{
			return true;
		}
		else
		{
			a_BlockFace = FindSuitableBlockFace(a_Player.GetWorld(), a_BlockX, a_BlockY, a_BlockZ);
			if (a_BlockFace == BLOCK_FACE_NONE)
			{
				// No attachable face found - don't place block
				return false;
			}
			else
			{
				a_BlockMeta = DirectionToMetaData(a_BlockFace);
				return true;
			}
		}
	}





	static bool IsValidDirection(eBlockFace a_Direction)
	{
		switch (a_Direction)
		{
			case BLOCK_FACE_ZM:
			case BLOCK_FACE_ZP:
			case BLOCK_FACE_XM:
			case BLOCK_FACE_XP:
			{
				return true;
			}
			default:
			{
				return false;
			}
		}
	}

	static NIBBLETYPE DirectionToMetaData(eBlockFace a_Direction)
	{
		switch (a_Direction)
		{
			case BLOCK_FACE_ZM: return 0x2;
			case BLOCK_FACE_ZP: return 0x3;
			case BLOCK_FACE_XM: return 0x4;
			case BLOCK_FACE_XP: return 0x5;
			case BLOCK_FACE_NONE:
			case BLOCK_FACE_YM:
			case BLOCK_FACE_YP:
			{
				return 0x2;
			}
		}
		UNREACHABLE("Unsupported block face");
	}





	static eBlockFace MetaDataToDirection(NIBBLETYPE a_MetaData)
	{
		switch (a_MetaData)
		{
			case 0x2: return BLOCK_FACE_ZM;
			case 0x3: return BLOCK_FACE_ZP;
			case 0x4: return BLOCK_FACE_XM;
			case 0x5: return BLOCK_FACE_XP;
			default:  return BLOCK_FACE_ZM;
		}
	}





	/** Finds a suitable Direction for the Ladder. Returns BLOCK_FACE_NONE on failure */
	eBlockFace FindSuitableBlockFace(cWorld * World, int a_BlockX, int a_BlockY, int a_BlockZ)
	{
		Vector3i Pos{ a_BlockX, a_BlockY, a_BlockZ };
		for (int FaceInt = BLOCK_FACE_ZM; FaceInt <= BLOCK_FACE_XP; FaceInt++)
		{
			eBlockFace Face = static_cast<eBlockFace>(FaceInt);
			NIBBLETYPE BlockMeta = DirectionToMetaData(Face);

			bool CheckResult = World->DoWithChunkAt(Pos, [&](cChunk & a_Chunk)
			{
				auto RelPos = cChunkDef::AbsoluteToRelative(Pos);
				cChunkInterface ChunkInterface(World->GetChunkMap());
				return CanBeAt(ChunkInterface, RelPos.x, RelPos.y, RelPos.z, a_Chunk, BlockMeta);
			});

			if (CheckResult)
			{
				return Face;
			}
		}
		return BLOCK_FACE_NONE;
	}





	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, int a_RelX, int a_RelY, int a_RelZ, const cChunk & a_Chunk, NIBBLETYPE a_BlockMeta) override
	{
		// Can only place at side of block
		eBlockFace Face = MetaDataToDirection(a_BlockMeta);
		if ((Face == BLOCK_FACE_BOTTOM) || (Face == BLOCK_FACE_TOP))
		{
			return false;
		}

		AddFaceDirection(a_RelX, a_RelY, a_RelZ, Face, true);
		BLOCKTYPE BlockIsOnType;
		a_Chunk.UnboundedRelGetBlockType(a_RelX, a_RelY, a_RelZ, BlockIsOnType);

		switch (BlockIsOnType)
		{
			// Can not be placed on glass, leaves, ice, slabs, glowstone, or sea lanterns
			case E_BLOCK_GLASS:
			case E_BLOCK_LEAVES:
			case E_BLOCK_ICE:
			case E_BLOCK_GLOWSTONE:
			case E_BLOCK_SEA_LANTERN:
			{
				return false;
			}
			default:
			{
				if (cBlockSlabHandler::IsAnySlabType(BlockIsOnType))
				{
					return false;
				}

				return cBlockInfo::IsSolid(BlockIsOnType);
			}
		}
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 0;
	}
} ;




