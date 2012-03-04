#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cRedstoneSimulator.h"
#include "cPiston.h"
#include "cWorld.h"
#include "BlockID.h"
#include "cCriticalSection.h"
#include "cTorch.h"

#include "cRedstone.h"



cRedstoneSimulator::cRedstoneSimulator( cWorld* a_World )
	: super(a_World)
{
}





cRedstoneSimulator::~cRedstoneSimulator()
{
}





void cRedstoneSimulator::WakeUp( int a_X, int a_Y, int a_Z )
{
	if( cRedstone::s_UseRedstone ) return; // Using the other/broken simulator

	cCSLock Lock( m_CS );
	m_Blocks.push_back( Vector3i( a_X, a_Y, a_Z ) );
}





void cRedstoneSimulator::Simulate( float a_Dt )
{
	if( cRedstone::s_UseRedstone ) return; // Using the other/broken simulator

	// Toggle torches on/off
	while( !m_RefreshTorchesAround.empty() )
	{
		Vector3i pos = m_RefreshTorchesAround.back();
		m_RefreshTorchesAround.pop_back();

		RefreshTorchesAround( pos );
	}

	// Handle changed blocks
	{
		cCSLock Lock( m_CS );
		std::swap( m_Blocks, m_BlocksBuffer );
	}
	for( BlockList::iterator itr = m_BlocksBuffer.begin(); itr != m_BlocksBuffer.end(); ++itr )
	{
		HandleChange( *itr );
	}
	m_BlocksBuffer.clear();
}





void cRedstoneSimulator::RefreshTorchesAround( const Vector3i & a_BlockPos )
{
	static Vector3i Surroundings [] = {
		Vector3i(-1, 0, 0),
		Vector3i( 1, 0, 0),
		Vector3i( 0, 0,-1),
		Vector3i( 0, 0, 1),
		Vector3i( 0, 1, 0), // Also toggle torch on top
	};
	char TargetBlockID = E_BLOCK_REDSTONE_TORCH_ON;
	if( IsPowered( a_BlockPos, true ) )
	{
		TargetBlockID = E_BLOCK_REDSTONE_TORCH_OFF;
		//if( m_World->GetBlock( a_BlockPos ) == E_BLOCK_DIRT )
		//{
		//	m_World->FastSetBlock( a_BlockPos.x, a_BlockPos.y, a_BlockPos.z, E_BLOCK_STONE, 0 );
		//}
	}
	else
	{
		//if( m_World->GetBlock( a_BlockPos ) == E_BLOCK_STONE )
		//{
		//	m_World->FastSetBlock( a_BlockPos.x, a_BlockPos.y, a_BlockPos.z, E_BLOCK_DIRT, 0 );
		//}
	}

	for( unsigned int i = 0; i < ARRAYCOUNT( Surroundings ); ++i )
	{
		Vector3i TorchPos = a_BlockPos + Surroundings[i];
		const char Block = m_World->GetBlock( TorchPos );
		if( Block == E_BLOCK_REDSTONE_TORCH_ON || Block == E_BLOCK_REDSTONE_TORCH_OFF )
		{
			if( Block != TargetBlockID )
			{
				char TorchMeta = m_World->GetBlockMeta( TorchPos );
				if( cTorch::IsAttachedTo( TorchPos, TorchMeta, a_BlockPos ) )
				{
					m_World->FastSetBlock( TorchPos.x, TorchPos.y, TorchPos.z, TargetBlockID, m_World->GetBlockMeta( TorchPos ) );
					m_Blocks.push_back( TorchPos );
				}
			}
		}
	}
}





