
// DungeonRoomsFinisher.cpp

// Declares the cDungeonRoomsFinisher class representing the finisher that generates dungeon rooms

#include "Globals.h"
#include "DungeonRoomsFinisher.h"
#include "../BlockInfo.h"
#include "../FastRandom.h"
#include "../BlockEntities/ChestEntity.h"
#include "../BlockEntities/MobSpawnerEntity.h"





/** Height, in blocks, of the internal dungeon room open space. This many air blocks Y-wise. */
static const int ROOM_HEIGHT = 4;





////////////////////////////////////////////////////////////////////////////////
// cDungeonRoom:

class cDungeonRoom:
	public cGridStructGen::cStructure
{
	using Super = cGridStructGen::cStructure;

public:

	cDungeonRoom(
		int a_GridX, int a_GridZ,
		int a_OriginX, int a_OriginZ,
		int a_HalfSizeX, int a_HalfSizeZ,
		int a_FloorHeight,
		cNoise & a_Noise
	):
		Super(a_GridX, a_GridZ, a_OriginX, a_OriginZ),
		m_StartX(a_OriginX - a_HalfSizeX),
		m_EndX(a_OriginX + a_HalfSizeX),
		m_StartZ(a_OriginZ - a_HalfSizeZ),
		m_EndZ(a_OriginZ + a_HalfSizeZ),
		m_FloorHeight(a_FloorHeight)
	{
		/*
		Pick coords next to the wall for the chests.
		This is done by indexing the possible coords, picking any one for the first chest
		and then picking another position for the second chest that is not adjacent to the first pos
		*/
		int rnd = a_Noise.IntNoise2DInt(a_OriginX, a_OriginZ) / 7;
		int SizeX = m_EndX - m_StartX - 1;
		int SizeZ = m_EndZ - m_StartZ - 1;
		int NumPositions = 2 * SizeX + 2 * SizeZ;
		int FirstChestPos = rnd % NumPositions;  // The corner positions are a bit more likely, but we don't mind
		rnd = rnd / 512;
		int SecondChestPos = (FirstChestPos + 2 + (rnd % (NumPositions - 3))) % NumPositions;
		m_Chest1 = DecodeChestCoords(FirstChestPos,  SizeX, SizeZ);
		m_Chest2 = DecodeChestCoords(SecondChestPos, SizeX, SizeZ);

		// Choose what the mobspawner will spawn.
		// 25% chance for a spider, 25% for a skeleton and 50% chance to get a zombie spawer.
		int MobType = (a_Noise.IntNoise3DInt(a_OriginX, m_FloorHeight, a_OriginZ) / 7) % 100;
		if (MobType <= 25)
		{
			m_MonsterType = mtSkeleton;
		}
		else if (MobType <= 50)
		{
			m_MonsterType = mtSpider;
		}
		else
		{
			m_MonsterType = mtZombie;
		}
	}

protected:

	// The X range of the room, start inclusive, end exclusive:
	int m_StartX, m_EndX;

	// The Z range of the room, start inclusive, end exclusive:
	int m_StartZ, m_EndZ;

	/** The Y coord of the floor of the room */
	int m_FloorHeight;

	/** The (absolute) coords of the first chest. The Y coord represents the chest's Meta value (facing). */
	Vector3i m_Chest1;

	/** The (absolute) coords of the second chest. The Y coord represents the chest's Meta value (facing). */
	Vector3i m_Chest2;

	/** The monster type for the mobspawner entity. */
	eMonsterType m_MonsterType;


	/** Decodes the position index along the room walls into a proper 2D position for a chest.
	The Y coord of the returned vector specifies the chest's meta value. */
	Vector3i DecodeChestCoords(int a_PosIdx, int a_SizeX, int a_SizeZ)
	{
		if (a_PosIdx < a_SizeX)
		{
			// Return a coord on the ZM side of the room:
			return Vector3i(m_StartX + a_PosIdx + 1, E_META_CHEST_FACING_ZP, m_StartZ + 1);
		}
		a_PosIdx -= a_SizeX;
		if (a_PosIdx < a_SizeZ)
		{
			// Return a coord on the XP side of the room:
			return Vector3i(m_EndX - 1, E_META_CHEST_FACING_XM, m_StartZ + a_PosIdx + 1);
		}
		a_PosIdx -= a_SizeZ;
		if (a_PosIdx < a_SizeX)
		{
			// Return a coord on the ZP side of the room:
			return Vector3i(m_StartX + a_PosIdx + 1, E_META_CHEST_FACING_ZM, m_StartZ + 1);
		}
		a_PosIdx -= a_SizeX;
		// Return a coord on the XM side of the room:
		return Vector3i(m_StartX + 1, E_META_CHEST_FACING_XP, m_StartZ + a_PosIdx + 1);
	}



	/** Fills the specified area of blocks in the chunk with the specified blocktype if they are one of the overwritten block types.
	The coords are absolute, start coords are inclusive, end coords are exclusive. */
	void ReplaceCuboid(cChunkDesc & a_ChunkDesc, int a_StartX, int a_StartY, int a_StartZ, int a_EndX, int a_EndY, int a_EndZ, BLOCKTYPE a_DstBlockType)
	{
		int BlockX = a_ChunkDesc.GetChunkX() * cChunkDef::Width;
		int BlockZ = a_ChunkDesc.GetChunkZ() * cChunkDef::Width;
		int RelStartX = Clamp(a_StartX - BlockX, 0, cChunkDef::Width);
		int RelStartZ = Clamp(a_StartZ - BlockZ, 0, cChunkDef::Width);
		int RelEndX   = Clamp(a_EndX - BlockX,   0, cChunkDef::Width);
		int RelEndZ   = Clamp(a_EndZ - BlockZ,   0, cChunkDef::Width);
		for (int y = a_StartY; y < a_EndY; y++)
		{
			for (int z = RelStartZ; z < RelEndZ; z++)
			{
				for (int x = RelStartX; x < RelEndX; x++)
				{
					if (cBlockInfo::CanBeTerraformed(a_ChunkDesc.GetBlockType(x, y, z)))
					{
						a_ChunkDesc.SetBlockType(x, y, z, a_DstBlockType);
					}
				}  // for x
			}  // for z
		}  // for z
	}



	/** Fills the specified area of blocks in the chunk with a random pattern of the specified blocktypes, if they are one of the overwritten block types.
	The coords are absolute, start coords are inclusive, end coords are exclusive. The first blocktype uses 75% chance, the second 25% chance. */
	void ReplaceCuboidRandom(cChunkDesc & a_ChunkDesc, int a_StartX, int a_StartY, int a_StartZ, int a_EndX, int a_EndY, int a_EndZ, BLOCKTYPE a_DstBlockType1, BLOCKTYPE a_DstBlockType2)
	{
		int BlockX = a_ChunkDesc.GetChunkX() * cChunkDef::Width;
		int BlockZ = a_ChunkDesc.GetChunkZ() * cChunkDef::Width;
		int RelStartX = Clamp(a_StartX - BlockX, 0, cChunkDef::Width - 1);
		int RelStartZ = Clamp(a_StartZ - BlockZ, 0, cChunkDef::Width - 1);
		int RelEndX   = Clamp(a_EndX - BlockX,   0, cChunkDef::Width);
		int RelEndZ   = Clamp(a_EndZ - BlockZ,   0, cChunkDef::Width);
		auto & rnd = GetRandomProvider();
		for (int y = a_StartY; y < a_EndY; y++)
		{
			for (int z = RelStartZ; z < RelEndZ; z++)
			{
				for (int x = RelStartX; x < RelEndX; x++)
				{
					if (cBlockInfo::CanBeTerraformed(a_ChunkDesc.GetBlockType(x, y, z)))
					{
						BLOCKTYPE BlockType = rnd.RandBool(0.75) ? a_DstBlockType1 : a_DstBlockType2;
						a_ChunkDesc.SetBlockType(x, y, z, BlockType);
					}
				}  // for x
			}  // for z
		}  // for z
	}



	/** Tries to place a chest at the specified (absolute) coords.
	Does nothing if the coords are outside the chunk. */
	void TryPlaceChest(cChunkDesc & a_ChunkDesc, const Vector3i & a_Chest)
	{
		int RelX = a_Chest.x - a_ChunkDesc.GetChunkX() * cChunkDef::Width;
		int RelZ = a_Chest.z - a_ChunkDesc.GetChunkZ() * cChunkDef::Width;
		if (
			(RelX < 0) || (RelX >= cChunkDef::Width) ||  // The X coord is not in this chunk
			(RelZ < 0) || (RelZ >= cChunkDef::Width)     // The Z coord is not in this chunk
		)
		{
			return;
		}
		a_ChunkDesc.SetBlockTypeMeta(RelX, m_FloorHeight + 1, RelZ, E_BLOCK_CHEST, static_cast<NIBBLETYPE>(a_Chest.y));

		// Fill the chest with random loot
		static const cLootProbab LootProbab[] =
		{
			// Item,                          MinAmount, MaxAmount, Weight
			{ cItem(E_ITEM_GOLDEN_APPLE),        1,         1,         1 },
			{ cItem(E_ITEM_DIAMOND_HORSE_ARMOR), 1,         1,         1 },
			{ cItem(E_ITEM_GOLD_HORSE_ARMOR),    1,         1,         2 },
			{ cItem(E_ITEM_GOLD),                1,         4,         2 },
			{ cItem(E_ITEM_13_DISC),             1,         1,         4 },
			{ cItem(E_ITEM_CAT_DISC),            1,         1,         4 },
			{ cItem(E_ITEM_IRON_HORSE_ARMOR),    1,         1,         5 },
			{ cItem(E_ITEM_IRON),                1,         4,        10 },
			{ cItem(E_ITEM_WHEAT),               1,         4,        10 },
			{ cItem(E_ITEM_GUNPOWDER),           1,         4,        10 },
			{ cItem(E_ITEM_STRING),              1,         4,        10 },
			{ cItem(E_ITEM_REDSTONE_DUST),       1,         4,        10 },
			{ cItem(E_ITEM_COAL),                1,         4,        10 },
			{ cItem(E_ITEM_BONE),                1,         4,        10 },
			{ cItem(E_ITEM_ROTTEN_FLESH),        1,         4,        10 },
			{ cItem(E_ITEM_SADDLE),              1,         1,        10 },
			{ cItem(E_ITEM_BUCKET),              1,         1,        10 },
			{ cItem(E_ITEM_BREAD),               1,         1,        10 },
			{ cItem(E_ITEM_NAME_TAG),            1,         1,        10 },
			{ cItem(E_ITEM_BEETROOT_SEEDS),      2,         4,        10 },
			{ cItem(E_ITEM_MELON_SEEDS),         2,         4,        10 },
			{ cItem(E_ITEM_PUMPKIN_SEEDS),       2,         4,        10 },
		} ;

		cChestEntity * ChestEntity = static_cast<cChestEntity *>(a_ChunkDesc.GetBlockEntity(RelX, m_FloorHeight + 1, RelZ));
		ASSERT((ChestEntity != nullptr) && (ChestEntity->GetBlockType() == E_BLOCK_CHEST));
		cNoise Noise(a_ChunkDesc.GetChunkX() ^ a_ChunkDesc.GetChunkZ());
		int NumSlots = 3 + ((Noise.IntNoise3DInt(a_Chest.x, a_Chest.y, a_Chest.z) / 11) % 4);
		int Seed = Noise.IntNoise2DInt(RelX, RelZ);
		ChestEntity->GetContents().GenerateRandomLootWithBooks(LootProbab, ARRAYCOUNT(LootProbab), NumSlots, Seed);
	}



	// cGridStructGen::cStructure override:
	virtual void DrawIntoChunk(cChunkDesc & a_ChunkDesc) override
	{
		if (
			(m_EndX   <  a_ChunkDesc.GetChunkX() * cChunkDef::Width) ||
			(m_StartX >= a_ChunkDesc.GetChunkX() * cChunkDef::Width + cChunkDef::Width) ||
			(m_EndZ   <  a_ChunkDesc.GetChunkZ() * cChunkDef::Width) ||
			(m_StartZ >= a_ChunkDesc.GetChunkZ() * cChunkDef::Width + cChunkDef::Width)
		)
		{
			// The chunk is not intersecting the room at all, bail out
			return;
		}

		int b = m_FloorHeight + 1;  // Bottom
		int t = m_FloorHeight + 1 + ROOM_HEIGHT;  // Top
		ReplaceCuboidRandom(a_ChunkDesc, m_StartX, m_FloorHeight, m_StartZ, m_EndX + 1, b, m_EndZ + 1, E_BLOCK_MOSSY_COBBLESTONE, E_BLOCK_COBBLESTONE);  // Floor
		ReplaceCuboid(a_ChunkDesc, m_StartX + 1, b, m_StartZ + 1, m_EndX, t, m_EndZ, E_BLOCK_AIR);  // Insides

		// Walls:
		ReplaceCuboid(a_ChunkDesc, m_StartX, b, m_StartZ, m_StartX + 1, t, m_EndZ,       E_BLOCK_COBBLESTONE);  // XM wall
		ReplaceCuboid(a_ChunkDesc, m_EndX,   b, m_StartZ, m_EndX + 1,   t, m_EndZ,       E_BLOCK_COBBLESTONE);  // XP wall
		ReplaceCuboid(a_ChunkDesc, m_StartX, b, m_StartZ, m_EndX + 1,   t, m_StartZ + 1, E_BLOCK_COBBLESTONE);  // ZM wall
		ReplaceCuboid(a_ChunkDesc, m_StartX, b, m_EndZ,   m_EndX + 1,   t, m_EndZ + 1,   E_BLOCK_COBBLESTONE);  // ZP wall

		// Place chests:
		TryPlaceChest(a_ChunkDesc, m_Chest1);
		TryPlaceChest(a_ChunkDesc, m_Chest2);

		// Place the spawner:
		int CenterX = (m_StartX + m_EndX) / 2 - a_ChunkDesc.GetChunkX() * cChunkDef::Width;
		int CenterZ = (m_StartZ + m_EndZ) / 2 - a_ChunkDesc.GetChunkZ() * cChunkDef::Width;
		if (
			(CenterX >= 0) && (CenterX < cChunkDef::Width) &&
			(CenterZ >= 0) && (CenterZ < cChunkDef::Width)
		)
		{
			a_ChunkDesc.SetBlockTypeMeta(CenterX, b, CenterZ, E_BLOCK_MOB_SPAWNER, 0);
			cMobSpawnerEntity * MobSpawner = static_cast<cMobSpawnerEntity *>(a_ChunkDesc.GetBlockEntity(CenterX, b, CenterZ));
			ASSERT((MobSpawner != nullptr) && (MobSpawner->GetBlockType() == E_BLOCK_MOB_SPAWNER));
			MobSpawner->SetEntity(m_MonsterType);
		}
	}
} ;





