
// Prefab.cpp

/*
Implements the cPrefab class, representing a cPiece descendant for the cPieceGenerator that
uses a prefabricate in a cBlockArea for drawing itself.
*/

#include "Globals.h"
#include "Prefab.h"
#include "../WorldStorage/SchematicFileSerializer.h"
#include "ChunkDesc.h"





cPrefab::cPrefab(const cPrefab::sDef & a_Def) :
	m_Size(a_Def.m_SizeX, a_Def.m_SizeY, a_Def.m_SizeZ),
	m_HitBox(0, 0, 0, a_Def.m_SizeX - 1, a_Def.m_SizeY - 1, a_Def.m_SizeZ - 1),
	m_AllowedRotations(a_Def.m_AllowedRotations),
	m_MergeStrategy(a_Def.m_MergeStrategy)
{
	m_BlockArea.Create(m_Size);
	CharMap cm;
	ParseCharMap(cm, a_Def.m_CharMap);
	ParseBlockImage(cm, a_Def.m_Image);
	ParseConnectors(a_Def.m_Connectors);
}





void cPrefab::Draw(cChunkDesc & a_Dest, const cPlacedPiece * a_Placement) const
{
	Vector3i Placement = a_Placement->GetCoords();
	int ChunkStartX = a_Dest.GetChunkX() * cChunkDef::Width;
	int ChunkStartZ = a_Dest.GetChunkZ() * cChunkDef::Width;
	Placement.Move(-ChunkStartX, 0, -ChunkStartZ);
	a_Dest.WriteBlockArea(m_BlockArea, Placement.x, Placement.y, Placement.z, m_MergeStrategy);
	
}





bool cPrefab::HasConnectorType(int a_ConnectorType) const
{
	for (cConnectors::const_iterator itr = m_Connectors.begin(), end = m_Connectors.end(); itr != end; ++itr)
	{
		if (itr->m_Type == a_ConnectorType)
		{
			return true;
		}
	}  // for itr - m_Connectors[]
	return false;
}





void cPrefab::ParseCharMap(CharMap & a_CharMapOut, const char * a_CharMapDef)
{
	// Initialize the charmap to all-invalid values:
	for (size_t i = 0; i < ARRAYCOUNT(a_CharMapOut); i++)
	{
		a_CharMapOut[i] = -1;
	}
	
	// Process the lines in the definition:
	AStringVector Lines = StringSplitAndTrim(a_CharMapDef, "\n");
	for (AStringVector::const_iterator itr = Lines.begin(), end = Lines.end(); itr != end; ++itr)
	{
		AStringVector CharDef = StringSplitAndTrim(*itr, ":");
		size_t NumElements = CharDef.size();
		if ((NumElements < 2) || CharDef[0].empty() || CharDef[1].empty())
		{
			LOGWARNING("Bad prefab CharMap definition line: \"%s\", skipping.", itr->c_str());
			continue;
		}
		unsigned char Src = (unsigned char)CharDef[0][0];
		BLOCKTYPE BlockType = (BLOCKTYPE)atoi(CharDef[1].c_str());
		NIBBLETYPE BlockMeta = 0;
		if ((NumElements >= 3) && !CharDef[2].empty())
		{
			BlockMeta = (NIBBLETYPE)atoi(CharDef[2].c_str());
			ASSERT((BlockMeta >= 0) && (BlockMeta <= 15));
		}
		ASSERT(a_CharMapOut[Src] == -1);  // Any duplicates letter-wise?
		a_CharMapOut[Src] = (BlockType << 4) | BlockMeta;
	}  // for itr - Lines[]
}





void cPrefab::ParseBlockImage(const CharMap & a_CharMap, const char * a_BlockImage)
{
	// Map each letter in the a_BlockImage (from the in-source definition) to real blocktype / blockmeta:
	for (int y = 0; y < m_Size.y; y++)
	{
		for (int z = 0; z < m_Size.z; z++)
		{
			const unsigned char * BlockImage = (const unsigned char *)a_BlockImage + y * m_Size.x * m_Size.z + z * m_Size.x;
			for (int x = 0; x < m_Size.x; x++)
			{
				int MappedValue = a_CharMap[BlockImage[x]];
				ASSERT(MappedValue != -1);  // Using a letter not defined in the CharMap?
				BLOCKTYPE BlockType = MappedValue >> 4;
				NIBBLETYPE BlockMeta = MappedValue & 0x0f;
				m_BlockArea.SetRelBlockTypeMeta(x, y, z, BlockType, BlockMeta);
			}
		}
	}
}





void cPrefab::ParseConnectors(const char * a_ConnectorsDef)
{
	AStringVector Lines = StringSplitAndTrim(a_ConnectorsDef, "\n");
	for (AStringVector::const_iterator itr = Lines.begin(), end = Lines.end(); itr != end; ++itr)
	{
		if (itr->empty())
		{
			continue;
		}
		// Split into components: "Type: X, Y, Z: Face":
		AStringVector Defs = StringSplitAndTrim(*itr, ":");
		if (Defs.size() != 3)
		{
			LOGWARNING("Bad prefab Connector definition line: \"%s\", skipping.", itr->c_str());
			continue;
		}
		AStringVector Coords = StringSplitAndTrim(Defs[1], ",");
		if (Coords.size() != 3)
		{
			LOGWARNING("Bad prefab Connector coords definition: \"%s\", skipping.", Defs[1].c_str());
			continue;
		}
		
		// Check that the BlockFace is within range:
		int BlockFace = atoi(Defs[2].c_str());
		if ((BlockFace < 0) || (BlockFace >= 6))
		{
			LOGWARNING("Bad prefab Connector Blockface: \"%s\", skipping.", Defs[2].c_str());
			continue;
		}

		// Add the connector:
		m_Connectors.push_back(cPiece::cConnector(
			atoi(Coords[0].c_str()), atoi(Coords[1].c_str()), atoi(Coords[2].c_str()),  // Connector pos
			atoi(Defs[0].c_str()),  // Connector type
			(eBlockFace)BlockFace
		));
	}  // for itr - Lines[]
}





cPiece::cConnectors cPrefab::GetConnectors(void) const
{
	return m_Connectors;
}





Vector3i cPrefab::GetSize(void) const
{
	return m_Size;
}





cCuboid cPrefab::GetHitBox(void) const
{
	return m_HitBox;
}





bool cPrefab::CanRotateCCW(int a_NumRotations) const
{
	return ((m_AllowedRotations & (1 << (a_NumRotations % 4))) != 0);
}




