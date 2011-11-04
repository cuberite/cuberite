#include "cRedstone.h"
#include "cWorld.h"


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
        if ( ( m_World->GetWorld()->GetBlock( fillx+1, filly, fillz ) == E_BLOCK_REDSTONE_WIRE)  ) {
                LightRedstone( fillx+1, filly, fillz, metadata );
        }
        if ( ((int)m_World->GetWorld()->GetBlock( fillx-1, filly, fillz ) == E_BLOCK_REDSTONE_WIRE)  ) {
                LightRedstone( fillx-1, filly, fillz, metadata );
        }
        if ( ((int)m_World->GetWorld()->GetBlock( fillx, filly, fillz+1 ) == E_BLOCK_REDSTONE_WIRE)  ) {
                LightRedstone( fillx, filly, fillz+1, metadata );
        }
        if ( ((int)m_World->GetWorld()->GetBlock( fillx, filly, fillz-1 ) == E_BLOCK_REDSTONE_WIRE)  ) {
                LightRedstone( fillx, filly, fillz-1, metadata );
        }

        if ( ((int)m_World->GetWorld()->GetBlock( fillx+1, filly, fillz ) == E_BLOCK_REDSTONE_WIRE)  ) {
                LightRedstone( fillx+1, filly-1, fillz, metadata );
        }
        if ( ((int)m_World->GetWorld()->GetBlock( fillx-1, filly, fillz ) == E_BLOCK_REDSTONE_WIRE)  ) {
                LightRedstone( fillx-1, filly-1, fillz, metadata );
        }
        if ( ((int)m_World->GetWorld()->GetBlock( fillx, filly, fillz+1 ) == E_BLOCK_REDSTONE_WIRE)  ) {
                LightRedstone( fillx, filly-1, fillz+1, metadata );
        }
        if ( ((int)m_World->GetWorld()->GetBlock( fillx, filly, fillz-1 ) == E_BLOCK_REDSTONE_WIRE)  ) {
                LightRedstone( fillx, filly-1, fillz-1, metadata );
        }

        if ( ((int)m_World->GetWorld()->GetBlock( fillx+1, filly, fillz ) == E_BLOCK_REDSTONE_WIRE)  ) {
                LightRedstone( fillx+1, filly+1, fillz, metadata );
        }
        if ( ((int)m_World->GetWorld()->GetBlock( fillx-1, filly, fillz ) == E_BLOCK_REDSTONE_WIRE)  ) {
                LightRedstone( fillx-1, filly+1, fillz, metadata );
        }
        if ( ((int)m_World->GetWorld()->GetBlock( fillx, filly, fillz+1 ) == E_BLOCK_REDSTONE_WIRE)  ) {
                LightRedstone( fillx, filly+1, fillz+1, metadata );
        }
        if ( ((int)m_World->GetWorld()->GetBlock( fillx, filly, fillz-1 ) == E_BLOCK_REDSTONE_WIRE)  ) {
                LightRedstone( fillx, filly+1, fillz-1, metadata );
        }
}

void cRedstone::LightRedstone( int fillx, int filly, int fillz, char metadata)
{
    if ( ( (int)m_World->GetWorld()->GetBlock( fillx, filly, fillz ) == E_BLOCK_REDSTONE_WIRE) && ( (int)m_World->GetWorld()->GetBlockMeta( fillx, filly, fillz) != metadata ) ) {
        m_World->GetWorld()->SetBlock( fillx, filly, fillz, (char)E_BLOCK_REDSTONE_WIRE, metadata );
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
