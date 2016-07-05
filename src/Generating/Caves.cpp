
// Caves.cpp

// Implements the various cave structure generators:
//   - cStructGenWormNestCaves
//   - cStructGenDualRidgeCaves
//   - cStructGenMarbleCaves
//   - cStructGenNetherCaves

/*
WormNestCave generator:
Caves are generated in "nests" - groups of tunnels generated from a single point.
For each chunk, all the nests that could intersect it are generated.
For each nest, first the schematic structure is generated (tunnel from ... to ..., branch, tunnel2 from ... to ...)
Then each tunnel is randomized by inserting points in between its ends.
Finally each tunnel is smoothed and Bresenham-3D-ed so that it is a collection of spheres with their centers next to each other.
When the tunnels are ready, they are simply carved into the chunk, one by one.
To optimize, each tunnel keeps track of its bounding box, so that it can be skipped for chunks that don't intersect it.

MarbleCaves generator:
For each voxel a 3D noise function is evaluated, if the value crosses a boundary, the voxel is dug out, otherwise it is kept.
Problem with this is the amount of CPU work needed for each chunk.
Also the overall shape of the generated holes is unsatisfactory - there are whole "sheets" of holes in the ground.

DualRidgeCaves generator:
Instead of evaluating a single noise function, two different noise functions are multiplied. This produces
regular tunnels instead of sheets. However due to the sheer amount of CPU work needed, the noise functions need to be
reduced in complexity in order for this generator to be useful, so the caves' shapes are "bubbly" at best.
*/

#include "Globals.h"
#include "Caves.h"





const int MIN_RADIUS = 3;
const int MAX_RADIUS = 8;





struct cCaveDefPoint
{
	int m_BlockX;
	int m_BlockY;
	int m_BlockZ;
	int m_Radius;

	cCaveDefPoint(int a_BlockX, int a_BlockY, int a_BlockZ, int a_Radius) :
		m_BlockX(a_BlockX),
		m_BlockY(a_BlockY),
		m_BlockZ(a_BlockZ),
		m_Radius(a_Radius)
	{
	}
} ;

typedef std::vector<cCaveDefPoint> cCaveDefPoints;





/** A single non-branching tunnel of a WormNestCave */
class cCaveTunnel
{
	// The bounding box, including the radii around defpoints:
	int m_MinBlockX, m_MaxBlockX;
	int m_MinBlockY, m_MaxBlockY;
	int m_MinBlockZ, m_MaxBlockZ;

	/** Generates the shaping defpoints for the cave, based on the cave block coords and noise */
	void Randomize(cNoise & a_Noise);

	/** Refines (adds and smooths) defpoints from a_Src into a_Dst; returns false if no refinement possible (segments too short) */
	bool RefineDefPoints(const cCaveDefPoints & a_Src, cCaveDefPoints & a_Dst);

	/** Does rounds of smoothing, two passes of RefineDefPoints(), as long as they return true */
	void Smooth(void);

	/** Linearly interpolates the points so that the maximum distance between two neighbors is max 1 block */
	void FinishLinear(void);

	/** Calculates the bounding box of the points present */
	void CalcBoundingBox(void);

public:
	cCaveDefPoints m_Points;

	cCaveTunnel(
		int a_BlockStartX, int a_BlockStartY, int a_BlockStartZ, int a_StartRadius,
		int a_BlockEndX,   int a_BlockEndY,   int a_BlockEndZ,   int a_EndRadius,
		cNoise & a_Noise
	);

	/** Carves the tunnel into the chunk specified */
	void ProcessChunk(
		int a_ChunkX, int a_ChunkZ,
		cChunkDef::BlockTypes & a_BlockTypes,
		cChunkDesc::BlockNibbleBytes & a_BlockMetas,
		cChunkDef::HeightMap & a_HeightMap
	);

	#ifdef _DEBUG
	AString ExportAsSVG(int a_Color, int a_OffsetX, int a_OffsetZ) const;
	#endif  // _DEBUG
} ;

typedef std::vector<cCaveTunnel *> cCaveTunnels;





