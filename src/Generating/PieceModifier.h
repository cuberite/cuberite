
// PieceModifier.h

// Declares the public interface for cPiece's cPieceModifier implementations





#pragma once

#include "PiecePool.h"





bool CreatePieceModifierFromString(const AString & a_Definition, std::shared_ptr<cPiece::cPieceModifiers> & a_Modifiers, bool a_LogWarnings);





/** Used to store block type, meta, weight and some more params */
class cRandomizedBlock
{
public:
	BlockType m_Type;

	int m_Weight;

	/** Minimum meta to randomize */
	int m_MinMeta = 0;

	/** Maximum meta to randomize */
	int m_MaxMeta = -1;

	/** Maximum meta in noise range */
	int m_MaxNoiseMeta = 0;

	/** Minimum meta in noise range */
	int m_MinNoiseMeta = 0;
};

typedef std::vector<cRandomizedBlock> cRandomizedBlocks;
