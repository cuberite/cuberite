
// RoughRavines.h

// Declares the cRoughRavines class representing the rough ravine generator




#pragma once

#include "GridStructGen.h"





class cRoughRavines :
	public cGridStructGen
{
	typedef cGridStructGen super;
	
public:
	cRoughRavines(
		int a_Seed,
		int a_MaxSize, int a_MinSize,
		float a_MaxCenterWidth, float a_MinCenterWidth,
		float a_MaxRoughness, float a_MinRoughness,
		int a_GridSize, int a_MaxOffset
	);
	
protected:
	int m_Seed;
	
	/** Maximum size of the ravine, in each of the X / Z axis */
	int m_MaxSize;
	
	/** Minimum size of the ravine */
	int m_MinSize;

	/** Maximum width of the ravine's center, in blocks */
	float m_MaxCenterWidth;
	
	/** Minimum width of the ravine's center, in blocks */
	float m_MinCenterWidth;
	
	/** Maximum roughness of the ravine */
	float m_MaxRoughness;

	/** Minimum roughness of the ravine */
	float m_MinRoughness;

	// cGridStructGen overrides:
	virtual cStructurePtr CreateStructure(int a_GridX, int a_GridZ, int a_OriginX, int a_OriginZ) override;
};