/** A collection of connected tunnels, possibly branching. */
class cStructGenWormNestCaves::cCaveSystem :
	public cGridStructGen::cStructure
{
	typedef cGridStructGen::cStructure super;

public:
	// The generating block position; is read directly in cStructGenWormNestCaves::GetCavesForChunk()
	int m_BlockX;
	int m_BlockZ;

	cCaveSystem(int a_GridX, int a_GridZ, int a_OriginX, int a_OriginZ, int a_MaxOffset, int a_Size, cNoise & a_Noise);
	~cCaveSystem();

protected:
	int m_Size;
	cCaveTunnels m_Tunnels;

	void Clear(void);

	/** Generates a_Segment successive tunnels, with possible branches. Generates the same output for the same [x, y, z, a_Segments] */
	void GenerateTunnelsFromPoint(
		int a_OriginX, int a_OriginY, int a_OriginZ,
		cNoise & a_Noise, int a_Segments
	);

	/** Returns a radius based on the location provided. */
	int GetRadius(cNoise & a_Noise, int a_OriginX, int a_OriginY, int a_OriginZ);

	// cGridStructGen::cStructure overrides:
	virtual void DrawIntoChunk(cChunkDesc & a_ChunkDesc) override;
} ;





////////////////////////////////////////////////////////////////////////////////
// cCaveTunnel:

cCaveTunnel::cCaveTunnel(
	int a_BlockStartX, int a_BlockStartY, int a_BlockStartZ, int a_StartRadius,
	int a_BlockEndX,   int a_BlockEndY,   int a_BlockEndZ,   int a_EndRadius,
	cNoise & a_Noise
)
{
	m_Points.push_back(cCaveDefPoint(a_BlockStartX, a_BlockStartY, a_BlockStartZ, a_StartRadius));
	m_Points.push_back(cCaveDefPoint(a_BlockEndX,   a_BlockEndY,   a_BlockEndZ,   a_EndRadius));

	if ((a_BlockStartY <= 0) && (a_BlockEndY <= 0))
	{
		// Don't bother detailing this cave, it's under the world anyway
		m_MinBlockX = m_MaxBlockX = 0;
		m_MinBlockY = m_MaxBlockY = -1;
		m_MinBlockZ = m_MaxBlockZ = 0;
		return;
	}

	Randomize(a_Noise);
	Smooth();

	// We know that the linear finishing won't affect the bounding box, so let's calculate it now, as we have less data:
	CalcBoundingBox();

	FinishLinear();
}





void cCaveTunnel::Randomize(cNoise & a_Noise)
{
	// Repeat 4 times:
	for (int i = 0; i < 4; i++)
	{
		// For each already present point, insert a point in between it and its predecessor, shifted randomly.
		cCaveDefPoint & Point = m_Points.front();
		int PrevX = Point.m_BlockX;
		int PrevY = Point.m_BlockY;
		int PrevZ = Point.m_BlockZ;
		int PrevR = Point.m_Radius;
		cCaveDefPoints Pts;
		Pts.reserve(m_Points.size() * 2 + 1);
		Pts.push_back(Point);
		for (cCaveDefPoints::const_iterator itr = m_Points.begin() + 1, end = m_Points.end(); itr != end; ++itr)
		{
			int Random = a_Noise.IntNoise3DInt(PrevX, PrevY, PrevZ + i) / 11;
			int len = (PrevX - itr->m_BlockX) * (PrevX - itr->m_BlockX);
			len += (PrevY - itr->m_BlockY) * (PrevY - itr->m_BlockY);
			len += (PrevZ - itr->m_BlockZ) * (PrevZ - itr->m_BlockZ);
			len = 3 * static_cast<int>(sqrt(static_cast<double>(len))) / 4;
			int Rad = std::min(MAX_RADIUS, std::max(MIN_RADIUS, (PrevR + itr->m_Radius) / 2 + (Random % 3) - 1));
			Random /= 4;
			int x = (itr->m_BlockX + PrevX) / 2 + (Random % (len + 1) - len / 2);
			Random /= 256;
			int y = (itr->m_BlockY + PrevY) / 2 + (Random % (len / 2 + 1) - len / 4);
			Random /= 256;
			int z = (itr->m_BlockZ + PrevZ) / 2 + (Random % (len + 1) - len / 2);
			Pts.push_back(cCaveDefPoint(x, y, z, Rad));
			Pts.push_back(*itr);
			PrevX = itr->m_BlockX;
			PrevY = itr->m_BlockY;
			PrevZ = itr->m_BlockZ;
			PrevR = itr->m_Radius;
		}
		std::swap(Pts, m_Points);
	}
}





