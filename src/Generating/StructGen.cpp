
// StructGen.h

#include "Globals.h"
#include "StructGen.h"
#include "Trees.h"
#include "../BlockArea.h"
#include "../LinearUpscale.h"





////////////////////////////////////////////////////////////////////////////////
// cStructGenTrees:

void cStructGenTrees::GenFinish(cChunkDesc & a_ChunkDesc)
{
	int ChunkX = a_ChunkDesc.GetChunkX();
	int ChunkZ = a_ChunkDesc.GetChunkZ();

	cChunkDesc WorkerDesc({ChunkX, ChunkZ});

	// Generate trees:
	for (int x = 0; x <= 2; x++)
	{
		int BaseX = ChunkX + x - 1;
		for (int z = 0; z <= 2; z++)
		{
			int BaseZ = ChunkZ + z - 1;

			cChunkDesc * Dest;

			if ((x != 1) || (z != 1))
			{
				Dest = &WorkerDesc;
				WorkerDesc.SetChunkCoords({BaseX, BaseZ});

				// TODO: This may cause a lot of wasted calculations, instead of pulling data out of a single (cChunkDesc) cache

				cChunkDesc::Shape workerShape;
				m_BiomeGen->GenBiomes           ({BaseX, BaseZ}, WorkerDesc.GetBiomeMap());
				m_ShapeGen->GenShape            ({BaseX, BaseZ}, workerShape);
				WorkerDesc.SetHeightFromShape   (workerShape);
				m_CompositionGen->ComposeTerrain(WorkerDesc, workerShape);
			}
			else
			{
				Dest = &a_ChunkDesc;
			}

			int NumTrees = GetNumTrees(BaseX, BaseZ, Dest->GetBiomeMap());

			sSetBlockVector OutsideLogs, OutsideOther;
			for (int i = 0; i < NumTrees; i++)
			{
				GenerateSingleTree(BaseX, BaseZ, i, *Dest, OutsideLogs, OutsideOther);
			}

			sSetBlockVector IgnoredOverflow;
			IgnoredOverflow.reserve(OutsideOther.size());
			ApplyTreeImage(ChunkX, ChunkZ, a_ChunkDesc, OutsideOther, IgnoredOverflow);
			IgnoredOverflow.clear();
			IgnoredOverflow.reserve(OutsideLogs.size());
			ApplyTreeImage(ChunkX, ChunkZ, a_ChunkDesc, OutsideLogs, IgnoredOverflow);
		}  // for z
	}  // for x

	a_ChunkDesc.UpdateHeightmap();
}