void cRedstoneSimulator::HandleChange( const Vector3i & a_BlockPos )
{
	std::deque< Vector3i > SpreadStack;

	Vector3i Surroundings[] = {
		Vector3i( 1, 0, 0 ),
		Vector3i( 1, 1, 0 ),
		Vector3i( 1,-1, 0 ),
		Vector3i(-1, 0, 0 ),
		Vector3i(-1, 1, 0 ),
		Vector3i(-1,-1, 0 ),
		Vector3i( 0, 0, 1 ),
		Vector3i( 0, 1, 1 ),
		Vector3i( 0,-1, 1 ),
		Vector3i( 0, 0,-1 ),
		Vector3i( 0, 1,-1 ),
		Vector3i( 0,-1,-1 ),
		Vector3i( 0,-1, 0 ),
	};

	char Block = m_World->GetBlock( a_BlockPos );

	// First check whether torch should be on or off
	if( Block == E_BLOCK_REDSTONE_TORCH_ON || Block == E_BLOCK_REDSTONE_TORCH_OFF )
	{
		static Vector3i Surroundings [] = {
			Vector3i(-1, 0, 0),
			Vector3i( 1, 0, 0),
			Vector3i( 0, 0,-1),
			Vector3i( 0, 0, 1),
			Vector3i( 0,-1, 0),
		};
		for( unsigned int i = 0; i < ARRAYCOUNT( Surroundings ); ++i )
		{
			Vector3i pos = a_BlockPos + Surroundings[i];
			char OtherBlock = m_World->GetBlock( pos );
			if( (OtherBlock != E_BLOCK_AIR) && (OtherBlock != E_BLOCK_REDSTONE_TORCH_ON) && (OtherBlock != E_BLOCK_REDSTONE_TORCH_OFF) )
			{
				RefreshTorchesAround( pos );
			}
		}
		Block = m_World->GetBlock( a_BlockPos ); // Make sure we got the updated block
	}

	BlockList Sources;
	// If torch is still on, use it as a source
	if( Block == E_BLOCK_REDSTONE_TORCH_ON )
	{
		Sources.push_back( a_BlockPos );
	}

	// Power all blocks legally connected to the sources
	{
		BlockList NewSources = RemoveCurrent( a_BlockPos );
		Sources.insert( Sources.end(), NewSources.begin(), NewSources.end() );
		while( !Sources.empty() )
		{
			static Vector3i Surroundings [] = {
				Vector3i(-1, 0, 0),
				Vector3i( 1, 0, 0),
				Vector3i( 0, 0,-1),
				Vector3i( 0, 0, 1),
			};

			Vector3i SourcePos = Sources.back();
			for( unsigned int i = 0; i < ARRAYCOUNT( Surroundings ); ++i )
			{
				Vector3i OtherPos =  SourcePos + Surroundings[i];
				if( PowerBlock( OtherPos, a_BlockPos, 0xf ) )
				{
					SpreadStack.push_back( OtherPos ); // Changed, so add to stack
				}
			}
			Sources.pop_back();
		}
	}


	// Do a floodfill
	while( !SpreadStack.empty() )
	{
		Vector3i pos = SpreadStack.back();
		SpreadStack.pop_back();

		char Meta = m_World->GetBlockMeta( pos );

		for( unsigned int i = 0; i < ARRAYCOUNT( Surroundings ); ++i )
		{
			Vector3i OtherPos =  pos + Surroundings[i];
			if( PowerBlock( OtherPos, pos, Meta-1 ) )
			{
				SpreadStack.push_back( OtherPos ); // Changed, so add to stack
			}
		}
	}

	// Only after a redstone area has been completely simulated the redstone entities can react
	while( !m_RefreshPistons.empty() )
	{
		Vector3i pos = m_RefreshPistons.back();
		m_RefreshPistons.pop_back();

		char Block = m_World->GetBlock( pos );
		switch( Block )
		{
		case E_BLOCK_PISTON:
			if( IsPowered( pos ) )
			{
				cPiston Piston( m_World );
				Piston.ExtendPiston( pos.x, pos.y, pos.z );
			}
			else
			{
				cPiston Piston( m_World );
				Piston.RetractPiston( pos.x, pos.y, pos.z );
			}
			break;
		default:
			break;
		};
	}
}





bool cRedstoneSimulator::PowerBlock( const Vector3i & a_BlockPos, const Vector3i & a_FromBlock, char a_Power )
{
	char Block = m_World->GetBlock( a_BlockPos );
	switch( Block )
	{
	case E_BLOCK_REDSTONE_WIRE:
		{
			if( m_World->GetBlockMeta( a_BlockPos ) < a_Power )
			{
				m_World->SetBlockMeta( a_BlockPos, a_Power );
				return true;
			}
		}
		break;
	case E_BLOCK_PISTON:
		{
			m_RefreshPistons.push_back( a_BlockPos );
		}
		break;
	default:
		{
			if( Block != E_BLOCK_AIR && Block != E_BLOCK_REDSTONE_TORCH_ON && Block != E_BLOCK_REDSTONE_TORCH_OFF )
			{
				if( IsPowered( a_BlockPos, true ) )
				{
					m_RefreshTorchesAround.push_back( a_BlockPos );
				}
			}
		}
		break;
	};

	return false;
}