bool cCaveTunnel::RefineDefPoints(const cCaveDefPoints & a_Src, cCaveDefPoints & a_Dst)
{
	if (a_Src.size() < 2)
	{
		// There are no midpoints, nothing to smooth
		return true;
	}

	// Smoothing: for each line segment, add points on its 1 / 4 lengths
	bool res = false;
	size_t Num = a_Src.size() - 2;  // this many intermediary points
	a_Dst.clear();
	a_Dst.reserve(Num * 2 + 2);
	cCaveDefPoints::const_iterator itr = a_Src.begin() + 1;
	const cCaveDefPoint & Source = a_Src.front();
	a_Dst.push_back(Source);
	int PrevX = Source.m_BlockX;
	int PrevY = Source.m_BlockY;
	int PrevZ = Source.m_BlockZ;
	int PrevR = Source.m_Radius;
	for (size_t i = 0; i <= Num; ++i, ++itr)
	{
		int dx = itr->m_BlockX - PrevX;
		int dy = itr->m_BlockY - PrevY;
		int dz = itr->m_BlockZ - PrevZ;
		if (abs(dx) + abs(dz) + abs(dy) < 6)
		{
			// Too short a segment to smooth-subdivide into quarters
			PrevX = itr->m_BlockX;
			PrevY = itr->m_BlockY;
			PrevZ = itr->m_BlockZ;
			PrevR = itr->m_Radius;
			continue;
		}
		int dr = itr->m_Radius - PrevR;
		int Rad1 = std::max(PrevR + 1 * dr / 4, 1);
		int Rad2 = std::max(PrevR + 3 * dr / 4, 1);
		a_Dst.push_back(cCaveDefPoint(PrevX + 1 * dx / 4, PrevY + 1 * dy / 4, PrevZ + 1 * dz / 4, Rad1));
		a_Dst.push_back(cCaveDefPoint(PrevX + 3 * dx / 4, PrevY + 3 * dy / 4, PrevZ + 3 * dz / 4, Rad2));
		PrevX = itr->m_BlockX;
		PrevY = itr->m_BlockY;
		PrevZ = itr->m_BlockZ;
		PrevR = itr->m_Radius;
		res = true;
	}
	a_Dst.push_back(a_Src.back());
	return res && (a_Src.size() < a_Dst.size());
}





void cCaveTunnel::Smooth(void)
{
	cCaveDefPoints Pts;
	for (;;)
	{
		if (!RefineDefPoints(m_Points, Pts))
		{
			std::swap(Pts, m_Points);
			return;
		}
		if (!RefineDefPoints(Pts, m_Points))
		{
			return;
		}
	}
}





void cCaveTunnel::FinishLinear(void)
{
	// For each segment, use Bresenham's 3D line algorithm to draw a "line" of defpoints
	cCaveDefPoints Pts;
	std::swap(Pts, m_Points);

	m_Points.reserve(Pts.size() * 3);
	cCaveDefPoint & PrevPoint = Pts.front();
	int PrevX = PrevPoint.m_BlockX;
	int PrevY = PrevPoint.m_BlockY;
	int PrevZ = PrevPoint.m_BlockZ;
	for (cCaveDefPoints::const_iterator itr = Pts.begin() + 1, end = Pts.end(); itr != end; ++itr)
	{
		int x1 = itr->m_BlockX;
		int y1 = itr->m_BlockY;
		int z1 = itr->m_BlockZ;
		int dx = abs(x1 - PrevX);
		int dy = abs(y1 - PrevY);
		int dz = abs(z1 - PrevZ);
		int sx = (PrevX < x1) ? 1 : -1;
		int sy = (PrevY < y1) ? 1 : -1;
		int sz = (PrevZ < z1) ? 1 : -1;
		int R = itr->m_Radius;

		if (dx >= std::max(dy, dz))  // x dominant
		{
			int yd = dy - dx / 2;
			int zd = dz - dx / 2;

			for (;;)
			{
				m_Points.push_back(cCaveDefPoint(PrevX, PrevY, PrevZ, R));

				if (PrevX == x1)
				{
					break;
				}

				if (yd >= 0)  // move along y
				{
					PrevY += sy;
					yd -= dx;
				}

				if (zd >= 0)  // move along z
				{
					PrevZ += sz;
					zd -= dx;
				}

				// move along x
				PrevX  += sx;
				yd += dy;
				zd += dz;
			}
		}
		else if (dy >= std::max(dx, dz))  // y dominant
		{
			int xd = dx - dy / 2;
			int zd = dz - dy / 2;

			for (;;)
			{
				m_Points.push_back(cCaveDefPoint(PrevX, PrevY, PrevZ, R));

				if (PrevY == y1)
				{
					break;
				}

				if (xd >= 0)  // move along x
				{
					PrevX += sx;
					xd -= dy;
				}

				if (zd >= 0)  // move along z
				{
					PrevZ += sz;
					zd -= dy;
				}

				// move along y
				PrevY += sy;
				xd += dx;
				zd += dz;
			}
		}
		else
		{
			// z dominant
			ASSERT(dz >= std::max(dx, dy));
			int xd = dx - dz / 2;
			int yd = dy - dz / 2;

			for (;;)
			{
				m_Points.push_back(cCaveDefPoint(PrevX, PrevY, PrevZ, R));

				if (PrevZ == z1)
				{
					break;
				}

				if (xd >= 0)  // move along x
				{
					PrevX += sx;
					xd -= dz;
				}

				if (yd >= 0)  // move along y
				{
					PrevY += sy;
					yd -= dz;
				}

				// move along z
				PrevZ += sz;
				xd += dx;
				yd += dy;
			}
		}  // if (which dimension is dominant)
	}  // for itr
}





