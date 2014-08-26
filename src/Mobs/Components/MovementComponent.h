#pragma once

class cMonster;
class cEntity;
class cChunk;

class cMovementComponent
{
protected:
	cMonster * m_Self;
public:
	cMovementComponent(cMonster * a_Entity);
	virtual ~cMovementComponent(){}
	
	virtual void Tick(float a_Dt, cChunk & a_Chunk){}
	
	/** Finds the first non-air block position (not the highest, as cWorld::GetHeight does)
		If current Y is nonsolid, goes down to try to find a solid block, then returns that + 1
		If current Y is solid, goes up to find first nonsolid block, and returns that */
	int FindFirstNonAirBlockPosition(double a_PosX, double a_PosZ);
	/** Returns if a monster can actually reach a given height by jumping or walking */
	bool IsNextYPosReachable(int a_PosY);
	/** Returns if a monster can reach a given height by jumping */
	bool DoesPosYRequireJump(int a_PosY);
};
