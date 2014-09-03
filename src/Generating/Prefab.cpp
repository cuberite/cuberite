
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

	// Hitbox (relative to bounding box):
	0, 0, 0,  // MinX, MinY, MinZ
	6, 5, 6,  // MaxX, MaxY, MaxZ
	
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
	cBlockArea::msImprint,
	
	// ShouldExtendFloor:
	false,

	// DefaultWeight:
	10,

	// DepthWeight:
	"",

	// AddWeightIfSame:
	1000,
	
	// MoveToGround:
	false,
};

static cPrefab g_TestPrefab(g_TestPrefabDef);
#endif





cPrefab::cPrefab(const cPrefab::sDef & a_Def) :
	m_Size(a_Def.m_SizeX, a_Def.m_SizeY, a_Def.m_SizeZ),
	m_HitBox(
		a_Def.m_HitboxMinX, a_Def.m_HitboxMinY, a_Def.m_HitboxMinZ,
		a_Def.m_HitboxMaxX, a_Def.m_HitboxMaxY, a_Def.m_HitboxMaxZ
	),
	m_AllowedRotations(a_Def.m_AllowedRotations),
	m_MergeStrategy(a_Def.m_MergeStrategy),
	m_ShouldExtendFloor(a_Def.m_ShouldExtendFloor),
	m_DefaultWeight(a_Def.m_DefaultWeight),
	m_AddWeightIfSame(a_Def.m_AddWeightIfSame),
	m_MoveToGround(a_Def.m_MoveToGround)
{
	m_BlockArea[0].Create(m_Size);
	CharMap cm;
	ParseCharMap(cm, a_Def.m_CharMap);
	ParseBlockImage(cm, a_Def.m_Image);
	ParseConnectors(a_Def.m_Connectors);
	ParseDepthWeight(a_Def.m_DepthWeight);
	
	AddRotatedBlockAreas();
}





cPrefab::cPrefab(const cBlockArea & a_Image, int a_AllowedRotations) :
	m_Size(a_Image.GetSize()),
	m_AllowedRotations(a_AllowedRotations),
	m_MergeStrategy(cBlockArea::msOverwrite),
	m_ShouldExtendFloor(false),
	m_DefaultWeight(1),
	m_AddWeightIfSame(0),
	m_MoveToGround(false)
{
	m_HitBox.p1.Set(0, 0, 0);
	m_HitBox.p2.Set(m_Size.x - 1, m_Size.y - 1, m_Size.z - 1);
	m_BlockArea[0].CopyFrom(a_Image);
	AddRotatedBlockAreas();
}





void cPrefab::AddRotatedBlockAreas(void)
{
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
	Draw(a_Dest, a_Placement->GetCoords(), a_Placement->GetNumCCWRotations());
}