void cStructGenTrees::GenerateSingleTree(
	int a_ChunkX, int a_ChunkZ, int a_Seq,
	cChunkDesc & a_ChunkDesc,
	sSetBlockVector & a_OutsideLogs,
	sSetBlockVector & a_OutsideOther
)
{
	int x = (m_Noise.IntNoise3DInt(a_ChunkX + a_ChunkZ, a_ChunkZ, a_Seq) / 19) % cChunkDef::Width;
	int z = (m_Noise.IntNoise3DInt(a_ChunkX - a_ChunkZ, a_Seq, a_ChunkZ) / 19) % cChunkDef::Width;

	int Height = a_ChunkDesc.GetHeight(x, z);

	if ((Height <= 0) || (Height >= 230))
	{
		return;
	}

	// Check the block underneath the tree:
	BLOCKTYPE TopBlock = a_ChunkDesc.GetBlockType(x, Height, z);
	if ((TopBlock != E_BLOCK_DIRT) && (TopBlock != E_BLOCK_GRASS) && (TopBlock != E_BLOCK_FARMLAND))
	{
		return;
	}

	sSetBlockVector TreeLogs, TreeOther;
	GetTreeImageByBiome(
		{ a_ChunkX * cChunkDef::Width + x, Height + 1, a_ChunkZ * cChunkDef::Width + z },
		m_Noise, a_Seq,
		a_ChunkDesc.GetBiome(x, z),
		TreeLogs, TreeOther
	);

	// Check if the generated image fits the terrain. Only the logs are checked:
	for (sSetBlockVector::const_iterator itr = TreeLogs.begin(); itr != TreeLogs.end(); ++itr)
	{
		if ((itr->m_ChunkX != a_ChunkX) || (itr->m_ChunkZ != a_ChunkZ))
		{
			// Outside the chunk
			continue;
		}
		if (itr->m_RelY >= cChunkDef::Height)
		{
			// Above the chunk, cut off (this shouldn't happen too often, we're limiting trees to y < 230)
			continue;
		}

		BLOCKTYPE Block = a_ChunkDesc.GetBlockType(itr->m_RelX, itr->m_RelY, itr->m_RelZ);
		switch (Block)
		{
			CASE_TREE_ALLOWED_BLOCKS:
			{
				break;
			}
			default:
			{
				// There's something in the way, abort this tree altogether
				return;
			}
		}
	}

	ApplyTreeImage(a_ChunkX, a_ChunkZ, a_ChunkDesc, TreeOther, a_OutsideOther);
	ApplyTreeImage(a_ChunkX, a_ChunkZ, a_ChunkDesc, TreeLogs,  a_OutsideLogs);
}





void cStructGenTrees::ApplyTreeImage(
	int a_ChunkX, int a_ChunkZ,
	cChunkDesc & a_ChunkDesc,
	const sSetBlockVector & a_Image,
	sSetBlockVector & a_Overflow
)
{
	// Put the generated image into a_BlockTypes, push things outside this chunk into a_Blocks
	for (sSetBlockVector::const_iterator itr = a_Image.begin(), end = a_Image.end(); itr != end; ++itr)
	{
		if ((itr->m_ChunkX == a_ChunkX) && (itr->m_ChunkZ == a_ChunkZ) && (itr->m_RelY < cChunkDef::Height))
		{
			// Inside this chunk, integrate into a_ChunkDesc:
			switch (a_ChunkDesc.GetBlockType(itr->m_RelX, itr->m_RelY, itr->m_RelZ))
			{
				case E_BLOCK_NEW_LEAVES:
				case E_BLOCK_LEAVES:
				{
					if ((itr->m_BlockType != E_BLOCK_LOG) && (itr->m_BlockType != E_BLOCK_NEW_LOG))
					{
						break;
					}
					// fallthrough:
				}
				CASE_TREE_OVERWRITTEN_BLOCKS:
				{
					a_ChunkDesc.SetBlockTypeMeta(itr->m_RelX, itr->m_RelY, itr->m_RelZ, itr->m_BlockType, itr->m_BlockMeta);
					// If grass is below our tree, turn it to dirt
					if (
						(cBlockInfo::IsSolid(itr->m_BlockType)) &&
						(a_ChunkDesc.GetBlockType(itr->m_RelX, itr->m_RelY - 1, itr->m_RelZ) == E_BLOCK_GRASS)
					)
					{
						a_ChunkDesc.SetBlockType(itr->m_RelX, itr->m_RelY - 1, itr->m_RelZ, E_BLOCK_DIRT);
					}
					break;
				}

			}  // switch (GetBlock())
			continue;
		}

		// Outside the chunk, push into a_Overflow.
		// Don't check if already present there, by separating logs and others we don't need the checks anymore:
		a_Overflow.push_back(*itr);
	}
}





