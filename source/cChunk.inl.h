
#ifndef __C_CHUNK_INL_H__
#define __C_CHUNK_INL_H__

#ifndef MAX
# define MAX(a,b) (((a)>(b))?(a):(b))
#endif





__C_CHUNK_INLINE__
void cChunk::SpreadLightOfBlock(NIBBLETYPE * a_LightBuffer, int a_X, int a_Y, int a_Z, char a_Falloff)
{
	unsigned char CurrentLight = cChunkDef::GetNibble( a_LightBuffer, a_X, a_Y, a_Z );
	cChunkDef::SetNibble( a_LightBuffer, a_X-1, a_Y, a_Z, MAX(cChunkDef::GetNibble( a_LightBuffer, a_X-1, a_Y, a_Z ), MAX(0,CurrentLight-a_Falloff) ) );
	cChunkDef::SetNibble( a_LightBuffer, a_X+1, a_Y, a_Z, MAX(cChunkDef::GetNibble( a_LightBuffer, a_X+1, a_Y, a_Z ), MAX(0,CurrentLight-a_Falloff) ) );
	cChunkDef::SetNibble( a_LightBuffer, a_X, a_Y-1, a_Z, MAX(cChunkDef::GetNibble( a_LightBuffer, a_X, a_Y-1, a_Z ), MAX(0,CurrentLight-a_Falloff) ) );
	cChunkDef::SetNibble( a_LightBuffer, a_X, a_Y+1, a_Z, MAX(cChunkDef::GetNibble( a_LightBuffer, a_X, a_Y+1, a_Z ), MAX(0,CurrentLight-a_Falloff) ) );
	cChunkDef::SetNibble( a_LightBuffer, a_X, a_Y, a_Z-1, MAX(cChunkDef::GetNibble( a_LightBuffer, a_X, a_Y, a_Z-1 ), MAX(0,CurrentLight-a_Falloff) ) );
	cChunkDef::SetNibble( a_LightBuffer, a_X, a_Y, a_Z+1, MAX(cChunkDef::GetNibble( a_LightBuffer, a_X, a_Y, a_Z+1 ), MAX(0,CurrentLight-a_Falloff) ) );
	MarkDirty();
}





#endif