////////////////////////////////////////////////////////////////////////////////
// cDungeonRoomsFinisher:

cDungeonRoomsFinisher::cDungeonRoomsFinisher(cTerrainShapeGenPtr a_ShapeGen, int a_Seed, int a_GridSize, int a_MaxSize, int a_MinSize, const AString & a_HeightDistrib) :
	Super(a_Seed + 100, a_GridSize, a_GridSize, a_GridSize, a_GridSize, a_MaxSize, a_MaxSize, 1024),
	m_ShapeGen(std::move(a_ShapeGen)),
	m_MaxHalfSize((a_MaxSize + 1) / 2),
	m_MinHalfSize((a_MinSize + 1) / 2),
	m_HeightProbability(cChunkDef::Height)
{
	// Initialize the height probability distribution:
	m_HeightProbability.SetDefString(a_HeightDistrib);

	// Normalize the min and max size:
	if (m_MinHalfSize > m_MaxHalfSize)
	{
		std::swap(m_MinHalfSize, m_MaxHalfSize);
	}
}





cDungeonRoomsFinisher::cStructurePtr cDungeonRoomsFinisher::CreateStructure(int a_GridX, int a_GridZ, int a_OriginX, int a_OriginZ)
{
	// Select a random room size in each direction:
	int rnd = m_Noise.IntNoise2DInt(a_OriginX, a_OriginZ) / 7;
	int HalfSizeX = m_MinHalfSize + (rnd % (m_MaxHalfSize - m_MinHalfSize + 1));
	rnd = rnd / 32;
	int HalfSizeZ = m_MinHalfSize + (rnd % (m_MaxHalfSize - m_MinHalfSize + 1));
	rnd = rnd / 32;

	// Select a random floor height for the room, based on the height generator:
	int ChunkX, ChunkZ;
	int RelX = a_OriginX, RelY = 0, RelZ = a_OriginZ;
	cChunkDef::AbsoluteToRelative(RelX, RelY, RelZ, ChunkX, ChunkZ);
	cChunkDesc::Shape shape;
	m_ShapeGen->GenShape({ChunkX, ChunkZ}, shape);
	int height = 0;
	int idx = RelX * 256 + RelZ * 16 * 256;
	for (int y = 6; y < cChunkDef::Height; y++)
	{
		if (shape[idx + y] != 0)
		{
			continue;
		}
		height = Clamp(m_HeightProbability.MapValue(rnd % m_HeightProbability.GetSum()), 10, y - 5);
	}

	// Create the dungeon room descriptor:
	return cStructurePtr(new cDungeonRoom(a_GridX, a_GridZ, a_OriginX, a_OriginZ, HalfSizeX, HalfSizeZ, height, m_Noise));
}




