
// HeightMap.cpp

// Implements the cHeightMap class representing a cCallback descendant that draws a B & W map of heights for the world

#include "Globals.h"
#include "HeightMap.h"




static const unsigned char g_BMPHeader[] =
{
	0x42, 0x4D, 0x36, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00,
	0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0xfe, 0xff, 0xff, 0x01, 0x00, 0x20, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x13, 0x0B, 0x00, 0x00, 0x13, 0x0B, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00
} ;





cHeightMap::cHeightMap(void) :
	m_CurrentRegionX(0),
	m_CurrentRegionZ(0),
	m_IsCurrentRegionValid(false)
{
}





void cHeightMap::Finish(void)
{
	if (m_IsCurrentRegionValid)
	{
		StartNewRegion(0, 0);
	}
}





bool cHeightMap::OnNewChunk(int a_ChunkX, int a_ChunkZ)
{
	int RegionX = (a_ChunkX < 0) ? (a_ChunkX - 31) / 32 : a_ChunkX / 32;
	int RegionZ = (a_ChunkZ < 0) ? (a_ChunkZ - 31) / 32 : a_ChunkZ / 32;
	if ((RegionX != m_CurrentRegionX) || (RegionZ != m_CurrentRegionZ))
	{
		if (m_IsCurrentRegionValid)
		{
			StartNewRegion(RegionX, RegionZ);
		}
		m_CurrentRegionX = RegionX;
		m_CurrentRegionZ = RegionZ;
	}
	m_IsCurrentRegionValid = true;
	m_CurrentChunkX = a_ChunkX;
	m_CurrentChunkZ = a_ChunkZ;
	m_CurrentChunkOffX = m_CurrentChunkX - m_CurrentRegionX * 32;
	m_CurrentChunkOffZ = m_CurrentChunkZ - m_CurrentRegionZ * 32;
	memset(m_BlockTypes, 0, sizeof(m_BlockTypes));
	return false;
}





bool cHeightMap::OnHeightMap(const int * a_HeightMapBE)
{
	ASSERT(m_CurrentChunkOffX >= 0);
	ASSERT(m_CurrentChunkOffX < 32);
	ASSERT(m_CurrentChunkOffZ >= 0);
	ASSERT(m_CurrentChunkOffZ < 32);
	int * BaseHeight = m_Height + m_CurrentChunkOffZ * 16 * 512 + m_CurrentChunkOffX * 16;
	for (int z = 0; z < 16; z++)
	{
		int * Row = BaseHeight + z * 512;
		for (int x = 0; x < 16; x++)
		{
			Row[x] = ntohl(a_HeightMapBE[z * 16 + x]);
		}
	}  // for z
	return false;  // Still want blockdata to remove trees from the heightmap
}





bool cHeightMap::OnSection(
	unsigned char a_Y,
	const BLOCKTYPE * a_BlockTypes,
	const NIBBLETYPE * a_BlockAdditional,
	const NIBBLETYPE * a_BlockMeta,
	const NIBBLETYPE * a_BlockLight,
	const NIBBLETYPE * a_BlockSkyLight
)
{
	// Copy the section data into the appropriate place in the internal buffer
	memcpy(m_BlockTypes + a_Y * 16 * 16 * 16, a_BlockTypes, 16 * 16 * 16);
	return false;
}





bool cHeightMap::OnSectionsFinished(void)
{
	// Remove trees from the heightmap:
	for (int z = 0; z < 16; z++)
	{
		for (int x = 0; x < 16; x++)
		{
			for (int y = m_Height[512 * (16 * m_CurrentChunkOffZ + z) + 16 * m_CurrentChunkOffX + x]; y >= 0; y--)
			{
				if (IsGround(m_BlockTypes[256 * y + 16 * z + x]))
				{
					m_Height[512 * (16 * m_CurrentChunkOffZ + z) + 16 * m_CurrentChunkOffX + x] = y;
					break;  // for y
				}
			}  // for y
		}  // for x
	}  // for z
	return true;
}





void cHeightMap::StartNewRegion(int a_RegionX, int a_RegionZ)
{
	AString FileName;
	Printf(FileName, "Height.%d.%d.bmp", m_CurrentRegionX, m_CurrentRegionZ);
	cFile f;
	if (!f.Open(FileName, cFile::fmWrite))
	{
		LOG("Cannot open file \"%s\" for writing the height map. Data for this region lost.", FileName.c_str());
	}
	else
	{
		f.Write(g_BMPHeader, sizeof(g_BMPHeader));
		for (int z = 0; z < 512; z++)
		{
			int RowData[512];
			int * HeightRow = m_Height + z * 512;
			for (int x = 0; x < 512; x++)
			{
				RowData[x] = std::max(std::min(HeightRow[x], 255), 0) * 0x010101;
			}
			f.Write(RowData, sizeof(RowData));
		}  // for z
	}

	memset(m_Height, 0, sizeof(m_Height));
	m_CurrentRegionX = a_RegionX;
	m_CurrentRegionZ = a_RegionZ;
}