int cStructGenTrees::GetNumTrees(
	int a_ChunkX, int a_ChunkZ,
	const cChunkDef::BiomeMap & a_Biomes
)
{
	auto BiomeTrees = [](EMCSBiome a_Biome)
	{
		switch (a_Biome)
		{
			case biOcean:                return 2;
			case biPlains:               return 1;
			case biDesert:               return 0;
			case biExtremeHills:         return 3;
			case biForest:               return 30;
			case biTaiga:                return 30;
			case biSwampland:            return 8;
			case biRiver:                return 0;
			case biNether:               return 0;
			case biEnd:                  return 0;
			case biFrozenOcean:          return 0;
			case biFrozenRiver:          return 0;
			case biIcePlains:            return 1;
			case biIceMountains:         return 1;
			case biMushroomIsland:       return 3;
			case biMushroomShore:        return 3;
			case biBeach:                return 0;
			case biDesertHills:          return 0;
			case biForestHills:          return 20;
			case biTaigaHills:           return 20;
			case biExtremeHillsEdge:     return 5;
			case biJungle:               return 120;
			case biJungleHills:          return 90;
			case biJungleEdge:           return 90;
			case biDeepOcean:            return 0;
			case biStoneBeach:           return 0;
			case biColdBeach:            return 0;
			case biBirchForest:          return 30;
			case biBirchForestHills:     return 20;
			case biRoofedForest:         return 50;
			case biColdTaiga:            return 20;
			case biColdTaigaHills:       return 15;
			case biMegaTaiga:            return 30;
			case biMegaTaigaHills:       return 25;
			case biExtremeHillsPlus:     return 3;
			case biSavanna:              return 8;
			case biSavannaPlateau:       return 12;
			case biMesa:                 return 2;
			case biMesaPlateauF:         return 8;
			case biMesaPlateau:          return 8;
			// Biome variants
			case biSunflowerPlains:      return 1;
			case biDesertM:              return 0;
			case biExtremeHillsM:        return 4;
			case biFlowerForest:         return 30;
			case biTaigaM:               return 30;
			case biSwamplandM:           return 8;
			case biIcePlainsSpikes:      return 1;
			case biJungleM:              return 120;
			case biJungleEdgeM:          return 90;
			case biBirchForestM:         return 30;
			case biBirchForestHillsM:    return 20;
			case biRoofedForestM:        return 40;
			case biColdTaigaM:           return 30;
			case biMegaSpruceTaiga:      return 30;
			case biMegaSpruceTaigaHills: return 30;
			case biExtremeHillsPlusM:    return 4;
			case biSavannaM:             return 8;
			case biSavannaPlateauM:      return 12;
			case biMesaBryce:            return 4;
			case biMesaPlateauFM:        return 12;
			case biMesaPlateauM:         return 12;
			// Non-biomes
			case biInvalidBiome:
			case biNumBiomes:
			case biVariant:
			case biNumVariantBiomes:
			{
				ASSERT(!"Invalid biome in cStructGenTrees::GetNumTrees");
				return 0;
			}
		}
		UNREACHABLE("Unsupported biome");
	};

	int NumTrees = 0;
	for (auto Biome : a_Biomes)
	{
		NumTrees += BiomeTrees(Biome);
	}
	return NumTrees / 1024;
}





////////////////////////////////////////////////////////////////////////////////
// cStructGenLakes:

void cStructGenLakes::GenFinish(cChunkDesc & a_ChunkDesc)
{
	int ChunkX = a_ChunkDesc.GetChunkX();
	int ChunkZ = a_ChunkDesc.GetChunkZ();

	for (int z = -1; z < 2; z++) for (int x = -1; x < 2; x++)
	{
		if (((m_Noise.IntNoise2DInt(ChunkX + x, ChunkZ + z) / 17) % 100) > m_Probability)
		{
			continue;
		}

		cBlockArea Lake;
		CreateLakeImage(ChunkX + x, ChunkZ + z, a_ChunkDesc.GetMinHeight(), Lake);

		int OfsX = Lake.GetOriginX() + x * cChunkDef::Width;
		int OfsZ = Lake.GetOriginZ() + z * cChunkDef::Width;

		// Merge the lake into the current data
		a_ChunkDesc.WriteBlockArea(Lake, OfsX, Lake.GetOriginY(), OfsZ, cBlockArea::msLake);
	}  // for x, z - neighbor chunks
}





