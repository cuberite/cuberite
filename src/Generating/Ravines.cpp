
// Ravines.cpp

// Implements the cStructGenRavines class representing the ravine structure generator

#include "Globals.h"
#include "Ravines.h"




static const int NUM_RAVINE_POINTS = 4;





struct cRavDefPoint
{
	int m_BlockX;
	int m_BlockZ;
	int m_Radius;
	int m_Top;
	int m_Bottom;

	cRavDefPoint(int a_BlockX, int a_BlockZ, int a_Radius, int a_Top, int a_Bottom) :
		m_BlockX(a_BlockX),
		m_BlockZ(a_BlockZ),
		m_Radius(a_Radius),
		m_Top   (a_Top),
		m_Bottom(a_Bottom)
	{
	}
} ;

using cRavDefPoints = std::vector<cRavDefPoint>;





class cStructGenRavines::cRavine:
	public cGridStructGen::cStructure
{
	using Super = cGridStructGen::cStructure;

	cRavDefPoints m_Points;


	/** Generates the shaping defpoints for the ravine, based on the ravine block coords and noise */
	void GenerateBaseDefPoints(int a_BlockX, int a_BlockZ, int a_Size, cNoise & a_Noise);

	/** Refines (adds and smooths) defpoints from a_Src into a_Dst */
	void RefineDefPoints(const cRavDefPoints & a_Src, cRavDefPoints & a_Dst);

	/** Does one round of smoothing, two passes of RefineDefPoints() */
	void Smooth(void);

	/** Linearly interpolates the points so that the maximum distance between two neighbors is max 1 block */
	void FinishLinear(void);

public:

	cRavine(int a_GridX, int a_GridZ, int a_OriginX, int a_OriginZ, int a_Size, cNoise & a_Noise);

	#ifdef _DEBUG
	/** Exports itself as a SVG line definition */
	AString ExportAsSVG(int a_Color, int a_OffsetX = 0, int a_OffsetZ = 0) const;
	#endif  // _DEBUG

protected:
	// cGridStructGen::cStructure overrides:
	virtual void DrawIntoChunk(cChunkDesc & a_ChunkDesc) override;
} ;





////////////////////////////////////////////////////////////////////////////////
// cStructGenRavines:

cStructGenRavines::cStructGenRavines(int a_Seed, int a_Size) :
	Super(a_Seed, a_Size, a_Size, a_Size, a_Size, a_Size * 2, a_Size * 2, 100),
	m_Size(a_Size)
{
}





cGridStructGen::cStructurePtr cStructGenRavines::CreateStructure(int a_GridX, int a_GridZ, int a_OriginX, int a_OriginZ)
{
	return cStructurePtr(new cRavine(a_GridX, a_GridZ, a_OriginX, a_OriginZ, m_Size, m_Noise));
}





////////////////////////////////////////////////////////////////////////////////
// cStructGenRavines::cRavine

cStructGenRavines::cRavine::cRavine(int a_GridX, int a_GridZ, int a_OriginX, int a_OriginZ, int a_Size, cNoise & a_Noise) :
	Super(a_GridX, a_GridZ, a_OriginX, a_OriginZ)
{
	// Calculate the ravine shape-defining points:
	GenerateBaseDefPoints(a_OriginX, a_OriginZ, a_Size, a_Noise);

	// Smooth the ravine. Two passes are needed:
	Smooth();
	Smooth();

	// Linearly interpolate the neighbors so that they're close enough together:
	FinishLinear();
}





