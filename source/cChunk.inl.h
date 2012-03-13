
#ifndef __C_CHUNK_INL_H__
#define __C_CHUNK_INL_H__

#ifndef MAX
# define MAX(a,b) (((a)>(b))?(a):(b))
#endif





__C_CHUNK_INLINE__
char cChunk::GetNibble(char * a_Buffer, int a_BlockIdx)
{
	if ((a_BlockIdx > -1) && (a_BlockIdx < c_NumBlocks))
	{
		return (a_Buffer[a_BlockIdx / 2] >> ((a_BlockIdx & 1) * 4)) & 0x0f;
	}
	return 0;
}





__C_CHUNK_INLINE__
char cChunk::GetNibble(char * a_Buffer, int x, int y, int z)
{
	if ((x < c_ChunkWidth) && (x > -1) && (y < c_ChunkHeight) && (y > -1) && (z < c_ChunkWidth) && (z > -1))
	{
		int Index = MakeIndexNoCheck(x, y, z);
		return (a_Buffer[Index / 2] >> ((Index & 1) * 4)) & 0x0f;
	}
	return 0;
}





__C_CHUNK_INLINE__
void cChunk::SetNibble(char * a_Buffer, int a_BlockIdx, char a_Nibble)
{
	if ((a_BlockIdx > -1) && (a_BlockIdx < c_NumBlocks))
	{
		a_Buffer[a_BlockIdx / 2] = (
			(a_Buffer[a_BlockIdx / 2] & (0xf0 >> ((a_BlockIdx & 1) * 4))) |  // The untouched nibble
			((a_Nibble & 0x0f) << ((a_BlockIdx & 1) * 4))  // The nibble being set
		);
	}
}





__C_CHUNK_INLINE__
void cChunk::SetNibble(char * a_Buffer, int x, int y, int z, char a_Nibble)
{
	if ((x < c_ChunkWidth) && (x > -1) && (y < c_ChunkHeight) && (y > -1) && (z < c_ChunkWidth) && (z > -1))
	{
		int Index = MakeIndexNoCheck(x, y, z);
		a_Buffer[Index / 2] = (
			(a_Buffer[Index / 2] & (0xf0 >> ((Index & 1) * 4))) |  // The untouched nibble
			((a_Nibble & 0x0f) << ((Index & 1) * 4))  // The nibble being set
		);
	}
}





__C_CHUNK_INLINE__
void cChunk::SpreadLightOfBlock(char * a_LightBuffer, int a_X, int a_Y, int a_Z, char a_Falloff)
{
	unsigned char CurrentLight = GetNibble( a_LightBuffer, a_X, a_Y, a_Z );
	SetNibble( a_LightBuffer, a_X-1, a_Y, a_Z, MAX(GetNibble( a_LightBuffer, a_X-1, a_Y, a_Z ), MAX(0,CurrentLight-a_Falloff) ) );
	SetNibble( a_LightBuffer, a_X+1, a_Y, a_Z, MAX(GetNibble( a_LightBuffer, a_X+1, a_Y, a_Z ), MAX(0,CurrentLight-a_Falloff) ) );
	SetNibble( a_LightBuffer, a_X, a_Y-1, a_Z, MAX(GetNibble( a_LightBuffer, a_X, a_Y-1, a_Z ), MAX(0,CurrentLight-a_Falloff) ) );
	SetNibble( a_LightBuffer, a_X, a_Y+1, a_Z, MAX(GetNibble( a_LightBuffer, a_X, a_Y+1, a_Z ), MAX(0,CurrentLight-a_Falloff) ) );
	SetNibble( a_LightBuffer, a_X, a_Y, a_Z-1, MAX(GetNibble( a_LightBuffer, a_X, a_Y, a_Z-1 ), MAX(0,CurrentLight-a_Falloff) ) );
	SetNibble( a_LightBuffer, a_X, a_Y, a_Z+1, MAX(GetNibble( a_LightBuffer, a_X, a_Y, a_Z+1 ), MAX(0,CurrentLight-a_Falloff) ) );
	MarkDirty();
}





#endif