void cStructGenLakes::CreateLakeImage(int a_ChunkX, int a_ChunkZ, int a_MaxLakeHeight, cBlockArea & a_Lake)
{
	a_Lake.Create(16, 8, 16);
	a_Lake.Fill(cBlockArea::baTypes, E_BLOCK_SPONGE);  // Sponge is the NOP blocktype for lake merging strategy

	// Make a random position in the chunk by using a random 16 block XZ offset and random height up to chunk's max height minus 6
	int MinHeight = std::max(a_MaxLakeHeight - 6, 2);
	int Rnd = m_Noise.IntNoise3DInt(a_ChunkX, 128, a_ChunkZ) / 11;
	// Random offset [-8 .. 8], with higher probability around 0; add up four three-bit-wide randoms [0 .. 28], divide and subtract to get range
	int OffsetX = 4 * ((Rnd & 0x07) + ((Rnd & 0x38) >> 3) + ((Rnd & 0x1c0) >> 6) + ((Rnd & 0xe00) >> 9)) / 7 - 8;
	Rnd >>= 12;
	// Random offset [-8 .. 8], with higher probability around 0; add up four three-bit-wide randoms [0 .. 28], divide and subtract to get range
	int OffsetZ = 4 * ((Rnd & 0x07) + ((Rnd & 0x38) >> 3) + ((Rnd & 0x1c0) >> 6) + ((Rnd & 0xe00) >> 9)) / 7 - 8;
	Rnd = m_Noise.IntNoise3DInt(a_ChunkX, 512, a_ChunkZ) / 13;
	// Random height [1 .. MinHeight] with preference to center heights
	int HeightY = 1 + (((Rnd & 0x1ff) % MinHeight) + (((Rnd >> 9) & 0x1ff) % MinHeight)) / 2;

	a_Lake.SetOrigin(OffsetX, HeightY, OffsetZ);

	// Hollow out a few bubbles inside the blockarea:
	int NumBubbles = 4 + ((Rnd >> 18) & 0x03);  // 4 .. 7 bubbles
	BLOCKTYPE * BlockTypes = a_Lake.GetBlockTypes();
	for (int i = 0; i < NumBubbles; i++)
	{
		int BubbleRnd = m_Noise.IntNoise3DInt(a_ChunkX, i, a_ChunkZ) / 13;
		const int BubbleR = 2 + (BubbleRnd & 0x03);  // 2 .. 5
		const int Range = 16 - 2 * BubbleR;
		const int BubbleX = BubbleR + (BubbleRnd % Range);
		BubbleRnd >>= 4;
		const int BubbleY = 4 + (BubbleRnd & 0x01);  // 4 .. 5
		BubbleRnd >>= 1;
		const int BubbleZ = BubbleR + (BubbleRnd % Range);
		const int HalfR = BubbleR / 2;  // 1 .. 2
		const int RSquared = BubbleR * BubbleR;
		for (int y = -HalfR; y <= HalfR; y++)
		{
			// BubbleY + y is in the [0, 7] bounds
			int DistY = 4 * y * y / 3;
			int IdxY = (BubbleY + y) * 16 * 16;
			for (int z = -BubbleR; z <= BubbleR; z++)
			{
				int DistYZ = DistY + z * z;
				if (DistYZ >= RSquared)
				{
					continue;
				}
				int IdxYZ = BubbleX + IdxY + (BubbleZ + z) * 16;
				for (int x = -BubbleR; x <= BubbleR; x++)
				{
					if (x * x + DistYZ < RSquared)
					{
						BlockTypes[x + IdxYZ] = E_BLOCK_AIR;
					}
				}  // for x
			}  // for z
		}  // for y
	}  // for i - bubbles

	// Turn air in the bottom half into liquid:
	for (int y = 0; y < 4; y++)
	{
		for (int z = 0; z < 16; z++) for (int x = 0; x < 16; x++)
		{
			if (BlockTypes[x + z * 16 + y * 16 * 16] == E_BLOCK_AIR)
			{
				BlockTypes[x + z * 16 + y * 16 * 16] = m_Fluid;
			}
		}  // for z, x
	}  // for y

	// TODO: Turn sponge next to lava into stone

	// a_Lake.SaveToSchematicFile(Printf("Lake_%d_%d.schematic", a_ChunkX, a_ChunkZ));
}





