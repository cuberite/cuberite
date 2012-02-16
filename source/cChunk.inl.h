
#ifndef __C_CHUNK_INL_H__
#define __C_CHUNK_INL_H__

#ifndef MAX
# define MAX(a,b) (((a)>(b))?(a):(b))
#endif





__C_CHUNK_INLINE__
char cChunk::GetLight(char* a_Buffer, int a_BlockIdx)
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
char cChunk::GetLight(char* a_Buffer, int x, int y, int z)
{
	if( x < 16 && x > -1 && y < 128 && y > -1 && z < 16 && z > -1 )
	{
		const int cindex = (y/2) + (z * 64) + (x * 64 * 16);
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
void cChunk::SetLight(char* a_Buffer, int a_BlockIdx, char a_Light)
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
		MarkDirty();
	}
}





__C_CHUNK_INLINE__
void cChunk::SetLight(char* a_Buffer, int x, int y, int z, char light)
{
	if( x < 16 && x > -1 && y < 128 && y > -1 && z < 16 && z > -1 )
	{
		int cindex = (y/2) + (z * 64) + (x * 64 * 16);
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
		MarkDirty();
	}
}





__C_CHUNK_INLINE__
void cChunk::SpreadLightOfBlock(char* a_LightBuffer, int a_X, int a_Y, int a_Z, char a_Falloff)
{
	unsigned char CurrentLight = GetLight( a_LightBuffer, a_X, a_Y, a_Z );
	SetLight( a_LightBuffer, a_X-1, a_Y, a_Z, MAX(GetLight( a_LightBuffer, a_X-1, a_Y, a_Z ), MAX(0,CurrentLight-a_Falloff) ) );
	SetLight( a_LightBuffer, a_X+1, a_Y, a_Z, MAX(GetLight( a_LightBuffer, a_X+1, a_Y, a_Z ), MAX(0,CurrentLight-a_Falloff) ) );
	SetLight( a_LightBuffer, a_X, a_Y-1, a_Z, MAX(GetLight( a_LightBuffer, a_X, a_Y-1, a_Z ), MAX(0,CurrentLight-a_Falloff) ) );
	SetLight( a_LightBuffer, a_X, a_Y+1, a_Z, MAX(GetLight( a_LightBuffer, a_X, a_Y+1, a_Z ), MAX(0,CurrentLight-a_Falloff) ) );
	SetLight( a_LightBuffer, a_X, a_Y, a_Z-1, MAX(GetLight( a_LightBuffer, a_X, a_Y, a_Z-1 ), MAX(0,CurrentLight-a_Falloff) ) );
	SetLight( a_LightBuffer, a_X, a_Y, a_Z+1, MAX(GetLight( a_LightBuffer, a_X, a_Y, a_Z+1 ), MAX(0,CurrentLight-a_Falloff) ) );
}





__C_CHUNK_INLINE__
void cChunk::SpreadLightOfBlockX(char* a_LightBuffer, int a_X, int a_Y, int a_Z)
{
	unsigned char CurrentLight = GetLight( a_LightBuffer, a_X, a_Y, a_Z );
	SetLight( a_LightBuffer, a_X-1, a_Y, a_Z, MAX(GetLight( a_LightBuffer, a_X-1, a_Y, a_Z ), CurrentLight-1) );
	SetLight( a_LightBuffer, a_X+1, a_Y, a_Z, MAX(GetLight( a_LightBuffer, a_X+1, a_Y, a_Z ), CurrentLight-1) );
}





__C_CHUNK_INLINE__
void cChunk::SpreadLightOfBlockY(char* a_LightBuffer, int a_X, int a_Y, int a_Z)
{
	unsigned char CurrentLight = GetLight( a_LightBuffer, a_X, a_Y, a_Z );
	SetLight( a_LightBuffer, a_X, a_Y-1, a_Z, MAX(GetLight( a_LightBuffer, a_X, a_Y-1, a_Z ), CurrentLight-1) );
	SetLight( a_LightBuffer, a_X, a_Y+1, a_Z, MAX(GetLight( a_LightBuffer, a_X, a_Y+1, a_Z ), CurrentLight-1) );
}





__C_CHUNK_INLINE__
void cChunk::SpreadLightOfBlockZ(char* a_LightBuffer, int a_X, int a_Y, int a_Z)
{
	unsigned char CurrentLight = GetLight( a_LightBuffer, a_X, a_Y, a_Z );
	SetLight( a_LightBuffer, a_X, a_Y, a_Z-1, MAX(GetLight( a_LightBuffer, a_X, a_Y, a_Z-1 ), CurrentLight-1) );
	SetLight( a_LightBuffer, a_X, a_Y, a_Z+1, MAX(GetLight( a_LightBuffer, a_X, a_Y, a_Z+1 ), CurrentLight-1) );
}





#endif