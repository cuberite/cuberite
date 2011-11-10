#include "cRedstone.h"
#include "cPiston.h"
#include "cRoot.h"
#include "cWorld.h"
#include "BlockID.h"
#include <iostream>
#include <vector>
#include <string>

cRedstone::cRedstone( cWorld* a_World )
	:m_World ( a_World )
	,m_Metadata ( 0 )
	,m_Direction ( 0 )
{

}

void cRedstone::ChangeRedstone( int fillx, int filly, int fillz, bool added )
{
char before;
//int tempX;
//int tempY;
//int tempZ;
//int state;
//m_Direction
// 0 = x+
// 1 = x-
// 2 = z+
// 3 = z-
// 4 = y+
// 5 = v-


        if (added) {
                m_Metadata = 15;
        } else {
                m_Metadata = 0;
        }
		before = m_Metadata;

		//printf("etb1\n");
		CalculatetRedstone( fillx, filly, fillz ); //calculate all item centers.
		//printf("etb2\n");

		int Block = (int)m_World->GetBlock( fillx, filly, fillz );
		
		switch (Block)//these blocks won't trigger the normal calculaton because they are affected by the redstone around them. So we check each possible channel around them instead.
		{
			case E_BLOCK_REDSTONE_TORCH_ON:
			case E_BLOCK_REDSTONE_TORCH_OFF:
			case E_BLOCK_AIR:
			case E_BLOCK_PISTON_EXTENSION:
			case E_BLOCK_PISTON:
			case E_BLOCK_STICKY_PISTON:
				{
					m_Metadata = 0;
					m_Direction = 0;
					CalculatetRedstone( fillx+1, filly, fillz );
					m_Metadata = 0;
					m_Direction = 1;
					CalculatetRedstone( fillx-1, filly, fillz );
					m_Metadata = 0;
					m_Direction = 2;
					CalculatetRedstone( fillx, filly, fillz+1 );
					m_Metadata = 0;
					m_Direction = 3;
					CalculatetRedstone( fillx, filly, fillz-1 );
					m_Metadata = 0;
					CalculatetRedstone( fillx, filly-1, fillz );
					break;
				}
			case E_BLOCK_REDSTONE_WIRE: //special case for redstone wire.
				{
					m_Direction = 0;
					CalculatetRedstone( fillx+1, filly, fillz );
					m_Direction = 1;
					CalculatetRedstone( fillx-1, filly, fillz );
					m_Direction = 2;
					CalculatetRedstone( fillx, filly, fillz+1 );
					m_Direction = 3;
					CalculatetRedstone( fillx, filly, fillz-1 );
					m_Metadata = 0;
					CalculatetRedstone( fillx, filly-1, fillz );

					m_Direction = 4;
					CalculatetRedstone( fillx+1, filly+1, fillz );
					CalculatetRedstone( fillx-1, filly+1, fillz );
					CalculatetRedstone( fillx, filly+1, fillz+1 );
					CalculatetRedstone( fillx, filly+1, fillz-1 );

					m_Direction = 5;
					CalculatetRedstone( fillx+1, filly-1, fillz );
					CalculatetRedstone( fillx-1, filly-1, fillz );
					CalculatetRedstone( fillx, filly-1, fillz+1 );
					CalculatetRedstone( fillx, filly-1, fillz-1 );
					break;
 				}
		}

	//printf("done here\n");
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

	}  else if ( ( (int)m_World->GetBlock( fillx, filly, fillz ) == E_ITEM_LEVER ) || ( (int)m_World->GetBlock( fillx, filly, fillz ) == E_ITEM_STONE_BUTTON ) ) {

		if ( (int)m_World->GetBlockMeta( fillx, filly, fillz) > 6 ) { //button powered
			m_Metadata = 15; //change meta to 15 only if redstone power device in on possition is found.
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
		} else {
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

	} else if ( ( (int)m_World->GetBlock( fillx, filly, fillz ) == E_BLOCK_STONE_PRESSURE_PLATE ) || ( (int)m_World->GetBlock( fillx, filly, fillz ) == E_BLOCK_STONE_BUTTON ) ) {

		if ( (int)m_World->GetBlockMeta( fillx, filly, fillz) == 1 ) { //plate powered
			m_Metadata = 15; //change meta to 15 only if redstone power device in on possition is found.
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

		} else {

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

	} else if ( (int)m_World->GetBlock( fillx, filly, fillz ) == E_BLOCK_REDSTONE_TORCH_ON ) { //If torch is on
		//printf("found torch on\n");
		m_Metadata = 15; //change meta to 15 only if redstone torch in on possition is found.
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
		//printf("found torch off\n");
		// no need to change meta here.
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

	} else if ( (int)m_World->GetBlock( fillx, filly, fillz ) == E_BLOCK_REDSTONE_WIRE ) { //simple fill algorithm for redstone wire.

			if ( (int)m_World->GetBlockMeta( fillx, filly, fillz) != m_Metadata ) {
	        m_World->FastSetBlock( fillx, filly, fillz, (char)E_BLOCK_REDSTONE_WIRE, m_Metadata );
					m_Direction = 0;
					CalculatetRedstone( fillx+1, filly, fillz );
					m_Direction = 1;
					CalculatetRedstone( fillx-1, filly, fillz );
					m_Direction = 2;
					CalculatetRedstone( fillx, filly, fillz+1 );
					m_Direction = 3;
					CalculatetRedstone( fillx, filly, fillz-1 );
					CalculatetRedstone( fillx, filly-1, fillz ); //check one block below //similar to same plane

					m_Direction = 4;
					CalculatetRedstone( fillx+1, filly+1, fillz );
					CalculatetRedstone( fillx-1, filly+1, fillz );
					CalculatetRedstone( fillx, filly+1, fillz+1 );
					CalculatetRedstone( fillx, filly+1, fillz-1 );

					m_Direction = 5;
					CalculatetRedstone( fillx+1, filly-1, fillz );
					CalculatetRedstone( fillx-1, filly-1, fillz );
					CalculatetRedstone( fillx, filly-1, fillz+1 );
					CalculatetRedstone( fillx, filly-1, fillz-1 );
			}

	} else { //default, check item for torch attached to it. If meta > 0 then turn that torch off, otherwise turn it on. This change needs to be passed to the next world tick.
			//check for torch to east with meta 1 //turn off
			//check for torch to west with meta 2 //turn off
			//check for torch to south with meta 3 //turn off
			//check for torch to north with meta 4 //turn off
			//check for torch above with meta 5 //turn off
		if ( (int)m_World->GetBlock( fillx, filly, fillz ) != E_BLOCK_AIR ) {
			if (m_Direction < 4) { //redstone wire can only power blocks on the same plane or directly below
				if ( (int)m_Metadata > 0 ) { //wire powered

					//printf("bird: %i dog: %i \n",(int)m_World->GetBlock( fillx, filly+1, fillz ),(int)m_World->GetBlockMeta( fillx, filly+1, fillz));
					if ( ( (int)m_World->GetBlock( fillx+1, filly, fillz ) == E_BLOCK_REDSTONE_TORCH_ON) && ( (int)m_World->GetBlockMeta( fillx+1, filly, fillz) == 1 ) ) { //east
						m_World->m_RSList.push_back(fillx+1);
						m_World->m_RSList.push_back(filly);
						m_World->m_RSList.push_back(fillz);
						m_World->m_RSList.push_back(00000);
					}
					if ( ( (int)m_World->GetBlock( fillx-1, filly, fillz ) == E_BLOCK_REDSTONE_TORCH_ON) && ( (int)m_World->GetBlockMeta( fillx-1, filly, fillz) == 2 ) ) { //west
						m_World->m_RSList.push_back(fillx-1);
						m_World->m_RSList.push_back(filly);
						m_World->m_RSList.push_back(fillz);
						m_World->m_RSList.push_back(00000);
					}
					if ( ( (int)m_World->GetBlock( fillx, filly, fillz+1 ) == E_BLOCK_REDSTONE_TORCH_ON) && ( (int)m_World->GetBlockMeta( fillx, filly, fillz+1) == 3 ) ) { //south
						m_World->m_RSList.push_back(fillx);
						m_World->m_RSList.push_back(filly);
						m_World->m_RSList.push_back(fillz+1);
						m_World->m_RSList.push_back(00000);
					}
					if ( ( (int)m_World->GetBlock( fillx, filly, fillz-1 ) == E_BLOCK_REDSTONE_TORCH_ON) && ( (int)m_World->GetBlockMeta( fillx, filly, fillz-1) == 4 ) ) {  //north
						m_World->m_RSList.push_back(fillx);
						m_World->m_RSList.push_back(filly);
						m_World->m_RSList.push_back(fillz-1);
						m_World->m_RSList.push_back(00000);
					}
					if ( ( (int)m_World->GetBlock( fillx, filly+1, fillz ) == E_BLOCK_REDSTONE_TORCH_ON) && ( (int)m_World->GetBlockMeta( fillx, filly+1, fillz) == 5 ) ) { //top
						m_World->m_RSList.push_back(fillx);
						m_World->m_RSList.push_back(filly+1);
						m_World->m_RSList.push_back(fillz);
						m_World->m_RSList.push_back(00000);
					}

				} else { //wire not powered

					bool BlockPowered = IsBlockPowered( fillx, filly, fillz ); //chck this block for other wire turned on or torch turned on below:
					if (BlockPowered == false) { //if block is not bowered by something else then I need to check for off torches and turn them on.

						//printf("echo: %i cruiser: %i \n",(int)m_World->GetBlock( fillx, filly+1, fillz ),(int)m_World->GetBlockMeta( fillx, filly+1, fillz));
						if ( ( (int)m_World->GetBlock( fillx+1, filly, fillz ) == E_BLOCK_REDSTONE_TORCH_OFF) && ( (int)m_World->GetBlockMeta( fillx+1, filly, fillz) == 1 ) ) { //east
							m_World->m_RSList.push_back(fillx+1);
							m_World->m_RSList.push_back(filly);
							m_World->m_RSList.push_back(fillz);
							m_World->m_RSList.push_back(11111);
						}
						if ( ( (int)m_World->GetBlock( fillx-1, filly, fillz ) == E_BLOCK_REDSTONE_TORCH_OFF) && ( (int)m_World->GetBlockMeta( fillx-1, filly, fillz) == 2 ) ) { //west
							m_World->m_RSList.push_back(fillx-1);
							m_World->m_RSList.push_back(filly);
							m_World->m_RSList.push_back(fillz);
							m_World->m_RSList.push_back(11111);
						}
						if ( ( (int)m_World->GetBlock( fillx, filly, fillz+1 ) == E_BLOCK_REDSTONE_TORCH_OFF) && ( (int)m_World->GetBlockMeta( fillx, filly, fillz+1) == 3 ) ) { //south
							m_World->m_RSList.push_back(fillx);
							m_World->m_RSList.push_back(filly);
							m_World->m_RSList.push_back(fillz+1);
							m_World->m_RSList.push_back(11111);;
						}
						if ( ( (int)m_World->GetBlock( fillx, filly, fillz-1 ) == E_BLOCK_REDSTONE_TORCH_OFF) && ( (int)m_World->GetBlockMeta( fillx, filly, fillz-1) == 4 ) ) {  //north
							m_World->m_RSList.push_back(fillx);
							m_World->m_RSList.push_back(filly);
							m_World->m_RSList.push_back(fillz-1);
							m_World->m_RSList.push_back(11111);
						}
						if ( ( (int)m_World->GetBlock( fillx, filly+1, fillz ) == E_BLOCK_REDSTONE_TORCH_OFF) && ( (int)m_World->GetBlockMeta( fillx, filly+1, fillz) == 5 ) ) { //top
							m_World->m_RSList.push_back(fillx);
							m_World->m_RSList.push_back(filly+1);
							m_World->m_RSList.push_back(fillz);
							m_World->m_RSList.push_back(11111);
						}

					}

				}

			}

		}

	}

}


bool cRedstone::IsBlockPowered( int fillx, int filly, int fillz )
{

	if ( (int)m_World->GetBlock( fillx, filly-1, fillz ) == E_BLOCK_REDSTONE_TORCH_ON) { return true; }
	if ( (int)m_World->GetBlock( fillx+1, filly, fillz ) == E_BLOCK_REDSTONE_TORCH_ON) { return true; }
	if ( (int)m_World->GetBlock( fillx-1, filly, fillz ) == E_BLOCK_REDSTONE_TORCH_ON) { return true; }
	if ( (int)m_World->GetBlock( fillx, filly, fillz+1 ) == E_BLOCK_REDSTONE_TORCH_ON) { return true; }
	if ( (int)m_World->GetBlock( fillx, filly, fillz-1 ) == E_BLOCK_REDSTONE_TORCH_ON) { return true; }
	if ( ( (int)m_World->GetBlock( fillx+1, filly, fillz ) == E_BLOCK_REDSTONE_WIRE) && ( (int)m_World->GetBlockMeta( fillx+1, filly, fillz) > 0 ) ) { return true; }
	if ( ( (int)m_World->GetBlock( fillx-1, filly, fillz ) == E_BLOCK_REDSTONE_WIRE) && ( (int)m_World->GetBlockMeta( fillx-1, filly, fillz) > 0 ) ) { return true; }
	if ( ( (int)m_World->GetBlock( fillx, filly, fillz+1 ) == E_BLOCK_REDSTONE_WIRE) && ( (int)m_World->GetBlockMeta( fillx, filly, fillz+1) > 0 ) ) { return true; }
	if ( ( (int)m_World->GetBlock( fillx, filly, fillz-1 ) == E_BLOCK_REDSTONE_WIRE) && ( (int)m_World->GetBlockMeta( fillx, filly, fillz-1) > 0 ) ) { return true; }
	if ( ( (int)m_World->GetBlock( fillx, filly+1, fillz ) == E_BLOCK_REDSTONE_WIRE) && ( (int)m_World->GetBlockMeta( fillx, filly+1, fillz) > 0 ) ) { return true; }
	return false;
}