////////////////////////////////////////////////////////////////////////////////
// cStructGenDirectOverhangs:

cStructGenDirectOverhangs::cStructGenDirectOverhangs(int a_Seed) :
	m_Noise1(a_Seed),
	m_Noise2(a_Seed + 1000)
{
}





void cStructGenDirectOverhangs::GenFinish(cChunkDesc & a_ChunkDesc)
{
	// If there is no column of the wanted biome, bail out:
	if (!HasWantedBiome(a_ChunkDesc))
	{
		return;
	}

	HEIGHTTYPE MaxHeight = a_ChunkDesc.GetMaxHeight();

	const int SEGMENT_HEIGHT = 8;
	const int INTERPOL_X = 16;  // Must be a divisor of 16
	const int INTERPOL_Z = 16;  // Must be a divisor of 16
	// Interpolate the chunk in 16 * SEGMENT_HEIGHT * 16 "segments", each SEGMENT_HEIGHT blocks high and each linearly interpolated separately.
	// Have two buffers, one for the lowest floor and one for the highest floor, so that Y-interpolation can be done between them
	// Then swap the buffers and use the previously-top one as the current-bottom, without recalculating it.

	int FloorBuf1[17 * 17];
	int FloorBuf2[17 * 17];
	int * FloorHi = FloorBuf1;
	int * FloorLo = FloorBuf2;
	int BaseX = a_ChunkDesc.GetChunkX() * cChunkDef::Width;
	int BaseZ = a_ChunkDesc.GetChunkZ() * cChunkDef::Width;
	int BaseY = 63;

	// Interpolate the lowest floor:
	for (int z = 0; z <= 16 / INTERPOL_Z; z++) for (int x = 0; x <= 16 / INTERPOL_X; x++)
	{
		FloorLo[INTERPOL_X * x + 17 * INTERPOL_Z * z] =
			m_Noise1.IntNoise3DInt(BaseX + INTERPOL_X * x, BaseY, BaseZ + INTERPOL_Z * z) *
			m_Noise2.IntNoise3DInt(BaseX + INTERPOL_X * x, BaseY, BaseZ + INTERPOL_Z * z) /
			256;
	}  // for x, z - FloorLo[]
	LinearUpscale2DArrayInPlace<17, 17, INTERPOL_X, INTERPOL_Z>(FloorLo);

	// Interpolate segments:
	for (int Segment = BaseY; Segment < MaxHeight; Segment += SEGMENT_HEIGHT)
	{
		// First update the high floor:
		for (int z = 0; z <= 16 / INTERPOL_Z; z++) for (int x = 0; x <= 16 / INTERPOL_X; x++)
		{
			FloorHi[INTERPOL_X * x + 17 * INTERPOL_Z * z] = (
				m_Noise1.IntNoise3DInt(BaseX + INTERPOL_X * x, Segment + SEGMENT_HEIGHT, BaseZ + INTERPOL_Z * z) *
				m_Noise2.IntNoise3DInt(BaseX + INTERPOL_Z * x, Segment + SEGMENT_HEIGHT, BaseZ + INTERPOL_Z * z) / 256
			);
		}  // for x, z - FloorLo[]
		LinearUpscale2DArrayInPlace<17, 17, INTERPOL_X, INTERPOL_Z>(FloorHi);

		// Interpolate between FloorLo and FloorHi:
		for (int z = 0; z < 16; z++) for (int x = 0; x < 16; x++)
		{
			EMCSBiome biome = a_ChunkDesc.GetBiome(x, z);

			if ((biome == biExtremeHills) || (biome == biExtremeHillsEdge))
			{
				int Lo = FloorLo[x + 17 * z] / 256;
				int Hi = FloorHi[x + 17 * z] / 256;
				for (int y = 0; y < SEGMENT_HEIGHT; y++)
				{
					int Val = Lo + (Hi - Lo) * y / SEGMENT_HEIGHT;
					if (Val < 0)
					{
						a_ChunkDesc.SetBlockType(x, y + Segment, z, E_BLOCK_AIR);
					}
				}  // for y
				break;
			}  // if (biome)
		}  // for z, x

		// Swap the floors:
		std::swap(FloorLo, FloorHi);
	}
}





