
#ifndef __C_CHUNK_INL_H__
#define __C_CHUNK_INL_H__

#ifndef MAX
# define MAX(a,b) (((a)>(b))?(a):(b))
#endif





__C_CHUNK_INLINE__
char cChunk::GetNibble(char* a_Buffer, int a_BlockIdx)
{
	if( a_BlockIdx > -1 && a_BlockIdx < c_NumBlocks )
	{
		const int cindex = (a_BlockIdx/2);
		if( (a_BlockIdx & 1) == 0 )
		{	// First half byte
			return (a_Buffer[cindex] & 0x0f);
		}
		else
		{
			return ((a_Buffer[cindex] & 0xf0) >> 4);
		}
	}
	return 0;
}





__C_CHUNK_INLINE__
char cChunk::GetNibble(char* a_Buffer, int x, int y, int z)
{
	if( x < c_ChunkWidth && x > -1 && y < c_ChunkHeight && y > -1 && z < c_ChunkWidth && z > -1 )
	{
		const int cindex = MakeIndexNoCheck(x, y, z)/2;
		if( (y & 1) == 0 )
		{	// First half byte
			return (a_Buffer[cindex] & 0x0f);
		}
		else
		{
			return ((a_Buffer[cindex] & 0xf0) >> 4);
		}
	}
	return 0;
}





__C_CHUNK_INLINE__
void cChunk::SetNibble(char* a_Buffer, int a_BlockIdx, char a_Light)
{
	if( a_BlockIdx > -1 && a_BlockIdx < c_NumBlocks )
	{
		const int cindex = (a_BlockIdx/2);
		if( (a_BlockIdx & 1) == 0 )
		{	// First half byte
			a_Buffer[cindex] &= 0xf0; // Set first half to 0
			a_Buffer[cindex] |= (a_Light) & 0x0f;
		}
		else
		{
			a_Buffer[cindex] &= 0x0f; // Set second half to 0
			a_Buffer[cindex] |= (a_Light << 4) & 0xf0;
		}
	}
}





__C_CHUNK_INLINE__
void cChunk::SetNibble(char* a_Buffer, int x, int y, int z, char light)
{
	if( x < c_ChunkWidth && x > -1 && y < c_ChunkHeight && y > -1 && z < c_ChunkWidth && z > -1 )
	{
		int cindex = MakeIndexNoCheck(x, y, z)/2;
		if( (y & 1) == 0 )
		{	// First half byte
			a_Buffer[cindex] &= 0xf0; // Set first half to 0
			a_Buffer[cindex] |= (light) & 0x0f;
		}
		else
		{
			a_Buffer[cindex] &= 0x0f; // Set second half to 0
			a_Buffer[cindex] |= (light << 4) & 0xf0;
		}
	}
}





__C_CHUNK_INLINE__
void cChunk::SpreadLightOfBlock(char* a_LightBuffer, int a_X, int a_Y, int a_Z, char a_Falloff)
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





__C_CHUNK_INLINE__
void cChunk::SpreadLightOfBlockX(char* a_LightBuffer, int a_X, int a_Y, int a_Z)
{
	unsigned char CurrentLight = GetNibble( a_LightBuffer, a_X, a_Y, a_Z );
	SetNibble( a_LightBuffer, a_X-1, a_Y, a_Z, MAX(GetNibble( a_LightBuffer, a_X-1, a_Y, a_Z ), CurrentLight-1) );
	SetNibble( a_LightBuffer, a_X+1, a_Y, a_Z, MAX(GetNibble( a_LightBuffer, a_X+1, a_Y, a_Z ), CurrentLight-1) );
	MarkDirty();
}





__C_CHUNK_INLINE__
void cChunk::SpreadLightOfBlockY(char* a_LightBuffer, int a_X, int a_Y, int a_Z)
{
	unsigned char CurrentLight = GetNibble( a_LightBuffer, a_X, a_Y, a_Z );
	SetNibble( a_LightBuffer, a_X, a_Y-1, a_Z, MAX(GetNibble( a_LightBuffer, a_X, a_Y-1, a_Z ), CurrentLight-1) );
	SetNibble( a_LightBuffer, a_X, a_Y+1, a_Z, MAX(GetNibble( a_LightBuffer, a_X, a_Y+1, a_Z ), CurrentLight-1) );
	MarkDirty();
}





__C_CHUNK_INLINE__
void cChunk::SpreadLightOfBlockZ(char* a_LightBuffer, int a_X, int a_Y, int a_Z)
{
	unsigned char CurrentLight = GetNibble( a_LightBuffer, a_X, a_Y, a_Z );
	SetNibble( a_LightBuffer, a_X, a_Y, a_Z-1, MAX(GetNibble( a_LightBuffer, a_X, a_Y, a_Z-1 ), CurrentLight-1) );
	SetNibble( a_LightBuffer, a_X, a_Y, a_Z+1, MAX(GetNibble( a_LightBuffer, a_X, a_Y, a_Z+1 ), CurrentLight-1) );
	MarkDirty();
}





#endif