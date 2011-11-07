#include "cRedstone.h"
#include "cPiston.h"
#include "cRoot.h"
#include "cWorld.h"
#include "BlockID.h"

cRedstone::cRedstone( cWorld* a_World )
	:m_World ( a_World )
	,m_Metadata ( 0 )
{

}

void cRedstone::ChangeRedstone( int fillx, int filly, int fillz, bool added )
{
        if (added) {
                m_Metadata = 15;
        } else {
                m_Metadata = 0;
        }

		CalculatetRedstone( fillx, filly, fillz );

		if ( ( (int)m_World->GetBlock( fillx, filly, fillz ) == E_BLOCK_AIR ) || ( (int)m_World->GetBlock( fillx, filly, fillz ) == E_BLOCK_PISTON_EXTENSION ) ) {

			m_Metadata = 0;
			CalculatetRedstone( fillx+1, filly, fillz );
			m_Metadata = 0;
			CalculatetRedstone( fillx-1, filly, fillz );
			m_Metadata = 0;
			CalculatetRedstone( fillx, filly, fillz+1 );
			m_Metadata = 0;
			CalculatetRedstone( fillx, filly, fillz-1 );
			m_Metadata = 0;

			CalculatetRedstone( fillx+1, filly+1, fillz );
			m_Metadata = 0;
			CalculatetRedstone( fillx-1, filly+1, fillz );
			m_Metadata = 0;
			CalculatetRedstone( fillx, filly+1, fillz+1 );
			m_Metadata = 0;
			CalculatetRedstone( fillx, filly+1, fillz-1 );
			m_Metadata = 0;
			
			CalculatetRedstone( fillx+1, filly-1, fillz );
			m_Metadata = 0;
			CalculatetRedstone( fillx-1, filly-1, fillz );
			m_Metadata = 0;
			CalculatetRedstone( fillx, filly-1, fillz+1 );
			m_Metadata = 0;
			CalculatetRedstone( fillx, filly-1, fillz-1 );
			m_Metadata = 0;

		}
		
		if ( (int)m_World->GetBlock( fillx, filly, fillz ) == E_BLOCK_REDSTONE_WIRE ) { //we added dust

			CalculatetRedstone( fillx+1, filly, fillz );
			CalculatetRedstone( fillx-1, filly, fillz );
			CalculatetRedstone( fillx, filly, fillz+1 );
			CalculatetRedstone( fillx, filly, fillz-1 );
			
			CalculatetRedstone( fillx+1, filly+1, fillz );
			CalculatetRedstone( fillx-1, filly+1, fillz );
			CalculatetRedstone( fillx, filly+1, fillz+1 );
			CalculatetRedstone( fillx, filly+1, fillz-1 );
			
			CalculatetRedstone( fillx+1, filly-1, fillz );
			CalculatetRedstone( fillx-1, filly-1, fillz );
			CalculatetRedstone( fillx, filly-1, fillz+1 );
			CalculatetRedstone( fillx, filly-1, fillz-1 );
 
		}
		
		if ( ( (int)m_World->GetBlock( fillx, filly, fillz ) == E_BLOCK_PISTON ) || ( (int)m_World->GetBlock( fillx, filly, fillz ) == E_BLOCK_STICKY_PISTON ) ) { //we added a piston

			m_Metadata = 0;
			CalculatetRedstone( fillx+1, filly, fillz );
			m_Metadata = 0;
			CalculatetRedstone( fillx-1, filly, fillz );
			m_Metadata = 0;
			CalculatetRedstone( fillx, filly, fillz+1 );
			m_Metadata = 0;
			CalculatetRedstone( fillx, filly, fillz-1 );
			m_Metadata = 0;

			CalculatetRedstone( fillx+1, filly+1, fillz );
			m_Metadata = 0;
			CalculatetRedstone( fillx-1, filly+1, fillz );
			m_Metadata = 0;
			CalculatetRedstone( fillx, filly+1, fillz+1 );
			m_Metadata = 0;
			CalculatetRedstone( fillx, filly+1, fillz-1 );
			m_Metadata = 0;
			
			CalculatetRedstone( fillx+1, filly-1, fillz );
			m_Metadata = 0;
			CalculatetRedstone( fillx-1, filly-1, fillz );
			m_Metadata = 0;
			CalculatetRedstone( fillx, filly-1, fillz+1 );
			m_Metadata = 0;
			CalculatetRedstone( fillx, filly-1, fillz-1 );
			m_Metadata = 0;
 
		}
 
}

