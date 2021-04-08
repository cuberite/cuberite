
// BlockBed.cpp

#include "Globals.h"
#include "BlockBed.h"

#include "BroadcastInterface.h"
#include "../Entities/Player.h"
#include "../World.h"
#include "../BoundingBox.h"
#include "../Mobs/Monster.h"
#include "../BlockEntities/BedEntity.h"





void cBlockBedHandler::OnBroken(
	cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface,
	const Vector3i a_BlockPos,
	BlockState a_OldBlock,
	const cEntity * a_Digger
) const
{
	UNUSED(a_Digger);
	auto Direction = GetBlockFace(a_OldBlock);
	if (IsHeadPart(a_OldBlock))
	{
		// Was pillow
		Vector3i FootPos(a_BlockPos - BlockFaceToDirection(Direction));
		if (a_ChunkInterface.GetBlock(FootPos).Type() == a_OldBlock.Type())
		{
			// First replace the bed with air
			a_ChunkInterface.FastSetBlock(FootPos, Block::Air::Air());

			// Then destroy the bed entity
			a_ChunkInterface.SetBlock(FootPos, Block::Air::Air());
		}
	}
	else
	{
		// Was foot end
		Vector3i PillowPos(a_BlockPos + BlockFaceToDirection(Direction));
		if (a_ChunkInterface.GetBlock(PillowPos) == E_BLOCK_BED)
		{
			// First replace the bed with air
			a_ChunkInterface.FastSetBlock(PillowPos, Block::Air::Air());

			// Then destroy the bed entity
			a_ChunkInterface.SetBlock(PillowPos, Block::Air::Air());
		}
	}
}





bool cBlockBedHandler::OnUse(
	cChunkInterface & a_ChunkInterface,
	cWorldInterface & a_WorldInterface,
	cPlayer & a_Player,
	const Vector3i a_BlockPos,
	eBlockFace a_BlockFace,
	const Vector3i a_CursorPos
) const
{
	// Sleeping in bed only allowed in Overworld, beds explode elsewhere:
	if (a_WorldInterface.GetDimension() != dimOverworld)
	{
		auto PosCopy = a_BlockPos;
		a_WorldInterface.DoExplosionAt(5, a_BlockPos.x, a_BlockPos.y, a_BlockPos.z, true, esBed, &PosCopy);
		return true;
	}

	// Sleeping is allowed only during night and thunderstorms:
	if (
		!(((a_WorldInterface.GetTimeOfDay() > 12541) && (a_WorldInterface.GetTimeOfDay() < 23458)) ||
		(a_Player.GetWorld()->GetWeather() == wThunderstorm))
	)  // Source: https://minecraft.gamepedia.com/Bed#Sleeping
	{
		a_Player.SendAboveActionBarMessage("You can only sleep at night and during thunderstorms");

		// Try to set home position anyway:
		SetBedPos(a_Player, a_BlockPos);
		return true;
	}

	auto Self = a_ChunkInterface.GetBlock(a_BlockPos);

	// Check if the bed is occupied:
	if (IsBedOccupied(Self))
	{
		a_Player.SendMessageFailure("This bed is occupied");
		return true;
	}

	// Cannot sleep if there are hostile mobs nearby:
	auto FindMobs = [](cEntity & a_Entity)
	{
		return (
			(a_Entity.GetEntityType() == cEntity::etMonster) &&
			(static_cast<cMonster&>(a_Entity).GetMobFamily() == cMonster::mfHostile)
		);
	};
	if (!a_Player.GetWorld()->ForEachEntityInBox(cBoundingBox(a_Player.GetPosition() - Vector3i(0, 5, 0), 8, 10), FindMobs))
	{
		a_Player.SendAboveActionBarMessage("You may not rest now, there are monsters nearby");
		return true;
	}

	// Broadcast the "Use bed" for the pillow block:
	if (IsHeadPart(Self))
	{
		// Is pillow
		a_WorldInterface.GetBroadcastManager().BroadcastUseBed(a_Player, a_BlockPos);
	}
	else
	{
		// Is foot end
		VERIFY(!IsBedOccupied(Self));  // Occupied flag should never be set, else our compilator (intended) is broken

		auto PillowPos = a_BlockPos + BlockFaceToDirection(GetBlockFace(Self));
		if (a_ChunkInterface.GetBlock(PillowPos).Type() == Self.Type())  // Must always use pillow location for sleeping
		{
			a_WorldInterface.GetBroadcastManager().BroadcastUseBed(a_Player, PillowPos);
		}
	}

	// Occupy the bed:
	SetBedPos(a_Player, a_BlockPos);
	SetBedOccupationState(a_ChunkInterface, a_Player.GetLastBedPos(), true);
	a_Player.SetIsInBed(true);
	a_Player.GetStatManager().AddValue(Statistic::SleepInBed);

	// Fast-forward the time if all players in the world are in their beds:
	auto TimeFastForwardTester = [](cPlayer & a_OtherPlayer)
	{
		return !a_OtherPlayer.IsInBed();
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
		SetBedOccupationState(a_ChunkInterface, a_Player.GetLastBedPos(), false);
	}
	return true;
}





void cBlockBedHandler::OnPlacedByPlayer(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer & a_Player, const sSetBlock & a_BlockChange) const
{
	a_Player.GetWorld()->DoWithBlockEntityAt(a_BlockChange.GetAbsolutePos(), [&a_Player](cBlockEntity & a_BlockEntity)
	{
		ASSERT(IsBlockBed(a_BlockEntity.GetBlock()));
		static_cast<cBedEntity &>(a_BlockEntity).SetColor(a_Player.GetEquippedItem().m_ItemDamage);
		return false;
	});
}





void cBlockBedHandler::SetBedPos(cPlayer & a_Player, const Vector3i a_BedPosition)
{
	if (a_Player.GetLastBedPos() != a_BedPosition)
	{
		a_Player.SetBedPos(a_BedPosition);
		a_Player.SendMessageSuccess("Home position set successfully");
	}
}
