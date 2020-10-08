
#include "Globals.h"

#include "EnderDragonFightStructuresGen.h"
#include "../Chunk.h"
#include "../Entities/EnderCrystal.h"

cEnderDragonFightStructuresGen::cEnderDragonFightStructuresGen(int a_Seed) :
	m_Noise(a_Seed)
{
	// Generate 10 Positions where the tower should spawn

	// A random angle in radian
	double Angle = m_Noise.IntNoise1D(a_Seed) * M_PI + M_PI;
	// Generate 10 Positions in a circle with thr radius of 43
	for (int i = 0; i < 10; i++)
	{
		auto TowerPos = Vector3i(static_cast<int>(FloorC(43 * cos(Angle))), 0, static_cast<int>(FloorC(43 * sin(Angle))));
		auto ChunkX = static_cast<int>(FloorC(TowerPos.x / cChunkDef::Width));
		if (TowerPos.x < 0)
		{
			ChunkX--;
		}
		auto ChunkZ = static_cast<int>(FloorC(TowerPos.z / cChunkDef::Width));
		if (TowerPos.z < 0)
		{
			ChunkZ--;
		}

		m_TowerPos[cChunkCoords(ChunkX, ChunkZ)] = TowerPos;
		Angle = fmod(Angle + (M_PI / 5.0f), 2.0f * M_PI);
	}
}





void cEnderDragonFightStructuresGen::GenFinish(cChunkDesc & a_ChunkDesc)
{
	// If not in the chunks to write
	if ((a_ChunkDesc.GetChunkX() > 6) ||
		(a_ChunkDesc.GetChunkX() < -6) ||
		(a_ChunkDesc.GetChunkZ() > 6) ||
		(a_ChunkDesc.GetChunkZ() < -6))
	{
		return;
	}
	// Places the exit portal
	auto Coords = a_ChunkDesc.GetChunkCoords();
	if (Coords == cChunkCoords({0, 0}))
	{
		PlaceFountainSouthEast(a_ChunkDesc);
		return;
	}
	else if (Coords == cChunkCoords({0, -1}))
	{
		PlaceFountainNorthEast(a_ChunkDesc);
		return;
	}
	else if (Coords == cChunkCoords({-1, 0}))
	{
		PlaceFountainSouthWest(a_ChunkDesc);
		return;
	}
	else if (Coords == cChunkCoords({-1, -1}))
	{
		PlaceFountainNorthWest(a_ChunkDesc);
		return;
	}
	auto It = m_TowerPos.find(Coords);
	if (It == m_TowerPos.end())
	{
		return;
	}
	PlaceTower(a_ChunkDesc, It->second);
}





void cEnderDragonFightStructuresGen::PlaceFountainSouthEast(cChunkDesc & a_ChunkDesc) const
{
	// Places the center post
	for (int i = 62; i < 67; i++)
	{
		a_ChunkDesc.SetBlockType(0, i, 0, E_BLOCK_BEDROCK);
	}
	// Places torches
	a_ChunkDesc.SetBlockTypeMeta(0, 65, 1, E_BLOCK_TORCH, E_META_TORCH_SOUTH);
	a_ChunkDesc.SetBlockTypeMeta(1, 65, 0, E_BLOCK_TORCH, E_META_TORCH_EAST);

	// Places floor
	a_ChunkDesc.SetBlockType(0, 62, 1, E_BLOCK_BEDROCK);
	a_ChunkDesc.SetBlockType(0, 62, 2, E_BLOCK_BEDROCK);
	a_ChunkDesc.SetBlockType(0, 63, 3, E_BLOCK_BEDROCK);

	a_ChunkDesc.SetBlockType(1, 62, 0, E_BLOCK_BEDROCK);
	a_ChunkDesc.SetBlockType(1, 62, 1, E_BLOCK_BEDROCK);
	a_ChunkDesc.SetBlockType(1, 62, 2, E_BLOCK_BEDROCK);
	a_ChunkDesc.SetBlockType(1, 63, 3, E_BLOCK_BEDROCK);

	a_ChunkDesc.SetBlockType(2, 62, 0, E_BLOCK_BEDROCK);
	a_ChunkDesc.SetBlockType(2, 62, 1, E_BLOCK_BEDROCK);
	a_ChunkDesc.SetBlockType(2, 63, 2, E_BLOCK_BEDROCK);

	a_ChunkDesc.SetBlockType(3, 63, 0, E_BLOCK_BEDROCK);
	a_ChunkDesc.SetBlockType(3, 63, 1, E_BLOCK_BEDROCK);
}