void cCaveTunnel::CalcBoundingBox(void)
{
	cCaveDefPoint & Point = m_Points.front();
	m_MinBlockX = m_MaxBlockX = Point.m_BlockX;
	m_MinBlockY = m_MaxBlockY = Point.m_BlockY;
	m_MinBlockZ = m_MaxBlockZ = Point.m_BlockZ;
	for (cCaveDefPoints::const_iterator itr = m_Points.begin() + 1, end = m_Points.end(); itr != end; ++itr)
	{
		m_MinBlockX = std::min(m_MinBlockX, itr->m_BlockX - itr->m_Radius);
		m_MaxBlockX = std::max(m_MaxBlockX, itr->m_BlockX + itr->m_Radius);
		m_MinBlockY = std::min(m_MinBlockY, itr->m_BlockY - itr->m_Radius);
		m_MaxBlockY = std::max(m_MaxBlockY, itr->m_BlockY + itr->m_Radius);
		m_MinBlockZ = std::min(m_MinBlockZ, itr->m_BlockZ - itr->m_Radius);
		m_MaxBlockZ = std::max(m_MaxBlockZ, itr->m_BlockZ + itr->m_Radius);
	}  // for itr - m_Points[]
}





void cCaveTunnel::ProcessChunk(
	int a_ChunkX, int a_ChunkZ,
	cChunkDef::BlockTypes & a_BlockTypes,
	cChunkDesc::BlockNibbleBytes & a_BlockMetas,
	cChunkDef::HeightMap & a_HeightMap
)
{
	int BaseX = a_ChunkX * cChunkDef::Width;
	int BaseZ = a_ChunkZ * cChunkDef::Width;
	if (
		(BaseX > m_MaxBlockX) || (BaseX + cChunkDef::Width < m_MinBlockX) ||
		(BaseZ > m_MaxBlockZ) || (BaseZ + cChunkDef::Width < m_MinBlockZ)
	)
	{
		// Tunnel does not intersect the chunk at all, bail out
		return;
	}

	int BlockStartX = a_ChunkX * cChunkDef::Width;
	int BlockStartZ = a_ChunkZ * cChunkDef::Width;
	int BlockEndX = BlockStartX + cChunkDef::Width;
	int BlockEndZ = BlockStartZ + cChunkDef::Width;
	for (cCaveDefPoints::const_iterator itr = m_Points.begin(), end = m_Points.end(); itr != end; ++itr)
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

		// Carve out a sphere around the xyz point, m_Radius in diameter; skip 3 / 7 off the top and bottom:
		int DifX = itr->m_BlockX - BlockStartX;  // substitution for faster calc
		int DifY = itr->m_BlockY;
		int DifZ = itr->m_BlockZ - BlockStartZ;  // substitution for faster calc
		int Bottom = std::max(itr->m_BlockY - 3 * itr->m_Radius / 7, 1);
		int Top    = std::min(itr->m_BlockY + 3 * itr->m_Radius / 7, static_cast<int>(cChunkDef::Height));
		int SqRad  = itr->m_Radius * itr->m_Radius;
		for (int z = 0; z < cChunkDef::Width; z++) for (int x = 0; x < cChunkDef::Width; x++)
		{
			for (int y = Bottom; y <= Top; y++)
			{
				int SqDist = (DifX - x) * (DifX - x) + (DifY - y) * (DifY - y) + (DifZ - z) * (DifZ - z);
				if (4 * SqDist <= SqRad)
				{
					if (cBlockInfo::CanBeTerraformed(cChunkDef::GetBlock(a_BlockTypes, x, y, z)))
					{
						cChunkDef::SetBlock(a_BlockTypes, x, y, z, E_BLOCK_AIR);
					}
				}
				else if (SqDist <= SqRad * 2)
				{
					if (cChunkDef::GetBlock(a_BlockTypes, x, y, z) == E_BLOCK_SAND)
					{
						int Index = cChunkDef::MakeIndexNoCheck(x, y, z);
						if (a_BlockMetas[Index] == 1)
						{
							a_BlockMetas[Index] = 0;
							cChunkDef::SetBlock(a_BlockTypes, x, y, z, E_BLOCK_RED_SANDSTONE);
						}
						else
						{
							cChunkDef::SetBlock(a_BlockTypes, x, y, z, E_BLOCK_SANDSTONE);
						}
					}
				}
			}  // for y
		}  // for x, z
	}  // for itr - m_Points[]

	/*
	#ifdef _DEBUG
	// For debugging purposes, outline the shape of the cave using glowstone, after carving the entire cave:
	for (cCaveDefPoints::const_iterator itr = m_Points.begin(), end = m_Points.end(); itr != end; ++itr)
	{
		int DifX = itr->m_BlockX - BlockStartX;  // substitution for faster calc
		int DifZ = itr->m_BlockZ - BlockStartZ;  // substitution for faster calc
		if (
			(DifX >= 0) && (DifX < cChunkDef::Width) &&
			(itr->m_BlockY > 0) && (itr->m_BlockY < cChunkDef::Height) &&
			(DifZ >= 0) && (DifZ < cChunkDef::Width)
		)
		{
			cChunkDef::SetBlock(a_BlockTypes, DifX, itr->m_BlockY, DifZ, E_BLOCK_GLOWSTONE);
		}
	}  // for itr - m_Points[]
	#endif  // _DEBUG
	//*/
}





