#pragma once

#include "Simulator.h"
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
	struct sRepeaterChange
	{
		Vector3i Position;
		int Ticks;
		bool bPowerOn;
		bool bPowerOffNextTime;
	};

	typedef std::deque <Vector3i> BlockList;

	typedef std::deque< sRepeaterChange > RepeaterList;
	RepeaterList m_SetRepeaters;
	void SetRepeater( const Vector3i & a_Position, int a_Ticks, bool a_bPowerOn )
	{
		for( RepeaterList::iterator itr = m_SetRepeaters.begin(); itr != m_SetRepeaters.end(); ++itr )
		{
			sRepeaterChange & Change = *itr;
			if( Change.Position.Equals( a_Position ) )
			{
				if( Change.bPowerOn && a_bPowerOn == false )
				{
					Change.bPowerOffNextTime = true;
				}
				if( a_bPowerOn == true )
				{
					Change.bPowerOffNextTime = false;
				}
				Change.bPowerOn |= a_bPowerOn;
				return;
			}
		}

		sRepeaterChange RC;
		RC.Position = a_Position;
		RC.Ticks = a_Ticks;
		RC.bPowerOn = a_bPowerOn;
		RC.bPowerOffNextTime = false;
		m_SetRepeaters.push_back( RC );
	}

	virtual void AddBlock(int a_X, int a_Y, int a_Z) {}

	void HandleChange( const Vector3i & a_BlockPos );
	BlockList RemoveCurrent( const Vector3i & a_BlockPos );

	bool PowerBlock( const Vector3i & a_BlockPos, const Vector3i & a_FromBlock, char a_Power );
	int UnPowerBlock( const Vector3i & a_BlockPos, const Vector3i & a_FromBlock );

	bool IsPowered( const Vector3i & a_BlockPos, bool a_bOnlyByWire = false );
	bool IsPowering( const Vector3i & a_PowerPos, const Vector3i & a_BlockPos, eRedstoneDirection a_WireDirection, bool a_bOnlyByWire );
	
	BlockList m_Blocks;
	BlockList m_BlocksBuffer;

	BlockList m_RefreshPistons;

	BlockList m_RefreshTorchesAround;
	
	void RefreshTorchesAround( const Vector3i & a_BlockPos );

	cCriticalSection m_CS;
};