int cRedstoneSimulator::UnPowerBlock( const Vector3i & a_BlockPos )
{
	char Block = m_World->GetBlock( a_BlockPos );
	switch( Block )
	{
	case E_BLOCK_REDSTONE_WIRE:
		{
			if( m_World->GetBlockMeta( a_BlockPos ) > 0 )
			{
				m_World->SetBlockMeta( a_BlockPos, 0 );
				return 1;
			}
		}
		break;
	case E_BLOCK_PISTON:
		{
			m_RefreshPistons.push_back( a_BlockPos );
		}
		break;
	case E_BLOCK_REDSTONE_TORCH_ON:
		{
			return 2;
		}
		break;
	default:
		if( Block != E_BLOCK_AIR && Block != E_BLOCK_REDSTONE_TORCH_ON && Block != E_BLOCK_REDSTONE_TORCH_OFF )
		{
			if( !IsPowered( a_BlockPos, true ) )
			{
				m_RefreshTorchesAround.push_back( a_BlockPos );
			}
		}
		break;
	};

	return 0;
}





// Removes current from all powered redstone wires until it reaches an energy source.
// Also returns all energy sources it encountered
cRedstoneSimulator::BlockList cRedstoneSimulator::RemoveCurrent( const Vector3i & a_BlockPos )
{
	std::deque< Vector3i > SpreadStack;
	std::deque< Vector3i > FoundSources;

	Vector3i Surroundings[] = {
		Vector3i( 1, 0, 0 ),
		Vector3i( 1, 1, 0 ),
		Vector3i( 1,-1, 0 ),
		Vector3i(-1, 0, 0 ),
		Vector3i(-1, 1, 0 ),
		Vector3i(-1,-1, 0 ),
		Vector3i( 0, 0, 1 ),
		Vector3i( 0, 1, 1 ),
		Vector3i( 0,-1, 1 ),
		Vector3i( 0, 0,-1 ),
		Vector3i( 0, 1,-1 ),
		Vector3i( 0,-1,-1 ),
		Vector3i( 0,-1, 0 ),
	};

	char Block = m_World->GetBlock( a_BlockPos );
	if( Block == E_BLOCK_REDSTONE_TORCH_OFF || Block == E_BLOCK_REDSTONE_TORCH_ON )
	{
		static Vector3i Surroundings [] = {	// Torches only spread on the same level
			Vector3i(-1, 0, 0),
			Vector3i( 1, 0, 0),
			Vector3i( 0, 0,-1),
			Vector3i( 0, 0, 1),
		};

		for( unsigned int i = 0; i < ARRAYCOUNT( Surroundings ); ++i )
		{
			Vector3i pos = Vector3i( a_BlockPos ) + Surroundings[i];
			int RetVal = UnPowerBlock( pos );
			if( RetVal == 1 )
			{
				SpreadStack.push_back( pos ); // Changed, so add to stack
			}
			else if( RetVal == 2 )
			{
				FoundSources.push_back( pos );
			}
		}
	}
	else
	{
		SpreadStack.push_back( a_BlockPos );
	}


	while( !SpreadStack.empty() )
	{
		Vector3i pos = SpreadStack.back();
		SpreadStack.pop_back();

		for( unsigned int i = 0; i < ARRAYCOUNT( Surroundings ); ++i )
		{
			Vector3i OtherPos =  pos + Surroundings[i];
			int RetVal = UnPowerBlock( OtherPos );
			if( RetVal == 1 )
			{
				SpreadStack.push_back( OtherPos ); // Changed, so add to stack
			}
			else if( RetVal == 2 )
			{
				FoundSources.push_back( OtherPos );
			}
		}
	}

	return FoundSources;
}





