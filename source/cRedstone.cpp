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
        char metadata = 0;
        if (added) {
                metadata = 15;
                m_Metadata = 15;
        } else {
                metadata = 0;
                m_Metadata = 0;
        }

		LightRedstone( fillx, filly, fillz, metadata );

		if ( (int)m_World->GetBlock( fillx, filly, fillz ) == 0 ) { //we removed an item

				//se we check each possible current connection around it.
        //if ( ( (int)m_World->GetBlock( fillx+1, filly, fillz ) == E_BLOCK_REDSTONE_WIRE)  ) {
                LightRedstone( fillx+1, filly, fillz, m_Metadata );
        //}
        //if ( ( (int)m_World->GetBlock( fillx-1, filly, fillz ) == E_BLOCK_REDSTONE_WIRE)  ) {
                LightRedstone( fillx-1, filly, fillz, m_Metadata );
        //}
        //if ( ( (int)m_World->GetBlock( fillx, filly, fillz+1 ) == E_BLOCK_REDSTONE_WIRE)  ) {
                LightRedstone( fillx, filly, fillz+1, m_Metadata );
        //}
        //if ( ( (int)m_World->GetBlock( fillx, filly, fillz-1 ) == E_BLOCK_REDSTONE_WIRE)  ) {
                LightRedstone( fillx, filly, fillz-1, m_Metadata );
        //}
 
		}

	//cWorld* World = cRoot::Get()->GetWorld();
/*
		while ((int)metadata == 0) {
        if ( ( (int)m_World->GetBlock( fillx+1, filly, fillz ) == E_BLOCK_REDSTONE_WIRE)  ) {
                metadata = LightRedstone( fillx+1, filly, fillz, metadata );
        }
        if ( ( (int)m_World->GetBlock( fillx-1, filly, fillz ) == E_BLOCK_REDSTONE_WIRE)  ) {
                metadata = LightRedstone( fillx-1, filly, fillz, metadata );
        }
        if ( ( (int)m_World->GetBlock( fillx, filly, fillz+1 ) == E_BLOCK_REDSTONE_WIRE)  ) {
                metadata = LightRedstone( fillx, filly, fillz+1, metadata );
        }
        if ( ( (int)m_World->GetBlock( fillx, filly, fillz-1 ) == E_BLOCK_REDSTONE_WIRE)  ) {
                metadata = LightRedstone( fillx, filly, fillz-1, metadata );
        }

        if ( ( (int)m_World->GetBlock( fillx+1, filly, fillz ) == E_BLOCK_REDSTONE_WIRE)  ) {
                metadata = LightRedstone( fillx+1, filly-1, fillz, metadata );
        }
        if ( ( (int)m_World->GetBlock( fillx-1, filly, fillz ) == E_BLOCK_REDSTONE_WIRE)  ) {
                metadata = LightRedstone( fillx-1, filly-1, fillz, metadata );
        }
        if ( ( (int)m_World->GetBlock( fillx, filly, fillz+1 ) == E_BLOCK_REDSTONE_WIRE)  ) {
                metadata = LightRedstone( fillx, filly-1, fillz+1, metadata );
        }
        if ( ( (int)m_World->GetBlock( fillx, filly, fillz-1 ) == E_BLOCK_REDSTONE_WIRE)  ) {
                metadata = LightRedstone( fillx, filly-1, fillz-1, metadata );
        }

        if ( ( (int)m_World->GetBlock( fillx+1, filly, fillz ) == E_BLOCK_REDSTONE_WIRE)  ) {
                metadata = LightRedstone( fillx+1, filly+1, fillz, metadata );
        }
        if ( ( (int)m_World->GetBlock( fillx-1, filly, fillz ) == E_BLOCK_REDSTONE_WIRE)  ) {
                metadata = LightRedstone( fillx-1, filly+1, fillz, metadata );
        }
        if ( ( (int)m_World->GetBlock( fillx, filly, fillz+1 ) == E_BLOCK_REDSTONE_WIRE)  ) {
                metadata = LightRedstone( fillx, filly+1, fillz+1, metadata );
        }
        if ( ( (int)m_World->GetBlock( fillx, filly, fillz-1 ) == E_BLOCK_REDSTONE_WIRE)  ) {
                metadata = LightRedstone( fillx, filly+1, fillz-1, metadata );
        }
        break;
		}
*/   
}