void cStructGenRavines::cRavine::GenerateBaseDefPoints(int a_BlockX, int a_BlockZ, int a_Size, cNoise & a_Noise)
{
	// Modify the size slightly to have different-sized ravines (1 / 2 to 1 / 1 of a_Size):
	a_Size = (512 + ((a_Noise.IntNoise3DInt(19 * a_BlockX, 11 * a_BlockZ, a_BlockX + a_BlockZ) / 17) % 512)) * a_Size / 1024;

	// The complete offset of the ravine from its cellpoint, up to 2 * a_Size in each direction
	int OffsetX = (((a_Noise.IntNoise3DInt(50 * a_BlockX, 30 * a_BlockZ, 0)    / 9) % (2 * a_Size)) + ((a_Noise.IntNoise3DInt(30 * a_BlockX, 50 * a_BlockZ, 1000) / 7) % (2 * a_Size)) - 2 * a_Size) / 2;
	int OffsetZ = (((a_Noise.IntNoise3DInt(50 * a_BlockX, 30 * a_BlockZ, 2000) / 7) % (2 * a_Size)) + ((a_Noise.IntNoise3DInt(30 * a_BlockX, 50 * a_BlockZ, 3000) / 9) % (2 * a_Size)) - 2 * a_Size) / 2;
	int CenterX = a_BlockX + OffsetX;
	int CenterZ = a_BlockZ + OffsetZ;

	// Get the base angle in which the ravine "axis" goes:
	float Angle = static_cast<float>((static_cast<float>((a_Noise.IntNoise3DInt(20 * a_BlockX, 70 * a_BlockZ, 6000) / 9) % 16384)) / 16384.0 * M_PI);
	float xc = sinf(Angle);
	float zc = cosf(Angle);

	// Calculate the definition points and radii:
	int MaxRadius = static_cast<int>(sqrt(12.0 + ((a_Noise.IntNoise2DInt(61 * a_BlockX, 97 * a_BlockZ) / 13) % a_Size) / 16));
	int Top       = 32 + ((a_Noise.IntNoise2DInt(13 * a_BlockX, 17 * a_BlockZ) / 23) % 32);
	int Bottom    = 5 + ((a_Noise.IntNoise2DInt(17 * a_BlockX, 29 * a_BlockZ) / 13) % 32);
	int Mid = (Top + Bottom) / 2;
	int DefinitionPointX = CenterX - static_cast<int>(xc * a_Size / 2);
	int DefinitionPointZ = CenterZ - static_cast<int>(zc * a_Size / 2);
	m_Points.push_back(cRavDefPoint(DefinitionPointX, DefinitionPointZ, 0, (Mid + Top) / 2, (Mid + Bottom) / 2));
	for (int i = 1; i < NUM_RAVINE_POINTS - 1; i++)
	{
		int LineX = CenterX + static_cast<int>(xc * a_Size * (i - NUM_RAVINE_POINTS / 2) / NUM_RAVINE_POINTS);
		int LineZ = CenterZ + static_cast<int>(zc * a_Size * (i - NUM_RAVINE_POINTS / 2) / NUM_RAVINE_POINTS);
		// Amplitude is the amount of blocks that this point is away from the ravine "axis"
		int Amplitude = (a_Noise.IntNoise3DInt(70 * a_BlockX, 20 * a_BlockZ + 31 * i, 10000 * i) / 9) % a_Size;
		Amplitude = Amplitude / 4 - a_Size / 8;  // Amplitude is in interval [-a_Size / 4, a_Size / 4]
		int PointX = LineX + static_cast<int>(zc * Amplitude);
		int PointZ = LineZ - static_cast<int>(xc * Amplitude);
		int Radius = MaxRadius - abs(i - NUM_RAVINE_POINTS / 2);  // TODO: better radius function
		int ThisTop    = Top    + ((a_Noise.IntNoise3DInt(7 *  a_BlockX, 19 * a_BlockZ, i * 31) / 13) % 8) - 4;
		int ThisBottom = Bottom + ((a_Noise.IntNoise3DInt(19 * a_BlockX, 7 *  a_BlockZ, i * 31) / 13) % 8) - 4;
		m_Points.push_back(cRavDefPoint(PointX, PointZ, Radius, ThisTop, ThisBottom));
	}  // for i - m_Points[]
	DefinitionPointX = CenterX + static_cast<int>(xc * a_Size / 2);
	DefinitionPointZ = CenterZ + static_cast<int>(zc * a_Size / 2);
	m_Points.push_back(cRavDefPoint(DefinitionPointX, DefinitionPointZ, 0, Mid, Mid));
}





