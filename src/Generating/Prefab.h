
// Prefab.h

/*
Declares the cPrefab class, representing a cPiece descendant for the cPieceGenerator that
uses a prefabricate in a cBlockArea for drawing itself.
The class can be constructed from data that is stored directly in the executable, in a sPrefabDef structure
declared in this file as well; the Gallery server exports areas in this format.
*/





#pragma once

#include "PieceGenerator.h"
#include "../BlockArea.h"
#include "ChunkDesc.h"





// fwd:
class cChunkDesc;





class cPrefab :
	public cPiece
{
public:
	struct sDef
	{
		int m_SizeX;
		int m_SizeY;
		int m_SizeZ;
		const char * m_CharMap;
		const char * m_Image;
		const char * m_Connectors;
		int m_AllowedRotations;
		cBlockArea::eMergeStrategy m_MergeStrategy;
	};
	
	cPrefab(const sDef & a_Def);
	
	/** Draws the prefab into the specified chunk, according to the placement stored in the PlacedPiece. */
	void Draw(cChunkDesc & a_Dest, const cPlacedPiece * a_Placement) const;
	
	/** Returns true if the prefab has any connector of the specified type. */
	bool HasConnectorType(int a_ConnectorType) const;

protected:
	/** Maps letters in the sDef::m_Image onto a number, BlockType * 16 | BlockMeta */
	typedef int CharMap[256];
	
	
	/** The cBlockArea that contains the block definitions for the prefab.
	The index identifies the number of CCW rotations applied (0 = no rotation, 1 = 1 CCW rotation, ...). */
	cBlockArea m_BlockArea[4];
	
	/** The size of the prefab */
	Vector3i m_Size;
	
	/** The hitbox of the prefab. In first version is the same as the m_BlockArea dimensions */
	cCuboid m_HitBox;
	
	/** The connectors through which the piece connects to other pieces */
	cConnectors m_Connectors;
	
	/** Bitmask, bit N set -> N rotations CCW supported */
	int m_AllowedRotations;
	
	/** The merge strategy to use when drawing the prefab into a block area */
	cBlockArea::eMergeStrategy m_MergeStrategy;
	
	
	// cPiece overrides:	
	virtual cConnectors GetConnectors(void) const override;
	virtual Vector3i GetSize(void) const override;
	virtual cCuboid GetHitBox(void) const override;
	virtual bool CanRotateCCW(int a_NumRotations) const override;
	
	/** Parses the CharMap in the definition into a CharMap binary data used for translating the definition into BlockArea. */
	void ParseCharMap(CharMap & a_CharMapOut, const char * a_CharMapDef);
	
	/** Parses the Image in the definition into m_BlockArea[0]'s block types and metas, using the specified CharMap. */
	void ParseBlockImage(const CharMap & a_CharMap, const char * a_BlockImage);
	
	/** Parses the connectors definition text into m_Connectors member. */
	void ParseConnectors(const char * a_ConnectorsDef);
};




