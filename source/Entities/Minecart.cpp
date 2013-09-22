
// Minecart.cpp

// Implements the cMinecart class representing a minecart in the world
// Indiana Jones!

#include "Globals.h"
#include "Minecart.h"
#include "../World.h"
#include "../ClientHandle.h"
#include "../Chunk.h"
#include "Player.h"





cMinecart::cMinecart(ePayload a_Payload, double a_X, double a_Y, double a_Z) :
	super(etMinecart, a_X, a_Y, a_Z, 0.98, 0.7),
	m_Payload(a_Payload)
{
	SetMass(20.f);
	SetMaxHealth(6);
	SetHealth(6);
}




void cMinecart::SpawnOn(cClientHandle & a_ClientHandle)
{
	char SubType = 0;
	switch (m_Payload)
	{
		case mpNone:    SubType = 0; break;
		case mpChest:   SubType = 1; break;
		case mpFurnace: SubType = 2; break;
		case mpTNT:     SubType = 3; break;
		case mpHopper:  SubType = 5; break;
		default:
		{
			ASSERT(!"Unknown payload, cannot spawn on client");
			return;
		}
	}
	a_ClientHandle.SendSpawnVehicle(*this, 10, SubType); // 10 = Minecarts, SubType = What type of Minecart
}





void cMinecart::HandlePhysics(float a_Dt, cChunk & a_Chunk)
{
	int PosY = (int)floor(GetPosY());
	if ((PosY <= 0) || (PosY >= cChunkDef::Height))
	{
		// Outside the world, just process normal falling physics
		super::HandlePhysics(a_Dt, a_Chunk);
		BroadcastMovementUpdate();
		return;
	}
	
	int RelPosX = (int)floor(GetPosX()) - a_Chunk.GetPosX() * cChunkDef::Width;
	int RelPosZ = (int)floor(GetPosZ()) - a_Chunk.GetPosZ() * cChunkDef::Width;
	cChunk * Chunk = a_Chunk.GetRelNeighborChunkAdjustCoords(RelPosX, RelPosZ);
	if (Chunk == NULL)
	{
		// Inside an unloaded chunk, bail out all processing
		return;
	}
	BLOCKTYPE BelowType = Chunk->GetBlock(RelPosX, PosY - 1, RelPosZ);
	BLOCKTYPE InsideType = Chunk->GetBlock(RelPosX, PosY, RelPosZ);

	if (IsBlockRail(BelowType))
	{
		HandleRailPhysics(a_Dt, *Chunk);
	}
	else
	{
		if (IsBlockRail(InsideType))
		{
			SetPosY(PosY + 1);
			HandleRailPhysics(a_Dt, *Chunk);
		}
		else
		{
			super::HandlePhysics(a_Dt, *Chunk);
			BroadcastMovementUpdate();
		}
	}
}





static const double MAX_SPEED = 8;
static const double MAX_SPEED_NEGATIVE = (0 - MAX_SPEED);

