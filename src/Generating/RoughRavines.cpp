
// RoughRavines.cpp

// Implements the cRoughRavines class representing the rough ravine generator

#include "Globals.h"

#include "RoughRavines.h"





////////////////////////////////////////////////////////////////////////////////
// cRoughRavine:

class cRoughRavine :
	public cGridStructGen::cStructure
{
	typedef cGridStructGen::cStructure super;
	
public:
	cRoughRavine(int a_Seed, int a_Size, float a_CenterWidth, float a_Roughness, int a_GridX, int a_GridZ, int a_OriginX, int a_OriginZ) :
		super(a_GridX, a_GridZ, a_OriginX, a_OriginZ),
		m_Seed(a_Seed + 100),
		m_Noise(a_Seed + 100),
		m_Roughness(a_Roughness)
	{
		// Create the basic structure - 2 lines meeting at the centerpoint:
		int Max = 2 * a_Size;
		int Half = a_Size;  // m_DefPoints[Half] will be the centerpoint
		m_DefPoints.resize(Max + 1);
		int rnd = m_Noise.IntNoise2DInt(a_OriginX, a_OriginZ) / 7;
		float Len = (float)a_Size;
		float Angle = (float)rnd;  // Angle is in radians, will be wrapped in the "sin" and "cos" operations
		float OfsX = sin(Angle) * Len;
		float OfsZ = cos(Angle) * Len;
		m_DefPoints[0].Set   (a_OriginX - OfsX, a_OriginZ - OfsZ, 1,             42, 34);
		m_DefPoints[Half].Set((float)a_OriginX, (float)a_OriginZ, a_CenterWidth, 62, 16);
		m_DefPoints[Max].Set (a_OriginX + OfsX, a_OriginZ + OfsZ, 1,             44, 32);
		
		// Calculate the points in between, recursively:
		SubdivideLine(0, Half);
		SubdivideLine(Half, Max);
		
		// Initialize the per-height radius modifiers:
		InitPerHeightRadius(a_GridX, a_GridZ);
	}
	
protected:
	struct sRavineDefPoint
	{
		float m_X;
		float m_Z;
		float m_Radius;
		float m_Top;
		float m_Bottom;
		
		void Set(float a_X, float a_Z, float a_Radius, float a_Top, float a_Bottom)
		{
			m_X = a_X;
			m_Z = a_Z;
			m_Radius = a_Radius;
			m_Top = a_Top;
			m_Bottom = a_Bottom;
		}
	};
	typedef std::vector<sRavineDefPoint> sRavineDefPoints;
	
	int m_Seed;
	
	cNoise m_Noise;
	
	int m_MaxSize;
	
	sRavineDefPoints m_DefPoints;
	
	float m_Roughness;
	
	/** Number to add to the radius based on the height. This creates the "ledges" in the ravine walls. */
	float m_PerHeightRadius[cChunkDef::Height];
	
	
	/** Recursively subdivides the line between the points of the specified index.
	Sets the midpoint to the center of the line plus or minus a random offset, then calls itself for each half
	of the new line. */
	void SubdivideLine(int a_Idx1, int a_Idx2)
	{
		// Calculate the midpoint:
		const sRavineDefPoint & p1 = m_DefPoints[a_Idx1];
		const sRavineDefPoint & p2 = m_DefPoints[a_Idx2];
		float MidX = (p1.m_X + p2.m_X) / 2;
		float MidZ = (p1.m_Z + p2.m_Z) / 2;
		float MidR = (p1.m_Radius + p2.m_Radius) / 2 + 0.1f;
		float MidT = (p1.m_Top    + p2.m_Top)    / 2;
		float MidB = (p1.m_Bottom + p2.m_Bottom) / 2;

		// Adjust the midpoint by a small amount of perpendicular vector in a random one of its two directions:
		float dx = p2.m_X - p1.m_X;
		float dz = p2.m_Z - p1.m_Z;
		if ((m_Noise.IntNoise2DInt((int)MidX, (int)MidZ) / 11) % 2 == 0)
		{
			MidX += dz * m_Roughness;
			MidZ -= dx * m_Roughness;
		}
		else
		{
			MidX -= dz * m_Roughness;
			MidZ += dx * m_Roughness;
		}
		int MidIdx = (a_Idx1 + a_Idx2) / 2;
		m_DefPoints[MidIdx].Set(MidX, MidZ, MidR, MidT, MidB);
		
		// Recurse the two halves, if they are worth recursing:
		if (MidIdx - a_Idx1 > 1)
		{
			SubdivideLine(a_Idx1, MidIdx);
		}
		if (a_Idx2 - MidIdx > 1)
		{
			SubdivideLine(MidIdx, a_Idx2);
		}
	}
	
	
	void InitPerHeightRadius(int a_GridX, int a_GridZ)
	{
		int h = 0;
		while (h < cChunkDef::Height)
		{
			m_Noise.SetSeed(m_Seed + h);
			int rnd = m_Noise.IntNoise2DInt(a_GridX, a_GridZ) / 13;
			int NumBlocks = (rnd % 3) + 2;
			rnd = rnd / 4;
			float Val = (float)(rnd % 256) / 128 - 1;  // Random float in range [-1, +1]
			if (h + NumBlocks > cChunkDef::Height)
			{
				NumBlocks = cChunkDef::Height - h;
			}
			for (int i = 0; i < NumBlocks; i++)
			{
				m_PerHeightRadius[h + i] = Val;
			}
			h += NumBlocks;
		}
	}
	
	
	virtual void DrawIntoChunk(cChunkDesc & a_ChunkDesc) override
	{
		int BlockStartX = a_ChunkDesc.GetChunkX() * cChunkDef::Width;
		int BlockStartZ = a_ChunkDesc.GetChunkZ() * cChunkDef::Width;
		int BlockEndX = BlockStartX + cChunkDef::Width;
		int BlockEndZ = BlockStartZ + cChunkDef::Width;
		for (sRavineDefPoints::const_iterator itr = m_DefPoints.begin(), end = m_DefPoints.end(); itr != end; ++itr)
		{
			if (
				(ceilf (itr->m_X + itr->m_Radius + 2) < BlockStartX) ||
				(floorf(itr->m_X - itr->m_Radius - 2) > BlockEndX) ||
				(ceilf (itr->m_Z + itr->m_Radius + 2) < BlockStartZ) ||
				(floorf(itr->m_Z - itr->m_Radius - 2) > BlockEndZ)
			)
			{
				// Cannot intersect, bail out early
				continue;
			}
			
			// Carve out a cylinder around the xz point, up to (m_Radius + 2) in diameter, from Bottom to Top:
			// On each height level, use m_PerHeightRadius[] to modify the actual radius used
			// EnlargedRadiusSq is the square of the radius enlarged by the maximum m_PerHeightRadius offset - anything outside it will never be touched.
			float RadiusSq = (itr->m_Radius + 2) * (itr->m_Radius + 2);
			float DifX = BlockStartX - itr->m_X;  // substitution for faster calc
			float DifZ = BlockStartZ - itr->m_Z;  // substitution for faster calc
			for (int x = 0; x < cChunkDef::Width; x++) for (int z = 0; z < cChunkDef::Width; z++)
			{
				#ifdef _DEBUG
				// DEBUG: Make the roughravine shapepoints visible on a single layer (so that we can see with Minutor what's going on)
				if ((DifX + x == 0) && (DifZ + z == 0))
				{
					a_ChunkDesc.SetBlockType(x, 4, z, E_BLOCK_LAPIS_ORE);
				}
				#endif  // _DEBUG
				
				// If the column is outside the enlarged radius, bail out completely
				float DistSq = (DifX + x) * (DifX + x) + (DifZ + z) * (DifZ + z);
				if (DistSq > RadiusSq)
				{
					continue;
				}
				
				int Top = std::min((int)ceilf(itr->m_Top), +cChunkDef::Height);
				for (int y = std::max((int)floorf(itr->m_Bottom), 1); y <= Top; y++)
				{
					if ((itr->m_Radius + m_PerHeightRadius[y]) * (itr->m_Radius + m_PerHeightRadius[y]) < DistSq)
					{
						continue;
					}
					switch (a_ChunkDesc.GetBlockType(x, y, z))
					{
						// Only carve out these specific block types
						case E_BLOCK_DIRT:
						case E_BLOCK_GRASS:
						case E_BLOCK_STONE:
						case E_BLOCK_COBBLESTONE:
						case E_BLOCK_GRAVEL:
						case E_BLOCK_SAND:
						case E_BLOCK_SANDSTONE:
						case E_BLOCK_NETHERRACK:
						case E_BLOCK_COAL_ORE:
						case E_BLOCK_IRON_ORE:
						case E_BLOCK_GOLD_ORE:
						case E_BLOCK_DIAMOND_ORE:
						case E_BLOCK_REDSTONE_ORE:
						case E_BLOCK_REDSTONE_ORE_GLOWING:
						{
							a_ChunkDesc.SetBlockType(x, y, z, E_BLOCK_AIR);
							break;
						}
						default: break;
					}  // switch (BlockType)
				}  // for y
			}  // for x, z - a_BlockTypes
		}  // for itr - m_Points[]
	}
};





