
// ClassicFluidSimulator.h

// Interfaces to the cClassicFluidSimulator class representing the original MCServer's fluid simulator





#pragma once

#include "FluidSimulator.h"





class cClassicFluidSimulator :
	public cFluidSimulator
{
public:
	cClassicFluidSimulator(cWorld * a_World, BLOCKTYPE a_Fluid, BLOCKTYPE a_StationaryFluid, NIBBLETYPE a_MaxHeight, NIBBLETYPE a_Falloff);
	~cClassicFluidSimulator();

	// cSimulator overrides:
	virtual void Simulate(float a_Dt) override;
	virtual void AddBlock(int a_BlockX, int a_BlockY, int a_BlockZ) override;
	virtual bool IsAllowedBlock(BLOCKTYPE a_BlockType) override;

protected:
	NIBBLETYPE GetHighestLevelAround(int a_BlockX, int a_BlockY, int a_BlockZ);

	/// Applies special rules such as generation of water between sources, returns false if it is necessary to apply general rules
	bool UniqueSituation(Vector3i a_Pos);
	
	void ApplyUniqueToNearest(Vector3i a_Pos);

	float m_Timer;

	// fwd: ClassicFluidSimulator.cpp
	class FluidData;
	
	FluidData * m_Data;

	NIBBLETYPE m_MaxHeight;
	NIBBLETYPE m_Falloff;
} ;