bool cStructGenDirectOverhangs::HasWantedBiome(cChunkDesc & a_ChunkDesc) const
{
	cChunkDef::BiomeMap & Biomes = a_ChunkDesc.GetBiomeMap();
	for (size_t i = 0; i < ARRAYCOUNT(Biomes); i++)
	{
		switch (Biomes[i])
		{
			case biExtremeHills:
			case biExtremeHillsEdge:
			{
				return true;
			}
			default:
			{
				break;
			}
		}
	}  // for i
	return false;
}





////////////////////////////////////////////////////////////////////////////////
// cStructGenDistortedMembraneOverhangs:

cStructGenDistortedMembraneOverhangs::cStructGenDistortedMembraneOverhangs(int a_Seed) :
	m_NoiseX(a_Seed + 1000),
	m_NoiseY(a_Seed + 2000),
	m_NoiseZ(a_Seed + 3000),
	m_NoiseH(a_Seed + 4000)
{
}





void cStructGenDistortedMembraneOverhangs::GenFinish(cChunkDesc & a_ChunkDesc)
{
	const NOISE_DATATYPE Frequency = static_cast<NOISE_DATATYPE>(16);
	const NOISE_DATATYPE Amount = static_cast<NOISE_DATATYPE>(1);
	for (int y = 50; y < 128; y++)
	{
		NOISE_DATATYPE NoiseY = static_cast<NOISE_DATATYPE>(y) / 32;
		// TODO: proper water level - where to get?
		BLOCKTYPE ReplacementBlock = (y > 62) ? E_BLOCK_AIR : E_BLOCK_STATIONARY_WATER;
		for (int z = 0; z < cChunkDef::Width; z++)
		{
			NOISE_DATATYPE NoiseZ = static_cast<NOISE_DATATYPE>(a_ChunkDesc.GetChunkZ() * cChunkDef::Width + z) / Frequency;
			for (int x = 0; x < cChunkDef::Width; x++)
			{
				NOISE_DATATYPE NoiseX = static_cast<NOISE_DATATYPE>(a_ChunkDesc.GetChunkX() * cChunkDef::Width + x) / Frequency;
				NOISE_DATATYPE DistortX = m_NoiseX.CubicNoise3D(NoiseX, NoiseY, NoiseZ) * Amount;
				NOISE_DATATYPE DistortY = m_NoiseY.CubicNoise3D(NoiseX, NoiseY, NoiseZ) * Amount;
				NOISE_DATATYPE DistortZ = m_NoiseZ.CubicNoise3D(NoiseX, NoiseY, NoiseZ) * Amount;
				int MembraneHeight = 96 - static_cast<int>((DistortY + m_NoiseH.CubicNoise2D(NoiseX + DistortX, NoiseZ + DistortZ)) * 30);
				if (MembraneHeight < y)
				{
					a_ChunkDesc.SetBlockType(x, y, z, ReplacementBlock);
				}
			}  // for y
		}  // for x
	}  // for z
}




