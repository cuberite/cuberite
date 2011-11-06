#include "cPiston.h"
#include "cRoot.h"
#include "cWorld.h"
#include "BlockID.h"

cPiston::cPiston( cWorld* a_World )
	:m_World ( a_World )
{

}
//Todo: All piston stuff below.
void cPiston::ExtendPiston( int pistx, int pisty, int pistz )
{
	//cWorld* World = cRoot::Get()->GetWorld();
	char metadata = 0;
	char extmetadata = 0;
	int FirstFluidBlock = 0;
	char piston;

	piston = m_World->GetBlock( pistx, pisty, pistz );

	printf("psPiston1\n");

	if ( piston  == E_BLOCK_STICKY_PISTON ) {
		printf("psPiston2\n");
		extmetadata = 8;
	}//if sticky piston than top nibble bit is set to 1;


	if ( ( piston  == E_BLOCK_STICKY_PISTON ) || ( piston == E_BLOCK_PISTON ) ) {
		metadata = m_World->GetBlockMeta( pistx, pisty, pistz);
		 printf("psPiston3\n");
		printf("metadata %c\n",metadata);

		if (metadata < 5) { //piston not extended

			printf("metadata %c\n",metadata);
			metadata -= 8; //removing 8 from retracts it
			switch  (metadata) {
				case 0:
					FirstFluidBlock = FindFluidBlock ( pistx, pisty, pistz, pistx, pisty-16, pistz );
					if (FirstFluidBlock > 0) {
						ChainMove ( pistx, pisty, pistz, pistx, pisty-FirstFluidBlock, pistz );
					}
					m_World->FastSetBlock( pistx, pisty, pistz, piston, (char)metadata + 8 );
					m_World->FastSetBlock( pistx, pisty-1, pistz, E_BLOCK_PISTON_EXTENSION, extmetadata );
					break;
				case 1:
                                        FirstFluidBlock = FindFluidBlock (pistx, pisty, pistz, pistx, pisty+16, pistz);
                                        if (FirstFluidBlock > 0) {
                                                ChainMove ( pistx, pisty, pistz, pistx, pisty+FirstFluidBlock, pistz );
                                        }
					m_World->FastSetBlock( pistx, pisty, pistz, piston, (char)metadata + 8 );
                                        m_World->FastSetBlock( pistx, pisty+1, pistz, E_BLOCK_PISTON_EXTENSION, extmetadata+1 );
					break;
				case 2:
                                        FirstFluidBlock = FindFluidBlock (pistx, pisty, pistz, pistx, pisty, pistz-16);
                                        if (FirstFluidBlock > 0) {
                                                ChainMove ( pistx, pisty, pistz, pistx, pisty, pistz-FirstFluidBlock );
                                        }
					m_World->FastSetBlock( pistx, pisty, pistz, piston, (char)metadata + 8 );
                                        m_World->FastSetBlock( pistx, pisty, pistz-1, E_BLOCK_PISTON_EXTENSION, extmetadata+2 );
					break;
				case 3:
                                        FirstFluidBlock = FindFluidBlock (pistx, pisty, pistz, pistx, pisty, pistz+16);
                                        if (FirstFluidBlock > 0) {
                                                ChainMove ( pistx, pisty, pistz, pistx, pisty, pistz+FirstFluidBlock );
                                        }
                                        m_World->FastSetBlock( pistx, pisty, pistz, piston, (char)metadata + 8 );
                                        m_World->FastSetBlock( pistx, pisty, pistz+1, E_BLOCK_PISTON_EXTENSION, extmetadata+3 );
					break;
				case 4:
                                        FirstFluidBlock = FindFluidBlock (pistx, pisty, pistz, pistx-16, pisty, pistz);
                                        if (FirstFluidBlock > 0) {
                                                ChainMove ( pistx, pisty, pistz, pistx-FirstFluidBlock, pisty, pistz );
                                        }
                                        m_World->FastSetBlock( pistx, pisty, pistz, piston, metadata + 8 );
                                        m_World->FastSetBlock( pistx, pisty+1, pistz, E_BLOCK_PISTON_EXTENSION, extmetadata+4 );
					break;
				case 5:
                                        FirstFluidBlock = FindFluidBlock (pistx, pisty, pistz, pistx+16, pisty, pistz);
                                        if (FirstFluidBlock > 0) {
                                                ChainMove ( pistx, pisty, pistz, pistx+FirstFluidBlock, pisty, pistz );
                                        }
                                        m_World->FastSetBlock( pistx, pisty, pistz, piston, metadata + 8 );
                                        m_World->FastSetBlock( pistx, pisty+1, pistz, E_BLOCK_PISTON_EXTENSION, extmetadata+5 );
					break;
			}
		}
	}
}

