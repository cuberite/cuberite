
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
	struct sTowerProperties
	{
		Vector3i m_Pos;
		int m_Height;
		int m_Radius;
		bool m_HasCage;
	};

	cNoise m_Noise;
	std::map<cChunkCoords, std::vector<sTowerProperties>> m_TowerPos;
	static const std::array<Vector3i, 48> m_CagePos;
	static const std::array<Vector3i, 26> m_CageAir;
	cBlockArea m_Fountain;

	int m_MinX = -1, m_MaxX = 1, m_MinZ = -1, m_MaxZ = 1;

	void GenFinish(cChunkDesc &a_ChunkDesc) override;
	void PlaceTower(cChunkDesc & a_ChunkDesc, const sTowerProperties & a_TowerProperties);
};
