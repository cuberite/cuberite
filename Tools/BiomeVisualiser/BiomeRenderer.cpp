
// BiomeRenderer.cpp

// Implements the cBiomeRenderer class representing the rendering engine

#include "Globals.h"
#include "BiomeRenderer.h"
#include "Pixmap.h"
#include "Timer.h"
#include "BiomeColors.h"





cBiomeRenderer::cBiomeRenderer(void) :
	m_OriginX(160),
	m_OriginY(160),
	m_Zoom(1)
{
}




void cBiomeRenderer::SetSource(cBiomeSource * a_Source)
{
	m_Cache.SetSource(a_Source);
}





bool cBiomeRenderer::Render(cPixmap & a_Pixmap)
{
	cTimer Timer("cBiomeRenderer::Render");
	
	int Wid = a_Pixmap.GetWidth();
	int Hei = a_Pixmap.GetHeight();
	
	// Hint the approximate view area to the biome source so that it can adjust its caches:
	int MinBlockX = (    - m_OriginX) * m_Zoom;
	int MaxBlockX = (Wid - m_OriginX) * m_Zoom;
	int MinBlockZ = (    - m_OriginY) * m_Zoom;
	int MaxBlockZ = (Hei - m_OriginY) * m_Zoom;
	m_Cache.HintViewArea(MinBlockX / 16 - 1, MaxBlockX / 16 + 1, MinBlockZ / 16 - 1, MaxBlockZ / 16 + 1);
	
	// Hold one current chunk of biome data:
	int CurChunkX = -10000;
	int CurChunkZ = -10000;
	cChunkDef::BiomeMap CurBiomes;
	
	bool res = false;
	
	for (int y = 0; y < Hei; y++)
	{
		int BlockZ = (y - m_OriginY) * m_Zoom;
		int ChunkZ = (BlockZ >= 0) ? (BlockZ / 16) : ((BlockZ + 1) / 16 - 1);
		int RelZ = BlockZ - ChunkZ * 16;
		for (int x = 0; x < Wid; x++)
		{
			int BlockX = (x - m_OriginX) * m_Zoom;
			int ChunkX = (BlockX >= 0) ? (BlockX / 16) : ((BlockX + 1) / 16 - 1);
			int RelX = BlockX - ChunkX * 16;
			if ((ChunkZ != CurChunkZ) || (ChunkX != CurChunkX))
			{
				CurChunkX = ChunkX;
				CurChunkZ = ChunkZ;
				switch (m_Cache.GetBiome(CurChunkX, CurChunkZ, CurBiomes))
				{
					case cBiomeSource::baLater:
					{
						res = true; 
						// fallthrough:
					}
					case cBiomeSource::baNever:
					{
						for (int i = 0; i < ARRAYCOUNT(CurBiomes); i++)
						{
							CurBiomes[i] = (EMCSBiome)-1;
						}
						break;
					}
				}  // switch (Biome availability)
			}
			EMCSBiome Biome = cChunkDef::GetBiome(CurBiomes, RelX, RelZ);
			a_Pixmap.SetPixel(x, y, GetBiomeColor(Biome));
		}  // for x
	}  // for y
	return res;
}





int cBiomeRenderer::GetBiomeColor(EMCSBiome a_Biome)
{
	if ((a_Biome < 0) || (a_Biome >= ARRAYCOUNT(g_BiomeColors)))
	{
		return 0xff0000;
	}
	return g_BiomeColors[a_Biome];
}





void cBiomeRenderer::MoveViewBy(int a_OffsX, int a_OffsY)
{
	m_OriginX += a_OffsX;
	m_OriginY += a_OffsY;
}




