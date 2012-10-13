
#pragma once

#include "Simulator.h"





class cRedstoneSimulator :
	public cSimulator
{
	typedef cSimulator super;
public:
	cRedstoneSimulator( cWorld* a_World );
	~cRedstoneSimulator();

	virtual void Simulate( float a_Dt ) override;
	virtual bool IsAllowedBlock( BLOCKTYPE a_BlockType ) override { return true; }

	virtual void WakeUp( int a_X, int a_Y, int a_Z ) override;

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

	static bool IsRepeaterPointingTo  (const Vector3i & a_RepeaterPos, char a_MetaData, const Vector3i & a_BlockPos);
	static bool IsRepeaterPointingAway(const Vector3i & a_RepeaterPos, char a_MetaData, const Vector3i & a_BlockPos);
	static NIBBLETYPE RepeaterRotationToMetaData(float a_Rotation);
	static Vector3i GetRepeaterDirection(NIBBLETYPE a_MetaData);


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
	
	void SetRepeater(const Vector3i & a_Position, int a_Ticks, bool a_bPowerOn);

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