void cEnderDragonFightStructuresGen::PlaceFountainNorthEast(cChunkDesc &a_ChunkDesc) const
{
	// Places torch
	a_ChunkDesc.SetBlockTypeMeta(0, 65, 15, E_BLOCK_TORCH, E_META_TORCH_NORTH);

	// Places floor
	a_ChunkDesc.SetBlockType(0, 62, 15, E_BLOCK_BEDROCK);
	a_ChunkDesc.SetBlockType(1, 62, 15, E_BLOCK_BEDROCK);
	a_ChunkDesc.SetBlockType(2, 62, 15, E_BLOCK_BEDROCK);
	a_ChunkDesc.SetBlockType(3, 63, 15, E_BLOCK_BEDROCK);

	a_ChunkDesc.SetBlockType(0, 62, 14, E_BLOCK_BEDROCK);
	a_ChunkDesc.SetBlockType(1, 62, 14, E_BLOCK_BEDROCK);
	a_ChunkDesc.SetBlockType(2, 63, 14, E_BLOCK_BEDROCK);

	a_ChunkDesc.SetBlockType(0, 63, 13, E_BLOCK_BEDROCK);
	a_ChunkDesc.SetBlockType(1, 63, 13, E_BLOCK_BEDROCK);
}





void cEnderDragonFightStructuresGen::PlaceFountainSouthWest(cChunkDesc &a_ChunkDesc) const
{
	// Places torch
	a_ChunkDesc.SetBlockTypeMeta(15, 65, 0, E_BLOCK_TORCH, E_META_TORCH_WEST);

	// Places floor
	a_ChunkDesc.SetBlockType(15, 62, 0, E_BLOCK_BEDROCK);
	a_ChunkDesc.SetBlockType(15, 62, 1, E_BLOCK_BEDROCK);
	a_ChunkDesc.SetBlockType(15, 62, 2, E_BLOCK_BEDROCK);
	a_ChunkDesc.SetBlockType(15, 63, 3, E_BLOCK_BEDROCK);

	a_ChunkDesc.SetBlockType(14, 62, 0, E_BLOCK_BEDROCK);
	a_ChunkDesc.SetBlockType(14, 62, 1, E_BLOCK_BEDROCK);
	a_ChunkDesc.SetBlockType(14, 63, 2, E_BLOCK_BEDROCK);

	a_ChunkDesc.SetBlockType(13, 63, 0, E_BLOCK_BEDROCK);
	a_ChunkDesc.SetBlockType(13, 63, 1, E_BLOCK_BEDROCK);
}





void cEnderDragonFightStructuresGen::PlaceFountainNorthWest(cChunkDesc &a_ChunkDesc) const
{
	// Places floor
	a_ChunkDesc.SetBlockType(15, 62, 15, E_BLOCK_BEDROCK);
	a_ChunkDesc.SetBlockType(15, 62, 14, E_BLOCK_BEDROCK);
	a_ChunkDesc.SetBlockType(15, 63, 13, E_BLOCK_BEDROCK);

	a_ChunkDesc.SetBlockType(14, 62, 15, E_BLOCK_BEDROCK);
	a_ChunkDesc.SetBlockType(14, 63, 14, E_BLOCK_BEDROCK);

	a_ChunkDesc.SetBlockType(13, 63, 15, E_BLOCK_BEDROCK);
}





