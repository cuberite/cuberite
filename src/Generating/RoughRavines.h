
// RoughRavines.h

// Declares the cRoughRavines class representing the rough ravine generator




#pragma once

#include "GridStructGen.h"





class cRoughRavines:
	public cGridStructGen
{
	using Super = cGridStructGen;

public:

	cRoughRavines(
		int a_Seed,
		int a_MaxSize, int a_MinSize,
		float a_MaxCenterWidth, float a_MinCenterWidth,
		float a_MaxRoughness,   float a_MinRoughness,
		float a_MaxFloorHeightEdge,     float a_MinFloorHeightEdge,
		float a_MaxFloorHeightCenter,   float a_MinFloorHeightCenter,
		float a_MaxCeilingHeightEdge,   float a_MinCeilingHeightEdge,
		float a_MaxCeilingHeightCenter, float a_MinCeilingHeightCenter,
		int a_GridSize, int a_MaxOffset
	);

protected:
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

	/** Maximum floor height at the ravine's edge */
	float m_MaxFloorHeightEdge;

	/** Minimum floor height at the ravine's edge */
	float m_MinFloorHeightEdge;

	/** Maximum floor height at the ravine's center */
	float m_MaxFloorHeightCenter;

	/** Minimum floor height at the ravine's center */
	float m_MinFloorHeightCenter;

	/** Maximum ceiling height at the ravine's edge */
	float m_MaxCeilingHeightEdge;

	/** Minimum ceiling height at the ravine's edge */
	float m_MinCeilingHeightEdge;

	/** Maximum ceiling height at the ravine's center */
	float m_MaxCeilingHeightCenter;

	/** Minimum ceiling height at the ravine's center */
	float m_MinCeilingHeightCenter;

	// cGridStructGen overrides:
	virtual cStructurePtr CreateStructure(int a_GridX, int a_GridZ, int a_OriginX, int a_OriginZ) override;
};