void cRedstone::CalculatetRedstone( int fillx, int filly, int fillz)
{

	if ( ( (int)m_World->GetBlock( fillx, filly, fillz ) == E_BLOCK_STICKY_PISTON ) || ( (int)m_World->GetBlock( fillx, filly, fillz ) == E_BLOCK_PISTON ) ) {
		char pistonMeta = m_World->GetBlockMeta( fillx, filly, fillz );
		if (m_Metadata > 0) {
			if (pistonMeta < 6) { // only extend if piston is not already extended
				cPiston Piston(m_World);
				Piston.ExtendPiston(fillx, filly, fillz);
			}	
		} else {
			if (pistonMeta > 6) { // only retract if piston is not already retracted
				cPiston Piston(m_World);
				Piston.RetractPiston(fillx, filly, fillz);
			}
		}
	}

	if ( (int)m_World->GetBlock( fillx, filly, fillz ) == E_BLOCK_REDSTONE_TORCH_ON ) { //If torch is on
		m_Metadata = 15;
		if ( ( (int)m_World->GetBlock( fillx-1, filly, fillz ) == E_BLOCK_REDSTONE_WIRE) && ( (int)m_World->GetBlockMeta( fillx-1, filly, fillz) != m_Metadata ) ) {
			CalculatetRedstone(fillx-1,filly,fillz);
		}
		if ( ( (int)m_World->GetBlock( fillx+1, filly, fillz ) == E_BLOCK_REDSTONE_WIRE) && ( (int)m_World->GetBlockMeta( fillx+1, filly, fillz) != m_Metadata ) ) {
			CalculatetRedstone(fillx+1,filly,fillz);
		}
		if ( ( (int)m_World->GetBlock( fillx, filly, fillz-1 ) == E_BLOCK_REDSTONE_WIRE) && ( (int)m_World->GetBlockMeta( fillx, filly, fillz-1) != m_Metadata ) ) {
			CalculatetRedstone(fillx,filly,fillz-1);
		}
		if ( ( (int)m_World->GetBlock( fillx, filly, fillz+1 ) == E_BLOCK_REDSTONE_WIRE) && ( (int)m_World->GetBlockMeta( fillx, filly, fillz+1) != m_Metadata ) ) {
			CalculatetRedstone(fillx,filly,fillz+1);
		}
	} else if ( (int)m_World->GetBlock( fillx, filly, fillz ) == E_BLOCK_REDSTONE_TORCH_OFF ) { //if the torch is off
		if ( ( (int)m_World->GetBlock( fillx-1, filly, fillz ) == E_BLOCK_REDSTONE_WIRE) && ( (int)m_World->GetBlockMeta( fillx-1, filly, fillz) != m_Metadata ) ) {
			CalculatetRedstone(fillx-1,filly,fillz);
		}
		if ( ( (int)m_World->GetBlock( fillx+1, filly, fillz ) == E_BLOCK_REDSTONE_WIRE) && ( (int)m_World->GetBlockMeta( fillx+1, filly, fillz) != m_Metadata ) ) {
			CalculatetRedstone(fillx+1,filly,fillz);
		}
		if ( ( (int)m_World->GetBlock( fillx, filly, fillz-1 ) == E_BLOCK_REDSTONE_WIRE) && ( (int)m_World->GetBlockMeta( fillx, filly, fillz-1) != m_Metadata ) ) {
			CalculatetRedstone(fillx,filly,fillz-1);
		}
		if ( ( (int)m_World->GetBlock( fillx, filly, fillz+1 ) == E_BLOCK_REDSTONE_WIRE) && ( (int)m_World->GetBlockMeta( fillx, filly, fillz+1) != m_Metadata ) ) {
			CalculatetRedstone(fillx,filly,fillz+1);
		}
	}


	if ( ( (int)m_World->GetBlock( fillx, filly, fillz ) == E_BLOCK_REDSTONE_WIRE) && ( (int)m_World->GetBlockMeta( fillx, filly, fillz) != m_Metadata ) ) {
	        m_World->FastSetBlock( fillx, filly, fillz, (char)E_BLOCK_REDSTONE_WIRE, m_Metadata );
       	 	CalculatetRedstone(fillx-1,filly,fillz);
       	 	CalculatetRedstone(fillx+1,filly,fillz);
       		CalculatetRedstone(fillx,filly,fillz-1);
       		CalculatetRedstone(fillx,filly,fillz+1);

        	CalculatetRedstone(fillx-1,filly-1,fillz);
        	CalculatetRedstone(fillx+1,filly-1,fillz);
        	CalculatetRedstone(fillx,filly-1,fillz-1);
        	CalculatetRedstone(fillx,filly-1,fillz+1);

        	CalculatetRedstone(fillx-1,filly+1,fillz);
        	CalculatetRedstone(fillx+1,filly+1,fillz);
        	CalculatetRedstone(fillx,filly+1,fillz-1);
        	CalculatetRedstone(fillx,filly+1,fillz+1);
	}

}