void cMonster::HandleDaylightBurning(cChunk & a_Chunk, bool WouldBurn)
{
	if (!m_BurnsInDaylight)
	{
		return;
	}

	int RelY = POSY_TOINT;
	if ((RelY < 0) || (RelY >= cChunkDef::Height))
	{
		// Outside the world
		return;
	}
	if (!a_Chunk.IsLightValid())
	{
		m_World->QueueLightChunk(GetChunkX(), GetChunkZ());
		return;
	}

	if (!IsOnFire() && WouldBurn)
	{
		// Burn for 100 ticks, then decide again
		StartBurning(100);
	}
}




bool cMonster::WouldBurnAt(Vector3d a_Location, cChunk & a_Chunk)
{
	int RelY = FloorC(a_Location.y);
	if (RelY <= 0)
	{
		// The mob is about to die, no point in burning
		return false;
	}
	if (RelY >= cChunkDef::Height)
	{
		// Always burn above the world
		return true;
	}

	PREPARE_REL_AND_CHUNK(a_Location, a_Chunk);
	if (!RelSuccess)
	{
		return false;
	}

	if (
		(Chunk->GetBlock(Rel.x, Rel.y, Rel.z) != E_BLOCK_SOULSAND) &&  // Not on soulsand
		(GetWorld()->GetTimeOfDay() < 12000 + 1000) &&              // Daytime
		GetWorld()->IsWeatherSunnyAt(POSX_TOINT, POSZ_TOINT)        // Not raining
	)
	{
		int MobHeight = static_cast<int>(a_Location.y) + round(GetHeight()) - 1;  // The height of the mob head
		if (MobHeight >= cChunkDef::Height)
		{
			return true;
		}
		// Start with the highest block and scan down to the mob's head.
		// If a non transparent is found, return false (do not burn). Otherwise return true.
		// Note that this loop is not a performance concern as transparent blocks are rare and the loop almost always bailes out
		// instantly.(An exception is e.g. standing under a long column of glass).
		int CurrentBlock = Chunk->GetHeight(Rel.x, Rel.z);
		while (CurrentBlock >= MobHeight)
		{
			BLOCKTYPE Block = Chunk->GetBlock(Rel.x, CurrentBlock, Rel.z);
			if (
				// Do not burn if a block above us meets one of the following conditions:
				(!cBlockInfo::IsTransparent(Block)) ||
				(Block == E_BLOCK_LEAVES) ||
				(Block == E_BLOCK_NEW_LEAVES) ||
				(IsBlockWater(Block))
			)
			{
				return false;
			}
			--CurrentBlock;
		}
		return true;

	}
	return false;
}
