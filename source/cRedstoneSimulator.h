#pragma once

#include "cSimulator.h"
#include "Vector3i.h"

class cRedstoneSimulator : public cSimulator
{
	typedef cSimulator super;
public:
	cRedstoneSimulator( cWorld* a_World );
	~cRedstoneSimulator();

	virtual void Simulate( float a_Dt );
	virtual bool IsAllowedBlock( char a_BlockID ) { return true; }

	virtual void WakeUp( int a_X, int a_Y, int a_Z );

	enum eRedstoneDirection
	{
		REDSTONE_NONE =  0,
		REDSTONE_X_POS = 0x1,
		REDSTONE_X_NEG = 0x2,
		REDSTONE_Z_POS = 0x4,
		REDSTONE_Z_NEG = 0x8,
	};
	eRedstoneDirection GetDirection( int a_X, int a_Y, int a_Z );
	eRedstoneDirection GetDirection( const Vector3i & a_Pos ) { return GetDirection( a_Pos.x, a_Pos.y, a_Pos.z ); }
private:
	typedef std::deque <Vector3i> BlockList;

	virtual void AddBlock(int a_X, int a_Y, int a_Z) {}

	void HandleChange( const Vector3i & a_BlockPos );
	BlockList RemoveCurrent( const Vector3i & a_BlockPos );

	bool PowerBlock( const Vector3i & a_BlockPos, const Vector3i & a_FromBlock, char a_Power );
	int UnPowerBlock( const Vector3i & a_BlockPos );

	bool IsPowered( const Vector3i & a_BlockPos, bool a_bOnlyByWire = false );
	
	BlockList m_Blocks;
	BlockList m_BlocksBuffer;

	BlockList m_RefreshPistons;

	BlockList m_RefreshTorchesAround;
	void RefreshTorchesAround( const Vector3i & a_BlockPos );

	cCriticalSection m_CS;
};