#include "cRedstone.h"
#include "cPiston.h"
#include "cRoot.h"
#include "cWorld.h"
#include "BlockID.h"

cRedstone::cRedstone( cWorld* a_World )
	:m_World ( a_World )
{

}

void cRedstone::ChangeRedstoneTorch( int fillx, int filly, int fillz, bool added )
{
        char metadata = 0;
        if (added) {
                metadata = 15;
        } else {
                metadata = 0;
        }

	//cWorld* World = cRoot::Get()->GetWorld();

        if ( ( (int)m_World->GetBlock( fillx+1, filly, fillz ) == E_BLOCK_REDSTONE_WIRE)  ) {
                LightRedstone( fillx+1, filly, fillz, metadata );
        }
        if ( ( (int)m_World->GetBlock( fillx-1, filly, fillz ) == E_BLOCK_REDSTONE_WIRE)  ) {
                LightRedstone( fillx-1, filly, fillz, metadata );
        }
        if ( ( (int)m_World->GetBlock( fillx, filly, fillz+1 ) == E_BLOCK_REDSTONE_WIRE)  ) {
                LightRedstone( fillx, filly, fillz+1, metadata );
        }
        if ( ( (int)m_World->GetBlock( fillx, filly, fillz-1 ) == E_BLOCK_REDSTONE_WIRE)  ) {
                LightRedstone( fillx, filly, fillz-1, metadata );
        }

        if ( ( (int)m_World->GetBlock( fillx+1, filly, fillz ) == E_BLOCK_REDSTONE_WIRE)  ) {
                LightRedstone( fillx+1, filly-1, fillz, metadata );
        }
        if ( ( (int)m_World->GetBlock( fillx-1, filly, fillz ) == E_BLOCK_REDSTONE_WIRE)  ) {
                LightRedstone( fillx-1, filly-1, fillz, metadata );
        }
        if ( ( (int)m_World->GetBlock( fillx, filly, fillz+1 ) == E_BLOCK_REDSTONE_WIRE)  ) {
                LightRedstone( fillx, filly-1, fillz+1, metadata );
        }
        if ( ( (int)m_World->GetBlock( fillx, filly, fillz-1 ) == E_BLOCK_REDSTONE_WIRE)  ) {
                LightRedstone( fillx, filly-1, fillz-1, metadata );
        }

        if ( ( (int)m_World->GetBlock( fillx+1, filly, fillz ) == E_BLOCK_REDSTONE_WIRE)  ) {
                LightRedstone( fillx+1, filly+1, fillz, metadata );
        }
        if ( ( (int)m_World->GetBlock( fillx-1, filly, fillz ) == E_BLOCK_REDSTONE_WIRE)  ) {
                LightRedstone( fillx-1, filly+1, fillz, metadata );
        }
        if ( ( (int)m_World->GetBlock( fillx, filly, fillz+1 ) == E_BLOCK_REDSTONE_WIRE)  ) {
                LightRedstone( fillx, filly+1, fillz+1, metadata );
        }
        if ( ( (int)m_World->GetBlock( fillx, filly, fillz-1 ) == E_BLOCK_REDSTONE_WIRE)  ) {
                LightRedstone( fillx, filly+1, fillz-1, metadata );
        }
}

void cRedstone::LightRedstone( int fillx, int filly, int fillz, char metadata)
{
	if ( ( (int)m_World->GetBlock( fillx, filly, fillz ) == E_BLOCK_STICKY_PISTON ) || ( (int)m_World->GetBlock( fillx, filly, fillz ) == E_BLOCK_PISTON ) ) {
		printf("rsPiston 1\n");
		cPiston Piston(m_World);
		if (metadata > 0) {
			printf("rsPiston 2\n");
			Piston.ExtendPiston(fillx, filly, fillz);
		} else {
			printf("rsPiston 3\n");
			Piston.RetractPiston(fillx, filly, fillz);
		}
	} else if ( ( (int)m_World->GetBlock( fillx, filly, fillz ) == E_BLOCK_REDSTONE_WIRE) && ( (int)m_World->GetBlockMeta( fillx, filly, fillz) != metadata ) ) {
	        m_World->FastSetBlock( fillx, filly, fillz, (char)E_BLOCK_REDSTONE_WIRE, metadata );
       	 	LightRedstone(fillx-1,filly,fillz,metadata);
       	 	LightRedstone(fillx+1,filly,fillz,metadata);
       		LightRedstone(fillx,filly,fillz-1,metadata);
       		LightRedstone(fillx,filly,fillz+1,metadata);

        	LightRedstone(fillx-1,filly-1,fillz,metadata);
        	LightRedstone(fillx+1,filly-1,fillz,metadata);
        	LightRedstone(fillx,filly-1,fillz-1,metadata);
        	LightRedstone(fillx,filly-1,fillz+1,metadata);

        	LightRedstone(fillx-1,filly+1,fillz,metadata);
        	LightRedstone(fillx+1,filly+1,fillz,metadata);
        	LightRedstone(fillx,filly+1,fillz-1,metadata);
        	LightRedstone(fillx,filly+1,fillz+1,metadata);
	}

}
