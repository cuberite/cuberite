
// BlockArea.cpp

// Implements the cBlockArea object representing an area of block data that can be queried from cWorld and then accessed again without further queries
// The object also supports writing the blockdata back into cWorld, even into other coords

#include "Globals.h"
#include "BlockArea.h"
#include "World.h"
#include "OSSupport/GZipFile.h"
#include "WorldStorage/FastNBT.h"
#include "Blocks/BlockHandler.h"





// This wild construct allows us to pass a function argument and still have it inlined by the compiler :)
/// Merges two blocktypes and blockmetas of the specified sizes and offsets using the specified combinator function
template<typename Combinator> void InternalMergeBlocks(
	BLOCKTYPE * a_DstTypes, const BLOCKTYPE * a_SrcTypes,
	NIBBLETYPE * a_DstMetas, const NIBBLETYPE * a_SrcMetas, 
	int a_SizeX, int a_SizeY, int a_SizeZ,
	int a_SrcOffX, int a_SrcOffY, int a_SrcOffZ,
	int a_DstOffX, int a_DstOffY, int a_DstOffZ,
	int a_SrcSizeX, int a_SrcSizeY, int a_SrcSizeZ,
	int a_DstSizeX, int a_DstSizeY, int a_DstSizeZ,
	Combinator a_Combinator
)
{
	UNUSED(a_SrcSizeY);
	UNUSED(a_DstSizeY);
	for (int y = 0; y < a_SizeY; y++)
	{
		int SrcBaseY = (y + a_SrcOffY) * a_SrcSizeX * a_SrcSizeZ;
		int DstBaseY = (y + a_DstOffY) * a_DstSizeX * a_DstSizeZ;
		for (int z = 0; z < a_SizeZ; z++)
		{
			int SrcBaseZ = SrcBaseY + (z + a_SrcOffZ) * a_SrcSizeX;
			int DstBaseZ = DstBaseY + (z + a_DstOffZ) * a_DstSizeX;
			int SrcIdx = SrcBaseZ + a_SrcOffX;
			int DstIdx = DstBaseZ + a_DstOffX;
			for (int x = 0; x < a_SizeX; x++)
			{
				a_Combinator(a_DstTypes[DstIdx], a_SrcTypes[SrcIdx], a_DstMetas[DstIdx], a_SrcMetas[SrcIdx]);
				++DstIdx;
				++SrcIdx;
			}  // for x
		}  // for z
	}  // for y
}





/// Combinator used for cBlockArea::msOverwrite merging
static void MergeCombinatorOverwrite(BLOCKTYPE & a_DstType, BLOCKTYPE a_SrcType, NIBBLETYPE & a_DstMeta, NIBBLETYPE a_SrcMeta)
{
	a_DstType = a_SrcType;
	a_DstMeta = a_SrcMeta;
}





/// Combinator used for cBlockArea::msFillAir merging
static void MergeCombinatorFillAir(BLOCKTYPE & a_DstType, BLOCKTYPE a_SrcType, NIBBLETYPE & a_DstMeta, NIBBLETYPE a_SrcMeta)
{
	if (a_DstType == E_BLOCK_AIR)
	{
		a_DstType = a_SrcType;
		a_DstMeta = a_SrcMeta;
	}
	// "else" is the default, already in place
}





/// Combinator used for cBlockArea::msImprint merging
static void MergeCombinatorImprint(BLOCKTYPE & a_DstType, BLOCKTYPE a_SrcType, NIBBLETYPE & a_DstMeta, NIBBLETYPE a_SrcMeta)
{
	if (a_SrcType != E_BLOCK_AIR)
	{
		a_DstType = a_SrcType;
		a_DstMeta = a_SrcMeta;
	}
	// "else" is the default, already in place
}





