
// HeightBiomeMap.cpp

// Declares the cHeightBiomeMap class representing a stats module that produces an image of heights and biomes combined

#include "Globals.h"
#include "HeightBiomeMap.h"
#include "HeightMap.h"





cHeightBiomeMap::cHeightBiomeMap(void) :
	super("HeBi"),
	m_MinRegionX(100000),
	m_MaxRegionX(-100000),
	m_MinRegionZ(100000),
	m_MaxRegionZ(-100000)
{
}





bool cHeightBiomeMap::OnNewRegion(int a_RegionX, int a_RegionZ)
{
	if (a_RegionX < m_MinRegionX)
	{
		m_MinRegionX = a_RegionX;
	}
	if (a_RegionX > m_MaxRegionX)
	{
		m_MaxRegionX = a_RegionX;
	}
	if (a_RegionZ < m_MinRegionZ)
	{
		m_MinRegionZ = a_RegionZ;
	}
	if (a_RegionZ > m_MaxRegionZ)
	{
		m_MaxRegionZ = a_RegionZ;
	}
	return super::OnNewRegion(a_RegionX, a_RegionZ);
}





bool cHeightBiomeMap::OnNewChunk(int a_ChunkX, int a_ChunkZ)
{
	m_CurrentChunkX = a_ChunkX;
	m_CurrentChunkZ = a_ChunkZ;
	m_CurrentChunkRelX = m_CurrentChunkX - m_CurrentRegionX * 32;
	m_CurrentChunkRelZ = m_CurrentChunkZ - m_CurrentRegionZ * 32;

	ASSERT((m_CurrentChunkRelX >= 0) && (m_CurrentChunkRelX < 32));
	ASSERT((m_CurrentChunkRelZ >= 0) && (m_CurrentChunkRelZ < 32));

	memset(m_BlockTypes, 0, sizeof(m_BlockTypes));

	return CALLBACK_CONTINUE;
}






bool cHeightBiomeMap::OnBiomes(const unsigned char * a_BiomeData)
{
	memcpy(m_ChunkBiomes, a_BiomeData, sizeof(m_ChunkBiomes));

	return CALLBACK_CONTINUE;
}





bool cHeightBiomeMap::OnHeightMap(const int * a_HeightMapBE)
{
	for (int i = 0; i < ARRAYCOUNT(m_ChunkHeight); i++)
	{
		m_ChunkHeight[i] = ntohl(a_HeightMapBE[i]);
	}  // for i - m_ChunkHeight

	return CALLBACK_CONTINUE;
}





bool cHeightBiomeMap::OnSection(
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
	return CALLBACK_CONTINUE;
}





bool cHeightBiomeMap::OnSectionsFinished(void)
{
	static const int BiomePalette[] =
	{
		// ARGB:
		0xff0000ff, /* Ocean */
		0xff00cf3f, /* Plains */
		0xffffff00, /* Desert */
		0xff7f7f7f, /* Extreme Hills */
		0xff00cf00, /* Forest */
		0xff007f3f, /* Taiga */
		0xff3f7f00, /* Swampland */
		0xff003fff, /* River */
		0xff7f0000, /* Hell */
		0xff007fff, /* Sky */
		0xff3f3fff, /* Frozen Ocean */
		0xff3f3fff, /* Frozen River */
		0xff7fffcf, /* Ice Plains */
		0xff3fcf7f, /* Ice Mountains */
		0xffcf00cf, /* Mushroom Island */
		0xff7f00ff, /* Mushroom Island Shore */
		0xffffff3f, /* Beach */
		0xffcfcf00, /* Desert Hills */
		0xff00cf3f, /* Forest Hills */
		0xff006f1f, /* Taiga Hills */
		0xff7f8f7f, /* Extreme Hills Edge */
		0xff004f00, /* Jungle */
		0xff003f00, /* Jungle Hills */
	} ;

	// Remove trees and other unwanted stuff from the heightmap:
	for (int z = 0; z < 16; z++)
	{
		int PixelLine[16];  // line of 16 pixels that is used as a buffer for setting the image pixels
		for (int x = 0; x < 16; x++)
		{
			int Height = m_ChunkHeight[16 * z + x];
			for (int y = Height; y >= 0; y--)
			{
				if (cHeightMap::IsGround(m_BlockTypes[256 * y + 16 * z + x]))
				{
					Height = y;
					break;  // for y
				}
			}  // for y

			// Set the color based on the biome and height:
			char Biome = m_ChunkBiomes[16 * z + x];
			PixelLine[x] = ShadeColor(BiomePalette[Biome], Height);
		}  // for x

		// Set the pixelline into the image:
		SetPixelURow(m_CurrentChunkRelX * 16, m_CurrentChunkRelZ * 16 + z, 16, PixelLine);
	}  // for z
	return CALLBACK_ABORT;
}





////////////////////////////////////////////////////////////////////////////////
// cHeightBiomeMapFactory:

cHeightBiomeMapFactory::~cHeightBiomeMapFactory()
{
	// Get the min and max region coords:
	int MinRegionX = 100000;
	int MaxRegionX = -100000;
	int MinRegionZ = 100000;
	int MaxRegionZ = -100000;
	for (cCallbacks::iterator itr = m_Callbacks.begin(), end = m_Callbacks.end(); itr != end; ++itr)
	{
		cHeightBiomeMap * cb = (cHeightBiomeMap *)(*itr);
		if (cb->m_MinRegionX < MinRegionX)
		{
			MinRegionX = cb->m_MinRegionX;
		}
		if (cb->m_MaxRegionX > MaxRegionX)
		{
			MaxRegionX = cb->m_MaxRegionX;
		}
		if (cb->m_MinRegionZ < MinRegionZ)
		{
			MinRegionZ = cb->m_MinRegionZ;
		}
		if (cb->m_MaxRegionZ > MaxRegionZ)
		{
			MaxRegionZ = cb->m_MaxRegionZ;
		}
	}

	// If the size is small enough, write an HTML file referencing all the images in a table:
	if ((MaxRegionX >= MinRegionX) && (MaxRegionZ >= MinRegionZ) && (MaxRegionX - MinRegionX < 100) && (MaxRegionZ - MinRegionZ < 100))
	{
		cFile HTML("HeBi.html", cFile::fmWrite);
		if (HTML.IsOpen())
		{
			HTML.Printf("<html><body><table cellspacing=0 cellpadding=0>\n");
			for (int z = MinRegionZ; z <= MaxRegionZ; z++)
			{
				HTML.Printf("<tr>");
				for (int x = MinRegionX; x <= MaxRegionX; x++)
				{
					HTML.Printf("<td><img src=\"HeBi.%d.%d.bmp\" /></td>", x, z);
				}
				HTML.Printf("</tr>\n");
			}
			HTML.Printf("</table></body></html>");
		}
	}
}
