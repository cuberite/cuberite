#include "Globals.h"
#include "BlockBed.h"





void cBlockBedHandler::OnPlacedByPlayer(
	cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer * a_Player,
	int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace,
	int a_CursorX, int a_CursorY, int a_CursorZ,
	BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta
)
{
	if (a_BlockMeta < 8)
	{
		Vector3i Direction = MetaDataToDirection(a_BlockMeta);
		a_ChunkInterface.SetBlock(a_WorldInterface, a_BlockX + Direction.x, a_BlockY, a_BlockZ + Direction.z, E_BLOCK_BED, a_BlockMeta | 0x8);
	}
}





void cBlockBedHandler::OnDestroyed(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	NIBBLETYPE OldMeta = a_ChunkInterface.GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ);

	Vector3i ThisPos( a_BlockX, a_BlockY, a_BlockZ);
	Vector3i Direction = MetaDataToDirection( OldMeta & 0x7);
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
	cPlayerBedStateUnsetter(Vector3i a_Position, cWorldInterface & a_WorldInterface) :
		m_Position(a_Position), m_WorldInterface(a_WorldInterface)
	{
	}

	virtual bool Item(cPlayer * a_Player) override
	{
		a_Player->SetIsInBed(false);
		m_WorldInterface.GetBroadcastManager().BroadcastEntityAnimation(*a_Player, 2);
		return false;
	}

private:
	Vector3i m_Position;
	cWorldInterface & m_WorldInterface;
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
			if (Meta & 0x4)
			{
				a_Player->SendMessageFailure("This bed is occupied.");
			}
			else
			{
				Vector3i PillowDirection(0, 0, 0);

				if (Meta & 0x8)
				{
					// Is pillow
					a_WorldInterface.GetBroadcastManager().BroadcastUseBed(*a_Player, a_BlockX, a_BlockY, a_BlockZ);
				}
				else
				{
					// Is foot end
					VERIFY((Meta & 0x4) != 0x4);  // Occupied flag should never be set, else our compilator (intended) is broken

					PillowDirection = MetaDataToDirection(Meta & 0x7);
					if (a_ChunkInterface.GetBlock(a_BlockX + PillowDirection.x, a_BlockY, a_BlockZ + PillowDirection.z) == E_BLOCK_BED)  // Must always use pillow location for sleeping
					{
						a_WorldInterface.GetBroadcastManager().BroadcastUseBed(*a_Player, a_BlockX + PillowDirection.x, a_BlockY, a_BlockZ + PillowDirection.z);
					}
				}

				a_ChunkInterface.SetBlockMeta(a_BlockX, a_BlockY, a_BlockZ, Meta | 0x4);  // Where 0x4 = occupied bit
				a_Player->SetIsInBed(true);

				cTimeFastForwardTester Tester;
				if (a_WorldInterface.ForEachPlayer(Tester))
				{
					cPlayerBedStateUnsetter Unsetter(Vector3i(a_BlockX + PillowDirection.x, a_BlockY, a_BlockZ + PillowDirection.z), a_WorldInterface);
					a_WorldInterface.ForEachPlayer(Unsetter);
					a_WorldInterface.SetTimeOfDay(0);
					a_ChunkInterface.SetBlockMeta(a_BlockX, a_BlockY, a_BlockZ, Meta & 0xB);  // Where 0xB = 1011, and zero is to make sure 'occupied' bit is always unset
				}
			}
		}
		else
		{
			a_Player->SendMessageFailure("You can only sleep at night");
		}
	}
}






