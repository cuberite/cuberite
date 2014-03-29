
// Prefab.cpp

/*
Implements the cPrefab class, representing a cPiece descendant for the cPieceGenerator that
uses a prefabricate in a cBlockArea for drawing itself.
*/

#include "Globals.h"
#include "Prefab.h"
#include "../WorldStorage/SchematicFileSerializer.h"
#include "ChunkDesc.h"





#ifdef SELF_TEST

// Create one static prefab to test the parser:
static const cPrefab::sDef g_TestPrefabDef =
{
	// Size:
	7, 6, 7,  // SizeX = 7, SizeY = 6, SizeZ = 7

	// Block definitions:
	".:  0: 0\n"  /* 0 */
	"a:112: 0\n"  /* netherbrick */
	"b:113: 0\n"  /* netherbrickfence */,

	// Block data:
	// Level 1
	"aaaaaaa"
	"aaaaaaa"
	"aaaaaaa"
	"aaaaaaa"
	"aaaaaaa"
	"aaaaaaa"
	"aaaaaaa"

	// Level 2
	"aa...aa"
	"a.....a"
	"......."
	"......."
	"......."
	"a.....a"
	"aa...aa"

	// Level 3
	"aa...aa"
	"a.....a"
	"......."
	"......."
	"......."
	"a.....a"
	"aa...aa"

	// Level 4
	"aa...aa"
	"a.....a"
	"......."
	"......."
	"......."
	"a.....a"
	"aa...aa"

	// Level 5
	"aabbbaa"
	"a.....a"
	"b.....b"
	"b.....b"
	"b.....b"
	"a.....a"
	"aabbbaa"

	// Level 6
	"aaaaaaa"
	"a.....a"
	"a.....a"
	"a.....a"
	"a.....a"
	"a.....a"
	"aaaaaaa",
	
	// Connections:
	"0: 0, 3, 2: 4\n"
	"0: 2, 3, 0: 2\n",
	
	// AllowedRotations:
	7,  /* 1, 2, 3 CCW rotations */
	
	// Merge strategy:
	cBlockArea::msImprint
};

static cPrefab g_TestPrefab(g_TestPrefabDef);
#endif





cPrefab::cPrefab(const cPrefab::sDef & a_Def) :
	m_Size(a_Def.m_SizeX, a_Def.m_SizeY, a_Def.m_SizeZ),
	m_HitBox(0, 0, 0, a_Def.m_SizeX - 1, a_Def.m_SizeY - 1, a_Def.m_SizeZ - 1),
	m_AllowedRotations(a_Def.m_AllowedRotations),
	m_MergeStrategy(a_Def.m_MergeStrategy)
{
	m_BlockArea[0].Create(m_Size);
	CharMap cm;
	ParseCharMap(cm, a_Def.m_CharMap);
	ParseBlockImage(cm, a_Def.m_Image);
	ParseConnectors(a_Def.m_Connectors);
	
	// 1 CCW rotation:
	if ((m_AllowedRotations & 0x01) != 0)
	{
		m_BlockArea[1].CopyFrom(m_BlockArea[0]);
		m_BlockArea[1].RotateCCW();
	}
	
	// 2 rotations are the same as mirroring twice; mirroring is faster because it has no reallocations
	if ((m_AllowedRotations & 0x02) != 0)
	{
		m_BlockArea[2].CopyFrom(m_BlockArea[0]);
		m_BlockArea[2].MirrorXY();
		m_BlockArea[2].MirrorYZ();
	}	
	
	// 3 CCW rotations = 1 CW rotation:
	if ((m_AllowedRotations & 0x04) != 0)
	{
		m_BlockArea[3].CopyFrom(m_BlockArea[0]);
		m_BlockArea[3].RotateCW();
	}
}





void cPrefab::Draw(cChunkDesc & a_Dest, const cPlacedPiece * a_Placement) const
{
	Vector3i Placement = a_Placement->GetCoords();
	int ChunkStartX = a_Dest.GetChunkX() * cChunkDef::Width;
	int ChunkStartZ = a_Dest.GetChunkZ() * cChunkDef::Width;
	Placement.Move(-ChunkStartX, 0, -ChunkStartZ);
	a_Dest.WriteBlockArea(m_BlockArea[a_Placement->GetNumCCWRotations()], Placement.x, Placement.y, Placement.z, m_MergeStrategy);
	
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
		a_CharMapOut[i].m_BlockType = 0;
		a_CharMapOut[i].m_BlockMeta = 16;  // Mark unassigned entries with a meta that is impossible otherwise
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
		ASSERT(a_CharMapOut[Src].m_BlockMeta == 16);  // This letter has not been assigned yet?
		a_CharMapOut[Src].m_BlockType = (BLOCKTYPE)atoi(CharDef[1].c_str());
		NIBBLETYPE BlockMeta = 0;
		if ((NumElements >= 3) && !CharDef[2].empty())
		{
			BlockMeta = (NIBBLETYPE)atoi(CharDef[2].c_str());
			ASSERT((BlockMeta >= 0) && (BlockMeta <= 15));
		}
		a_CharMapOut[Src].m_BlockMeta = BlockMeta;
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
				const sBlockTypeDef & MappedValue = a_CharMap[BlockImage[x]];
				ASSERT(MappedValue.m_BlockMeta != 16);  // Using a letter not defined in the CharMap?
				m_BlockArea[0].SetRelBlockTypeMeta(x, y, z, MappedValue.m_BlockType, MappedValue.m_BlockMeta);
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
	// Either zero rotations
	// Or the proper bit in m_AllowedRotations is set
	return (a_NumRotations == 0) || ((m_AllowedRotations & (1 << ((a_NumRotations + 3) % 4))) != 0);
}




