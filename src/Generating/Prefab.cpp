
// Prefab.cpp

/*
Implements the cPrefab class, representing a cPiece descendant for the cPieceGenerator that
uses a prefabricate in a cBlockArea for drawing itself.
*/

#include "Globals.h"
#include "Prefab.h"
#include "../WorldStorage/SchematicFileSerializer.h"





cPrefab::cPrefab(const cPrefab::sDef & a_Def) :
	m_Size(a_Def.m_SizeX, a_Def.m_SizeY, a_Def.m_SizeZ),
	m_HitBox(0, 0, 0, a_Def.m_SizeX, a_Def.m_SizeY, a_Def.m_SizeZ),
	m_AllowedRotations(7),  // TODO: All rotations allowed (not in the definition yet)
	m_MergeStrategy(cBlockArea::msImprint)
{
	m_BlockArea.Create(m_Size);
	CharMap cm;
	ParseCharMap(cm, a_Def.m_CharMap);
	ParseBlockImage(cm, a_Def.m_Image);
}





void cPrefab::Draw(cBlockArea & a_Dest, const cPlacedPiece * a_Placement)
{
	Vector3i Placement = a_Placement->GetCoords();
	Placement.Move(a_Dest.GetOrigin() * (-1));
	a_Dest.Merge(m_BlockArea, Placement, m_MergeStrategy);
	
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




