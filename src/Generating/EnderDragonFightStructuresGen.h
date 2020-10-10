
#pragma once

/*
https://minecraft.gamepedia.com/End_spike
*/

#include "FinishGen.h"

class cEnderDragonFightStructuresGen :
	public cFinishGen
{
public:
	cEnderDragonFightStructuresGen(int a_Seed, const AString & a_TowerProperties, int a_Radius);

protected:
	cNoise m_Noise;
	std::map<cChunkCoords, Vector3i> m_TowerPos;
	struct sTowerProperties
	{
		int m_Height;
		int m_Radius;
		bool m_HasCage;
	};
	std::vector<sTowerProperties> m_TowerProperties;
	cBlockArea m_Fountain;

	void GenFinish(cChunkDesc &a_ChunkDesc) override;
	void PlaceTower(cChunkDesc & a_ChunkDesc, const Vector3i & a_AbsPos);
};
