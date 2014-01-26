
#pragma once

class cBroadcastInterface 
{
public:

	virtual void BroadcastUseBed(const cEntity & a_Entity, int a_BlockX, int a_BlockY, int a_BlockZ ) = 0;
	virtual void BroadcastSoundEffect        (const AString & a_SoundName, int a_SrcX, int a_SrcY, int a_SrcZ, float a_Volume, float a_Pitch, const cClientHandle * a_Exclude = NULL) = 0; 
};