////////////////////////////////////////////////////////////////////////////////
// cRoughRavines:

cRoughRavines::cRoughRavines(
	int a_Seed,
	int a_MaxSize, int a_MinSize,
	float a_MaxCenterWidth, float a_MinCenterWidth,
	float a_MaxRoughness, float a_MinRoughness,
	int a_GridSize, int a_MaxOffset
) :
	super(a_Seed, a_GridSize, a_GridSize, a_MaxOffset, a_MaxOffset, a_MaxSize, a_MaxSize, 64),
	m_Seed(a_Seed),
	m_MaxSize(a_MaxSize),
	m_MinSize(a_MinSize),
	m_MaxCenterWidth(a_MaxCenterWidth),
	m_MinCenterWidth(a_MinCenterWidth),
	m_MaxRoughness(a_MaxRoughness),
	m_MinRoughness(a_MinRoughness)
{
	if (m_MinSize > m_MaxSize)
	{
		std::swap(m_MinSize, m_MaxSize);
		std::swap(a_MinSize, a_MaxSize);
	}
	if (m_MaxSize < 16)
	{
		m_MaxSize = 16;
		LOGWARNING("RoughRavines: MaxSize too small, adjusting request from %d to %d", a_MaxSize, m_MaxSize);
	}
	if (m_MinSize < 16)
	{
		m_MinSize = 16;
		LOGWARNING("RoughRavines: MinSize too small, adjusting request from %d to %d", a_MinSize, m_MinSize);
	}
	if (m_MinSize == m_MaxSize)
	{
		m_MaxSize = m_MinSize + 1;
	}
}





cGridStructGen::cStructurePtr cRoughRavines::CreateStructure(int a_GridX, int a_GridZ, int a_OriginX, int a_OriginZ)
{
	int Size = m_MinSize + (m_Noise.IntNoise2DInt(a_GridX, a_GridZ) / 7) % (m_MaxSize - m_MinSize);  // Random int from m_MinSize to m_MaxSize
	float CenterWidth = m_MinCenterWidth + abs(m_Noise.IntNoise2D(a_GridX, a_GridZ + 10)) * (m_MaxCenterWidth - m_MinCenterWidth);  // Random float from m_MinCenterWidth to m_MaxCenterWidth
	float Roughness = m_MinRoughness + abs(m_Noise.IntNoise2D(a_GridX + 10, a_GridZ)) * (m_MaxRoughness - m_MinRoughness);  // Random float from m_MinRoughness to m_MaxRoughness
	return cStructurePtr(new cRoughRavine(m_Seed, Size, CenterWidth, Roughness, a_GridX, a_GridZ, a_OriginX, a_OriginZ));
}




