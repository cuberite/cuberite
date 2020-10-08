
#pragma once

/*
https://minecraft.gamepedia.com/End_spike
*/

#include "FinishGen.h"

class cEnderDragonFightStructuresGen :
	public cFinishGen
{
public:
	cEnderDragonFightStructuresGen(int a_Seed);

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
	std::array<sTowerProperties, 10> m_TowerHeights =
	{
		sTowerProperties(76,  3, false),
		{79,  3, true},
		{82,  3, true},
		{85,  4, false},
		{88,  4, false},
		{91,  4, false},
		{94,  5, false},
		{97,  5, false},
		{100, 5, false},
		{103, 6, false},
	};

	void GenFinish(cChunkDesc &a_ChunkDesc) override;
	// Places the fountain elements
	void PlaceFountainSouthEast(cChunkDesc & a_ChunkDesc) const;
	void PlaceFountainNorthEast(cChunkDesc & a_ChunkDesc) const;
	void PlaceFountainSouthWest(cChunkDesc & a_ChunkDesc) const;
	void PlaceFountainNorthWest(cChunkDesc & a_ChunkDesc) const;
	void PlaceTower(cChunkDesc & a_ChunkDesc, const Vector3i & a_AbsPos);
};