void cPiston::RetractPiston( int pistx, int pisty, int pistz )
{
	char metadata = m_World->GetBlockMeta( pistx, pisty, pistz);

	if ( (int)m_World->GetBlock( pistx, pisty, pistz ) == E_BLOCK_PISTON ) {
                if (metadata > 5) { //piston retracted
			metadata -= 8;//set the piston to retracted state.

			m_World->FastSetBlock( pistx, pisty, pistz, m_World->GetBlock( pistx, pisty, pistz ), metadata  );
			switch  (metadata) {
			case 0:
				if ( m_World->GetBlock( pistx, pisty-1, pistz ) ==  E_BLOCK_PISTON_EXTENSION ) {
					m_World->SetBlock( pistx, pisty-1, pistz, 0, 0 );
				}
				break;

			case 1:
				if ( m_World->GetBlock( pistx, pisty+1, pistz ) ==  E_BLOCK_PISTON_EXTENSION ) {
                                	m_World->SetBlock( pistx, pisty+1, pistz, 0, 0 );
				}
				break;

			case 2:
				if ( m_World->GetBlock( pistx, pisty, pistz-1 ) ==  E_BLOCK_PISTON_EXTENSION ) {
                                	m_World->SetBlock( pistx, pisty, pistz-1, 0, 0 );
				}
				break;

			case 3:
				if ( m_World->GetBlock( pistx, pisty, pistz+1 ) ==  E_BLOCK_PISTON_EXTENSION ) {
                                	m_World->SetBlock( pistx, pisty, pistz+1, 0, 0 );
				}
				break;

			case 4:
				if ( m_World->GetBlock( pistx-1, pisty, pistz ) ==  E_BLOCK_PISTON_EXTENSION ) {
                                	m_World->SetBlock( pistx-1, pisty, pistz, 0, 0 );
				}
				break;

			case 5:
				if ( m_World->GetBlock( pistx+1, pisty, pistz ) ==  E_BLOCK_PISTON_EXTENSION ) {
                                	m_World->SetBlock( pistx+1, pisty, pistz, 0, 0 );
				}
				break;

			}
		}
        }


        if ( (int)m_World->GetBlock( pistx, pisty, pistz ) == E_BLOCK_PISTON ) {
                if (metadata > 5) { //piston retracted
                        metadata -= 8;//set the piston to retracted state.

                        m_World->FastSetBlock( pistx, pisty, pistz, m_World->GetBlock( pistx, pisty, pistz ), metadata  );
                        switch  (metadata) {
                        case 0:
                                if ( m_World->GetBlock( pistx, pisty-1, pistz ) ==  E_BLOCK_PISTON_EXTENSION ) {
                                	m_World->SetBlock( pistx, pisty-1, pistz, m_World->GetBlock( pistx, pisty-2, pistz ), m_World->GetBlockMeta( pistx, pisty-2, pistz ) );
					m_World->SetBlock( pistx, pisty-2, pistz, 0, 0 );
                                }
				break;

                        case 1:
                                if ( m_World->GetBlock( pistx, pisty+1, pistz ) == E_BLOCK_PISTON_EXTENSION ) {
                                        m_World->SetBlock( pistx, pisty-1, pistz, m_World->GetBlock( pistx, pisty-2, pistz ), m_World->GetBlockMeta( pistx, pisty-2, pistz ) );
                                        m_World->SetBlock( pistx, pisty-2, pistz, 0, 0 );
                                }
				break;

                        case 2:
                                if ( m_World->GetBlock( pistx, pisty, pistz-1) ==  E_BLOCK_PISTON_EXTENSION ) {
                                        m_World->SetBlock( pistx, pisty, pistz-1, m_World->GetBlock( pistx, pisty, pistz-2 ), m_World->GetBlockMeta( pistx, pisty, pistz-2 ) );
                                        m_World->SetBlock( pistx, pisty, pistz-2, 0, 0 );
                                }
				break;

                        case 3:
                                if ( m_World->GetBlock( pistx, pisty, pistz+1) ==  E_BLOCK_PISTON_EXTENSION ) {
                                        m_World->SetBlock( pistx, pisty, pistz+1, m_World->GetBlock( pistx, pisty, pistz+2 ), m_World->GetBlockMeta( pistx, pisty, pistz+2 ) );
                                        m_World->SetBlock( pistx, pisty, pistz+2, 0, 0 );
                                }
				break;

                        case 4:
                                if ( m_World->GetBlock( pistx-1, pisty, pistz) ==  E_BLOCK_PISTON_EXTENSION ) {
                                        m_World->SetBlock( pistx-1, pisty, pistz, m_World->GetBlock( pistx-2, pisty, pistz ), m_World->GetBlockMeta( pistx-2, pisty, pistz ) );
                                        m_World->SetBlock( pistx-2, pisty, pistz, 0, 0 );
                                }
				break;

                        case 5:
                                if ( m_World->GetBlock( pistx+1, pisty, pistz) == E_BLOCK_PISTON_EXTENSION ) {
                                        m_World->SetBlock( pistx+1, pisty, pistz, m_World->GetBlock( pistx+2, pisty, pistz ), m_World->GetBlockMeta( pistx+2, pisty, pistz ) );
                                        m_World->SetBlock( pistx+2, pisty, pistz, 0, 0 );
				}
                                break;

                        }
                }
        }
}

