
// BlockBed.cpp

#include "Globals.h"
#include "BlockBed.h"

#include "BroadcastInterface.h"
#include "../Entities/Player.h"
#include "../World.h"
#include "../BoundingBox.h"
#include "../Mobs/Monster.h"





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
		if (IsBlockBed(a_ChunkInterface.GetBlock(PillowPos)))
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
	Vector3i a_BlockPos,
	eBlockFace a_BlockFace,
	const Vector3i a_CursorPos
) const
{
	// Source: https://minecraft.wiki/w/Bed#Sleeping

	// Sleeping in bed only allowed in Overworld, beds explode elsewhere:
	if (a_WorldInterface.GetDimension() != dimOverworld)
	{
		auto PosCopy = a_BlockPos;
		a_WorldInterface.DoExplosionAt(5, a_BlockPos.x, a_BlockPos.y, a_BlockPos.z, true, esBed, &PosCopy);
		return true;
	}

	auto ClickedBlock = a_ChunkInterface.GetBlock(a_BlockPos);

	if (!IsHeadPart(ClickedBlock))
	{
		// Clicked on the foot of the bed, adjust to the head:
		a_BlockPos = AddFaceDirection(a_BlockPos, GetBlockFace(ClickedBlock));
		auto FootBlock = a_ChunkInterface.GetBlock(a_BlockPos);
		if (!IsBlockBed(FootBlock))
		{
			// Bed was incomplete, bail:
			return true;
		}
	}

	// Set the bed position to the pillow block:
	a_Player.SetBedPos(a_BlockPos);

	// Sleeping is allowed only during night and thunderstorms:
	if (
		!(((a_WorldInterface.GetTimeOfDay() > 12541_tick) && (a_WorldInterface.GetTimeOfDay() < 23458_tick)) ||
		(a_Player.GetWorld()->GetWeather() == wThunderstorm))
	)
	{
		a_Player.SendAboveActionBarMessage("You can only sleep at night and during thunderstorms");
		return true;
	}

	auto Self = a_ChunkInterface.GetBlock(a_BlockPos);

	// Check if the bed is occupied:
	if (IsBedOccupied(Self))
	{
		a_Player.SendAboveActionBarMessage("This bed is occupied");
		return true;
	}

	// Cannot sleep if there are hostile mobs nearby:
	if (
		!a_Player.GetWorld()->ForEachEntityInBox({ a_Player.GetPosition().addedY(-5), 8, 10 }, [](cEntity & a_Entity)
		{
			return a_Entity.IsMob() && (static_cast<cMonster&>(a_Entity).GetMobFamily() == cMonster::mfHostile);
		})
	)
	{
		a_Player.SendAboveActionBarMessage("You may not rest now, there are monsters nearby");
		return true;
	}

	// This will broadcast "use bed" for the pillow block, if the player can sleep:
	a_Player.SetIsInBed(true);

	// Check sleeping was successful, if not, bail:
	if (!a_Player.IsInBed())
	{
		return true;
	}

	// Occupy the bed, where 0x4 = occupied bit:
	SetBedOccupationState(a_ChunkInterface, a_BlockPos, true);
	a_Player.GetStatistics().Custom[CustomStatistic::SleepInBed]++;

	// When sleeping, the player's bounding box moves to approximately where his head is.
	// Set the player's position to somewhere close to the edge of the pillow block:
	a_Player.SetPosition(AddFaceDirection(Vector3f(0.4f, 1, 0.4f), GetBlockFace(Self)) + Vector3f(0.5f, 0.6875, 0.5f) + a_BlockPos);

	// Fast-forward the time if all players in the world are in their beds:
	if (a_WorldInterface.ForEachPlayer([](cPlayer & a_OtherPlayer) { return !a_OtherPlayer.IsInBed(); }))
	{
		a_WorldInterface.ForEachPlayer([&a_ChunkInterface](cPlayer & a_OtherPlayer)
		{
			VacateBed(a_ChunkInterface, a_OtherPlayer);
			return false;
		});
		a_WorldInterface.SetTimeOfDay(0_tick);
	}

	return true;
}
