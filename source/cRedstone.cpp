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

void cRedstone::ChangeRedstoneTorch( int fillx, int filly, int fillz, bool added )
{
        if (added) {
                m_Metadata = 15;
        } else {
                m_Metadata = 0;
        }

		LightRedstone( fillx, filly, fillz );

		if ( (int)m_World->GetBlock( fillx, filly, fillz ) == 0 ) { //we removed an item

			LightRedstone( fillx+1, filly, fillz );
			m_Metadata = 0;
			LightRedstone( fillx-1, filly, fillz );
			m_Metadata = 0;
			LightRedstone( fillx, filly, fillz+1 );
			m_Metadata = 0;
			LightRedstone( fillx, filly, fillz-1 );
			m_Metadata = 0;
 
		}
		
		if ( (int)m_World->GetBlock( fillx, filly, fillz ) == E_BLOCK_REDSTONE_WIRE ) { //we added dust

			LightRedstone( fillx+1, filly, fillz );
			LightRedstone( fillx-1, filly, fillz );
			LightRedstone( fillx, filly, fillz+1 );
			LightRedstone( fillx, filly, fillz-1 );
 
		}
 
}

void cRedstone::LightRedstone( int fillx, int filly, int fillz)
{

	if ( ( (int)m_World->GetBlock( fillx, filly, fillz ) == E_BLOCK_STICKY_PISTON ) || ( (int)m_World->GetBlock( fillx, filly, fillz ) == E_BLOCK_PISTON ) ) {
		cPiston Piston(m_World);
		if (m_Metadata > 0) {
			Piston.ExtendPiston(fillx, filly, fillz);
		} else {
			Piston.RetractPiston(fillx, filly, fillz);
		}
	}

	if ( (int)m_World->GetBlock( fillx, filly, fillz ) == E_BLOCK_REDSTONE_TORCH_OFF ) { //I don't think notch knows on from off. >.>
		m_Metadata = 15;
		if ( ( (int)m_World->GetBlock( fillx-1, filly, fillz ) == E_BLOCK_REDSTONE_WIRE) && ( (int)m_World->GetBlockMeta( fillx-1, filly, fillz) != m_Metadata ) ) {
			LightRedstone(fillx-1,filly,fillz);
		}
		if ( ( (int)m_World->GetBlock( fillx+1, filly, fillz ) == E_BLOCK_REDSTONE_WIRE) && ( (int)m_World->GetBlockMeta( fillx+1, filly, fillz) != m_Metadata ) ) {
			LightRedstone(fillx+1,filly,fillz);
		}
		if ( ( (int)m_World->GetBlock( fillx, filly, fillz-1 ) == E_BLOCK_REDSTONE_WIRE) && ( (int)m_World->GetBlockMeta( fillx, filly, fillz-1) != m_Metadata ) ) {
			LightRedstone(fillx,filly,fillz-1);
		}
		if ( ( (int)m_World->GetBlock( fillx, filly, fillz+1 ) == E_BLOCK_REDSTONE_WIRE) && ( (int)m_World->GetBlockMeta( fillx, filly, fillz+1) != m_Metadata ) ) {
			LightRedstone(fillx,filly,fillz+1);
		}
	} else if ( (int)m_World->GetBlock( fillx, filly, fillz ) == E_BLOCK_REDSTONE_TORCH_ON ) { //if the torch is off
		if ( ( (int)m_World->GetBlock( fillx-1, filly, fillz ) == E_BLOCK_REDSTONE_WIRE) && ( (int)m_World->GetBlockMeta( fillx-1, filly, fillz) != m_Metadata ) ) {
			LightRedstone(fillx-1,filly,fillz);
		}
		if ( ( (int)m_World->GetBlock( fillx+1, filly, fillz ) == E_BLOCK_REDSTONE_WIRE) && ( (int)m_World->GetBlockMeta( fillx+1, filly, fillz) != m_Metadata ) ) {
			LightRedstone(fillx+1,filly,fillz);
		}
		if ( ( (int)m_World->GetBlock( fillx, filly, fillz-1 ) == E_BLOCK_REDSTONE_WIRE) && ( (int)m_World->GetBlockMeta( fillx, filly, fillz-1) != m_Metadata ) ) {
			LightRedstone(fillx,filly,fillz-1);
		}
		if ( ( (int)m_World->GetBlock( fillx, filly, fillz+1 ) == E_BLOCK_REDSTONE_WIRE) && ( (int)m_World->GetBlockMeta( fillx, filly, fillz+1) != m_Metadata ) ) {
			LightRedstone(fillx,filly,fillz+1);
		}
	}


	if ( ( (int)m_World->GetBlock( fillx, filly, fillz ) == E_BLOCK_REDSTONE_WIRE) && ( (int)m_World->GetBlockMeta( fillx, filly, fillz) != m_Metadata ) ) {
	        m_World->FastSetBlock( fillx, filly, fillz, (char)E_BLOCK_REDSTONE_WIRE, m_Metadata );
       	 	LightRedstone(fillx-1,filly,fillz);
       	 	LightRedstone(fillx+1,filly,fillz);
       		LightRedstone(fillx,filly,fillz-1);
       		LightRedstone(fillx,filly,fillz+1);

        	LightRedstone(fillx-1,filly-1,fillz);
        	LightRedstone(fillx+1,filly-1,fillz);
        	LightRedstone(fillx,filly-1,fillz-1);
        	LightRedstone(fillx,filly-1,fillz+1);

        	LightRedstone(fillx-1,filly+1,fillz);
        	LightRedstone(fillx+1,filly+1,fillz);
        	LightRedstone(fillx,filly+1,fillz-1);
        	LightRedstone(fillx,filly+1,fillz+1);
	}

}