char cRedstone::LightRedstone( int fillx, int filly, int fillz, char metadata)
{
	//while (m_Metadata == metadata) {

	//printf("LightRedstone( %i, %i, %i, %i )\n)", fillx, filly, fillz, (int)metadata);
	if ( ( (int)m_World->GetBlock( fillx, filly, fillz ) == E_BLOCK_STICKY_PISTON ) || ( (int)m_World->GetBlock( fillx, filly, fillz ) == E_BLOCK_PISTON ) ) {
		cPiston Piston(m_World);
		if (m_Metadata > 0) {
			Piston.ExtendPiston(fillx, filly, fillz);
		} else {
			Piston.RetractPiston(fillx, filly, fillz);
		}
	}

	if ( (int)m_World->GetBlock( fillx, filly, fillz ) == E_BLOCK_REDSTONE_TORCH_OFF ) { //I don't think notch knows on from off. >.>
		//printf("1\n");
		metadata = 15;
		m_Metadata = 15;
		if ( ( (int)m_World->GetBlock( fillx-1, filly, fillz ) == E_BLOCK_REDSTONE_WIRE) && ( (int)m_World->GetBlockMeta( fillx-1, filly, fillz) != m_Metadata ) ) {
			LightRedstone(fillx-1,filly,fillz,m_Metadata);
		}
		if ( ( (int)m_World->GetBlock( fillx+1, filly, fillz ) == E_BLOCK_REDSTONE_WIRE) && ( (int)m_World->GetBlockMeta( fillx+1, filly, fillz) != m_Metadata ) ) {
			LightRedstone(fillx+1,filly,fillz,m_Metadata);
		}
		if ( ( (int)m_World->GetBlock( fillx, filly, fillz-1 ) == E_BLOCK_REDSTONE_WIRE) && ( (int)m_World->GetBlockMeta( fillx, filly, fillz-1) != m_Metadata ) ) {
			LightRedstone(fillx,filly,fillz-1,m_Metadata);
		}
		if ( ( (int)m_World->GetBlock( fillx, filly, fillz+1 ) == E_BLOCK_REDSTONE_WIRE) && ( (int)m_World->GetBlockMeta( fillx, filly, fillz+1) != m_Metadata ) ) {
			LightRedstone(fillx,filly,fillz+1,m_Metadata);
		}
	} else if ( (int)m_World->GetBlock( fillx, filly, fillz ) == E_BLOCK_REDSTONE_TORCH_ON ) { //if the torch is off
		//printf("2\n");
		metadata = 0;
		if ( ( (int)m_World->GetBlock( fillx-1, filly, fillz ) == E_BLOCK_REDSTONE_WIRE) && ( (int)m_World->GetBlockMeta( fillx-1, filly, fillz) != m_Metadata ) ) {
			LightRedstone(fillx-1,filly,fillz,m_Metadata);
		}
		if ( ( (int)m_World->GetBlock( fillx+1, filly, fillz ) == E_BLOCK_REDSTONE_WIRE) && ( (int)m_World->GetBlockMeta( fillx+1, filly, fillz) != m_Metadata ) ) {
			LightRedstone(fillx+1,filly,fillz,m_Metadata);
		}
		if ( ( (int)m_World->GetBlock( fillx, filly, fillz-1 ) == E_BLOCK_REDSTONE_WIRE) && ( (int)m_World->GetBlockMeta( fillx, filly, fillz-1) != m_Metadata ) ) {
			LightRedstone(fillx,filly,fillz-1,m_Metadata);
		}
		if ( ( (int)m_World->GetBlock( fillx, filly, fillz+1 ) == E_BLOCK_REDSTONE_WIRE) && ( (int)m_World->GetBlockMeta( fillx, filly, fillz+1) != m_Metadata ) ) {
			LightRedstone(fillx,filly,fillz+1,m_Metadata);
		}
	}





/*
	if ( ( (int)m_World->GetBlock( fillx, filly, fillz ) == E_BLOCK_REDSTONE_WIRE) && ( (int)m_World->GetBlockMeta( fillx, filly, fillz) == 15 ) ) { //if current block is redstone and is on
		if ((int)m_Metadata == 15) { //and we want it to be on then we do nothing
		} else { //we want it to be off 
			
			//if (m_Metadata == metadata) { // only do this if another fill in the same class hasn't found a lit torch yet

				m_World->FastSetBlock( fillx, filly, fillz, (char)E_BLOCK_REDSTONE_WIRE, metadata ); //turn it off then check each one around it
	
				if ( ( (int)m_World->GetBlock( fillx-1, filly, fillz ) == E_BLOCK_REDSTONE_WIRE) && ( (int)m_World->GetBlockMeta( fillx-1, filly, fillz) != m_Metadata ) ) {
					LightRedstone(fillx-1,filly,fillz,m_Metadata);
				}
				if ( ( (int)m_World->GetBlock( fillx+1, filly, fillz ) == E_BLOCK_REDSTONE_WIRE) && ( (int)m_World->GetBlockMeta( fillx+1, filly, fillz) != m_Metadata ) ) {
					LightRedstone(fillx+1,filly,fillz,m_Metadata);
				}
				if ( ( (int)m_World->GetBlock( fillx, filly, fillz-1 ) == E_BLOCK_REDSTONE_WIRE) && ( (int)m_World->GetBlockMeta( fillx, filly, fillz-1) != m_Metadata ) ) {
					LightRedstone(fillx,filly,fillz-1,m_Metadata);
				}
				if ( ( (int)m_World->GetBlock( fillx, filly, fillz+1 ) == E_BLOCK_REDSTONE_WIRE) && ( (int)m_World->GetBlockMeta( fillx, filly, fillz+1) != m_Metadata ) ) {
					LightRedstone(fillx,filly,fillz+1,m_Metadata);
				}

			//}

		}

	}






	if ( ( (int)m_World->GetBlock( fillx, filly, fillz ) == E_BLOCK_REDSTONE_WIRE) && ( (int)m_World->GetBlockMeta( fillx, filly, fillz) == 0 ) ) { //if current block is redstone and is off
		if ((int)m_Metadata == 15) { //and we want it to be on then we check each object around it

			m_World->FastSetBlock( fillx, filly, fillz, (char)E_BLOCK_REDSTONE_WIRE, m_Metadata ); //turn it off then check each one around it

			if ( ( (int)m_World->GetBlock( fillx-1, filly, fillz ) == E_BLOCK_REDSTONE_WIRE) && ( (int)m_World->GetBlockMeta( fillx-1, filly, fillz) != m_Metadata ) ) {
				LightRedstone(fillx-1,filly,fillz,m_Metadata);
			}
			if ( ( (int)m_World->GetBlock( fillx+1, filly, fillz ) == E_BLOCK_REDSTONE_WIRE) && ( (int)m_World->GetBlockMeta( fillx+1, filly, fillz) != m_Metadata ) ) {
				LightRedstone(fillx+1,filly,fillz,m_Metadata);
			}
			if ( ( (int)m_World->GetBlock( fillx, filly, fillz-1 ) == E_BLOCK_REDSTONE_WIRE) && ( (int)m_World->GetBlockMeta( fillx, filly, fillz-1) != m_Metadata ) ) {
				LightRedstone(fillx,filly,fillz-1,m_Metadata);
			}
			if ( ( (int)m_World->GetBlock( fillx, filly, fillz+1 ) == E_BLOCK_REDSTONE_WIRE) && ( (int)m_World->GetBlockMeta( fillx, filly, fillz+1) != m_Metadata ) ) {
				LightRedstone(fillx,filly,fillz+1,m_Metadata);
			}

		} else { //we want it to be off


		}

	}
	*/

	if ( ( (int)m_World->GetBlock( fillx, filly, fillz ) == E_BLOCK_REDSTONE_WIRE) && ( (int)m_World->GetBlockMeta( fillx, filly, fillz) != m_Metadata ) ) {
	//if ( ( (int)m_World->GetBlock( fillx, filly, fillz ) == E_BLOCK_REDSTONE_WIRE) ) {
					//printf("3\n");
	        m_World->FastSetBlock( fillx, filly, fillz, (char)E_BLOCK_REDSTONE_WIRE, m_Metadata );
       	 	LightRedstone(fillx-1,filly,fillz,m_Metadata);
       	 	LightRedstone(fillx+1,filly,fillz,m_Metadata);
       		LightRedstone(fillx,filly,fillz-1,m_Metadata);
       		LightRedstone(fillx,filly,fillz+1,m_Metadata);

        	LightRedstone(fillx-1,filly-1,fillz,m_Metadata);
        	LightRedstone(fillx+1,filly-1,fillz,m_Metadata);
        	LightRedstone(fillx,filly-1,fillz-1,m_Metadata);
        	LightRedstone(fillx,filly-1,fillz+1,m_Metadata);

        	LightRedstone(fillx-1,filly+1,fillz,m_Metadata);
        	LightRedstone(fillx+1,filly+1,fillz,m_Metadata);
        	LightRedstone(fillx,filly+1,fillz-1,m_Metadata);
        	LightRedstone(fillx,filly+1,fillz+1,m_Metadata);
	}
/*
	if ( ( (int)m_World->GetBlock( fillx, filly, fillz ) == E_BLOCK_REDSTONE_WIRE) && ( (int)m_World->GetBlockMeta( fillx, filly, fillz) != metadata ) ) {
		m_World->FastSetBlock( fillx, filly, fillz, (char)E_BLOCK_REDSTONE_WIRE, metadata );

		//if ( ( (int)m_World->GetBlock( fillx-1, filly, fillz ) == E_BLOCK_REDSTONE_WIRE) && ( (int)m_World->GetBlockMeta( fillx-1, filly, fillz) != metadata ) ) {
			LightRedstone(fillx-1,filly,fillz,metadata);
		//}
		//if ( ( (int)m_World->GetBlock( fillx+1, filly, fillz ) == E_BLOCK_REDSTONE_WIRE) && ( (int)m_World->GetBlockMeta( fillx+1, filly, fillz) != metadata ) ) {
			LightRedstone(fillx+1,filly,fillz,metadata);
		//}
		//if ( ( (int)m_World->GetBlock( fillx, filly, fillz-1 ) == E_BLOCK_REDSTONE_WIRE) && ( (int)m_World->GetBlockMeta( fillx, filly, fillz-1) != metadata ) ) {
			LightRedstone(fillx,filly,fillz-1,metadata);
		//}
		//if ( ( (int)m_World->GetBlock( fillx, filly, fillz+1 ) == E_BLOCK_REDSTONE_WIRE) && ( (int)m_World->GetBlockMeta( fillx, filly, fillz+1) != metadata ) ) {
			LightRedstone(fillx,filly,fillz+1,metadata);
		//}
		
	}

	if ( ( (int)m_World->GetBlock( fillx, filly, fillz ) == E_BLOCK_REDSTONE_WIRE) && ( (int)m_World->GetBlockMeta( fillx, filly, fillz) != metadata ) ) {
		m_World->FastSetBlock( fillx, filly, fillz, (char)E_BLOCK_REDSTONE_WIRE, metadata );

		//if ( ( (int)m_World->GetBlock( fillx-1, filly, fillz ) == E_BLOCK_REDSTONE_WIRE) && ( (int)m_World->GetBlockMeta( fillx-1, filly, fillz) != metadata ) ) {
			LightRedstone(fillx-1,filly,fillz,metadata);
		//}
		//if ( ( (int)m_World->GetBlock( fillx+1, filly, fillz ) == E_BLOCK_REDSTONE_WIRE) && ( (int)m_World->GetBlockMeta( fillx+1, filly, fillz) != metadata ) ) {
			LightRedstone(fillx+1,filly,fillz,metadata);
		//}
		//if ( ( (int)m_World->GetBlock( fillx, filly, fillz-1 ) == E_BLOCK_REDSTONE_WIRE) && ( (int)m_World->GetBlockMeta( fillx, filly, fillz-1) != metadata ) ) {
			LightRedstone(fillx,filly,fillz-1,metadata);
		//}
		//if ( ( (int)m_World->GetBlock( fillx, filly, fillz+1 ) == E_BLOCK_REDSTONE_WIRE) && ( (int)m_World->GetBlockMeta( fillx, filly, fillz+1) != metadata ) ) {
			LightRedstone(fillx,filly,fillz+1,metadata);
		//}
		
	}
*/
	return metadata;

}