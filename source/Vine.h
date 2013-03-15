
#pragma once





// tolua_begin
class cVine
{
public:
	
	static NIBBLETYPE DirectionToMetaData(char a_BlockFace)
	{
		switch (a_BlockFace)
		{
			case BLOCK_FACE_NORTH: return 0x1;
			case BLOCK_FACE_SOUTH: return 0x4;
			case BLOCK_FACE_WEST:  return 0x8;
			case BLOCK_FACE_EAST:  return 0x2;
			default: return 0x0;
		}
	}


	static char MetaDataToDirection(NIBBLETYPE a_MetaData)
	{
		switch(a_MetaData)
		{
			case 0x1: return BLOCK_FACE_NORTH;
			case 0x4: return BLOCK_FACE_SOUTH;
			case 0x8: return BLOCK_FACE_WEST;
			case 0x2: return BLOCK_FACE_EAST;
			default:  return BLOCK_FACE_TOP;
		}
	}
} ;
// tolua_end




