
#pragma once

#include "BlockHandler.h"
#include "../Entities/Player.h"
#include "Vector3.h"
#include "../LineBlockTracer.h"





class cBlockLilypadHandler :
	public cBlockHandler
{
	typedef cBlockHandler super;

public:
	cBlockLilypadHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{

	}

	virtual bool GetPlacementBlockTypeMeta(
		cChunkInterface & a_ChunkInterface, cPlayer * a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace,
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
		) override
	{
		if (a_BlockFace > 0)
		{
			return false;
		}

		class cCallbacks :
			public cBlockTracer::cCallbacks
		{
		public:
			cCallbacks(void) :
				m_HasHitFluid(false)
			{
			}

			virtual bool OnNextBlock(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, char a_EntryFace) override
			{
				if (IsBlockWater(a_BlockType) || IsBlockLava(a_BlockType))
				{
					if ((a_BlockMeta != 0) || (a_EntryFace == BLOCK_FACE_NONE)) // The hit block should be a source. The FACE_NONE check is for AddFaceDir below
					{
						return false;
					}
					m_HasHitFluid = true;
					AddFaceDirection(a_BlockX, a_BlockY, a_BlockZ, (eBlockFace)a_EntryFace);
					m_Pos.Set(a_BlockX, a_BlockY, a_BlockZ);
					return true;
				}
				return false;
			}

			Vector3i m_Pos;
			bool m_HasHitFluid;

		} Callbacks;

		cLineBlockTracer Tracer(*a_Player->GetWorld(), Callbacks);
		Vector3d Start(a_Player->GetEyePosition() + a_Player->GetLookVector());
		Vector3d End(a_Player->GetEyePosition() + a_Player->GetLookVector() * 5);

		Tracer.Trace(Start.x, Start.y, Start.z, End.x, End.y, End.z);

		if (Callbacks.m_HasHitFluid)
		{
			a_Player->GetWorld()->SetBlock(Callbacks.m_Pos.x, Callbacks.m_Pos.y, Callbacks.m_Pos.z, E_BLOCK_LILY_PAD, 0);
		}

		return false;
	}
};