#ifdef _DEBUG
AString cCaveTunnel::ExportAsSVG(int a_Color, int a_OffsetX, int a_OffsetZ) const
{
	AString SVG;
	SVG.reserve(m_Points.size() * 20 + 200);
	AppendPrintf(SVG, "<path style=\"fill:none;stroke:#%06x;stroke-width:1px;\"\nd=\"", a_Color);
	char Prefix = 'M';  // The first point needs "M" prefix, all the others need "L"
	for (cCaveDefPoints::const_iterator itr = m_Points.begin(); itr != m_Points.end(); ++itr)
	{
		AppendPrintf(SVG, "%c %d, %d ", Prefix, a_OffsetX + itr->m_BlockX, a_OffsetZ + itr->m_BlockZ);
		Prefix = 'L';
	}
	SVG.append("\"/>\n");
	return SVG;
}
#endif  // _DEBUG





////////////////////////////////////////////////////////////////////////////////
// cStructGenWormNestCaves::cCaveSystem:

cStructGenWormNestCaves::cCaveSystem::cCaveSystem(int a_GridX, int a_GridZ, int a_OriginX, int a_OriginZ, int a_MaxOffset, int a_Size, cNoise & a_Noise) :
	super(a_GridX, a_GridZ, a_OriginX, a_OriginZ),
	m_Size(a_Size)
{
	int Num = 1 + a_Noise.IntNoise2DInt(a_OriginX, a_OriginZ) % 3;
	for (int i = 0; i < Num; i++)
	{
		int OriginX = a_OriginX + (a_Noise.IntNoise3DInt(13 * a_OriginX, 17 * a_OriginZ, 11 * i) / 19) % a_MaxOffset;
		int OriginZ = a_OriginZ + (a_Noise.IntNoise3DInt(17 * a_OriginX, 13 * a_OriginZ, 11 * i) / 23) % a_MaxOffset;
		int OriginY  = 20 + (a_Noise.IntNoise3DInt(19 * a_OriginX, 13 * a_OriginZ, 11 * i) / 17) % 20;

		// Generate three branches from the origin point:
		// The tunnels generated depend on X, Y, Z and Branches,
		// for the same set of numbers it generates the same offsets!
		// That's why we add a +1 to X in the third line
		GenerateTunnelsFromPoint(OriginX,     OriginY, OriginZ, a_Noise, 3);
		GenerateTunnelsFromPoint(OriginX,     OriginY, OriginZ, a_Noise, 2);
		GenerateTunnelsFromPoint(OriginX + 1, OriginY, OriginZ, a_Noise, 3);
	}
}





