
#include "Globals.h"

#include "EnderDragonFightStructuresGen.h"
#include "../Chunk.h"
#include "../Entities/EnderCrystal.h"
#include "../Mobs/EnderDragon.h"
#include "../WorldStorage/SchematicFileSerializer.h"

const std::array<Vector3i, 48> cEnderDragonFightStructuresGen::m_CagePos =
{
	{
		// First layer
		{-2, -1, -2}, {-2, -1, -1}, {-2, -1, 0}, {-2, -1, 1}, {-2, -1, 2},
		{2,  -1, -2}, {2,  -1, -1}, {2,  -1, 0}, {2,  -1, 1}, {2,  -1, 2},
		{-1, -1, -2}, {0,  -1, -2}, {1,  -1, -2},
		{-1, -1, 2},  {0,  -1, 2},  {1,  -1, 2},

		// Second layer
		{-2, 0, -2}, {-2, 0, -1}, {-2, 0, 0}, {-2, 0, 1}, {-2, 0, 2},
		{2,  0, -2}, {2,  0, -1}, {2,  0, 0}, {2,  0, 1}, {2,  0, 2},
		{-1, 0, -2}, {0,  0, -2}, {1,  0, -2},
		{-1, 0, 2},  {0,  0, 2},  {1,  0, 2},

		// Third layer
		{-2, 1, -2}, {-2, 1, -1}, {-2, 1, 0}, {-2, 1, 1}, {-2, 1, 2},
		{2,  1, -2}, {2,  1, -1}, {2,  1, 0}, {2,  1, 1}, {2,  1, 2},
		{-1, 1, -2}, {0,  1, -2}, {1,  1, -2},
		{-1, 1, 2},  {0,  1, 2},  {1,  1, 2},
	}
};




const std::array<Vector3i, 26> cEnderDragonFightStructuresGen::m_CageAir =
{
	{
		// First layer
		{-1, -1, -1}, {0, -1, -1}, {1, -1, -1},
		{-1, -1, 1},  {0, -1, 1},  {1, -1, 1},
		{-1, -1, 0}, {1, -1, 0},

		// Second layer
		{-1, 0, -1}, {0, 0, -1}, {1, 0, -1},
		{-1, 0, 1},  {0, 0, 1},  {1, 0, 1},
		{-1, 0, 0}, {1, 0, 0}, {0, 0, 0},

		// Third layer
		{-1, 1, -1}, {0, 1, -1}, {1, 1, -1},
		{-1, 1, 1},  {0, 1, 1},  {1, 1, 1},
		{-1, 1, 0}, {1, 1, 0}, {0, 1, 0},
	}
};





