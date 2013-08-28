
// Minecart.cpp

// Implements the cMinecart class representing a minecart in the world
// Indiana Jones!

#include "Globals.h"
#include "Minecart.h"
#include "../World.h"
#include "../ClientHandle.h"
#include "Player.h"





cMinecart::cMinecart(ePayload a_Payload, double a_X, double a_Y, double a_Z) :
	super(etMinecart, a_X, a_Y, a_Z, 0.98, 0.7),
	m_Payload(a_Payload)
{
	SetMass(20.f);
	SetMaxHealth(6);
}




bool cMinecart::Initialize(cWorld * a_World)
{
	if (super::Initialize(a_World))
	{
		a_World->BroadcastSpawnEntity(*this);
		return true;
	}
	return false;
}





void cMinecart::SpawnOn(cClientHandle & a_ClientHandle)
{
	char TypeType = 0;
	switch (m_Payload)
	{
		case mpNone:    TypeType = 0; break;
		case mpChest:   TypeType = 1; break;
		case mpFurnace: TypeType = 2; break;
		case mpTNT:     TypeType = 3; break;
		case mpHopper:  TypeType = 5; break;
		default:
		{
			ASSERT(!"Unknown payload, cannot spawn on client");
			return;
		}
	}
	a_ClientHandle.SendSpawnVehicle(*this, 10, TypeType); // 10 = Minecarts, TypeType = What type of Minecart
}





enum ENUM_RAIL_DIRECTIONS
{
	E_RAIL_NORTH_SOUTH       = 0,
	E_RAIL_EAST_WEST         = 1,
	E_RAIL_ASCEND_EAST       = 2,
	E_RAIL_ASCEND_WEST       = 3,
	E_RAIL_ASCEND_NORTH      = 4,
	E_RAIL_ASCEND_SOUTH      = 5,
	E_RAIL_CURVED_SOUTH_EAST = 6,
	E_RAIL_CURVED_SOUTH_WEST = 7,
	E_RAIL_CURVED_NORTH_WEST = 8,
	E_RAIL_CURVED_NORTH_EAST = 9,
} ;





void cMinecart::Tick(float a_Dt, cChunk & a_Chunk)
{
	/*
	NOTE: Please bear in mind that taking away from negatives make them even more negative,
	adding to negatives make them positive, etc. Also remember that - North is -Z, South +Z, West -X, and East +Z
	*/
	
	super::Tick(a_Dt, a_Chunk);
	
	BLOCKTYPE BelowType;
	NIBBLETYPE BelowMeta;
	double SpeedX = GetSpeedX(), SpeedY = GetSpeedY(), SpeedZ = GetSpeedZ(); // Get current speed

	// Get block type & meta below the cart
	GetWorld()->GetBlockTypeMeta(floor(GetPosX()), floor(GetPosY() -1 ), floor(GetPosZ()), BelowType, BelowMeta);

	if ((BelowType == E_BLOCK_RAIL) || (BelowType == E_BLOCK_DETECTOR_RAIL) || (BelowType == E_BLOCK_ACTIVATOR_RAIL))
	{
		switch (BelowMeta)
		{

			case E_RAIL_NORTH_SOUTH:
			{
				SpeedY = 0; // Don't move vertically as on ground
				
				// Set Y as current Y rounded up to bypass friction
				// TODO: this causes positioning mismatches on the client, but Entity physics insists on friction!
				SetPosY(ceil(GetPosY()) + 0.05);

				if (SpeedZ != 0) // Don't do anything if cart is stationary
				{
					if (SpeedZ > 0)
					{
						// Going SOUTH, slow down
						SpeedZ = SpeedZ - 0.05;
					}
					else
					{
						// Going NORTH, slow down
						SpeedZ = SpeedZ + 0.05;
					}
				}
				break;
			}

			case E_RAIL_EAST_WEST:
			{
				SpeedY = 0;
				SetPosY(ceil(GetPosY()) + 0.05);

				if (SpeedX != 0)
				{
					if (SpeedX > 0)
					{
						SpeedX = SpeedX - 0.05;
					}
					else
					{
						SpeedX = SpeedX + 0.05;
					}
				}
				break;
			}

			case E_RAIL_ASCEND_NORTH:
			{
				if (SpeedZ >= 0)
				{
					// SpeedZ POSITIVE, going SOUTH
					if (SpeedZ <= 6) // Speed limit of 6 SOUTH (m/s??)
					{
						SpeedZ = SpeedZ + 1; // Speed up
						SpeedY = (0 - SpeedZ); // Downward movement is negative (0 minus positive numbers is negative)
					}
					else
					{
						SpeedZ = 6; // Enforce speed limit
						SpeedY = (0 - SpeedZ);
					}
				}
				else
				{
					// SpeedZ NEGATIVE, going NORTH
					SpeedZ = SpeedZ + 0.1; // Slow down
					SpeedY = (0 - SpeedZ); // Upward movement is positive (0 minus negative number is positive number)
				}
				break;
			}

			case E_RAIL_ASCEND_SOUTH:
			{
				if (SpeedX > 0)
				{
					// SpeedZ POSITIVE, going SOUTH
					SpeedZ = SpeedZ - 0.1; // Slow down
					SpeedY = SpeedZ; // Upward movement positive
				}
				else
				{
					if (SpeedZ >= -6) // Speed limit of 6 WEST (m/s??)
					{
						// SpeedZ NEGATIVE, going NORTH
						SpeedZ = SpeedZ - 1; // Speed up
						SpeedY = SpeedZ; // Downward movement negative
					}
					else
					{
						SpeedZ = 6; // Enforce speed limit
						SpeedY = SpeedZ;
					}
				}
				break;
			}

			case E_RAIL_ASCEND_WEST:
			{
				if (SpeedX >= 0)
				{
					if (SpeedX <= 6)
					{
						SpeedX = SpeedX + 1;
						SpeedY = (0 - SpeedX);
					}
					else
					{
						SpeedX = 6;
						SpeedY = (0 - SpeedX);
					}
				}
				else
				{
					SpeedX = SpeedX + 0.1;
					SpeedY = (0 - SpeedX);
				}
				break;
			}

			case E_RAIL_ASCEND_EAST:
			{
				if (SpeedX > 0)
				{
					SpeedX = SpeedX - 0.1;
					SpeedY = SpeedX;
				}
				else
				{
					if (SpeedX >= -6)
					{
						SpeedX = SpeedX - 1;
						SpeedY = SpeedX;
					}
					else
					{
						SpeedX = -6;
						SpeedY = SpeedX;
					}
				}
				break;
			}

			default:
			{
				ASSERT(!"Unhandled rail meta!");
				break;
			}

		}
	}
	
	// Set speed to speed variables
	SetSpeedX(SpeedX);
	SetSpeedY(SpeedY);
	SetSpeedZ(SpeedZ);

	// Pass to physics handlers in Entity.cpp and broadcast position to client
	HandlePhysics(a_Dt, a_Chunk);
	BroadcastMovementUpdate();

}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cEmptyMinecart:

