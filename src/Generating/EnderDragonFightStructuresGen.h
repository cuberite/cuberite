
#pragma once

/*
https://minecraft.gamepedia.com/End_spike
*/

#include "FinishGen.h"

class cEnderDragonFightStructuresGen :
	public cFinishGen
{
public:
	cEnderDragonFightStructuresGen(int a_Seed, AString & a_TowerProperties, int a_Radius);

protected:
	cNoise m_Noise;
	std::map<cChunkCoords, Vector3i> m_TowerPos;
	struct sTowerProperties
	{
		int m_Height;
		int m_Radius;
		bool m_HasCage;

		sTowerProperties(int a_Height, int a_Radius, bool a_HasCage) :
			m_Height(a_Height),
			m_Radius(a_Radius),
			m_HasCage(a_HasCage)
		{
		}
	};
	std::vector<sTowerProperties> m_TowerProperties;

	void GenFinish(cChunkDesc &a_ChunkDesc) override;
	// Places the fountain elements
	void PlaceFountainSouthEast(cChunkDesc & a_ChunkDesc) const;
	void PlaceFountainNorthEast(cChunkDesc & a_ChunkDesc) const;
	void PlaceFountainSouthWest(cChunkDesc & a_ChunkDesc) const;
	void PlaceFountainNorthWest(cChunkDesc & a_ChunkDesc) const;
	void PlaceTower(cChunkDesc & a_ChunkDesc, const Vector3i & a_AbsPos);
};
