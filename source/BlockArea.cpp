
// BlockArea.cpp

// Implements the cBlockArea object representing an area of block data that can be queried from cWorld and then accessed again without further queries
// The object also supports writing the blockdata back into cWorld, even into other coords

#include "Globals.h"
#include "BlockArea.h"
#include "World.h"





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
	return a_RelX + a_RelZ * m_SizeZ + a_RelY * m_SizeX * m_SizeZ;
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