void cEnderDragonFightStructuresGen::PlaceTower(cChunkDesc &a_ChunkDesc, const Vector3i & a_AbsPos)
{
	auto Pos = cChunk::AbsoluteToRelative(a_AbsPos);
	sTowerProperties Properties = {0, 0, false};
	// Choose random height
	int Index = m_Noise.IntNoise3DInt(a_AbsPos) % m_TowerHeights.size();
	do
	{
		Index = (Index + 1) % m_TowerHeights.size();
		Properties = m_TowerHeights[Index];
	} while (Properties.m_Height == 0);

	// Make sure the tower does not cross chunk boarders
	Pos.x = std::max(Pos.x, Properties.m_Radius - 1);
	Pos.x = std::min(Pos.x, cChunkDef::Width - Properties.m_Radius);
	Pos.z = std::max(Pos.z, Properties.m_Radius - 1);
	Pos.z = std::min(Pos.z, cChunkDef::Width - Properties.m_Radius);

	// Place obsidian pillar
	for (int Y = 0; Y <= Properties.m_Height - 2; Y++)
	{
		for (int X = -Properties.m_Radius; X < Properties.m_Radius; X++)
		{
			for (int Z = -Properties.m_Radius; Z < Properties.m_Radius; Z++)
			{
				Vector3i NewPos = {Pos.x + X, Y, Pos.z + Z};
				// The 3 was achieved by trial and error till the shape matched the notchian implementation
				if ((NewPos - Vector3i(Pos.x, Y, Pos.z)).SqrLength() < Properties.m_Radius * Properties.m_Radius - 3)
				{
					a_ChunkDesc.SetBlockType(NewPos.x, NewPos.y, NewPos.z, E_BLOCK_OBSIDIAN);
				}
			}
		}
	}
	// Spawn the bedrock
	a_ChunkDesc.SetBlockType(Pos.x, Properties.m_Height - 1, Pos.z, E_BLOCK_BEDROCK);
	// Spawn the Ender Crystal - the original passed position is not used because the tower might have been moved due to crossing chunk borders
	auto EnderCrystal = std::make_unique<cEnderCrystal>(Vector3d(Pos.x + cChunkDef::Width * a_ChunkDesc.GetChunkX() + 0.5, Properties.m_Height, Pos.z + cChunkDef::Width * a_ChunkDesc.GetChunkZ() + 0.5));
	a_ChunkDesc.GetEntities().emplace_back(std::move(EnderCrystal));
	// Spawn the fire
	a_ChunkDesc.SetBlockType(Pos.x, Properties.m_Height, Pos.z, E_BLOCK_FIRE);
	// Spawn the iron bars if there are some
	if (Properties.m_HasCage)
	{
		// Place walls
		for (int I = -2; I <= 2; I++)
		{
			a_ChunkDesc.SetBlockType(Pos.x - 2, Properties.m_Height - 1, Pos.z + I, E_BLOCK_IRON_BARS);
			a_ChunkDesc.SetBlockType(Pos.x - 2, Properties.m_Height, Pos.z + I, E_BLOCK_IRON_BARS);
			a_ChunkDesc.SetBlockType(Pos.x - 2, Properties.m_Height + 1, Pos.z + I, E_BLOCK_IRON_BARS);
			a_ChunkDesc.SetBlockType(Pos.x + 2, Properties.m_Height - 1, Pos.z + I, E_BLOCK_IRON_BARS);
			a_ChunkDesc.SetBlockType(Pos.x + 2, Properties.m_Height, Pos.z + I, E_BLOCK_IRON_BARS);
			a_ChunkDesc.SetBlockType(Pos.x + 2, Properties.m_Height + 1, Pos.z + I, E_BLOCK_IRON_BARS);

			a_ChunkDesc.SetBlockType(Pos.x + I, Properties.m_Height - 1, Pos.z - 2, E_BLOCK_IRON_BARS);
			a_ChunkDesc.SetBlockType(Pos.x + I, Properties.m_Height, Pos.z - 2, E_BLOCK_IRON_BARS);
			a_ChunkDesc.SetBlockType(Pos.x + I, Properties.m_Height + 1, Pos.z - 2, E_BLOCK_IRON_BARS);

			a_ChunkDesc.SetBlockType(Pos.x + I, Properties.m_Height - 1, Pos.z + 2, E_BLOCK_IRON_BARS);
			a_ChunkDesc.SetBlockType(Pos.x + I, Properties.m_Height, Pos.z + 2, E_BLOCK_IRON_BARS);
			a_ChunkDesc.SetBlockType(Pos.x + I, Properties.m_Height + 1, Pos.z + 2, E_BLOCK_IRON_BARS);
		}
		// Place roof
		for (int X = -2; X <= 2; ++X)
		{
			for (int Z = -2; Z <= 2; ++Z)
			{
				a_ChunkDesc.SetBlockType(Pos.x + X, Properties.m_Height + 2, Pos.z + Z, E_BLOCK_IRON_BARS);
			}
		}
	}
	// Set the height to zero that this tower has been generated
	m_TowerHeights[Index].m_Height = 0;
}