cEnderDragonFightStructuresGen::cEnderDragonFightStructuresGen(int a_Seed, const AString &a_TowerProperties, int a_Radius) :
	m_Noise(a_Seed)
{
	// Loads the fountain schematic
	if (!cSchematicFileSerializer::LoadFromSchematicFile(
		m_Fountain, AString("Prefabs") + cFile::GetPathSeparator() + "SinglePieceStructures" + cFile::GetPathSeparator() + "EndFountain.schematic"))
	{
		LOGWARNING("EnderDragonFightStructuresGen is missing it's end fountain prefab, please update your cuberite server files! There will be no end fountain!");
	}
	// Reads the given tower properties
	const auto TowerPropertiesVector = StringSplitAndTrim(a_TowerProperties, ";");
	std::vector<sTowerProperties> TowerProperties;
	for (const auto & TowerProperty : TowerPropertiesVector)
	{
		const auto TowerPropertyVector = StringSplitAndTrim(TowerProperty, "|");
		if (TowerPropertyVector.size() != 3)
		{
			LOGWARNING("Got unmatching parameters on generating obsidian pillars: %s, Please use \"Height|Radius|HasCage\", ...", TowerProperty);
			continue;
		}
		int Height = std::stoi(TowerPropertyVector[0]);
		int Radius = std::stoi(TowerPropertyVector[1]);
		bool HasCage;
		if (NoCaseCompare(TowerPropertyVector[2], "true") == 0)
		{
			HasCage = true;
		}
		else if (NoCaseCompare(TowerPropertyVector[2], "false") == 0)
		{
			HasCage = false;
		}
		else
		{
			LOGWARNING("Got unknown value for boolean if the tower: %s should have a cage! %s", TowerProperty, TowerPropertyVector[2]);
			continue;
		}
		TowerProperties.push_back({Vector3d(), Height, Radius, HasCage});
	}
	// A random angle in radian
	double Angle = m_Noise.IntNoise1D(a_Seed) * M_PI + M_PI;
	// Shuffles the order of the towers
	std::shuffle(TowerProperties.begin(), TowerProperties.end(), std::default_random_engine(a_Seed));
	// Generate Positions in a circle
	for (int I = 0; I < static_cast<int>(TowerProperties.size()); I++)
	{
		auto TowerPos = Vector3i(FloorC(a_Radius * cos(Angle)), 0, FloorC(a_Radius * sin(Angle)));
		TowerProperties[I].m_Pos = TowerPos;

		// Check all crossed chunks
		for (int X = -TowerProperties[I].m_Radius - cChunkDef::Width; X <= TowerProperties[I].m_Radius + cChunkDef::Width; X+=std::min(TowerProperties[I].m_Radius, cChunkDef::Width))
		{
			for (int Z = -TowerProperties[I].m_Radius - cChunkDef::Width; Z <= TowerProperties[I].m_Radius + cChunkDef::Width; Z+=std::min(TowerProperties[I].m_Radius, cChunkDef::Width))
			{
				auto Chunk = cChunkDef::BlockToChunk({TowerPos.x + X, 0, TowerPos.z + Z});
				// Update limits
				m_MinX = std::min(Chunk.m_ChunkX, m_MinX);
				m_MinZ = std::min(Chunk.m_ChunkZ, m_MinZ);

				m_MaxX = std::max(Chunk.m_ChunkX, m_MaxX);
				m_MaxZ = std::max(Chunk.m_ChunkZ, m_MaxZ);
				// If the tower is not in chunk put it in
				bool ShouldPlace = true;
				for (const auto & Property : m_TowerPos[Chunk])
				{
					ShouldPlace &= !(TowerPos == Property.m_Pos);
				}
				if (ShouldPlace)
				{
					m_TowerPos[Chunk].emplace_back(TowerProperties[I]);
				}
			}
		}
		// Generate angle of next tower
		Angle = fmod(Angle + (2.0 * M_PI / static_cast<double>(TowerProperties.size())), 2.0 * M_PI);
	}
}





void cEnderDragonFightStructuresGen::GenFinish(cChunkDesc &a_ChunkDesc)
{
	auto Coords = a_ChunkDesc.GetChunkCoords();
	// If not in the chunks to write
	if ((Coords.m_ChunkX > m_MaxX) ||
		(Coords.m_ChunkX < m_MinX) ||
		(Coords.m_ChunkZ > m_MaxZ) ||
		(Coords.m_ChunkZ < m_MinZ))
	{
		return;
	}
	// Places the exit portal
	if (Coords == cChunkCoords({0, 0}))
	{
		auto EnderDragon = std::make_unique<cEnderDragon>();
		EnderDragon->SetPosition({0.0, 80.0, 0.0});
		a_ChunkDesc.GetEntities().emplace_back(std::move(EnderDragon));
		a_ChunkDesc.WriteBlockArea(m_Fountain,
			static_cast<int>(FloorC(-m_Fountain.GetSizeX() / 2)),
			62,
			static_cast<int>(FloorC(-m_Fountain.GetSizeX() / 2)),
			cBlockArea::msSpongePrint
		);
	}
	else if (Coords == cChunkCoords({-1, 0}))
	{
		a_ChunkDesc.WriteBlockArea(m_Fountain,
			cChunkDef::Width - static_cast<int>(FloorC(m_Fountain.GetSizeX() / 2)),
			62,
			static_cast<int>(FloorC(-m_Fountain.GetSizeZ() / 2)),
			cBlockArea::msSpongePrint
		);
	}
	else if (Coords == cChunkCoords({0, -1}))
	{
		a_ChunkDesc.WriteBlockArea(m_Fountain,
			static_cast<int>(FloorC(-m_Fountain.GetSizeX() / 2)),
			62,
			cChunkDef::Width - static_cast<int>(FloorC(m_Fountain.GetSizeZ() / 2)),
			cBlockArea::msSpongePrint);
	}
	else if (Coords == cChunkCoords({-1, -1}))
	{
		a_ChunkDesc.WriteBlockArea(m_Fountain,
			cChunkDef::Width - static_cast<int>(FloorC(m_Fountain.GetSizeX() / 2)),
			62,
			cChunkDef::Width - static_cast<int>(FloorC(m_Fountain.GetSizeZ() / 2)),
			cBlockArea::msSpongePrint);
	}
	auto It = m_TowerPos.find(Coords);
	if (It == m_TowerPos.end())
	{
		return;
	}
	for (const auto & Property : It->second)
	{
		PlaceTower(a_ChunkDesc, Property);
	}
}