bool cHeightMap::IsGround(BLOCKTYPE a_BlockType)
{
	// Name all blocks that are NOT ground, return false for them:
	switch (a_BlockType)
	{
		case E_BLOCK_AIR:
		case E_BLOCK_BED:
		case E_BLOCK_BEETROOTS:
		case E_BLOCK_BREWING_STAND:
		case E_BLOCK_BROWN_MUSHROOM:
		case E_BLOCK_CACTUS:
		case E_BLOCK_CAKE:
		case E_BLOCK_CARROTS:
		case E_BLOCK_CAULDRON:
		case E_BLOCK_CHEST:
		case E_BLOCK_CHORUS_FLOWER:
		case E_BLOCK_CHORUS_PLANT:
		case E_BLOCK_COBBLESTONE_WALL:
		case E_BLOCK_COBWEB:
		case E_BLOCK_COCOA_POD:
		case E_BLOCK_CROPS:
		case E_BLOCK_DEAD_BUSH:
		case E_BLOCK_DETECTOR_RAIL:
		case E_BLOCK_DIRT:
		case E_BLOCK_DRAGON_EGG:
		case E_BLOCK_END_GATEWAY:
		case E_BLOCK_END_PORTAL:
		case E_BLOCK_END_ROD:
		case E_BLOCK_ENDER_CHEST:
		case E_BLOCK_FENCE:
		case E_BLOCK_FENCE_GATE:
		case E_BLOCK_FIRE:
		case E_BLOCK_FLOWER_POT:
		case E_BLOCK_HEAD:
		case E_BLOCK_IRON_BARS:
		case E_BLOCK_LADDER:
		case E_BLOCK_LAVA:
		case E_BLOCK_LEAVES:
		case E_BLOCK_LEVER:
		case E_BLOCK_LILY_PAD:
		case E_BLOCK_LOG:  // NOTE: This block is actually solid, but we don't want it because it's the thing that trees are made of, and we're getting rid of trees
		case E_BLOCK_MELON:
		case E_BLOCK_MELON_STEM:
		case E_BLOCK_NETHER_BRICK_FENCE:
		case E_BLOCK_NETHER_PORTAL:
		case E_BLOCK_POWERED_RAIL:
		case E_BLOCK_PUMPKIN:
		case E_BLOCK_PUMPKIN_STEM:
		case E_BLOCK_RAIL:
		case E_BLOCK_RED_ROSE:
		case E_BLOCK_RED_MUSHROOM:
		case E_BLOCK_REDSTONE_REPEATER_OFF:
		case E_BLOCK_REDSTONE_REPEATER_ON:
		case E_BLOCK_REDSTONE_TORCH_OFF:
		case E_BLOCK_REDSTONE_TORCH_ON:
		case E_BLOCK_REDSTONE_WIRE:
		case E_BLOCK_REEDS:
		case E_BLOCK_SAPLING:
		case E_BLOCK_SIGN_POST:
		case E_BLOCK_SNOW:
		case E_BLOCK_STATIONARY_LAVA:
		case E_BLOCK_STATIONARY_WATER:
		case E_BLOCK_STONE_BUTTON:
		case E_BLOCK_STONE_PRESSURE_PLATE:
		case E_BLOCK_STRUCTURE_VOID:
		case E_BLOCK_TALL_GRASS:
		case E_BLOCK_TORCH:
		case E_BLOCK_TRIPWIRE:
		case E_BLOCK_TRIPWIRE_HOOK:
		case E_BLOCK_VINES:
		case E_BLOCK_WALLSIGN:
		case E_BLOCK_WATER:
		case E_BLOCK_WOODEN_BUTTON:
		case E_BLOCK_WOODEN_PRESSURE_PLATE:
		case E_BLOCK_YELLOW_FLOWER:
		{
			return false;
		}
	}
	return true;
}





////////////////////////////////////////////////////////////////////////////////
// cHeightMapFactory:

cHeightMapFactory::~cHeightMapFactory()
{
	// Force all threads to save their last regions:
	for (cCallbacks::iterator itr = m_Callbacks.begin(), end = m_Callbacks.end(); itr != end; ++itr)
	{
		((cHeightMap *)(*itr))->Finish();
	}
	// TODO: Join all the files into one giant image file
}