/// Combinator used for cBlockArea::msLake merging
static void MergeCombinatorLake(BLOCKTYPE & a_DstType, BLOCKTYPE a_SrcType, NIBBLETYPE & a_DstMeta, NIBBLETYPE a_SrcMeta)
{
	// Sponge is the NOP block
	if (a_SrcType == E_BLOCK_SPONGE)
	{
		return;
	}

	// Air is always hollowed out	
	if (a_SrcType == E_BLOCK_AIR)
	{
		a_DstType = E_BLOCK_AIR;
		a_DstMeta = 0;
		return;
	}
	
	// Water and lava are never overwritten
	switch (a_DstType)
	{
		case E_BLOCK_WATER:
		case E_BLOCK_STATIONARY_WATER:
		case E_BLOCK_LAVA:
		case E_BLOCK_STATIONARY_LAVA:
		{
			return;
		}
	}
	
	// Water and lava always overwrite
	switch (a_SrcType)
	{
		case E_BLOCK_WATER:
		case E_BLOCK_STATIONARY_WATER:
		case E_BLOCK_LAVA:
		case E_BLOCK_STATIONARY_LAVA:
		{
			a_DstType = a_SrcType;
			a_DstMeta = a_SrcMeta;
			return;
		}
	}
	
	if (a_SrcType == E_BLOCK_STONE)
	{
		switch (a_DstType)
		{
			case E_BLOCK_DIRT:
			case E_BLOCK_GRASS:
			case E_BLOCK_MYCELIUM:
			{
				a_DstType = E_BLOCK_STONE;
				a_DstMeta = 0;
				return;
			}
		}
	}
	// Everything else is left as it is
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cBlockArea:

cBlockArea::cBlockArea(void) :
	m_OriginX(0),
	m_OriginY(0),
	m_OriginZ(0),
	m_SizeX(0),
	m_SizeY(0),
	m_SizeZ(0),
	m_BlockTypes(NULL),
	m_BlockMetas(NULL),
	m_BlockLight(NULL),
	m_BlockSkyLight(NULL)
{
}





cBlockArea::~cBlockArea()
{
	Clear();
}





void cBlockArea::Clear(void)
{
	delete[] m_BlockTypes;    m_BlockTypes = NULL;
	delete[] m_BlockMetas;    m_BlockMetas = NULL;
	delete[] m_BlockLight;    m_BlockLight = NULL;
	delete[] m_BlockSkyLight; m_BlockSkyLight = NULL;
	m_OriginX = 0;
	m_OriginY = 0;
	m_OriginZ = 0;
	m_SizeX = 0;
	m_SizeY = 0;
	m_SizeZ = 0;
}





void cBlockArea::Create(int a_SizeX, int a_SizeY, int a_SizeZ, int a_DataTypes)
{
	Clear();
	int BlockCount = a_SizeX * a_SizeY * a_SizeZ;
	if ((a_DataTypes & baTypes) != 0)
	{
		m_BlockTypes = new BLOCKTYPE[BlockCount];
		for (int i = 0; i < BlockCount; i++)
		{
			m_BlockTypes[i] = E_BLOCK_AIR;
		}
	}
	if ((a_DataTypes & baMetas) != 0)
	{
		m_BlockMetas = new NIBBLETYPE[BlockCount];
		for (int i = 0; i < BlockCount; i++)
		{
			m_BlockMetas[i] = 0;
		}
	}
	if ((a_DataTypes & baLight) != 0)
	{
		m_BlockLight = new NIBBLETYPE[BlockCount];
		for (int i = 0; i < BlockCount; i++)
		{
			m_BlockLight[i] = 0;
		}
	}
	if ((a_DataTypes & baSkyLight) != 0)
	{
		m_BlockSkyLight = new NIBBLETYPE[BlockCount];
		for (int i = 0; i < BlockCount; i++)
		{
			m_BlockSkyLight[i] = 0x0f;
		}
	}
	m_SizeX = a_SizeX;
	m_SizeY = a_SizeY;
	m_SizeZ = a_SizeZ;
	m_OriginX = 0;
	m_OriginY = 0;
	m_OriginZ = 0;
}





void cBlockArea::SetOrigin(int a_OriginX, int a_OriginY, int a_OriginZ)
{
	m_OriginX = a_OriginX;
	m_OriginY = a_OriginY;
	m_OriginZ = a_OriginZ;
}





bool cBlockArea::Read(cWorld * a_World, int a_MinBlockX, int a_MaxBlockX, int a_MinBlockY, int a_MaxBlockY, int a_MinBlockZ, int a_MaxBlockZ, int a_DataTypes)
{
	// Normalize the coords:
	if (a_MinBlockX > a_MaxBlockX)
	{
		std::swap(a_MinBlockX, a_MaxBlockX);
	}
	if (a_MinBlockY > a_MaxBlockY)
	{
		std::swap(a_MinBlockY, a_MaxBlockY);
	}
	if (a_MinBlockZ > a_MaxBlockZ)
	{
		std::swap(a_MinBlockZ, a_MaxBlockZ);
	}
	
	// Include the Max coords:
	a_MaxBlockX += 1;
	a_MaxBlockY += 1;
	a_MaxBlockZ += 1;
	
	// Check coords validity:
	if (a_MinBlockY < 0)
	{
		LOGWARNING("%s: MinBlockY less than zero, adjusting to zero", __FUNCTION__);
		a_MinBlockY = 0;
	}
	else if (a_MinBlockY >= cChunkDef::Height)
	{
		LOGWARNING("%s: MinBlockY more than chunk height, adjusting to chunk height", __FUNCTION__);
		a_MinBlockY = cChunkDef::Height - 1;
	}
	if (a_MaxBlockY < 0)
	{
		LOGWARNING("%s: MaxBlockY less than zero, adjusting to zero", __FUNCTION__);
		a_MaxBlockY = 0;
	}
	else if (a_MaxBlockY > cChunkDef::Height)
	{
		LOGWARNING("%s: MaxBlockY more than chunk height, adjusting to chunk height", __FUNCTION__);
		a_MaxBlockY = cChunkDef::Height;
	}
	
	// Allocate the needed memory:
	Clear();
	if (!SetSize(a_MaxBlockX - a_MinBlockX, a_MaxBlockY - a_MinBlockY, a_MaxBlockZ - a_MinBlockZ, a_DataTypes))
	{
		return false;
	}
	m_OriginX = a_MinBlockX;
	m_OriginY = a_MinBlockY;
	m_OriginZ = a_MinBlockZ;
	cChunkReader Reader(*this);
	
	// Convert block coords to chunks coords:
	int MinChunkX, MaxChunkX;
	int MinChunkZ, MaxChunkZ;
	cChunkDef::AbsoluteToRelative(a_MinBlockX, a_MinBlockY, a_MinBlockZ, MinChunkX, MinChunkZ);
	cChunkDef::AbsoluteToRelative(a_MaxBlockX, a_MaxBlockY, a_MaxBlockZ, MaxChunkX, MaxChunkZ);
	
	// Query block data:
	if (!a_World->ForEachChunkInRect(MinChunkX, MaxChunkX, MinChunkZ, MaxChunkZ, Reader))
	{
		Clear();
		return false;
	}
	
	return true;
}





bool cBlockArea::Write(cWorld * a_World, int a_MinBlockX, int a_MinBlockY, int a_MinBlockZ, int a_DataTypes)
{
	ASSERT((a_DataTypes & GetDataTypes()) == a_DataTypes);  // Are you requesting only the data that I have?
	a_DataTypes = a_DataTypes & GetDataTypes();  // For release builds, silently cut off the datatypes that I don't have

	// Check coords validity:
	if (a_MinBlockY < 0)
	{
		LOGWARNING("%s: MinBlockY less than zero, adjusting to zero", __FUNCTION__);
		a_MinBlockY = 0;
	}
	else if (a_MinBlockY > cChunkDef::Height - m_SizeY)
	{
		LOGWARNING("%s: MinBlockY + m_SizeY more than chunk height, adjusting to chunk height", __FUNCTION__);
		a_MinBlockY = cChunkDef::Height - m_SizeY;
	}

	return a_World->WriteBlockArea(*this, a_MinBlockX, a_MinBlockY, a_MinBlockZ, a_DataTypes);
}





void cBlockArea::CopyTo(cBlockArea & a_Into) const
{
	if (&a_Into == this)
	{
		LOGWARNING("Trying to copy a cBlockArea into self, ignoring.");
		return;
	}
	
	a_Into.Clear();
	a_Into.SetSize(m_SizeX, m_SizeY, m_SizeZ, GetDataTypes());
	a_Into.m_OriginX = m_OriginX;
	a_Into.m_OriginY = m_OriginY;
	a_Into.m_OriginZ = m_OriginZ;
	int BlockCount = GetBlockCount();
	if (HasBlockTypes())
	{
		memcpy(a_Into.m_BlockTypes, m_BlockTypes, BlockCount * sizeof(BLOCKTYPE));
	}
	if (HasBlockMetas())
	{
		memcpy(a_Into.m_BlockMetas, m_BlockMetas, BlockCount * sizeof(NIBBLETYPE));
	}
	if (HasBlockLights())
	{
		memcpy(a_Into.m_BlockLight, m_BlockLight, BlockCount * sizeof(NIBBLETYPE));
	}
	if (HasBlockSkyLights())
	{
		memcpy(a_Into.m_BlockSkyLight, m_BlockSkyLight, BlockCount * sizeof(NIBBLETYPE));
	}
}





void cBlockArea::CopyFrom(const cBlockArea & a_From)
{
	a_From.CopyTo(*this);
}





void cBlockArea::DumpToRawFile(const AString & a_FileName)
{
	cFile f;
	if (!f.Open(a_FileName, cFile::fmWrite))
	{
		LOGWARNING("cBlockArea: Cannot open file \"%s\" for raw dump", a_FileName.c_str());
		return;
	}
	UInt32 SizeX = ntohl(m_SizeX);
	UInt32 SizeY = ntohl(m_SizeY);
	UInt32 SizeZ = ntohl(m_SizeZ);
	f.Write(&SizeX, 4);
	f.Write(&SizeY, 4);
	f.Write(&SizeZ, 4);
	unsigned char DataTypes = GetDataTypes();
	f.Write(&DataTypes, 1);
	int NumBlocks = GetBlockCount();
	if (HasBlockTypes())
	{
		f.Write(m_BlockTypes, NumBlocks * sizeof(BLOCKTYPE));
	}
	if (HasBlockMetas())
	{
		f.Write(m_BlockMetas, NumBlocks);
	}
	if (HasBlockLights())
	{
		f.Write(m_BlockLight, NumBlocks);
	}
	if (HasBlockSkyLights())
	{
		f.Write(m_BlockSkyLight, NumBlocks);
	}
}





bool cBlockArea::LoadFromSchematicFile(const AString & a_FileName)
{
	// Un-GZip the contents:
	AString Contents;
	cGZipFile File;
	if (!File.Open(a_FileName, cGZipFile::fmRead))
	{
		LOG("Cannot open the schematic file \"%s\".", a_FileName.c_str());
		return false;
	}
	int NumBytesRead = File.ReadRestOfFile(Contents);
	if (NumBytesRead < 0)
	{
		LOG("Cannot read GZipped data in the schematic file \"%s\", error %d", a_FileName.c_str(), NumBytesRead);
		return false;
	}
	File.Close();
	
	// Parse the NBT:
	cParsedNBT NBT(Contents.data(), Contents.size());
	if (!NBT.IsValid())
	{
		LOG("Cannot parse the NBT in the schematic file \"%s\".", a_FileName.c_str());
		return false;
	}
	
	return LoadFromSchematicNBT(NBT);
}





bool cBlockArea::SaveToSchematicFile(const AString & a_FileName)
{
	cFastNBTWriter Writer("Schematic");
	Writer.AddShort("Width", m_SizeX);
	Writer.AddShort("Height", m_SizeY);
	Writer.AddShort("Length", m_SizeZ);
	Writer.AddString("Materials", "Alpha");
	if (HasBlockTypes())
	{
		Writer.AddByteArray("Blocks", (const char *)m_BlockTypes, GetBlockCount());
	}
	else
	{
		AString Dummy(GetBlockCount(), 0);
		Writer.AddByteArray("Blocks", Dummy.data(), Dummy.size());
	}
	if (HasBlockMetas())
	{
		Writer.AddByteArray("Data", (const char *)m_BlockMetas, GetBlockCount());
	}
	else
	{
		AString Dummy(GetBlockCount(), 0);
		Writer.AddByteArray("Data", Dummy.data(), Dummy.size());
	}
	// TODO: Save entities and block entities
	Writer.BeginList("Entities", TAG_Compound);
	Writer.EndList();
	Writer.BeginList("TileEntities", TAG_Compound);
	Writer.EndList();
	Writer.Finish();
	
	// Save to file
	cGZipFile File;
	if (!File.Open(a_FileName, cGZipFile::fmWrite))
	{
		LOG("Cannot open file \"%s\" for writing.", a_FileName.c_str());
		return false;
	}
	if (!File.Write(Writer.GetResult()))
	{
		LOG("Cannot write data to file \"%s\".", a_FileName.c_str());
		return false;
	}
	return true;
}





void cBlockArea::Crop(int a_AddMinX, int a_SubMaxX, int a_AddMinY, int a_SubMaxY, int a_AddMinZ, int a_SubMaxZ)
{
	if (
		(a_AddMinX + a_SubMaxX >= m_SizeX) ||
		(a_AddMinY + a_SubMaxY >= m_SizeY) ||
		(a_AddMinZ + a_SubMaxZ >= m_SizeZ)
	)
	{
		LOGWARNING("cBlockArea:Crop called with more croping than the dimensions: %d x %d x %d with cropping %d, %d and %d",
			m_SizeX, m_SizeY, m_SizeZ,
			a_AddMinX + a_SubMaxX, a_AddMinY + a_SubMaxY, a_AddMinZ + a_SubMaxZ
		);
		return;
	}
	
	if (HasBlockTypes())
	{
		CropBlockTypes(a_AddMinX, a_SubMaxX, a_AddMinY, a_SubMaxY, a_AddMinZ, a_SubMaxZ);
	}
	if (HasBlockMetas())
	{
		CropNibbles(m_BlockMetas, a_AddMinX, a_SubMaxX, a_AddMinY, a_SubMaxY, a_AddMinZ, a_SubMaxZ);
	}
	if (HasBlockLights())
	{
		CropNibbles(m_BlockLight, a_AddMinX, a_SubMaxX, a_AddMinY, a_SubMaxY, a_AddMinZ, a_SubMaxZ);
	}
	if (HasBlockSkyLights())
	{
		CropNibbles(m_BlockSkyLight, a_AddMinX, a_SubMaxX, a_AddMinY, a_SubMaxY, a_AddMinZ, a_SubMaxZ);
	}
	m_OriginX += a_AddMinX;
	m_OriginY += a_AddMinY;
	m_OriginZ += a_AddMinZ;
	m_SizeX -= a_AddMinX + a_SubMaxX;
	m_SizeY -= a_AddMinY + a_SubMaxY;
	m_SizeZ -= a_AddMinZ + a_SubMaxZ;
}





void cBlockArea::Expand(int a_SubMinX, int a_AddMaxX, int a_SubMinY, int a_AddMaxY, int a_SubMinZ, int a_AddMaxZ)
{
	if (HasBlockTypes())
	{
		ExpandBlockTypes(a_SubMinX, a_AddMaxX, a_SubMinY, a_AddMaxY, a_SubMinZ, a_AddMaxZ);
	}
	if (HasBlockMetas())
	{
		ExpandNibbles(m_BlockMetas, a_SubMinX, a_AddMaxX, a_SubMinY, a_AddMaxY, a_SubMinZ, a_AddMaxZ);
	}
	if (HasBlockLights())
	{
		ExpandNibbles(m_BlockLight, a_SubMinX, a_AddMaxX, a_SubMinY, a_AddMaxY, a_SubMinZ, a_AddMaxZ);
	}
	if (HasBlockSkyLights())
	{
		ExpandNibbles(m_BlockSkyLight, a_SubMinX, a_AddMaxX, a_SubMinY, a_AddMaxY, a_SubMinZ, a_AddMaxZ);
	}
	m_OriginX -= a_SubMinX;
	m_OriginY -= a_SubMinY;
	m_OriginZ -= a_SubMinZ;
	m_SizeX += a_SubMinX + a_AddMaxX;
	m_SizeY += a_SubMinY + a_AddMaxY;
	m_SizeZ += a_SubMinZ + a_AddMaxZ;
}





void cBlockArea::Merge(const cBlockArea & a_Src, int a_RelX, int a_RelY, int a_RelZ, eMergeStrategy a_Strategy)
{
	// Block types are compulsory, block metas are voluntary
	if (!HasBlockTypes() || !a_Src.HasBlockTypes())
	{
		LOGWARNING("%s: cannot merge because one of the areas doesn't have blocktypes.", __FUNCTION__);
		return;
	}
	
	// Dst is *this, Src is a_Src
	int SrcOffX = std::max(0, -a_RelX);  // Offset in Src where to start reading
	int DstOffX = std::max(0,  a_RelX);  // Offset in Dst where to start writing
	int SizeX   = std::min(a_Src.GetSizeX() - SrcOffX, GetSizeX() - DstOffX);  // How many blocks to copy

	int SrcOffY = std::max(0, -a_RelY);  // Offset in Src where to start reading
	int DstOffY = std::max(0,  a_RelY);  // Offset in Dst where to start writing
	int SizeY   = std::min(a_Src.GetSizeY() - SrcOffY, GetSizeY() - DstOffY);  // How many blocks to copy

	int SrcOffZ = std::max(0, -a_RelZ);  // Offset in Src where to start reading
	int DstOffZ = std::max(0,  a_RelZ);  // Offset in Dst where to start writing
	int SizeZ   = std::min(a_Src.GetSizeZ() - SrcOffZ, GetSizeZ() - DstOffZ);  // How many blocks to copy

	const NIBBLETYPE * SrcMetas = a_Src.GetBlockMetas();
	NIBBLETYPE * DstMetas = m_BlockMetas;
	bool IsDummyMetas = ((SrcMetas == NULL) || (DstMetas == NULL));
	
	if (IsDummyMetas)
	{
		SrcMetas = new NIBBLETYPE[a_Src.GetBlockCount()];
		DstMetas = new NIBBLETYPE[GetBlockCount()];
	}
	
	switch (a_Strategy)
	{
		case msOverwrite:
		{
			InternalMergeBlocks(
				m_BlockTypes, a_Src.GetBlockTypes(),
				DstMetas, SrcMetas,
				SizeX, SizeY, SizeZ,
				SrcOffX, SrcOffY, SrcOffZ,
				DstOffX, DstOffY, DstOffZ,
				a_Src.GetSizeX(), a_Src.GetSizeY(), a_Src.GetSizeZ(),
				m_SizeX, m_SizeY, m_SizeZ,
				MergeCombinatorOverwrite
			);
			break;
		}  // case msOverwrite
		
		case msFillAir:
		{
			InternalMergeBlocks(
				m_BlockTypes, a_Src.GetBlockTypes(),
				DstMetas, SrcMetas,
				SizeX, SizeY, SizeZ,
				SrcOffX, SrcOffY, SrcOffZ,
				DstOffX, DstOffY, DstOffZ,
				a_Src.GetSizeX(), a_Src.GetSizeY(), a_Src.GetSizeZ(),
				m_SizeX, m_SizeY, m_SizeZ,
				MergeCombinatorFillAir
			);
			break;
		}  // case msFillAir
		
		case msImprint:
		{
			InternalMergeBlocks(
				m_BlockTypes, a_Src.GetBlockTypes(),
				DstMetas, SrcMetas,
				SizeX, SizeY, SizeZ,
				SrcOffX, SrcOffY, SrcOffZ,
				DstOffX, DstOffY, DstOffZ,
				a_Src.GetSizeX(), a_Src.GetSizeY(), a_Src.GetSizeZ(),
				m_SizeX, m_SizeY, m_SizeZ,
				MergeCombinatorImprint
			);
			break;
		}  // case msImprint
		
		case msLake:
		{
			InternalMergeBlocks(
				m_BlockTypes, a_Src.GetBlockTypes(),
				DstMetas, SrcMetas,
				SizeX, SizeY, SizeZ,
				SrcOffX, SrcOffY, SrcOffZ,
				DstOffX, DstOffY, DstOffZ,
				a_Src.GetSizeX(), a_Src.GetSizeY(), a_Src.GetSizeZ(),
				m_SizeX, m_SizeY, m_SizeZ,
				MergeCombinatorLake
			);
			break;
		}  // case msLake
		
		default:
		{
			LOGWARNING("Unknown block area merge strategy: %d", a_Strategy);
			ASSERT(!"Unknown block area merge strategy");
			break;
		}
	}  // switch (a_Strategy)
	
	if (IsDummyMetas)
	{
		delete[] SrcMetas;
		delete[] DstMetas;
	}
}





void cBlockArea::Fill(int a_DataTypes, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, NIBBLETYPE a_BlockLight, NIBBLETYPE a_BlockSkyLight)
{
	if ((a_DataTypes & GetDataTypes()) != a_DataTypes)
	{
		LOGWARNING("%s: requested datatypes that are not present in the BlockArea object, trimming those away (req 0x%x, stor 0x%x)",
			__FUNCTION__, a_DataTypes, GetDataTypes()
		);
		a_DataTypes = a_DataTypes & GetDataTypes();
	}
	
	int BlockCount = GetBlockCount();
	if ((a_DataTypes & baTypes) != 0)
	{
		for (int i = 0; i < BlockCount; i++)
		{
			m_BlockTypes[i] = a_BlockType;
		}
	}
	if ((a_DataTypes & baMetas) != 0)
	{
		for (int i = 0; i < BlockCount; i++)
		{
			m_BlockMetas[i] = a_BlockMeta;
		}
	}
	if ((a_DataTypes & baLight) != 0)
	{
		for (int i = 0; i < BlockCount; i++)
		{
			m_BlockLight[i] = a_BlockLight;
		}
	}
	if ((a_DataTypes & baSkyLight) != 0)
	{
		for (int i = 0; i < BlockCount; i++)
		{
			m_BlockSkyLight[i] = a_BlockSkyLight;
		}
	}
}





void cBlockArea::FillRelCuboid(int a_MinRelX, int a_MaxRelX, int a_MinRelY, int a_MaxRelY, int a_MinRelZ, int a_MaxRelZ, 
	int a_DataTypes, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta,
	NIBBLETYPE a_BlockLight, NIBBLETYPE a_BlockSkyLight
)
{
	if ((a_DataTypes & GetDataTypes()) != a_DataTypes)
	{
		LOGWARNING("%s: requested datatypes that are not present in the BlockArea object, trimming those away (req 0x%x, stor 0x%x)",
			__FUNCTION__, a_DataTypes, GetDataTypes()
		);
		a_DataTypes = a_DataTypes & GetDataTypes();
	}
	
	if ((a_DataTypes & baTypes) != 0)
	{
		for (int y = a_MinRelY; y <= a_MaxRelY; y++) for (int z = a_MinRelZ; z <= a_MaxRelZ; z++) for (int x = a_MinRelX; x <= a_MaxRelX; x++)
		{
			m_BlockTypes[MakeIndex(x, y, z)] = a_BlockType;
		}  // for x, z, y
	}
	if ((a_DataTypes & baMetas) != 0)
	{
		for (int y = a_MinRelY; y <= a_MaxRelY; y++) for (int z = a_MinRelZ; z <= a_MaxRelZ; z++) for (int x = a_MinRelX; x <= a_MaxRelX; x++)
		{
			m_BlockMetas[MakeIndex(x, y, z)] = a_BlockMeta;
		}  // for x, z, y
	}
	if ((a_DataTypes & baLight) != 0)
	{
		for (int y = a_MinRelY; y <= a_MaxRelY; y++) for (int z = a_MinRelZ; z <= a_MaxRelZ; z++) for (int x = a_MinRelX; x <= a_MaxRelX; x++)
		{
			m_BlockLight[MakeIndex(x, y, z)] = a_BlockLight;
		}  // for x, z, y
	}
	if ((a_DataTypes & baSkyLight) != 0)
	{
		for (int y = a_MinRelY; y <= a_MaxRelY; y++) for (int z = a_MinRelZ; z <= a_MaxRelZ; z++) for (int x = a_MinRelX; x <= a_MaxRelX; x++)
		{
			m_BlockSkyLight[MakeIndex(x, y, z)] = a_BlockSkyLight;
		}  // for x, z, y
	}
}





void cBlockArea::RelLine(int a_RelX1, int a_RelY1, int a_RelZ1, int a_RelX2, int a_RelY2, int a_RelZ2,
	int a_DataTypes, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta,
	NIBBLETYPE a_BlockLight, NIBBLETYPE a_BlockSkyLight
)
{
	// Bresenham-3D algorithm for drawing lines:
	int dx = abs(a_RelX2 - a_RelX1);
	int dy = abs(a_RelY2 - a_RelY1);
	int dz = abs(a_RelZ2 - a_RelZ1);
	int sx = (a_RelX1 < a_RelX2) ? 1 : -1;
	int sy = (a_RelY1 < a_RelY2) ? 1 : -1;
	int sz = (a_RelZ1 < a_RelZ2) ? 1 : -1;

	if (dx >= std::max(dy, dz))  // x dominant
	{
		int yd = dy - dx / 2;
		int zd = dz - dx / 2;

		for (;;)
		{
			RelSetData(a_RelX1, a_RelY1, a_RelZ1, a_DataTypes, a_BlockType, a_BlockMeta, a_BlockLight, a_BlockSkyLight);

			if (a_RelX1 == a_RelX2)
			{
				break;
			}

			if (yd >= 0)  // move along y
			{
				a_RelY1 += sy;
				yd -= dx;
			}

			if (zd >= 0)  // move along z
			{
				a_RelZ1 += sz;
				zd -= dx;
			}

			// move along x
			a_RelX1  += sx;
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
			RelSetData(a_RelX1, a_RelY1, a_RelZ1, a_DataTypes, a_BlockType, a_BlockMeta, a_BlockLight, a_BlockSkyLight);

			if (a_RelY1 == a_RelY2)
			{
				break;
			}

			if (xd >= 0)  // move along x
			{
				a_RelX1 += sx;
				xd -= dy;
			}

			if (zd >= 0)  // move along z
			{
				a_RelZ1 += sz;
				zd -= dy;
			}

			// move along y
			a_RelY1 += sy;
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
			RelSetData(a_RelX1, a_RelY1, a_RelZ1, a_DataTypes, a_BlockType, a_BlockMeta, a_BlockLight, a_BlockSkyLight);

			if (a_RelZ1 == a_RelZ2)
			{
				break;
			}

			if (xd >= 0)  // move along x
			{
				a_RelX1 += sx;
				xd -= dz;
			}

			if (yd >= 0)  // move along y
			{
				a_RelY1 += sy;
				yd -= dz;
			}
			
			// move along z
			a_RelZ1 += sz;
			xd += dx;
			yd += dy;
		}
	}  // if (which dimension is dominant)
}





void cBlockArea::RotateCCW(void)
{
	if (!HasBlockTypes())
	{
		LOGWARNING("cBlockArea: Cannot rotate blockmeta without blocktypes!");
		return;
	}
	
	if (!HasBlockMetas())
	{
		// There are no blockmetas to rotate, just use the NoMeta function
		RotateCCWNoMeta();
		return;
	}
	
	// We are guaranteed that both blocktypes and blockmetas exist; rotate both at the same time:
	BLOCKTYPE * NewTypes = new BLOCKTYPE[m_SizeX * m_SizeY * m_SizeZ];
	NIBBLETYPE * NewMetas = new NIBBLETYPE[m_SizeX * m_SizeY * m_SizeZ];
	for (int x = 0; x < m_SizeX; x++)
	{
		int NewZ = m_SizeX - x - 1;
		for (int z = 0; z < m_SizeZ; z++)
		{
			int NewX = z;
			for (int y = 0; y < m_SizeY; y++)
			{
				int NewIdx = NewX + NewZ * m_SizeX + y * m_SizeX * m_SizeZ;
				int OldIdx = MakeIndex(x, y, z);
				NewTypes[NewIdx] = m_BlockTypes[OldIdx];
				NewMetas[NewIdx] = BlockHandler(m_BlockTypes[OldIdx])->MetaRotateCCW(m_BlockMetas[OldIdx]);
			}  // for y
		}  // for z
	}  // for x
	std::swap(m_BlockTypes, NewTypes);
	std::swap(m_BlockMetas, NewMetas);
	delete[] NewTypes;
	delete[] NewMetas;

	std::swap(m_SizeX, m_SizeZ);
}





void cBlockArea::RotateCW(void)
{
	if (!HasBlockTypes())
	{
		LOGWARNING("cBlockArea: Cannot rotate blockmeta without blocktypes!");
		return;
	}

	if (!HasBlockMetas())
	{
		// There are no blockmetas to rotate, just use the NoMeta function
		RotateCWNoMeta();
		return;
	}
	
	// We are guaranteed that both blocktypes and blockmetas exist; rotate both at the same time:
	BLOCKTYPE * NewTypes = new BLOCKTYPE[m_SizeX * m_SizeY * m_SizeZ];
	NIBBLETYPE * NewMetas = new NIBBLETYPE[m_SizeX * m_SizeY * m_SizeZ];
	for (int x = 0; x < m_SizeX; x++)
	{
		int NewZ = x;
		for (int z = 0; z < m_SizeZ; z++)
		{
			int NewX = m_SizeZ - z - 1;
			for (int y = 0; y < m_SizeY; y++)
			{
				int NewIdx = NewX + NewZ * m_SizeX + y * m_SizeX * m_SizeZ;
				int OldIdx = MakeIndex(x, y, z);
				NewTypes[NewIdx] = m_BlockTypes[OldIdx];
				NewMetas[NewIdx] = BlockHandler(m_BlockTypes[OldIdx])->MetaRotateCW(m_BlockMetas[OldIdx]);
			}  // for y
		}  // for z
	}  // for x
	std::swap(m_BlockTypes, NewTypes);
	std::swap(m_BlockMetas, NewMetas);
	delete[] NewTypes;
	delete[] NewMetas;

	std::swap(m_SizeX, m_SizeZ);
}





void cBlockArea::MirrorXY(void)
{
	if (!HasBlockTypes())
	{
		LOGWARNING("cBlockArea: Cannot mirror meta without blocktypes!");
		return;
	}
	
	if (!HasBlockMetas())
	{
		// There are no blockmetas to mirror, just use the NoMeta function
		MirrorXYNoMeta();
		return;
	}

	// We are guaranteed that both blocktypes and blockmetas exist; mirror both at the same time:
	int HalfZ = m_SizeZ / 2;
	int MaxZ = m_SizeZ - 1;
	for (int y = 0; y < m_SizeY; y++)
	{
		for (int z = 0; z < HalfZ; z++)
		{
			for (int x = 0; x < m_SizeX; x++)
			{
				int Idx1 = MakeIndex(x, y, z);
				int Idx2 = MakeIndex(x, y, MaxZ - z);
				std::swap(m_BlockTypes[Idx1], m_BlockTypes[Idx2]);
				NIBBLETYPE Meta1 = BlockHandler(m_BlockTypes[Idx2])->MetaMirrorXY(m_BlockMetas[Idx1]);
				NIBBLETYPE Meta2 = BlockHandler(m_BlockTypes[Idx1])->MetaMirrorXY(m_BlockMetas[Idx2]);
				m_BlockMetas[Idx1] = Meta2;
				m_BlockMetas[Idx2] = Meta1;
			}  // for x
		}  // for z
	}  // for y
}





void cBlockArea::MirrorXZ(void)
{
	if (!HasBlockTypes())
	{
		LOGWARNING("cBlockArea: Cannot mirror meta without blocktypes!");
		return;
	}

	if (!HasBlockMetas())
	{
		// There are no blockmetas to mirror, just use the NoMeta function
		MirrorXZNoMeta();
		return;
	}

	// We are guaranteed that both blocktypes and blockmetas exist; mirror both at the same time:
	int HalfY = m_SizeY / 2;
	int MaxY = m_SizeY - 1;
	for (int y = 0; y < HalfY; y++)
	{
		for (int z = 0; z < m_SizeZ; z++)
		{
			for (int x = 0; x < m_SizeX; x++)
			{
				int Idx1 = MakeIndex(x, y, z);
				int Idx2 = MakeIndex(x, MaxY - y, z);
				std::swap(m_BlockTypes[Idx1], m_BlockTypes[Idx2]);
				NIBBLETYPE Meta1 = BlockHandler(m_BlockTypes[Idx2])->MetaMirrorXZ(m_BlockMetas[Idx1]);
				NIBBLETYPE Meta2 = BlockHandler(m_BlockTypes[Idx1])->MetaMirrorXZ(m_BlockMetas[Idx2]);
				m_BlockMetas[Idx1] = Meta2;
				m_BlockMetas[Idx2] = Meta1;
			}  // for x
		}  // for z
	}  // for y
}





void cBlockArea::MirrorYZ(void)
{
	if (!HasBlockTypes())
	{
		LOGWARNING("cBlockArea: Cannot mirror meta without blocktypes!");
		return;
	}

	if (!HasBlockMetas())
	{
		// There are no blockmetas to mirror, just use the NoMeta function
		MirrorYZNoMeta();
		return;
	}

	// We are guaranteed that both blocktypes and blockmetas exist; mirror both at the same time:
	int HalfX = m_SizeX / 2;
	int MaxX = m_SizeX - 1;
	for (int y = 0; y < m_SizeY; y++)
	{
		for (int z = 0; z < m_SizeZ; z++)
		{
			for (int x = 0; x < HalfX; x++)
			{
				int Idx1 = MakeIndex(x, y, z);
				int Idx2 = MakeIndex(MaxX - x, y, z);
				std::swap(m_BlockTypes[Idx1], m_BlockTypes[Idx2]);
				NIBBLETYPE Meta1 = BlockHandler(m_BlockTypes[Idx2])->MetaMirrorYZ(m_BlockMetas[Idx1]);
				NIBBLETYPE Meta2 = BlockHandler(m_BlockTypes[Idx1])->MetaMirrorYZ(m_BlockMetas[Idx2]);
				m_BlockMetas[Idx1] = Meta2;
				m_BlockMetas[Idx2] = Meta1;
			}  // for x
		}  // for z
	}  // for y
}





void cBlockArea::RotateCCWNoMeta(void)
{
	if (HasBlockTypes())
	{
		BLOCKTYPE * NewTypes = new BLOCKTYPE[m_SizeX * m_SizeY * m_SizeZ];
		for (int x = 0; x < m_SizeX; x++)
		{
			int NewZ = m_SizeX - x - 1;
			for (int z = 0; z < m_SizeZ; z++)
			{
				int NewX = z;
				for (int y = 0; y < m_SizeY; y++)
				{
					NewTypes[NewX + NewZ * m_SizeX + y * m_SizeX * m_SizeZ] = m_BlockTypes[MakeIndex(x, y, z)];
				}  // for y
			}  // for z
		}  // for x
		std::swap(m_BlockTypes, NewTypes);
		delete[] NewTypes;
	}
	if (HasBlockMetas())
	{
		NIBBLETYPE * NewMetas = new NIBBLETYPE[m_SizeX * m_SizeY * m_SizeZ];
		for (int x = 0; x < m_SizeX; x++)
		{
			int NewZ = m_SizeX - x - 1;
			for (int z = 0; z < m_SizeZ; z++)
			{
				int NewX = z;
				for (int y = 0; y < m_SizeY; y++)
				{
					NewMetas[NewX + NewZ * m_SizeX + y * m_SizeX * m_SizeZ] = m_BlockMetas[MakeIndex(x, y, z)];
				}  // for y
			}  // for z
		}  // for x
		std::swap(m_BlockMetas, NewMetas);
		delete[] NewMetas;
	}
	std::swap(m_SizeX, m_SizeZ);
}





void cBlockArea::RotateCWNoMeta(void)
{
	if (HasBlockTypes())
	{
		BLOCKTYPE * NewTypes = new BLOCKTYPE[m_SizeX * m_SizeY * m_SizeZ];
		for (int z = 0; z < m_SizeZ; z++)
		{
			int NewX = m_SizeZ - z - 1;
			for (int x = 0; x < m_SizeX; x++)
			{
				int NewZ = x;
				for (int y = 0; y < m_SizeY; y++)
				{
					NewTypes[NewX + NewZ * m_SizeX + y * m_SizeX * m_SizeZ] = m_BlockTypes[MakeIndex(x, y, z)];
				}  // for y
			}  // for x
		}  // for z
		std::swap(m_BlockTypes, NewTypes);
		delete[] NewTypes;
	}
	if (HasBlockMetas())
	{
		NIBBLETYPE * NewMetas = new NIBBLETYPE[m_SizeX * m_SizeY * m_SizeZ];
		for (int z = 0; z < m_SizeZ; z++)
		{
			int NewX = m_SizeZ - z - 1;
			for (int x = 0; x < m_SizeX; x++)
			{
				int NewZ = x;
				for (int y = 0; y < m_SizeY; y++)
				{
					NewMetas[NewX + NewZ * m_SizeX + y * m_SizeX * m_SizeZ] = m_BlockMetas[MakeIndex(x, y, z)];
				}  // for y
			}  // for x
		}  // for z
		std::swap(m_BlockMetas, NewMetas);
		delete[] NewMetas;
	}
	std::swap(m_SizeX, m_SizeZ);
}





void cBlockArea::MirrorXYNoMeta(void)
{
	int HalfZ = m_SizeZ / 2;
	int MaxZ = m_SizeZ - 1;
	if (HasBlockTypes())
	{
		for (int y = 0; y < m_SizeY; y++)
		{
			for (int z = 0; z < HalfZ; z++)
			{
				for (int x = 0; x < m_SizeX; x++)
				{
					std::swap(m_BlockTypes[MakeIndex(x, y, z)], m_BlockTypes[MakeIndex(x, y, MaxZ - z)]);
				}  // for x
			}  // for z
		}  // for y
	}  // if (HasBlockTypes)
	
	if (HasBlockMetas())
	{
		for (int y = 0; y < m_SizeY; y++)
		{
			for (int z = 0; z < HalfZ; z++)
			{
				for (int x = 0; x < m_SizeX; x++)
				{
					std::swap(m_BlockMetas[MakeIndex(x, y, z)], m_BlockMetas[MakeIndex(x, y, MaxZ - z)]);
				}  // for x
			}  // for z
		}  // for y
	}  // if (HasBlockMetas)
}





void cBlockArea::MirrorXZNoMeta(void)
{
	int HalfY = m_SizeY / 2;
	int MaxY = m_SizeY - 1;
	if (HasBlockTypes())
	{
		for (int y = 0; y < HalfY; y++)
		{
			for (int z = 0; z < m_SizeZ; z++)
			{
				for (int x = 0; x < m_SizeX; x++)
				{
					std::swap(m_BlockTypes[MakeIndex(x, y, z)], m_BlockTypes[MakeIndex(x, MaxY - y, z)]);
				}  // for x
			}  // for z
		}  // for y
	}  // if (HasBlockTypes)
	
	if (HasBlockMetas())
	{
		for (int y = 0; y < HalfY; y++)
		{
			for (int z = 0; z < m_SizeZ; z++)
			{
				for (int x = 0; x < m_SizeX; x++)
				{
					std::swap(m_BlockMetas[MakeIndex(x, y, z)], m_BlockMetas[MakeIndex(x, MaxY - y, z)]);
				}  // for x
			}  // for z
		}  // for y
	}  // if (HasBlockMetas)
}





void cBlockArea::MirrorYZNoMeta(void)
{
	int HalfX = m_SizeX / 2;
	int MaxX = m_SizeX - 1;
	if (HasBlockTypes())
	{
		for (int y = 0; y < m_SizeY; y++)
		{
			for (int z = 0; z < m_SizeZ; z++)
			{
				for (int x = 0; x < HalfX; x++)
				{
					std::swap(m_BlockTypes[MakeIndex(x, y, z)], m_BlockTypes[MakeIndex(MaxX - x, y, z)]);
				}  // for x
			}  // for z
		}  // for y
	}  // if (HasBlockTypes)
	
	if (HasBlockMetas())
	{
		for (int y = 0; y < m_SizeY; y++)
		{
			for (int z = 0; z < m_SizeZ; z++)
			{
				for (int x = 0; x < HalfX; x++)
				{
					std::swap(m_BlockMetas[MakeIndex(x, y, z)], m_BlockMetas[MakeIndex(MaxX - x, y, z)]);
				}  // for x
			}  // for z
		}  // for y
	}  // if (HasBlockMetas)
}





void cBlockArea::SetRelBlockType(int a_RelX, int a_RelY, int a_RelZ, BLOCKTYPE a_BlockType)
{
	if (m_BlockTypes == NULL)
	{
		LOGWARNING("cBlockArea: BlockTypes have not been read!");
		return;
	}
	m_BlockTypes[MakeIndex(a_RelX, a_RelY, a_RelZ)] = a_BlockType;
}





void cBlockArea::SetBlockType(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType)
{
	SetRelBlockType(a_BlockX - m_OriginX, a_BlockY - m_OriginY, a_BlockZ - m_OriginZ, a_BlockType);
}





void cBlockArea::SetRelBlockMeta(int a_RelX, int a_RelY, int a_RelZ, NIBBLETYPE a_BlockMeta)
{
	SetRelNibble(a_RelX, a_RelY, a_RelZ, a_BlockMeta, m_BlockMetas);
}





void cBlockArea::SetBlockMeta(int a_BlockX, int a_BlockY, int a_BlockZ, NIBBLETYPE a_BlockMeta)
{
	SetNibble(a_BlockX, a_BlockY, a_BlockZ, a_BlockMeta, m_BlockMetas);
}





void cBlockArea::SetRelBlockLight(int a_RelX, int a_RelY, int a_RelZ, NIBBLETYPE a_BlockLight)
{
	SetRelNibble(a_RelX, a_RelY, a_RelZ, a_BlockLight, m_BlockLight);
}





void cBlockArea::SetBlockLight(int a_BlockX, int a_BlockY, int a_BlockZ, NIBBLETYPE a_BlockLight)
{
	SetNibble(a_BlockX, a_BlockY, a_BlockZ, a_BlockLight, m_BlockLight);
}





void cBlockArea::SetRelBlockSkyLight(int a_RelX, int a_RelY, int a_RelZ, NIBBLETYPE a_BlockSkyLight)
{
	SetRelNibble(a_RelX, a_RelY, a_RelZ, a_BlockSkyLight, m_BlockSkyLight);
}





void cBlockArea::SetBlockSkyLight(int a_BlockX, int a_BlockY, int a_BlockZ, NIBBLETYPE a_BlockSkyLight)
{
	SetNibble(a_BlockX, a_BlockY, a_BlockZ, a_BlockSkyLight, m_BlockSkyLight);
}





BLOCKTYPE cBlockArea::GetRelBlockType(int a_RelX, int a_RelY, int a_RelZ) const
{
	if (m_BlockTypes == NULL)
	{
		LOGWARNING("cBlockArea: BlockTypes have not been read!");
		return E_BLOCK_AIR;
	}
	return m_BlockTypes[MakeIndex(a_RelX, a_RelY, a_RelZ)];
}





BLOCKTYPE cBlockArea::GetBlockType(int a_BlockX, int a_BlockY, int a_BlockZ) const
{
	return GetRelBlockType(a_BlockX - m_OriginX, a_BlockY - m_OriginY, a_BlockZ - m_OriginZ);
}





NIBBLETYPE cBlockArea::GetRelBlockMeta(int a_RelX, int a_RelY, int a_RelZ) const
{
	return GetRelNibble(a_RelX, a_RelY, a_RelZ, m_BlockMetas);
}





NIBBLETYPE cBlockArea::GetBlockMeta(int a_BlockX, int a_BlockY, int a_BlockZ) const
{
	return GetNibble(a_BlockX, a_BlockY, a_BlockZ, m_BlockMetas);
}





NIBBLETYPE cBlockArea::GetRelBlockLight(int a_RelX, int a_RelY, int a_RelZ) const
{
	return GetRelNibble(a_RelX, a_RelY, a_RelZ, m_BlockLight);
}





NIBBLETYPE cBlockArea::GetBlockLight(int a_BlockX, int a_BlockY, int a_BlockZ) const
{
	return GetNibble(a_BlockX, a_BlockY, a_BlockZ, m_BlockLight);
}





NIBBLETYPE cBlockArea::GetRelBlockSkyLight(int a_RelX, int a_RelY, int a_RelZ) const
{
	return GetRelNibble(a_RelX, a_RelY, a_RelZ, m_BlockSkyLight);
}





NIBBLETYPE cBlockArea::GetBlockSkyLight(int a_BlockX, int a_BlockY, int a_BlockZ) const
{
	return GetNibble(a_BlockX, a_BlockY, a_BlockZ, m_BlockSkyLight);
}





void cBlockArea::SetBlockTypeMeta(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	SetRelBlockTypeMeta(a_BlockX - m_OriginX, a_BlockY - m_OriginY, a_BlockZ - m_OriginZ, a_BlockType, a_BlockMeta);
}





void cBlockArea::SetRelBlockTypeMeta(int a_RelX,   int a_RelY,   int a_RelZ,   BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	int idx = MakeIndex(a_RelX, a_RelY, a_RelZ);
	if (m_BlockTypes == NULL)
	{
		LOGWARNING("%s: BlockTypes not available but requested to be written to.", __FUNCTION__);
	}
	else
	{
		m_BlockTypes[idx] = a_BlockType;
	}
	if (m_BlockMetas == NULL)
	{
		LOGWARNING("%s: BlockMetas not available but requested to be written to.", __FUNCTION__);
	}
	else
	{
		m_BlockMetas[idx] = a_BlockMeta;
	}
}





void cBlockArea::GetBlockTypeMeta(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta) const
{
	return GetRelBlockTypeMeta(a_BlockX - m_OriginX, a_BlockY - m_OriginY, a_BlockZ - m_OriginZ, a_BlockType, a_BlockMeta);
}





void cBlockArea::GetRelBlockTypeMeta(int a_RelX, int a_RelY, int a_RelZ, BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta) const
{
	int idx = MakeIndex(a_RelX, a_RelY, a_RelZ);
	if (m_BlockTypes == NULL)
	{
		LOGWARNING("cBlockArea: BlockTypes have not been read!");
		a_BlockType = E_BLOCK_AIR;
	}
	else
	{
		a_BlockType = m_BlockTypes[idx];
	}
	
	if (m_BlockMetas == NULL)
	{
		LOGWARNING("cBlockArea: BlockMetas have not been read!");
		a_BlockMeta = 0;
	}
	else
	{
		a_BlockMeta = m_BlockMetas[idx];
	}
}





int cBlockArea::GetDataTypes(void) const
{
	int res = 0;
	if (m_BlockTypes != NULL)
	{
		res |= baTypes;
	}
	if (m_BlockMetas != NULL)
	{
		res |= baMetas;
	}
	if (m_BlockLight != NULL)
	{
		res |= baLight;
	}
	if (m_BlockSkyLight != NULL)
	{
		res |= baSkyLight;
	}
	return res;
}





bool cBlockArea::SetSize(int a_SizeX, int a_SizeY, int a_SizeZ, int a_DataTypes)
{
	ASSERT(m_BlockTypes == NULL);  // Has been cleared
	
	if (a_DataTypes & baTypes)
	{
		m_BlockTypes = new BLOCKTYPE[a_SizeX * a_SizeY * a_SizeZ];
		if (m_BlockTypes == NULL)
		{
			return false;
		}
	}
	if (a_DataTypes & baMetas)
	{
		m_BlockMetas = new NIBBLETYPE[a_SizeX * a_SizeY * a_SizeZ];
		if (m_BlockMetas == NULL)
		{
			delete[] m_BlockTypes;
			return false;
		}
	}
	if (a_DataTypes & baLight)
	{
		m_BlockLight = new NIBBLETYPE[a_SizeX * a_SizeY * a_SizeZ];
		if (m_BlockLight == NULL)
		{
			delete[] m_BlockMetas;
			delete[] m_BlockTypes;
			return false;
		}
	}
	if (a_DataTypes & baSkyLight)
	{
		m_BlockSkyLight = new NIBBLETYPE[a_SizeX * a_SizeY * a_SizeZ];
		if (m_BlockSkyLight == NULL)
		{
			delete[] m_BlockLight;
			delete[] m_BlockMetas;
			delete[] m_BlockTypes;
			return false;
		}
	}
	m_SizeX = a_SizeX;
	m_SizeY = a_SizeY;
	m_SizeZ = a_SizeZ;
	return true;
}





int cBlockArea::MakeIndex(int a_RelX, int a_RelY, int a_RelZ) const
{
	ASSERT(a_RelX >= 0);
	ASSERT(a_RelX < m_SizeX);
	ASSERT(a_RelY >= 0);
	ASSERT(a_RelY < m_SizeY);
	ASSERT(a_RelZ >= 0);
	ASSERT(a_RelZ < m_SizeZ);
	
	return a_RelX + a_RelZ * m_SizeX + a_RelY * m_SizeX * m_SizeZ;
}





void cBlockArea::SetRelNibble(int a_RelX, int a_RelY, int a_RelZ, NIBBLETYPE a_Value, NIBBLETYPE * a_Array)
{
	if (a_Array == NULL)
	{
		LOGWARNING("cBlockArea: datatype has not been read!");
		return;
	}
	a_Array[MakeIndex(a_RelX, a_RelY, a_RelZ)] = a_Value;
}





void cBlockArea::SetNibble(int a_BlockX, int a_BlockY, int a_BlockZ, NIBBLETYPE a_Value, NIBBLETYPE * a_Array)
{
	SetRelNibble(a_BlockX - m_OriginX, a_BlockY - m_OriginY, a_BlockZ - m_OriginZ, a_Value, a_Array);
}





NIBBLETYPE cBlockArea::GetRelNibble(int a_RelX, int a_RelY, int a_RelZ, NIBBLETYPE * a_Array) const
{
	if (a_Array == NULL)
	{
		LOGWARNING("cBlockArea: datatype has not been read!");
		return 16;
	}
	return a_Array[MakeIndex(a_RelX, a_RelY, a_RelZ)];
}





NIBBLETYPE cBlockArea::GetNibble(int a_BlockX, int a_BlockY, int a_BlockZ, NIBBLETYPE * a_Array) const
{
	return GetRelNibble(a_BlockX - m_OriginX, a_BlockY - m_OriginY, a_BlockZ - m_OriginZ, a_Array);
}






///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cBlockArea::cChunkReader:

cBlockArea::cChunkReader::cChunkReader(cBlockArea & a_Area) :
	m_Area(a_Area),
	m_OriginX(a_Area.m_OriginX),
	m_OriginY(a_Area.m_OriginY),
	m_OriginZ(a_Area.m_OriginZ)
{
}





void cBlockArea::cChunkReader::CopyNibbles(NIBBLETYPE * a_AreaDst, const NIBBLETYPE * a_ChunkSrc)
{
	int SizeY = m_Area.m_SizeY;
	int MinY = m_OriginY;
	
	// SizeX, SizeZ are the dmensions of the block data to copy from the current chunk (size of the geometric union)
	// OffX, OffZ are the offsets of the current chunk data from the area origin
	// BaseX, BaseZ are the offsets of the area data within the current chunk from the chunk borders
	int SizeX = cChunkDef::Width;
	int SizeZ = cChunkDef::Width;
	int OffX, OffZ;
	int BaseX, BaseZ;
	OffX = m_CurrentChunkX * cChunkDef::Width - m_OriginX;
	if (OffX < 0)
	{
		BaseX = -OffX;
		SizeX += OffX;  // SizeX is decreased, OffX is negative
		OffX = 0;
	}
	else
	{
		BaseX = 0;
	}
	OffZ = m_CurrentChunkZ * cChunkDef::Width - m_OriginZ;
	if (OffZ < 0)
	{
		BaseZ = -OffZ;
		SizeZ += OffZ;  // SizeZ is decreased, OffZ is negative
		OffZ = 0;
	}
	else
	{
		BaseZ = 0;
	}
	// If the chunk extends beyond the area in the X or Z axis, cut off the Size:
	if ((m_CurrentChunkX + 1) * cChunkDef::Width > m_OriginX + m_Area.m_SizeX)
	{
		SizeX -= (m_CurrentChunkX + 1) * cChunkDef::Width - (m_OriginX + m_Area.m_SizeX);
	}
	if ((m_CurrentChunkZ + 1) * cChunkDef::Width > m_OriginZ + m_Area.m_SizeZ)
	{
		SizeZ -= (m_CurrentChunkZ + 1) * cChunkDef::Width - (m_OriginZ + m_Area.m_SizeZ);
	}

	for (int y = 0; y < SizeY; y++)
	{
		int ChunkY = MinY + y;
		int AreaY = y;
		for (int z = 0; z < SizeZ; z++)
		{
			int ChunkZ = BaseZ + z;
			int AreaZ = OffZ + z;
			for (int x = 0; x < SizeX; x++)
			{
				int ChunkX = BaseX + x;
				int AreaX = OffX + x;
				a_AreaDst[m_Area.MakeIndex(AreaX, AreaY, AreaZ)] = cChunkDef::GetNibble(a_ChunkSrc, ChunkX, ChunkY, ChunkZ);
			}  // for x
		}  // for z
	}  // for y
}





bool cBlockArea::cChunkReader::Coords(int a_ChunkX, int a_ChunkZ)
{
	m_CurrentChunkX = a_ChunkX;
	m_CurrentChunkZ = a_ChunkZ;
	return true;
}





void cBlockArea::cChunkReader::BlockTypes(const BLOCKTYPE * a_BlockTypes)
{
	if (m_Area.m_BlockTypes == NULL)
	{
		// Don't want BlockTypes
		return;
	}
	
	int SizeY = m_Area.m_SizeY;
	int MinY = m_OriginY;
	
	// SizeX, SizeZ are the dmensions of the block data to copy from the current chunk (size of the geometric union)
	// OffX, OffZ are the offsets of the current chunk data from the area origin
	// BaseX, BaseZ are the offsets of the area data within the current chunk from the chunk borders
	int SizeX = cChunkDef::Width;
	int SizeZ = cChunkDef::Width;
	int OffX, OffZ;
	int BaseX, BaseZ;
	OffX = m_CurrentChunkX * cChunkDef::Width - m_OriginX;
	if (OffX < 0)
	{
		BaseX = -OffX;
		SizeX += OffX;  // SizeX is decreased, OffX is negative
		OffX = 0;
	}
	else
	{
		BaseX = 0;
	}
	OffZ = m_CurrentChunkZ * cChunkDef::Width - m_OriginZ;
	if (OffZ < 0)
	{
		BaseZ = -OffZ;
		SizeZ += OffZ;  // SizeZ is decreased, OffZ is negative
		OffZ = 0;
	}
	else
	{
		BaseZ = 0;
	}
	// If the chunk extends beyond the area in the X or Z axis, cut off the Size:
	if ((m_CurrentChunkX + 1) * cChunkDef::Width > m_OriginX + m_Area.m_SizeX)
	{
		SizeX -= (m_CurrentChunkX + 1) * cChunkDef::Width - (m_OriginX + m_Area.m_SizeX);
	}
	if ((m_CurrentChunkZ + 1) * cChunkDef::Width > m_OriginZ + m_Area.m_SizeZ)
	{
		SizeZ -= (m_CurrentChunkZ + 1) * cChunkDef::Width - (m_OriginZ + m_Area.m_SizeZ);
	}

	for (int y = 0; y < SizeY; y++)
	{
		int ChunkY = MinY + y;
		int AreaY = y;
		for (int z = 0; z < SizeZ; z++)
		{
			int ChunkZ = BaseZ + z;
			int AreaZ = OffZ + z;
			for (int x = 0; x < SizeX; x++)
			{
				int ChunkX = BaseX + x;
				int AreaX = OffX + x;
				m_Area.m_BlockTypes[m_Area.MakeIndex(AreaX, AreaY, AreaZ)] = cChunkDef::GetBlock(a_BlockTypes, ChunkX, ChunkY, ChunkZ);
			}  // for x
		}  // for z
	}  // for y
}





void cBlockArea::cChunkReader::BlockMeta(const NIBBLETYPE * a_BlockMetas)
{
	if (m_Area.m_BlockMetas == NULL)
	{
		// Don't want metas
		return;
	}
	CopyNibbles(m_Area.m_BlockMetas, a_BlockMetas);
}





void cBlockArea::cChunkReader::BlockLight(const NIBBLETYPE * a_BlockLight)
{
	if (m_Area.m_BlockLight == NULL)
	{
		// Don't want light
		return;
	}
	CopyNibbles(m_Area.m_BlockLight, a_BlockLight);
}





void cBlockArea::cChunkReader::BlockSkyLight(const NIBBLETYPE * a_BlockSkyLight)
{
	if (m_Area.m_BlockSkyLight == NULL)
	{
		// Don't want skylight
		return;
	}
	CopyNibbles(m_Area.m_BlockSkyLight, a_BlockSkyLight);
}





void cBlockArea::CropBlockTypes(int a_AddMinX, int a_SubMaxX, int a_AddMinY, int a_SubMaxY, int a_AddMinZ, int a_SubMaxZ)
{
	int NewSizeX = GetSizeX() - a_AddMinX - a_SubMaxX;
	int NewSizeY = GetSizeY() - a_AddMinY - a_SubMaxY;
	int NewSizeZ = GetSizeZ() - a_AddMinZ - a_SubMaxZ;
	BLOCKTYPE * NewBlockTypes = new BLOCKTYPE[NewSizeX * NewSizeY * NewSizeZ];
	int idx = 0;
	for (int y = 0; y < NewSizeY; y++)
	{
		for (int z = 0; z < NewSizeZ; z++)
		{
			for (int x = 0; x < NewSizeX; x++)
			{
				int OldIndex = MakeIndex(x + a_AddMinX, y + a_AddMinY, z + a_AddMinZ);
				NewBlockTypes[idx++] = m_BlockTypes[OldIndex];
			}  // for x
		}  // for z
	}  // for y
	delete m_BlockTypes;
	m_BlockTypes = NewBlockTypes;
}





void cBlockArea::CropNibbles(NIBBLEARRAY & a_Array, int a_AddMinX, int a_SubMaxX, int a_AddMinY, int a_SubMaxY, int a_AddMinZ, int a_SubMaxZ)
{
	int NewSizeX = GetSizeX() - a_AddMinX - a_SubMaxX;
	int NewSizeY = GetSizeY() - a_AddMinY - a_SubMaxY;
	int NewSizeZ = GetSizeZ() - a_AddMinZ - a_SubMaxZ;
	NIBBLETYPE * NewNibbles = new NIBBLETYPE[NewSizeX * NewSizeY * NewSizeZ];
	int idx = 0;
	for (int y = 0; y < NewSizeY; y++)
	{
		for (int z = 0; z < NewSizeZ; z++)
		{
			for (int x = 0; x < NewSizeX; x++)
			{
				NewNibbles[idx++] = a_Array[MakeIndex(x + a_AddMinX, y + a_AddMinY, z + a_AddMinZ)];
			}  // for x
		}  // for z
	}  // for y
	delete a_Array;
	a_Array = NewNibbles;
}





void cBlockArea::ExpandBlockTypes(int a_SubMinX, int a_AddMaxX, int a_SubMinY, int a_AddMaxY, int a_SubMinZ, int a_AddMaxZ)
{
	int NewSizeX = m_SizeX + a_SubMinX + a_AddMaxX;
	int NewSizeY = m_SizeY + a_SubMinY + a_AddMaxY;
	int NewSizeZ = m_SizeZ + a_SubMinZ + a_AddMaxZ;
	int BlockCount = NewSizeX * NewSizeY * NewSizeZ;
	BLOCKTYPE * NewBlockTypes = new BLOCKTYPE[BlockCount];
	memset(NewBlockTypes, 0, BlockCount * sizeof(BLOCKTYPE));
	int OldIndex = 0;
	for (int y = 0; y < m_SizeY; y++)
	{
		int IndexBaseY = (y + a_SubMinY) * m_SizeX * m_SizeZ;
		for (int z = 0; z < m_SizeZ; z++)
		{
			int IndexBaseZ = IndexBaseY + (z + a_SubMinZ) * m_SizeX;
			int idx = IndexBaseZ + a_SubMinX;
			for (int x = 0; x < m_SizeX; x++)
			{
				NewBlockTypes[idx++] = m_BlockTypes[OldIndex++];
			}  // for x
		}  // for z
	}  // for y
	delete m_BlockTypes;
	m_BlockTypes = NewBlockTypes;
}





void cBlockArea::ExpandNibbles(NIBBLEARRAY & a_Array, int a_SubMinX, int a_AddMaxX, int a_SubMinY, int a_AddMaxY, int a_SubMinZ, int a_AddMaxZ)
{
	int NewSizeX = m_SizeX + a_SubMinX + a_AddMaxX;
	int NewSizeY = m_SizeY + a_SubMinY + a_AddMaxY;
	int NewSizeZ = m_SizeZ + a_SubMinZ + a_AddMaxZ;
	int BlockCount = NewSizeX * NewSizeY * NewSizeZ;
	NIBBLETYPE * NewNibbles = new NIBBLETYPE[BlockCount];
	memset(NewNibbles, 0, BlockCount * sizeof(NIBBLETYPE));
	int OldIndex = 0;
	for (int y = 0; y < m_SizeY; y++)
	{
		int IndexBaseY = (y + a_SubMinY) * m_SizeX * m_SizeZ;
		for (int z = 0; z < m_SizeZ; z++)
		{
			int IndexBaseZ = IndexBaseY + (z + a_SubMinZ) * m_SizeX;
			int idx = IndexBaseZ + a_SubMinX;
			for (int x = 0; x < m_SizeX; x++)
			{
				NewNibbles[idx++] = a_Array[OldIndex++];
			}  // for x
		}  // for z
	}  // for y
	delete a_Array;
	a_Array = NewNibbles;
}





bool cBlockArea::LoadFromSchematicNBT(cParsedNBT & a_NBT)
{
	int TMaterials = a_NBT.FindChildByName(a_NBT.GetRoot(), "Materials");
	if ((TMaterials > 0) && (a_NBT.GetType(TMaterials) == TAG_String))
	{
		AString Materials = a_NBT.GetString(TMaterials);
		if (Materials.compare("Alpha") != 0)
		{
			LOG("Materials tag is present and \"%s\" instead of \"Alpha\". Possibly a wrong-format schematic file.", Materials.c_str());
			return false;
		}
	}
	int TSizeX = a_NBT.FindChildByName(a_NBT.GetRoot(), "Width");
	int TSizeY = a_NBT.FindChildByName(a_NBT.GetRoot(), "Height");
	int TSizeZ = a_NBT.FindChildByName(a_NBT.GetRoot(), "Length");
	if (
		(TSizeX < 0) || (TSizeY < 0) || (TSizeZ < 0) ||
		(a_NBT.GetType(TSizeX) != TAG_Short) ||
		(a_NBT.GetType(TSizeY) != TAG_Short) ||
		(a_NBT.GetType(TSizeZ) != TAG_Short)
	)
	{
		LOG("Dimensions are missing from the schematic file (%d, %d, %d), (%d, %d, %d)",
			TSizeX, TSizeY, TSizeZ,
			a_NBT.GetType(TSizeX), a_NBT.GetType(TSizeY), a_NBT.GetType(TSizeZ)
		);
		return false;
	}
	
	int SizeX = a_NBT.GetShort(TSizeX);
	int SizeY = a_NBT.GetShort(TSizeY);
	int SizeZ = a_NBT.GetShort(TSizeZ);
	if ((SizeX < 1) || (SizeY < 1) || (SizeZ < 1))
	{
		LOG("Dimensions are invalid in the schematic file: %d, %d, %d", SizeX, SizeY, SizeZ);
		return false;
	}
	
	int TBlockTypes = a_NBT.FindChildByName(a_NBT.GetRoot(), "Blocks");
	int TBlockMetas = a_NBT.FindChildByName(a_NBT.GetRoot(), "Data");
	if ((TBlockTypes < 0) || (a_NBT.GetType(TBlockTypes) != TAG_ByteArray))
	{
		LOG("BlockTypes are invalid in the schematic file: %d", TBlockTypes);
		return false;
	}
	bool AreMetasPresent = (TBlockMetas > 0) && (a_NBT.GetType(TBlockMetas) == TAG_ByteArray);
	
	Clear();
	SetSize(SizeX, SizeY, SizeZ, AreMetasPresent ? (baTypes | baMetas) : baTypes);
	
	// Copy the block types and metas:
	int NumBytes = m_SizeX * m_SizeY * m_SizeZ;
	if (a_NBT.GetDataLength(TBlockTypes) < NumBytes)
	{
		LOG("BlockTypes truncated in the schematic file (exp %d, got %d bytes). Loading partial.",
			NumBytes, a_NBT.GetDataLength(TBlockTypes)
		);
		NumBytes = a_NBT.GetDataLength(TBlockTypes);
	}
	memcpy(m_BlockTypes, a_NBT.GetData(TBlockTypes), NumBytes);
	
	if (AreMetasPresent)
	{
		int NumBytes = m_SizeX * m_SizeY * m_SizeZ;
		if (a_NBT.GetDataLength(TBlockMetas) < NumBytes)
		{
			LOG("BlockMetas truncated in the schematic file (exp %d, got %d bytes). Loading partial.",
				NumBytes, a_NBT.GetDataLength(TBlockMetas)
			);
			NumBytes = a_NBT.GetDataLength(TBlockMetas);
		}
		memcpy(m_BlockMetas, a_NBT.GetData(TBlockMetas), NumBytes);
	}
	
	return true;
}




void cBlockArea::RelSetData(
	int a_RelX, int a_RelY, int a_RelZ,
	int a_DataTypes, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta,
	NIBBLETYPE a_BlockLight, NIBBLETYPE a_BlockSkyLight
)
{
	int Index = MakeIndex(a_RelX, a_RelY, a_RelZ);
	if ((a_DataTypes & baTypes) != 0)
	{
		m_BlockTypes[Index] = a_BlockType;
	}
	if ((a_DataTypes & baMetas) != 0)
	{
		m_BlockMetas[Index] = a_BlockMeta;
	}
	if ((a_DataTypes & baLight) != 0)
	{
		m_BlockLight[Index] = a_BlockLight;
	}
	if ((a_DataTypes & baSkyLight) != 0)
	{
		m_BlockSkyLight[Index] = a_BlockSkyLight;
	}
}