void cMinecart::HandleRailPhysics(float a_Dt, cChunk & a_Chunk)
{
	
	super::HandlePhysics(a_Dt, a_Chunk); // Main physics handling

	/*
	NOTE: Please bear in mind that taking away from negatives make them even more negative,
	adding to negatives make them positive, etc.
	*/
	
	// Get block meta below the cart
	int RelPosX = (int)floor(GetPosX()) - a_Chunk.GetPosX() * cChunkDef::Width;
	int RelPosZ = (int)floor(GetPosZ()) - a_Chunk.GetPosZ() * cChunkDef::Width;
	NIBBLETYPE BelowMeta = a_Chunk.GetMeta(RelPosX, (int)floor(GetPosY() - 1), RelPosZ);
	double SpeedX = GetSpeedX(), SpeedY = GetSpeedY(), SpeedZ = GetSpeedZ(); // Get current speed
	
	switch (BelowMeta)
	{
		case E_META_RAIL_ZM_ZP: // NORTHSOUTH
		{
			SetRotation(270);
			SpeedY = 0; // Don't move vertically as on ground
			SpeedX = 0; // Correct diagonal movement from curved rails
			
			if (SpeedZ != 0) // Don't do anything if cart is stationary
			{
				if (SpeedZ > 0)
				{
					// Going SOUTH, slow down
					SpeedZ = SpeedZ - 0.1;
				}
				else
				{
					// Going NORTH, slow down
					SpeedZ = SpeedZ + 0.1;
				}
			}
			break;
		}

		case E_META_RAIL_XM_XP: // EASTWEST
		{
			SetRotation(180);
			SpeedY = 0;
			SpeedZ = 0;

			if (SpeedX != 0)
			{
				if (SpeedX > 0)
				{
					SpeedX = SpeedX - 0.1;
				}
				else
				{
					SpeedX = SpeedX + 0.1;
				}
			}
			break;
		}

		case E_META_RAIL_ASCEND_ZM: // ASCEND NORTH
		{
			SetRotation(270);
			SetPosY(floor(GetPosY()) + 0.2); // It seems it doesn't work without levitation :/
			SpeedX = 0;

			if (SpeedZ >= 0)
			{
				// SpeedZ POSITIVE, going SOUTH
				if (SpeedZ <= MAX_SPEED) // Speed limit
				{
					SpeedZ = SpeedZ + 0.5; // Speed up
					SpeedY = (0 - SpeedZ); // Downward movement is negative (0 minus positive numbers is negative)
				}
				else
				{
					SpeedZ = MAX_SPEED; // Enforce speed limit
					SpeedY = (0 - SpeedZ);
				}
			}
			else
			{
				// SpeedZ NEGATIVE, going NORTH
				SpeedZ = SpeedZ + 0.4; // Slow down
				SpeedY = (0 - SpeedZ); // Upward movement is positive (0 minus negative number is positive number)
			}
			break;
		}

		case E_META_RAIL_ASCEND_ZP: // ASCEND SOUTH
		{
			SetRotation(270);
			SetPosY(floor(GetPosY()) + 0.2);
			SpeedX = 0;

			if (SpeedZ > 0)
			{
				// SpeedZ POSITIVE, going SOUTH
				SpeedZ = SpeedZ - 0.4; // Slow down
				SpeedY = SpeedZ; // Upward movement positive
			}
			else
			{
				if (SpeedZ >= MAX_SPEED_NEGATIVE) // Speed limit
				{
					// SpeedZ NEGATIVE, going NORTH
					SpeedZ = SpeedZ - 0.5; // Speed up
					SpeedY = SpeedZ; // Downward movement negative
				}
				else
				{
					SpeedZ = MAX_SPEED_NEGATIVE; // Enforce speed limit
					SpeedY = SpeedZ;
				}
			}
			break;
		}

		case E_META_RAIL_ASCEND_XM: // ASCEND EAST
		{
			SetRotation(180);
			SetPosY(floor(GetPosY()) + 0.2);
			SpeedZ = 0;

			if (SpeedX >= 0)
			{
				if (SpeedX <= MAX_SPEED)
				{
					SpeedX = SpeedX + 0.5;
					SpeedY = (0 - SpeedX);
				}
				else
				{
					SpeedX = MAX_SPEED;
					SpeedY = (0 - SpeedX);
				}
			}
			else
			{
				SpeedX = SpeedX + 0.4;
				SpeedY = (0 - SpeedX);
			}
			break;
		}

		case E_META_RAIL_ASCEND_XP: // ASCEND WEST
		{
			SetRotation(180);
			SetPosY(floor(GetPosY()) + 0.2);
			SpeedZ = 0;

			if (SpeedX > 0)
			{
				SpeedX = SpeedX - 0.4;
				SpeedY = SpeedX;
			}
			else
			{
				if (SpeedX >= MAX_SPEED_NEGATIVE)
				{
					SpeedX = SpeedX - 0.5;
					SpeedY = SpeedX;
				}
				else
				{
					SpeedX = MAX_SPEED_NEGATIVE;
					SpeedY = SpeedX;
				}
			}
			break;
		}

		case E_META_RAIL_CURVED_ZM_XM: // Ends pointing NORTH and WEST
		{
			SetRotation(315); // Set correct rotation server side
			SetPosY(floor(GetPosY()) + 0.2); // Levitate dat cart

			if (SpeedZ > 0) // Cart moving south
			{
				SpeedX = (0 - SpeedZ); // Diagonally move southwest (which will make cart hit a southwest rail)
			}
			else if (SpeedX > 0) // Cart moving east
			{
				SpeedZ = (0 - SpeedX); // Diagonally move northeast
			}
			break;
		}

		case E_META_RAIL_CURVED_ZM_XP: // Curved NORTH EAST
		{
			SetRotation(225);
			SetPosY(floor(GetPosY()) + 0.2);

			if (SpeedZ > 0)
			{
				SpeedX = SpeedZ;
			}
			else if (SpeedX < 0)
			{
				SpeedZ = SpeedX;
			}
			break;
		}

		case E_META_RAIL_CURVED_ZP_XM: // Curved SOUTH WEST
		{
			SetRotation(135);
			SetPosY(floor(GetPosY()) + 0.2);

			if (SpeedZ < 0)
			{
				SpeedX = SpeedZ;
			}
			else if (SpeedX > 0)
			{
				SpeedZ = SpeedX;
			}
			break;
		}

		case E_META_RAIL_CURVED_ZP_XP: // Curved SOUTH EAST
		{
			SetRotation(45);
			SetPosY(floor(GetPosY()) + 0.2);

			if (SpeedZ < 0)
			{
				SpeedX = (0 - SpeedZ);
			}
			else if (SpeedX < 0)
			{
				SpeedZ = (0 - SpeedX);
			}
			break;
		}

		default:
		{
			ASSERT(!"Unhandled rail meta!"); // Dun dun DUN!
			break;
		}
	}

	// Set speed to speed variables
	SetSpeedX(SpeedX);
	SetSpeedY(SpeedY);
	SetSpeedZ(SpeedZ);


	// Broadcast position to client
	BroadcastMovementUpdate();
}





void cMinecart::DoTakeDamage(TakeDamageInfo & TDI)
{
	super::DoTakeDamage(TDI);

	if (GetHealth() <= 0)
	{
		Destroy(true);
	}
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