void cPiston::ChainMove ( int ax, int ay, int az, int bx, int by, int bz )
{

char lastmeta;
char lastblock;

	if ( ( ax != bx ) && ( ay == by ) && ( az == bz  ) ) { //move x

		if ( ax < bx  ) {

			while ( ax > bx ) {

				lastmeta  = m_World->GetBlockMeta( bx+1, by, bz);
				lastblock = m_World->GetBlockMeta( bx+1, by, bz);
				m_World->SetBlock( bx, by, bz, lastblock, lastmeta );
				bx+1;

			}

		} else {

                        while ( ax < bx ) {

                                lastmeta  = m_World->GetBlockMeta( bx-1, by, bz);
                                lastblock = m_World->GetBlockMeta( bx-1, by, bz);
                                m_World->SetBlock( bx, by, bz, lastblock, lastmeta );
                                bx-1;

                        }

		}

		m_World->SetBlock( ax, ay, az, 0, 0 );

	} else if ( ( ax == bx ) && ( ay != by ) && ( az == bz  ) ) { //move y

		if ( ay < by  ) {

                        while ( ay > by ) {

                                lastmeta  = m_World->GetBlockMeta( bx, by+1, bz);
                                lastblock = m_World->GetBlockMeta( bx, by+1, bz);
                                m_World->SetBlock( bx, by, bz, lastblock, lastmeta );
                                by+1;

                        }

                } else {

                        while ( ay < by ) {

                                lastmeta  = m_World->GetBlockMeta( bx, by-1, bz);
                                lastblock = m_World->GetBlockMeta( bx, by-1, bz);
                                m_World->SetBlock( bx, by, bz, lastblock, lastmeta );
                                by-1;

                        }

                }

		m_World->SetBlock( ax, ay, az, 0, 0 );

	} else if ( ( ax == bx ) && ( ay == by ) && ( az != bz  ) ) { //move z

		if ( az < bz  ) {

                        while ( az > bz ) {

                                lastmeta  = m_World->GetBlockMeta( bx, by, bz+1 );
                                lastblock = m_World->GetBlockMeta( bx, by, bz+1 );
                                m_World->SetBlock( bx, by, bz, lastblock, lastmeta );
                                bz+1;

                        }

                } else {

                        while ( az < bz ) {

                                lastmeta  = m_World->GetBlockMeta( bx, by, bz-1 );
                                lastblock = m_World->GetBlockMeta( bx, by, bz-1 );
                                m_World->SetBlock( bx, by, bz, lastblock, lastmeta );
                                bz-1;

                        }

                }

		m_World->SetBlock( ax, ay, az, 0, 0 );

	} else {

		//LOG("Bad coords fed into cPiston::ChainMove");

	}


}


