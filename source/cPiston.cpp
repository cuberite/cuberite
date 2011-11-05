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
	cWorld* World = cRoot::Get()->GetWorld();
	char metadata = World->GetBlockMeta( pistx, pisty, pistz);


	if ( ( (int)World->GetBlock( pistx, pisty, pistz ) == E_BLOCK_STICKY_PISTON ) || ( (int)World->GetBlock( pistx, pisty, pistz ) == E_BLOCK_PISTON ) ) {
		//find direction piston is facing base on meta.
		//verify piston is not already extended.
		//check all 16 blocks in front of it for the first  airblock
		//if there's and air block move those found blocks by 1 and insert E_BLOCK_PISTON_EXTENSION in the spot infront of the piston facing right direction

	}

}

void cPiston::RetractPiston( int pistx, int pisty, int pistz )
{
	cWorld* World = cRoot::Get()->GetWorld();
	char metadata = World->GetBlockMeta( pistx, pisty, pistz);

	if ( (int)World->GetBlock( pistx, pisty, pistz ) == E_BLOCK_PISTON ) {
                //find direction piston is facing base on meta.
                //verify piston is extended.
		//check block in front to see if it's an E_BLOCK_PISTON_EXTENSION
		//delete that block
		//set piston meta to unextended
        }

        if ( (int)World->GetBlock( pistx, pisty, pistz ) == E_BLOCK_STICKY_PISTON ) {
                //find direction piston is facing base on meta.
                //verify piston is extended.
                //check block in front to see if it's an E_BLOCK_PISTON_EXTENSION
                //delete that block
		//move block one more in front of it to postion E_BLOCK_PISTON_EXTENSION was facing.
                //set piston meta to unextended
        }

}
