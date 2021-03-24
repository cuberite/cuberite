
// FinishGenGlowStone.cpp

#include "Globals.h"

#include "./FinishGenGlowStone.h"
#include "../../BlockInfo.h"

void cFinishGenGlowStone::GenFinish(cChunkDesc & a_ChunkDesc)
{
	int ChunkX = a_ChunkDesc.GetChunkX();
	int ChunkZ = a_ChunkDesc.GetChunkZ();

	// Change the number of attempts to create a vein depending on the maximum height of the chunk. A standard Nether could have 5 veins at most.
	int NumGlowStone = m_Noise.IntNoise2DInt(ChunkX, ChunkZ) % a_ChunkDesc.GetMaxHeight() / 23;

	for (int i = 1; i <= NumGlowStone; i++)
	{
		// The maximum size for a string of glowstone can get 3 - 5 blocks long
		int Size = 3 + m_Noise.IntNoise3DInt(ChunkX, i, ChunkZ) % 3;

		// Generate X / Z coordinates.
		int X = Size + (m_Noise.IntNoise2DInt(i, Size) % (cChunkDef::Width - Size * 2));
		int Z = Size + (m_Noise.IntNoise2DInt(X, i)    % (cChunkDef::Width - Size * 2));

		int Height = a_ChunkDesc.GetHeight(X, Z);
		for (int y = Height; y > Size; y--)
		{
			if (!cBlockInfo::IsSolid(a_ChunkDesc.GetBlockType(X, y, Z)))
			{
				// Current block isn't solid, bail out
				continue;
			}

			if (a_ChunkDesc.GetBlockType(X, y - 1, Z) != E_BLOCK_AIR)
			{
				// The block below isn't air, bail out
				continue;
			}

			if ((m_Noise.IntNoise3DInt(X, y, Z) % 100) < 95)
			{
				// Have a 5% chance of creating the glowstone
				continue;
			}

			TryPlaceGlowstone(a_ChunkDesc, X, y, Z, Size, 5 + m_Noise.IntNoise3DInt(X, y, Z) % 7);
			break;
		}
	}
}





void cFinishGenGlowStone::TryPlaceGlowstone(cChunkDesc & a_ChunkDesc, int a_RelX, int a_RelY, int a_RelZ, int a_Size, int a_NumStrings)
{
	// The starting point of every glowstone string
	Vector3i StartPoint = Vector3i(a_RelX, a_RelY, a_RelZ);

	// Array with possible directions for a string of glowstone to go to.
	const Vector3i AvailableDirections[] =
		{
			{ -1,  0,  0 }, { 1, 0, 0 },
			{  0, -1,  0 },  // Don't let the glowstone go up
			{  0,  0, -1 }, { 0, 0, 1 },

			// Diagonal direction. Only X or Z with Y.
			// If all were changed the glowstone string looks awkward
			{ 0, -1,  1 }, {  1, -1, 0 },
			{ 0, -1, -1 }, { -1, -1, 0 },

		};

	for (int i = 1; i <= a_NumStrings; i++)
	{
		// The current position of the string that is being generated
		Vector3i CurrentPos = Vector3i(StartPoint);

		// A vector where the previous direction of a glowstone string is stored.
		// This is used to make the strings change direction when going one block further
		Vector3i PreviousDirection = Vector3i();

		for (int j = 0; j < a_Size; j++)
		{
			Vector3i Direction = AvailableDirections[static_cast<size_t>(m_Noise.IntNoise3DInt(CurrentPos.x, CurrentPos.y * i, CurrentPos.z)) % ARRAYCOUNT(AvailableDirections)];
			int Attempts = 2;  // multiply by 1 would make no difference, so multiply by 2 instead

			while (Direction.Equals(PreviousDirection))
			{
				// To make the glowstone branches look better we want to make the direction change every time.
				Direction = AvailableDirections[static_cast<size_t>(m_Noise.IntNoise3DInt(CurrentPos.x, CurrentPos.y * i * Attempts, CurrentPos.z)) % ARRAYCOUNT(AvailableDirections)];
				Attempts++;
			}

			// Update the previous direction variable
			PreviousDirection = Direction;

			// Update the position of the glowstone string
			CurrentPos += Direction;
			if (cBlockInfo::IsSolid(a_ChunkDesc.GetBlockType(CurrentPos.x, CurrentPos.y, CurrentPos.z)) && (a_ChunkDesc.GetBlockType(CurrentPos.x, CurrentPos.y, CurrentPos.z) != E_BLOCK_GLOWSTONE))
			{
				// The glowstone hit something solid, and it wasn't glowstone. Stop the string.
				break;
			}

			// Place a glowstone block.
			a_ChunkDesc.SetBlockType(CurrentPos.x, CurrentPos.y, CurrentPos.z, E_BLOCK_GLOWSTONE);
		}
	}
}