bool cRedstoneSimulator::IsPowered( const Vector3i & a_BlockPos, bool a_bOnlyByWire /* = false */ )
{
	char NegX = m_World->GetBlock( a_BlockPos.x-1, a_BlockPos.y, a_BlockPos.z );
	if( !a_bOnlyByWire && NegX == E_BLOCK_REDSTONE_TORCH_ON )	return true;
	if( NegX == E_BLOCK_REDSTONE_WIRE )
	{
		if( m_World->GetBlockMeta( a_BlockPos.x-1, a_BlockPos.y, a_BlockPos.z ) > 0 )
		{
			if( GetDirection( a_BlockPos.x-1, a_BlockPos.y, a_BlockPos.z ) == REDSTONE_X_POS )
			{
				return true;
			}
		}
	}
	char PosX = m_World->GetBlock( a_BlockPos.x+1, a_BlockPos.y, a_BlockPos.z );
	if( !a_bOnlyByWire && PosX == E_BLOCK_REDSTONE_TORCH_ON )	return true;
	if( PosX == E_BLOCK_REDSTONE_WIRE )
	{
		if( m_World->GetBlockMeta( a_BlockPos.x+1, a_BlockPos.y, a_BlockPos.z ) > 0 )
		{
			if( GetDirection( a_BlockPos.x+1, a_BlockPos.y, a_BlockPos.z ) == REDSTONE_X_NEG )
			{
				return true;
			}
		}
	}
	char NegZ = m_World->GetBlock( a_BlockPos.x, a_BlockPos.y, a_BlockPos.z-1 );
	if( !a_bOnlyByWire && NegZ == E_BLOCK_REDSTONE_TORCH_ON )	return true;
	if( NegZ == E_BLOCK_REDSTONE_WIRE )
	{
		if( m_World->GetBlockMeta( a_BlockPos.x, a_BlockPos.y, a_BlockPos.z-1 ) > 0 )
		{
			if( GetDirection( a_BlockPos.x, a_BlockPos.y, a_BlockPos.z-1 ) == REDSTONE_Z_POS )
			{
				return true;
			}
		}
	}
	char PosZ = m_World->GetBlock( a_BlockPos.x, a_BlockPos.y, a_BlockPos.z+1 );
	if( !a_bOnlyByWire && PosZ == E_BLOCK_REDSTONE_TORCH_ON )	return true;
	if( PosZ == E_BLOCK_REDSTONE_WIRE )
	{
		if( m_World->GetBlockMeta( a_BlockPos.x, a_BlockPos.y, a_BlockPos.z+1 ) > 0 )
		{
			if( GetDirection( a_BlockPos.x, a_BlockPos.y, a_BlockPos.z+1 ) == REDSTONE_Z_NEG )
			{
				return true;
			}
		}
	}

	// Only wires can power the bottom block
	char PosY = m_World->GetBlock( a_BlockPos.x, a_BlockPos.y+1, a_BlockPos.z );
	if( PosY == E_BLOCK_REDSTONE_WIRE )
	{
		if( m_World->GetBlockMeta( a_BlockPos.x, a_BlockPos.y+1, a_BlockPos.z ) > 0 )
		{
			return true;
		}
	}

	return false;
}




// Believe me, it works!! 
cRedstoneSimulator::eRedstoneDirection cRedstoneSimulator::GetDirection( int a_X, int a_Y, int a_Z )
{
	int Dir = REDSTONE_NONE;

	char NegX = m_World->GetBlock( a_X-1, a_Y, a_Z );
	if( NegX == E_BLOCK_REDSTONE_WIRE || NegX == E_BLOCK_REDSTONE_TORCH_ON )
	{
		Dir |= (REDSTONE_X_POS);
	}
	char PosX = m_World->GetBlock( a_X+1, a_Y, a_Z );
	if( PosX == E_BLOCK_REDSTONE_WIRE || PosX == E_BLOCK_REDSTONE_TORCH_ON )
	{
		Dir |= (REDSTONE_X_NEG);
	}
	char NegZ = m_World->GetBlock( a_X, a_Y, a_Z-1 );
	if( NegZ == E_BLOCK_REDSTONE_WIRE || NegZ == E_BLOCK_REDSTONE_TORCH_ON )
	{
		if( (Dir & REDSTONE_X_POS) && !(Dir & REDSTONE_X_NEG ) ) //corner
		{
			Dir ^= REDSTONE_X_POS;
			Dir |= REDSTONE_X_NEG;
		}
		if( (Dir & REDSTONE_X_NEG) && !(Dir & REDSTONE_X_POS ) ) //corner
		{
			Dir ^= REDSTONE_X_NEG;
			Dir |= REDSTONE_X_POS;
		}
		Dir |= REDSTONE_Z_POS;
	}
	char PosZ = m_World->GetBlock( a_X, a_Y, a_Z+1 );
	if( PosZ == E_BLOCK_REDSTONE_WIRE || PosZ == E_BLOCK_REDSTONE_TORCH_ON )
	{
		if( (Dir & REDSTONE_X_POS) && !(Dir & REDSTONE_X_NEG ) ) //corner
		{
			Dir ^= REDSTONE_X_POS;
			Dir |= REDSTONE_X_NEG;
		}
		if( (Dir & REDSTONE_X_NEG) && !(Dir & REDSTONE_X_POS ) ) //corner
		{
			Dir ^= REDSTONE_X_NEG;
			Dir |= REDSTONE_X_POS;
		}
		Dir |= REDSTONE_Z_NEG;
	}

	return (eRedstoneDirection)Dir;
}