
// BlockBed.cpp

#include "Globals.h"
#include "BlockBed.h"

#include "BroadcastInterface.h"
#include "../Entities/Player.h"
#include "../World.h"
#include "../BoundingBox.h"
#include "../Mobs/Monster.h"
#include "../BlockEntities/BedEntity.h"





void cBlockBedHandler::OnBroken(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, Vector3i a_BlockPos, BLOCKTYPE a_OldBlockType, NIBBLETYPE a_OldBlockMeta)
{
	auto Direction = MetaDataToDirection(a_OldBlockMeta & 0x03);
	if ((a_OldBlockMeta & 0x08) != 0)
	{
		// Was pillow
		if (a_ChunkInterface.GetBlock(a_BlockPos - Direction) == E_BLOCK_BED)
		{
			// First replace the bed with air
			a_ChunkInterface.FastSetBlock(a_BlockPos - Direction, E_BLOCK_AIR, 0);

			// Then destroy the bed entity
			Vector3i PillowPos(a_BlockPos - Direction);
			a_ChunkInterface.SetBlock(PillowPos, E_BLOCK_AIR, 0);
		}
	}
	else
	{
		// Was foot end
		if (a_ChunkInterface.GetBlock(a_BlockPos + Direction) == E_BLOCK_BED)
		{
			// First replace the bed with air
			a_ChunkInterface.FastSetBlock(a_BlockPos + Direction, E_BLOCK_AIR, 0);

			// Then destroy the bed entity
			Vector3i FootPos(a_BlockPos + Direction);
			a_ChunkInterface.SetBlock(FootPos, E_BLOCK_AIR, 0);
		}
	}
}





bool cBlockBedHandler::OnUse(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ)
{
	Vector3i Coords(a_BlockX, a_BlockY, a_BlockZ);
	if (a_WorldInterface.GetDimension() != dimOverworld)
	{
		a_WorldInterface.DoExplosionAt(5, a_BlockX, a_BlockY, a_BlockZ, true, esBed, &Coords);
	}
	else if (!((a_WorldInterface.GetTimeOfDay() > 12541) && (a_WorldInterface.GetTimeOfDay() < 23458)))  // Source: https://minecraft.gamepedia.com/Bed#Sleeping
	{
		a_Player.SendMessageFailure("You can only sleep at night");
	}
	else
	{
		NIBBLETYPE Meta = a_ChunkInterface.GetBlockMeta(Coords);
		if ((Meta & 0x4) == 0x4)
		{
			a_Player.SendMessageFailure("This bed is occupied");
		}
		else
		{
			auto FindMobs = [](cEntity & a_Entity)
			{
				return (
					(a_Entity.GetEntityType() == cEntity::etMonster) &&
					(static_cast<cMonster&>(a_Entity).GetMobFamily() == cMonster::mfHostile)
				);
			};

			if (!a_Player.GetWorld()->ForEachEntityInBox(cBoundingBox(a_Player.GetPosition() - Vector3i(0, 5, 0), 8, 10), FindMobs))
			{
				a_Player.SendMessageFailure("You may not rest now, there are monsters nearby");
			}
			else
			{
				Vector3i PillowDirection(0, 0, 0);

				if ((Meta & 0x8) == 0x8)
				{
					// Is pillow
					a_WorldInterface.GetBroadcastManager().BroadcastUseBed(a_Player, { a_BlockX, a_BlockY, a_BlockZ });
				}
				else
				{
					// Is foot end
					VERIFY((Meta & 0x4) != 0x4);  // Occupied flag should never be set, else our compilator (intended) is broken

					PillowDirection = MetaDataToDirection(Meta & 0x3);
					if (a_ChunkInterface.GetBlock(Coords + PillowDirection) == E_BLOCK_BED)  // Must always use pillow location for sleeping
					{
						a_WorldInterface.GetBroadcastManager().BroadcastUseBed(a_Player, Vector3i{a_BlockX, a_BlockY, a_BlockZ} + PillowDirection);
					}
				}

				a_Player.SetBedPos(Coords);
				SetBedOccupationState(a_ChunkInterface, a_Player.GetLastBedPos(), true);
				a_Player.SetIsInBed(true);
				a_Player.SendMessageSuccess("Home position set successfully");

				auto TimeFastForwardTester = [](cPlayer & a_OtherPlayer)
				{
					if (!a_OtherPlayer.IsInBed())
					{
						return true;
					}
					return false;
				};

				if (a_WorldInterface.ForEachPlayer(TimeFastForwardTester))
				{
					a_WorldInterface.ForEachPlayer([&](cPlayer & a_OtherPlayer)
						{
							cBlockBedHandler::SetBedOccupationState(a_ChunkInterface, a_OtherPlayer.GetLastBedPos(), false);
							a_OtherPlayer.SetIsInBed(false);
							return false;
						}
					);
					a_WorldInterface.SetTimeOfDay(0);
					a_ChunkInterface.SetBlockMeta({a_BlockX, a_BlockY, a_BlockZ}, Meta & 0x0b);  // Clear the "occupied" bit of the bed's block
				}
			}
		}
	}
	return true;
}





void cBlockBedHandler::OnPlacedByPlayer(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer & a_Player, const sSetBlock & a_BlockChange)
{
	a_Player.GetWorld()->DoWithBedAt(a_BlockChange.GetX(), a_BlockChange.GetY(), a_BlockChange.GetZ(), [&](cBedEntity & a_Bed)
		{
			a_Bed.SetColor(a_Player.GetEquippedItem().m_ItemDamage);
			return true;
		}
	);
}





cItems cBlockBedHandler::ConvertToPickups(NIBBLETYPE a_BlockMeta, cBlockEntity * a_BlockEntity, const cEntity * a_Digger, const cItem * a_Tool)
{
	short color = E_META_WOOL_RED;
	if (a_BlockEntity != nullptr)
	{
		color = reinterpret_cast<cBedEntity *>(a_BlockEntity)->GetColor();
	}
	return cItem(E_ITEM_BED, 1, color);
}
