
// BlockArea.cpp

// Implements the cBlockArea object representing an area of block data that can be queried from cWorld and then accessed again without further queries
// The object also supports writing the blockdata back into cWorld, even into other coords

#include "Globals.h"
#include "BlockArea.h"
#include "World.h"
#include "OSSupport/GZipFile.h"
#include "WorldStorage/FastNBT.h"





cBlockArea::cBlockArea(void) :
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
	m_SizeX = 0;
	m_SizeY = 0;
	m_SizeZ = 0;
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
		LOGWARNING("cBlockArea:Read(): MinBlockY less than zero, adjusting to zero");
		a_MinBlockY = 0;
	}
	else if (a_MinBlockY >= cChunkDef::Height)
	{
		LOGWARNING("cBlockArea::Read(): MinBlockY more than chunk height, adjusting to chunk height");
		a_MinBlockY = cChunkDef::Height - 1;
	}
	if (a_MaxBlockY < 0)
	{
		LOGWARNING("cBlockArea:Read(): MaxBlockY less than zero, adjusting to zero");
		a_MaxBlockY = 0;
	}
	else if (a_MinBlockY >= cChunkDef::Height)
	{
		LOGWARNING("cBlockArea::Read(): MaxBlockY more than chunk height, adjusting to chunk height");
		a_MaxBlockY = cChunkDef::Height - 1;
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
		LOGWARNING("cBlockArea:Read(): MinBlockY less than zero, adjusting to zero");
		a_MinBlockY = 0;
	}
	else if (a_MinBlockY >= cChunkDef::Height - m_SizeY)
	{
		LOGWARNING("cBlockArea::Read(): MinBlockY + m_SizeY more than chunk height, adjusting to chunk height");
		a_MinBlockY = cChunkDef::Height - m_SizeY - 1;
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
	Writer.Finish();
	
	// TODO: Save to file
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