cStructGenWormNestCaves::cCaveSystem::~cCaveSystem()
{
	Clear();
}






void cStructGenWormNestCaves::cCaveSystem::DrawIntoChunk(cChunkDesc & a_ChunkDesc)
{
	int ChunkX = a_ChunkDesc.GetChunkX();
	int ChunkZ = a_ChunkDesc.GetChunkZ();
	cChunkDef::BlockTypes        & BlockTypes = a_ChunkDesc.GetBlockTypes();
	cChunkDef::HeightMap         &  HeightMap = a_ChunkDesc.GetHeightMap();
	cChunkDesc::BlockNibbleBytes & BlockMetas = a_ChunkDesc.GetBlockMetasUncompressed();
	for (cCaveTunnels::const_iterator itr = m_Tunnels.begin(), end = m_Tunnels.end(); itr != end; ++itr)
	{
		(*itr)->ProcessChunk(ChunkX, ChunkZ, BlockTypes, BlockMetas, HeightMap);
	}  // for itr - m_Tunnels[]
}





void cStructGenWormNestCaves::cCaveSystem::Clear(void)
{
	for (cCaveTunnels::const_iterator itr = m_Tunnels.begin(), end = m_Tunnels.end(); itr != end; ++itr)
	{
		delete *itr;
	}
	m_Tunnels.clear();
}





void cStructGenWormNestCaves::cCaveSystem::GenerateTunnelsFromPoint(
	int a_OriginX, int a_OriginY, int a_OriginZ,
	cNoise & a_Noise, int a_NumSegments
)
{
	int DoubleSize = m_Size * 2;
	int Radius = GetRadius(a_Noise, a_OriginX + a_OriginY, a_OriginY + a_OriginZ, a_OriginZ + a_OriginX);
	for (int i = a_NumSegments - 1; i >= 0; --i)
	{
		int EndX = a_OriginX + (((a_Noise.IntNoise3DInt(a_OriginX, a_OriginY, a_OriginZ + 11 * a_NumSegments) / 7) % DoubleSize) - m_Size) / 2;
		int EndY = a_OriginY + (((a_Noise.IntNoise3DInt(a_OriginY, 13 * a_NumSegments, a_OriginZ + a_OriginX) / 7) % DoubleSize) - m_Size) / 4;
		int EndZ = a_OriginZ + (((a_Noise.IntNoise3DInt(a_OriginZ + 17 * a_NumSegments, a_OriginX, a_OriginY) / 7) % DoubleSize) - m_Size) / 2;
		int EndR = GetRadius(a_Noise, a_OriginX + 7 * i, a_OriginY + 11 * i, a_OriginZ + a_OriginX);
		m_Tunnels.push_back(new cCaveTunnel(a_OriginX, a_OriginY, a_OriginZ, Radius, EndX, EndY, EndZ, EndR, a_Noise));
		GenerateTunnelsFromPoint(EndX, EndY, EndZ, a_Noise, i);
		a_OriginX = EndX;
		a_OriginY = EndY;
		a_OriginZ = EndZ;
		Radius = EndR;
	}  // for i - a_NumSegments
}