cEmptyMinecart::cEmptyMinecart(double a_X, double a_Y, double a_Z) :
	super(mpNone, a_X, a_Y, a_Z)
{
}





void cEmptyMinecart::OnRightClicked(cPlayer & a_Player)
{
	if (m_Attachee != NULL)
	{
		if (m_Attachee->GetUniqueID() == a_Player.GetUniqueID())
		{
			// This player is already sitting in, they want out.
			a_Player.Detach();
			return;
		}
		
		if (m_Attachee->IsPlayer())
		{
			// Another player is already sitting in here, cannot attach
			return;
		}
		
		// Detach whatever is sitting in this minecart now:
		m_Attachee->Detach();
	}
	
	// Attach the player to this minecart
	a_Player.AttachTo(this);
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cMinecartWithChest:

cMinecartWithChest::cMinecartWithChest(double a_X, double a_Y, double a_Z) :
	super(mpChest, a_X, a_Y, a_Z)
{
}





void cMinecartWithChest::SetSlot(int a_Idx, const cItem & a_Item)
{
	ASSERT((a_Idx >= 0) && (a_Idx < ARRAYCOUNT(m_Items)));
	
	m_Items[a_Idx] = a_Item;
}





void cMinecartWithChest::OnRightClicked(cPlayer & a_Player)
{
	// Show the chest UI window to the player
	// TODO
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cMinecartWithFurnace:

cMinecartWithFurnace::cMinecartWithFurnace(double a_X, double a_Y, double a_Z) :
	super(mpFurnace, a_X, a_Y, a_Z)
{
}





void cMinecartWithFurnace::OnRightClicked(cPlayer & a_Player)
{
	// Try to power the furnace with whatever the player is holding
	// TODO
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cMinecartWithTNT:

cMinecartWithTNT::cMinecartWithTNT(double a_X, double a_Y, double a_Z) :
	super(mpTNT, a_X, a_Y, a_Z)
{
}

// TODO: Make it activate when passing over activator rail





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cMinecartWithHopper:

cMinecartWithHopper::cMinecartWithHopper(double a_X, double a_Y, double a_Z) :
	super(mpHopper, a_X, a_Y, a_Z)
{
}

// TODO: Make it suck up blocks and travel further than any other cart and physics and put and take blocks
// AND AVARYTHING!!