void cEnderDragonFightStructuresGen::PlaceTower(cChunkDesc &a_ChunkDesc, const sTowerProperties & a_Properties)
{
	auto Pos = cChunk::AbsoluteToRelative(a_Properties.m_Pos, a_ChunkDesc.GetChunkCoords());

	// Place obsidian pillar
	for (int X = -a_Properties.m_Radius; X < a_Properties.m_Radius; X++)
	{
		for (int Z = -a_Properties.m_Radius; Z < a_Properties.m_Radius; Z++)
		{
			Vector3i NewPos = {Pos.x + X, 1, Pos.z + Z};
			if (cChunkDef::IsValidRelPos(NewPos))
			{
				// The 3 was achieved by trial and error till the shape matched the notchian implementation
				if ((NewPos - Vector3i(Pos.x, 1, Pos.z)).SqrLength() < a_Properties.m_Radius * a_Properties.m_Radius - 3)
				{
					for (int Y = 0; Y <= a_Properties.m_Height - 2; Y++)
					{
						a_ChunkDesc.SetBlockType(NewPos.x, Y, NewPos.z, E_BLOCK_OBSIDIAN);
					}
				}
			}
		}
	}

	// Spawn the iron bars if there are some
	if (a_Properties.m_HasCage)
	{
		// Place walls
		for (const auto & Offset : m_CagePos)
		{
			if (cChunkDef::IsValidRelPos(Vector3d(Pos.x, a_Properties.m_Height, Pos.z) + Offset))
			{
				a_ChunkDesc.SetBlockType(Pos.x + Offset.x, a_Properties.m_Height + Offset.y, Pos.z + Offset.z, E_BLOCK_IRON_BARS);
			}
		}
		// Remove any block that may generate inside the cage
		for (const auto & Offset : m_CageAir)
		{
			if (cChunkDef::IsValidRelPos(Pos + Offset))
			{
				a_ChunkDesc.SetBlockType(Pos.x + Offset.x, a_Properties.m_Height + Offset.y, Pos.z + Offset.z, E_BLOCK_AIR);
			}
		}
		// Place roof
		for (int X = -2; X <= 2; ++X)
		{
			for (int Z = -2; Z <= 2; ++Z)
			{
				if (cChunkDef::IsValidRelPos({Pos.x + X, 1, Pos.z + Z}))
				{
					a_ChunkDesc.SetBlockType(Pos.x + X, a_Properties.m_Height + 2, Pos.z + Z, E_BLOCK_IRON_BARS);
				}
			}
		}
	}
	// Place the top decoration if the origin is in this chunk
	if (cChunkDef::IsValidRelPos(Pos))
	{
		// Spawn the bedrock
		a_ChunkDesc.SetBlockType(Pos.x, a_Properties.m_Height - 1, Pos.z, E_BLOCK_BEDROCK);
		// Spawn the fire
		a_ChunkDesc.SetBlockType(Pos.x, a_Properties.m_Height, Pos.z, E_BLOCK_FIRE);
		// Spawn the ender crystal of the origin position is in this chunk
		auto EnderCrystal = std::make_unique<cEnderCrystal>(Vector3d(0.5, a_Properties.m_Height, 0.5) + a_Properties.m_Pos);
		a_ChunkDesc.GetEntities().emplace_back(std::move(EnderCrystal));
	}
}
