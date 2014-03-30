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

#pragma once


#include "Defines.h"


// fwd: World.h
class cWorld;





class cPiston
{
public:

	cPiston(cWorld * a_World);

	static NIBBLETYPE RotationPitchToMetaData(double a_Rotation, double a_Pitch)
	{
		if (a_Pitch >= 50)
		{
			return 0x1;
		}
		else if (a_Pitch <= -50)
		{
			return 0x0;
		}
		else
		{
			a_Rotation += 90 + 45;  // So its not aligned with axis

			if (a_Rotation > 360)
			{
				a_Rotation -= 360;
			}
			if ((a_Rotation >= 0) && (a_Rotation < 90))
			{
				return 0x4;
			}
			else if ((a_Rotation >= 180) && (a_Rotation < 270))
			{
				return 0x5;
			}
			else if ((a_Rotation >= 90) && (a_Rotation < 180))
			{
				return 0x2;
			}
			else
			{
				return 0x3;
			}
		}
	}
	
	static eBlockFace MetaDataToDirection(NIBBLETYPE a_MetaData)
	{
		switch (a_MetaData)
		{
			//case -1:  return BLOCK_FACE_NONE; //can never happen as metadata is unsigned
			case 0x0: return BLOCK_FACE_YM;
			case 0x1: return BLOCK_FACE_YP;
			case 0x2: return BLOCK_FACE_ZM;
			case 0x3: return BLOCK_FACE_ZP;
			case 0x4: return BLOCK_FACE_XM;
			case 0x5: return BLOCK_FACE_XP;
			default:
			{
				ASSERT(!"Invalid Metadata");
				return BLOCK_FACE_NONE;
			}
		}
	}

	void ExtendPiston( int, int, int );
	void RetractPiston( int, int, int );
	
	/// Returns true if the piston (specified by blocktype) is a sticky piston
	static bool IsSticky(BLOCKTYPE a_BlockType);
	
	/// Returns true if the piston (with the specified meta) is extended
	static bool IsExtended(NIBBLETYPE a_PistonMeta);
	
	/// Returns true if the specified block can be pushed by a piston (and left intact)
	static bool CanPush(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta);
	
	/// Returns true if the specified block can be pushed by a piston and broken / replaced
	static bool CanBreakPush(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta);
	
	/// Returns true if the specified block can be pulled by a sticky piston
	static bool CanPull(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta);
	
	/// Updates the coords by the specified amount in the direction a piston of the specified meta is facing
	static void AddDir(int & a_BlockX, int & a_BlockY, int & a_BlockZ, NIBBLETYPE a_PistonMeta, int a_Amount);
	
	
	cWorld * m_World;

private:
	void ChainMove( int, int, int, char, unsigned short * );
	
	/// Returns how many blocks the piston has to push (where the first free space is); <0 when unpushable
	int FirstPassthroughBlock(int a_PistonX, int a_PistonY, int a_PistonZ, NIBBLETYPE a_PistonMeta);
} ;




