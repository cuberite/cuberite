
#pragma once

class cBroadcastInterface
{
public:
	virtual ~cBroadcastInterface() {}

	virtual void BroadcastUseBed     (const cEntity & a_Entity, int a_BlockX, int a_BlockY, int a_BlockZ) = 0;
	virtual void BroadcastSoundEffect(const AString & a_SoundName, double a_X, double a_Y, double a_Z, float a_Volume, float a_Pitch, const cClientHandle * a_Exclude = nullptr) = 0;
	virtual void BroadcastEntityAnimation(const cEntity & a_Entity, char a_Animation, const cClientHandle * a_Exclude = nullptr) = 0;
	virtual void BroadcastSoundParticleEffect(const EffectID a_EffectID, int a_SrcX, int a_SrcY, int a_SrcZ, int a_Data, const cClientHandle * a_Exclude = nullptr) = 0;	
};