void cPrefab::Draw(cChunkDesc & a_Dest, const Vector3i & a_Placement, int a_NumRotations) const
{
	// Draw the basic image:
	Vector3i Placement(a_Placement);
	int ChunkStartX = a_Dest.GetChunkX() * cChunkDef::Width;
	int ChunkStartZ = a_Dest.GetChunkZ() * cChunkDef::Width;
	Placement.Move(-ChunkStartX, 0, -ChunkStartZ);
	const cBlockArea & Image = m_BlockArea[a_NumRotations];
	
	// If the placement is outside this chunk, bail out:
	if (
		(Placement.x > cChunkDef::Width) || (Placement.x + Image.GetSizeX() < 0) ||
		(Placement.z > cChunkDef::Width) || (Placement.z + Image.GetSizeZ() < 0)
	)
	{
		return;
	}
	
	// Write the image:
	a_Dest.WriteBlockArea(Image, Placement.x, Placement.y, Placement.z, m_MergeStrategy);
	
	// If requested, draw the floor (from the bottom of the prefab down to the nearest non-air)
	if (m_ShouldExtendFloor)
	{
		int MaxX = Image.GetSizeX();
		int MaxZ = Image.GetSizeZ();
		for (int z = 0; z < MaxZ; z++)
		{
			int RelZ = Placement.z + z;
			if ((RelZ < 0) || (RelZ >= cChunkDef::Width))
			{
				// Z coord outside the chunk
				continue;
			}
			for (int x = 0; x < MaxX; x++)
			{
				int RelX = Placement.x + x;
				if ((RelX < 0) || (RelX >= cChunkDef::Width))
				{
					// X coord outside the chunk
					continue;
				}
				BLOCKTYPE BlockType;
				NIBBLETYPE BlockMeta;
				Image.GetRelBlockTypeMeta(x, 0, z, BlockType, BlockMeta);
				if ((BlockType == E_BLOCK_AIR) || (BlockType == E_BLOCK_SPONGE))
				{
					// Do not expand air nor sponge blocks
					continue;
				}
				for (int y = Placement.y - 1; y >= 0; y--)
				{
					BLOCKTYPE ExistingBlock = a_Dest.GetBlockType(RelX, y, RelZ);
					if (ExistingBlock != E_BLOCK_AIR)
					{
						// End the expansion for this column, reached the end
						break;
					}
					a_Dest.SetBlockTypeMeta(RelX, y, RelZ, BlockType, BlockMeta);
				}  // for y
			}  // for x
		}  // for z
	}
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





int cPrefab::GetPieceWeight(const cPlacedPiece & a_PlacedPiece, const cPiece::cConnector & a_ExistingConnector) const
{
	// Use the default or per-depth weight:
	cDepthWeight::const_iterator itr = m_DepthWeight.find(a_PlacedPiece.GetDepth() + 1);
	int res = (itr == m_DepthWeight.end()) ? m_DefaultWeight : itr->second;
	
	// If the piece is the same as the parent, apply the m_AddWeightIfSame modifier:
	const cPiece * ParentPiece = &a_PlacedPiece.GetPiece();
	const cPiece * ThisPiece = this;
	if (ThisPiece == ParentPiece)
	{
		res += m_AddWeightIfSame;
	}
	return res;
}





void cPrefab::SetDefaultWeight(int a_DefaultWeight)
{
	m_DefaultWeight = a_DefaultWeight;
}





void cPrefab::AddConnector(int a_RelX, int a_RelY, int a_RelZ, eBlockFace a_Direction, int a_Type)
{
	m_Connectors.push_back(cConnector(a_RelX, a_RelY, a_RelZ, a_Type, a_Direction));
}





void cPrefab::ParseCharMap(CharMap & a_CharMapOut, const char * a_CharMapDef)
{
	ASSERT(a_CharMapDef != NULL);
	
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
			ASSERT((BlockMeta <= 15));
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
	ASSERT(a_ConnectorsDef != NULL);
	
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





void cPrefab::ParseDepthWeight(const char * a_DepthWeightDef)
{
	// The member needn't be defined at all, if so, skip:
	if (a_DepthWeightDef == NULL)
	{
		return;
	}
	
	// Split into individual records: "Record | Record | Record"
	AStringVector Defs = StringSplitAndTrim(a_DepthWeightDef, "|");
	
	// Add each record's contents:
	for (AStringVector::const_iterator itr = Defs.begin(), end = Defs.end(); itr != end; ++itr)
	{
		// Split into components: "Depth : Weight"
		AStringVector Components = StringSplitAndTrim(*itr, ":");
		if (Components.size() != 2)
		{
			LOGWARNING("Bad prefab DepthWeight record: \"%s\", skipping.", itr->c_str());
			continue;
		}
		
		// Parse depth:
		int Depth = atoi(Components[0].c_str());
		if ((Depth == 0) && (Components[0] != "0"))
		{
			LOGWARNING("Bad prefab DepthWeight record, cannot parse depth \"%s\", skipping.", Components[0].c_str());
			continue;
		}
		
		// Parse weight:
		int Weight = atoi(Components[1].c_str());
		if ((Weight == 0) && (Components[1] != "0"))
		{
			LOGWARNING("Bad prefab DepthWeight record, cannot parse weight \"%s\", skipping.", Components[1].c_str());
			continue;
		}
		
		// Save to map:
		ASSERT(m_DepthWeight.find(Depth) == m_DepthWeight.end());  // Not a duplicate
		m_DepthWeight[Depth] = Weight;
	}  // for itr - Defs[]
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




