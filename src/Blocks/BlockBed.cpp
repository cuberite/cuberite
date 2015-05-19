#include "Globals.h"
#include "BlockBed.h"

#include "BroadcastInterface.h"
#include "Entities/../World.h"
#include "Entities/Player.h"





void cBlockBedHandler::OnDestroyed(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	NIBBLETYPE OldMeta = a_ChunkInterface.GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ);

	Vector3i ThisPos( a_BlockX, a_BlockY, a_BlockZ);
	Vector3i Direction = MetaDataToDirection( OldMeta & 0x3);
	if (OldMeta & 0x8)
	{
		// Was pillow
		if (a_ChunkInterface.GetBlock(ThisPos - Direction) == E_BLOCK_BED)
		{
			a_ChunkInterface.FastSetBlock(ThisPos - Direction, E_BLOCK_AIR, 0);
		}
	}
	else
	{
		// Was foot end
		if (a_ChunkInterface.GetBlock(ThisPos + Direction) == E_BLOCK_BED)
		{
			a_ChunkInterface.FastSetBlock(ThisPos + Direction, E_BLOCK_AIR, 0);
		}
	}
}





class cTimeFastForwardTester :
	public cPlayerListCallback
{
	virtual bool Item(cPlayer * a_Player) override
	{
		if (!a_Player->IsInBed())
		{
			return true;
		}

		return false;
	}
};





class cPlayerBedStateUnsetter :
	public cPlayerListCallback
{
public:
	cPlayerBedStateUnsetter(Vector3i a_Position, cChunkInterface & a_ChunkInterface) :
		m_Position(a_Position),
		m_ChunkInterface(a_ChunkInterface)
	{
	}

	virtual bool Item(cPlayer * a_Player) override
	{
		cBlockBedHandler::SetBedOccupationState(m_ChunkInterface, a_Player->GetLastBedPos(), false);
		a_Player->SetIsInBed(false);
		return false;
	}

private:
	Vector3i m_Position;
	cChunkInterface & m_ChunkInterface;
};





void cBlockBedHandler::OnUse(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ)
{
	if (a_WorldInterface.GetDimension() != dimOverworld)
	{
		Vector3i Coords(a_BlockX, a_BlockY, a_BlockZ);
		a_WorldInterface.DoExplosionAt(5, a_BlockX, a_BlockY, a_BlockZ, true, esBed, &Coords);
	}
	else
	{
		if (a_WorldInterface.GetTimeOfDay() > 13000)
		{
			NIBBLETYPE Meta = a_ChunkInterface.GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ);
			if ((Meta & 0x4) == 0x4)
			{
				a_Player->SendMessageFailure("This bed is occupied");
			}
			else
			{
				Vector3i PillowDirection(0, 0, 0);

				if ((Meta & 0x8) == 0x8)
				{
					// Is pillow
					a_WorldInterface.GetBroadcastManager().BroadcastUseBed(*a_Player, a_BlockX, a_BlockY, a_BlockZ);
				}
				else
				{
					// Is foot end
					VERIFY((Meta & 0x4) != 0x4);  // Occupied flag should never be set, else our compilator (intended) is broken

					PillowDirection = MetaDataToDirection(Meta & 0x3);
					if (a_ChunkInterface.GetBlock(a_BlockX + PillowDirection.x, a_BlockY, a_BlockZ + PillowDirection.z) == E_BLOCK_BED)  // Must always use pillow location for sleeping
					{
						a_WorldInterface.GetBroadcastManager().BroadcastUseBed(*a_Player, a_BlockX + PillowDirection.x, a_BlockY, a_BlockZ + PillowDirection.z);
					}
				}

				a_Player->SetBedPos(Vector3i(a_BlockX, a_BlockY, a_BlockZ));
				SetBedOccupationState(a_ChunkInterface, a_Player->GetLastBedPos(), true);
				a_Player->SetIsInBed(true);
				a_Player->SendMessageSuccess("Home position set successfully");

				cTimeFastForwardTester Tester;
				if (a_WorldInterface.ForEachPlayer(Tester))
				{
					cPlayerBedStateUnsetter Unsetter(Vector3i(a_BlockX + PillowDirection.x, a_BlockY, a_BlockZ + PillowDirection.z), a_ChunkInterface);
					a_WorldInterface.ForEachPlayer(Unsetter);
					a_WorldInterface.SetTimeOfDay(0);
					a_ChunkInterface.SetBlockMeta(a_BlockX, a_BlockY, a_BlockZ, Meta & 0x0b);  // Clear the "occupied" bit of the bed's block
				}
			}
		}
		else
		{
			a_Player->SendMessageFailure("You can only sleep at night");
		}
	}
}






