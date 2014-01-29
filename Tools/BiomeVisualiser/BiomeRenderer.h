
// BiomeRenderer.h

// Declares the cBiomeRenderer class representing the rendering engine





#pragma once

#include "BiomeCache.h"





// fwd: Pixmap.h
class cPixmap;





class cBiomeRenderer
{
public:
	cBiomeRenderer(void);
	
	void SetSource(cBiomeSource * a_Source);  // Takes ownership of the source
	
	/// Renders the biomes into the given pixmap. Returns true if some biome data was missing and can be retrieved later
	bool Render(cPixmap & a_Pixmap);
	
	/// Returns the RGB color value for the specified biome
	int GetBiomeColor(EMCSBiome a_Biome);
	
	void MoveViewBy(int a_OffsX, int a_OffsY);
	
	void SetZoom(int a_NewZoom)
	{
		m_Zoom = a_NewZoom;
	}
	
protected:
	cBiomeCache m_Cache;
	
	int m_OriginX;
	int m_OriginY;
	int m_Zoom;
} ;




