/**
 * Copyright 2014 MCServer Contributors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

// POCPieceGenerator.h

// Declares the cPOCPieceGenerator class representing a Proof-Of_Concept structure generator using the cPieceGenerator technique
// The generator generates a maze of rooms at {0, 100, 0}





#pragma once

#include "PieceGenerator.h"
#include "ComposableGenerator.h"





class cPOCPieceGenerator :
	public cFinishGen,
	protected cPiecePool
{
public:
	cPOCPieceGenerator(int a_Seed);
	~cPOCPieceGenerator();
	
protected:
	int m_Seed;
	
	/** The pieces from which the generated structure is built. */
	cPieces m_AvailPieces;
	
	/** The placed pieces of the generated structure. */
	cPlacedPieces m_Pieces;
	
	/** Bounds of the complete structure, to save on processing outside chunks. */
	cCuboid m_Bounds;


	// cFinishGen overrides:
	virtual void GenFinish(cChunkDesc & a_ChunkDesc) override;
	
	// cPiecePool overrides:
	virtual cPieces GetPiecesWithConnector(int a_ConnectorType) override;
	virtual cPieces GetStartingPieces(void) override;
	virtual void PiecePlaced(const cPiece & a_Piece) override;
	virtual void Reset(void) override;
} ;