void cStructGenRavines::cRavine::RefineDefPoints(const cRavDefPoints & a_Src, cRavDefPoints & a_Dst)
{
	if (a_Src.size() < 2)
	{
		// No midpoints, nothing to refine
		return;
	}

	// Smoothing: for each line segment, add points on its 1 / 4 lengths
	size_t Num = a_Src.size() - 2;  // this many intermediary points
	a_Dst.clear();
	a_Dst.reserve(Num * 2 + 2);
	cRavDefPoints::const_iterator itr = a_Src.begin() + 1;
	const cRavDefPoint & Source = a_Src.front();
	a_Dst.push_back(Source);
	int PrevX = Source.m_BlockX;
	int PrevZ = Source.m_BlockZ;
	int PrevR = Source.m_Radius;
	int PrevT = Source.m_Top;
	int PrevB = Source.m_Bottom;
	for (size_t i = 0; i <= Num; ++i, ++itr)
	{
		int dx = itr->m_BlockX - PrevX;
		int dz = itr->m_BlockZ - PrevZ;
		if (abs(dx) + abs(dz) < 4)
		{
			// Too short a segment to smooth-subdivide into quarters
			continue;
		}
		int dr = itr->m_Radius - PrevR;
		int dt = itr->m_Top    - PrevT;
		int db = itr->m_Bottom - PrevB;
		int Rad1 = std::max(PrevR + 1 * dr / 4, 1);
		int Rad2 = std::max(PrevR + 3 * dr / 4, 1);
		a_Dst.push_back(cRavDefPoint(PrevX + 1 * dx / 4, PrevZ + 1 * dz / 4, Rad1, PrevT + 1 * dt / 4, PrevB + 1 * db / 4));
		a_Dst.push_back(cRavDefPoint(PrevX + 3 * dx / 4, PrevZ + 3 * dz / 4, Rad2, PrevT + 3 * dt / 4, PrevB + 3 * db / 4));
		PrevX = itr->m_BlockX;
		PrevZ = itr->m_BlockZ;
		PrevR = itr->m_Radius;
		PrevT = itr->m_Top;
		PrevB = itr->m_Bottom;
	}
	a_Dst.push_back(a_Src.back());
}





void cStructGenRavines::cRavine::Smooth(void)
{
	cRavDefPoints Pts;
	RefineDefPoints(m_Points, Pts);  // Refine m_Points -> Pts
	RefineDefPoints(Pts, m_Points);  // Refine Pts -> m_Points
}





void cStructGenRavines::cRavine::FinishLinear(void)
{
	// For each segment, use Bresenham's line algorithm to draw a "line" of defpoints
	// _X 2012_07_20: I tried modifying this algorithm to produce "thick" lines (only one coord change per point)
	//   But the results were about the same as the original, so I disposed of it again - no need to use twice the count of points

	cRavDefPoints Pts;
	std::swap(Pts, m_Points);

	m_Points.reserve(Pts.size() * 3);
	int PrevX = Pts.front().m_BlockX;
	int PrevZ = Pts.front().m_BlockZ;
	for (cRavDefPoints::const_iterator itr = Pts.begin() + 1, end = Pts.end(); itr != end; ++itr)
	{
		int x1 = itr->m_BlockX;
		int z1 = itr->m_BlockZ;
		int dx = abs(x1 - PrevX);
		int dz = abs(z1 - PrevZ);
		int sx = (PrevX < x1) ? 1 : -1;
		int sz = (PrevZ < z1) ? 1 : -1;
		int err = dx - dz;
		int R = itr->m_Radius;
		int T = itr->m_Top;
		int B = itr->m_Bottom;
		for (;;)
		{
			m_Points.push_back(cRavDefPoint(PrevX, PrevZ, R, T, B));
			if ((PrevX == x1) && (PrevZ == z1))
			{
				break;
			}
			int e2 = 2 * err;
			if (e2 > -dz)
			{
				err -= dz;
				PrevX += sx;
			}
			if (e2 < dx)
			{
				err += dx;
				PrevZ += sz;
			}
		}  // while (true)
	}  // for itr
}