int cPiston::FindFluidBlock ( int ax, int ay, int az, int bx, int by, int bz )
{

char thisblock;
int cnt = 0;
        if ( ( ax != bx ) && ( ay == by ) && ( az == bz  ) ) { //check x

                if ( ax < bx  ) {

                        while ( ax < bx ) {

                                thisblock = m_World->GetBlock( ax, ay, az);
                              	if ( (thisblock == 0) || (thisblock == E_BLOCK_STATIONARY_WATER) || (thisblock == E_BLOCK_WATER) || (thisblock == E_BLOCK_STATIONARY_LAVA) || (thisblock == E_BLOCK_LAVA) ) {
                                        return cnt;
				}
				cnt++;
				ax+1;
                        }

                } else {

                        while ( ax > bx ) {

                                thisblock = m_World->GetBlock( ax, ay, az);
                              	if ( (thisblock == 0) || (thisblock == E_BLOCK_STATIONARY_WATER) || (thisblock == E_BLOCK_WATER) || (thisblock == E_BLOCK_STATIONARY_LAVA) || (thisblock == E_BLOCK_LAVA) ) {
                                        return cnt;
				}
				cnt++;
                                ax-1;

                        }

                }

                m_World->SetBlock( ax, ay, az, 0, 0 );

        } else if ( ( ax == bx ) && ( ay != by ) && ( az == bz  ) ) { //check y

                if ( ay < by  ) {

                        while ( ay < by ) {

                                thisblock = m_World->GetBlock( bx, by, bz);
                              	if ( (thisblock == 0) || (thisblock == E_BLOCK_STATIONARY_WATER) || (thisblock == E_BLOCK_WATER) || (thisblock == E_BLOCK_STATIONARY_LAVA) || (thisblock == E_BLOCK_LAVA) ) {
                                        return cnt;
				}
				cnt++;
                                ay+1;

                        }

                } else {

                        while ( ay > by ) {

                                thisblock = m_World->GetBlock( bx, by, bz);
                              	if ( (thisblock == 0) || (thisblock == E_BLOCK_STATIONARY_WATER) || (thisblock == E_BLOCK_WATER) || (thisblock == E_BLOCK_STATIONARY_LAVA) || (thisblock == E_BLOCK_LAVA) ) {
                                        return cnt;
				}
				cnt++;
                                ay-1;

                        }

                }

                m_World->SetBlock( ax, ay, az, 0, 0 );

        } else if ( ( ax == bx ) && ( ay == by ) && ( az != bz  ) ) { //check z

                if ( az < bz  ) {

                        while ( az < bz ) {

                                thisblock = m_World->GetBlock( ax, ay, az);
                              	if ( (thisblock == 0) || (thisblock == E_BLOCK_STATIONARY_WATER) || (thisblock == E_BLOCK_WATER) || (thisblock == E_BLOCK_STATIONARY_LAVA) || (thisblock == E_BLOCK_LAVA) ) {
                                        return cnt;
				}
				cnt++;
                                az+1;

                        }

                } else {

                        while ( az > bz ) {

                                thisblock = m_World->GetBlock( ax, ay, az);
                              	if ( (thisblock == 0) || (thisblock == E_BLOCK_STATIONARY_WATER) || (thisblock == E_BLOCK_WATER) || (thisblock == E_BLOCK_STATIONARY_LAVA) || (thisblock == E_BLOCK_LAVA) ) {
                                        return cnt;
				}
				cnt++;
                                az-1;

                        }

                }

        } else {

                return cnt;

        }


}