int cStructGenWormNestCaves::cCaveSystem::GetRadius(cNoise & a_Noise, int a_OriginX, int a_OriginY, int a_OriginZ)
{
	// Instead of a flat distribution noise function, we need to shape it, so that most caves are smallish and only a few select are large
	int rnd = a_Noise.IntNoise3DInt(a_OriginX, a_OriginY, a_OriginZ) / 11;
	/*
	// Not good enough:
	// The algorithm of choice: emulate gauss-distribution noise by adding 3 flat noises, then fold it in half using absolute value.
	// To save on processing, use one random value and extract 3 bytes to be separately added as the gaussian noise
	int sum = (rnd & 0xff) + ((rnd >> 8) & 0xff) + ((rnd >> 16) & 0xff);
	// sum is now a gaussian-distribution noise within [0 .. 767], with center at 384.
	// We want mapping 384 -> 3, 0 -> 19, 768 -> 19, so divide by 24 to get [0 .. 31] with center at 16, then use abs() to fold around the center
	int res = 3 + abs((sum / 24) - 16);
	*/

	// Algorithm of choice: random value in the range of zero to random value - heavily towards zero
	int res = MIN_RADIUS + (rnd >> 8) % ((rnd % (MAX_RADIUS - MIN_RADIUS)) + 1);
	return res;
}





////////////////////////////////////////////////////////////////////////////////
// cStructGenWormNestCaves:

cGridStructGen::cStructurePtr cStructGenWormNestCaves::CreateStructure(int a_GridX, int a_GridZ, int a_OriginX, int a_OriginZ)
{
	return cStructurePtr(new cCaveSystem(a_GridX, a_GridZ, a_OriginX, a_OriginZ, m_MaxOffset, m_Size, m_Noise));
}






////////////////////////////////////////////////////////////////////////////////
// cStructGenMarbleCaves:

static float GetMarbleNoise( float x, float y, float z, cNoise & a_Noise)
{
	static const float PI_2 = 1.57079633f;
	float oct1 = (a_Noise.CubicNoise3D(x * 0.1f, y * 0.1f, z * 0.1f)) * 4;

	oct1 = oct1 * oct1 * oct1;
	if (oct1 < 0.f)
	{
		oct1 = PI_2;
	}
	if (oct1 > PI_2)
	{
		oct1 = PI_2;
	}

	return oct1;
}





void cStructGenMarbleCaves::GenFinish(cChunkDesc & a_ChunkDesc)
{
	cNoise Noise(m_Seed);
	for (int z = 0; z < cChunkDef::Width; z++)
	{
		const float zz = static_cast<float>(a_ChunkDesc.GetChunkZ() * cChunkDef::Width + z);
		for (int x = 0; x < cChunkDef::Width; x++)
		{
			const float xx = static_cast<float>(a_ChunkDesc.GetChunkX() * cChunkDef::Width + x);

			int Top = a_ChunkDesc.GetHeight(x, z);
			for (int y = 1; y < Top; ++y)
			{
				if (a_ChunkDesc.GetBlockType(x, y, z) != E_BLOCK_STONE)
				{
					continue;
				}

				const float yy = static_cast<float>(y);
				const float WaveNoise = 1;
				if (cosf(GetMarbleNoise(xx, yy * 0.5f, zz, Noise)) * fabs(cosf(yy * 0.2f + WaveNoise * 2) * 0.75f + WaveNoise) > 0.0005f)
				{
					a_ChunkDesc.SetBlockType(x, y, z, E_BLOCK_AIR);
				}
			}  // for y
		}  // for x
	}  // for z
}





////////////////////////////////////////////////////////////////////////////////
// cStructGenDualRidgeCaves:

void cStructGenDualRidgeCaves::GenFinish(cChunkDesc & a_ChunkDesc)
{
	for (int z = 0; z < cChunkDef::Width; z++)
	{
		const float zz = static_cast<float>(a_ChunkDesc.GetChunkZ() * cChunkDef::Width + z) / 10;
		for (int x = 0; x < cChunkDef::Width; x++)
		{
			const float xx = static_cast<float>(a_ChunkDesc.GetChunkX() * cChunkDef::Width + x) / 10;

			int Top = a_ChunkDesc.GetHeight(x, z);
			for (int y = 1; y <= Top; ++y)
			{
				const float yy = static_cast<float>(y / 10);
				float n1 = m_Noise1.CubicNoise3D(xx, yy, zz);
				float n2 = m_Noise2.CubicNoise3D(xx, yy, zz);
				float n3 = m_Noise1.CubicNoise3D(xx * 4, yy * 4, zz * 4) / 4;
				float n4 = m_Noise2.CubicNoise3D(xx * 4, yy * 4, zz * 4) / 4;
				if ((std::abs(n1 + n3) * std::abs(n2 + n4)) > m_Threshold)
				{
					a_ChunkDesc.SetBlockType(x, y, z, E_BLOCK_AIR);
				}
			}  // for y
		}  // for x
	}  // for z
}




