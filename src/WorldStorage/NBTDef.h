
#pragma once

// Defines the NBT tag types used by FastNBT.h and TreeNBT.h


enum eTagType
{
	TAG_Min       = 0,  // The minimum value for a tag type
	TAG_End       = 0,
	TAG_Byte      = 1,
	TAG_Short     = 2,
	TAG_Int       = 3,
	TAG_Long      = 4,
	TAG_Float     = 5,
	TAG_Double    = 6,
	TAG_ByteArray = 7,
	TAG_String    = 8,
	TAG_List      = 9,
	TAG_Compound  = 10,
	TAG_IntArray  = 11,
	TAG_Max       = 11,  // The maximum value for a tag type
} ;