#ifdef _DEBUG
AString cStructGenRavines::cRavine::ExportAsSVG(int a_Color, int a_OffsetX, int a_OffsetZ) const
{
	AString SVG;
	AppendPrintf(SVG, "<path style=\"fill:none;stroke:#%06x;stroke-width:1px;\"\nd=\"", a_Color);
	char Prefix = 'M';  // The first point needs "M" prefix, all the others need "L"
	for (cRavDefPoints::const_iterator itr = m_Points.begin(); itr != m_Points.end(); ++itr)
	{
		AppendPrintf(SVG, "%c %d, %d ", Prefix, a_OffsetX + itr->m_BlockX, a_OffsetZ + itr->m_BlockZ);
		Prefix = 'L';
	}
	SVG.append("\"/>\n");

	// Base point highlight:
	AppendPrintf(SVG, "<path style=\"fill:none;stroke:#ff0000;stroke-width:1px;\"\nd=\"M %d, %d L %d, %d\"/>\n",
		a_OffsetX + m_OriginX - 5, a_OffsetZ + m_OriginZ, a_OffsetX + m_OriginX + 5, a_OffsetZ + m_OriginZ
	);
	AppendPrintf(SVG, "<path style=\"fill:none;stroke:#ff0000;stroke-width:1px;\"\nd=\"M %d, %d L %d, %d\"/>\n",
		a_OffsetX + m_OriginX, a_OffsetZ + m_OriginZ - 5, a_OffsetX + m_OriginX, a_OffsetZ + m_OriginZ + 5
	);

	// A gray line from the base point to the first point of the ravine, for identification:
	AppendPrintf(SVG, "<path style=\"fill:none;stroke:#cfcfcf;stroke-width:1px;\"\nd=\"M %d, %d L %d, %d\"/>\n",
		a_OffsetX + m_OriginX, a_OffsetZ + m_OriginZ, a_OffsetX + m_Points.front().m_BlockX, a_OffsetZ + m_Points.front().m_BlockZ
	);

	// Offset guides:
	if (a_OffsetX > 0)
	{
		AppendPrintf(SVG, "<path style=\"fill:none;stroke:#0000ff;stroke-width:1px;\"\nd=\"M %d, 0 L %d, 1024\"/>\n",
			a_OffsetX, a_OffsetX
		);
	}
	if (a_OffsetZ > 0)
	{
		AppendPrintf(SVG, "<path style=\"fill:none;stroke:#0000ff;stroke-width:1px;\"\nd=\"M 0, %d L 1024, %d\"/>\n",
			a_OffsetZ, a_OffsetZ
		);
	}
	return SVG;
}
#endif  // _DEBUG





void cStructGenRavines::cRavine::DrawIntoChunk(cChunkDesc & a_ChunkDesc)
{
	int BlockStartX = a_ChunkDesc.GetChunkX() * cChunkDef::Width;
	int BlockStartZ = a_ChunkDesc.GetChunkZ() * cChunkDef::Width;
	int BlockEndX = BlockStartX + cChunkDef::Width;
	int BlockEndZ = BlockStartZ + cChunkDef::Width;
	for (cRavDefPoints::const_iterator itr = m_Points.begin(), end = m_Points.end(); itr != end; ++itr)
	{
		if (
			(itr->m_BlockX + itr->m_Radius < BlockStartX) ||
			(itr->m_BlockX - itr->m_Radius > BlockEndX) ||
			(itr->m_BlockZ + itr->m_Radius < BlockStartZ) ||
			(itr->m_BlockZ - itr->m_Radius > BlockEndZ)
		)
		{
			// Cannot intersect, bail out early
			continue;
		}

		// Carve out a cylinder around the xz point, m_Radius in diameter, from Bottom to Top:
		int RadiusSq = itr->m_Radius * itr->m_Radius;  // instead of doing sqrt for each distance, we do sqr of the radius
		int DifX = BlockStartX - itr->m_BlockX;  // substitution for faster calc
		int DifZ = BlockStartZ - itr->m_BlockZ;  // substitution for faster calc
		for (int x = 0; x < cChunkDef::Width; x++) for (int z = 0; z < cChunkDef::Width; z++)
		{
			#ifdef _DEBUG
			// DEBUG: Make the ravine shapepoints visible on a single layer (so that we can see with Minutor what's going on)
			if ((DifX + x == 0) && (DifZ + z == 0))
			{
				a_ChunkDesc.SetBlockType(x, 4, z, E_BLOCK_LAPIS_ORE);
			}
			#endif  // _DEBUG

			int DistSq = (DifX + x) * (DifX + x) + (DifZ + z) * (DifZ + z);
			if (DistSq <= RadiusSq)
			{
				int Top = std::min(itr->m_Top, static_cast<int>(cChunkDef::Height));  // Stupid gcc needs int cast
				for (int y = std::max(itr->m_Bottom, 1); y <= Top; y++)
				{
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
					}
				}
			}
		}  // for x, z - a_BlockTypes
	}  // for itr - m_Points[]
}




