
// Minecart.cpp

// Implements the cMinecart class representing a minecart in the world
// Indiana Jones!

#include "Globals.h"
#include "Minecart.h"
#include "../World.h"
#include "../ClientHandle.h"
#include "../Chunk.h"
#include "Player.h"

#define MAX_SPEED 8
#define MAX_SPEED_NEGATIVE -MAX_SPEED





cMinecart::cMinecart(ePayload a_Payload, double a_X, double a_Y, double a_Z) :
	super(etMinecart, a_X, a_Y, a_Z, 0.98, 0.7),
	m_Payload(a_Payload),
	m_LastDamage(0)
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

	BLOCKTYPE InsideType;
	NIBBLETYPE InsideMeta;
	Chunk->GetBlockTypeMeta(RelPosX, PosY, RelPosZ, InsideType, InsideMeta);

	if (!IsBlockRail(InsideType))
	{
		Chunk->GetBlockTypeMeta(RelPosX, PosY + 1, RelPosZ, InsideType, InsideMeta);
		if (IsBlockRail(InsideType)) AddPosY(1);
	}

	if (IsBlockRail(InsideType))
	{
		SnapToRail(InsideMeta);

		switch (InsideType)
		{
			case E_BLOCK_RAIL: HandleRailPhysics(InsideMeta); break;
			case E_BLOCK_DETECTOR_RAIL: break;
			case E_BLOCK_ACTIVATOR_RAIL: break;
			case E_BLOCK_POWERED_RAIL: HandlePoweredRailPhysics(InsideMeta); break;
			default: VERIFY(!"Unhandled rail type despite checking if block was rail!"); break;
		}

		AddPosition(GetSpeed() * (a_Dt / 1000));
	}
	else
	{
		SetPosY(floor(GetPosY()) + 0.35); // HandlePhysics overrides this if minecart can fall, else, it is to stop ground clipping minecart bottom when off-rail
		super::HandlePhysics(a_Dt, *Chunk);
	}
	
	BroadcastMovementUpdate();
}





void cMinecart::HandleRailPhysics(NIBBLETYPE a_RailMeta)
{
	/*
	NOTE: Please bear in mind that taking away from negatives make them even more negative,
	adding to negatives make them positive, etc.
	*/
	
	switch (a_RailMeta)
	{
		case E_META_RAIL_ZM_ZP: // NORTHSOUTH
		{
			SetRotation(270);
			SetPosY(floor(GetPosY()) + 0.55);
			SetSpeedY(0); // Don't move vertically as on ground
			SetSpeedX(0); // Correct diagonal movement from curved rails
			
			if (GetSpeedZ() != 0) // Don't do anything if cart is stationary
			{
				if (GetSpeedZ() > 0)
				{
					// Going SOUTH, slow down
					AddSpeedZ(-0.1);
				}
				else
				{
					// Going NORTH, slow down
					AddSpeedZ(0.1);
				}
			}
			break;
		}
		case E_META_RAIL_XM_XP: // EASTWEST
		{
			SetRotation(180);
			SetPosY(floor(GetPosY()) + 0.55);
			SetSpeedY(0);
			SetSpeedZ(0);

			if (GetSpeedX() != 0)
			{
				if (GetSpeedX() > 0)
				{
					AddSpeedX(-0.1);
				}
				else
				{
					AddSpeedX(0.1);
				}
			}
			break;
		}
		case E_META_RAIL_ASCEND_ZM: // ASCEND NORTH
		{
			SetRotation(270);
			SetSpeedX(0);

			if (GetSpeedZ() >= 0)
			{
				// SpeedZ POSITIVE, going SOUTH
				if (GetSpeedZ() <= MAX_SPEED) // Speed limit
				{
					AddSpeedZ(0.5); // Speed up
					SetSpeedY(-GetSpeedZ()); // Downward movement is negative (0 minus positive numbers is negative)
				}
			}
			else
			{
				// SpeedZ NEGATIVE, going NORTH
				AddSpeedZ(1); // Slow down
				SetSpeedY(-GetSpeedZ()); // Upward movement is positive (0 minus negative number is positive number)
			}
			break;
		}
		case E_META_RAIL_ASCEND_ZP: // ASCEND SOUTH
		{
			SetRotation(270);
			SetSpeedX(0);

			if (GetSpeedZ() > 0)
			{
				// SpeedZ POSITIVE, going SOUTH
				AddSpeedZ(-1); // Slow down
				SetSpeedY(GetSpeedZ()); // Upward movement positive
			}
			else
			{
				if (GetSpeedZ() >= MAX_SPEED_NEGATIVE) // Speed limit
				{
					// SpeedZ NEGATIVE, going NORTH
					AddSpeedZ(-0.5); // Speed up
					SetSpeedY(GetSpeedZ()); // Downward movement negative
				}
			}
			break;
		}
		case E_META_RAIL_ASCEND_XM: // ASCEND EAST
		{
			SetRotation(180);
			SetSpeedZ(0);

			if (GetSpeedX() >= 0)
			{
				if (GetSpeedX() <= MAX_SPEED)
				{
					AddSpeedX(0.5);
					SetSpeedY(-GetSpeedX());
				}
			}
			else
			{
				AddSpeedX(1);
				SetSpeedY(-GetSpeedX());
			}
			break;
		}
		case E_META_RAIL_ASCEND_XP: // ASCEND WEST
		{
			SetRotation(180);
			SetSpeedZ(0);

			if (GetSpeedX() > 0)
			{
				AddSpeedX(-1);
				SetSpeedY(GetSpeedX());
			}
			else
			{
				if (GetSpeedX() >= MAX_SPEED_NEGATIVE)
				{
					AddSpeedX(-0.5);
					SetSpeedY(GetSpeedX());
				}
			}
			break;
		}
		case E_META_RAIL_CURVED_ZM_XM: // Ends pointing NORTH and WEST
		{
			SetRotation(315); // Set correct rotation server side
			SetPosY(floor(GetPosY()) + 0.3); // Levitate dat cart

			if (GetSpeedZ() > 0) // Cart moving south
			{
				SetSpeedX(-GetSpeedZ()); // Diagonally move southwest (which will make cart hit a southwest rail)
			}
			else if (GetSpeedX() > 0) // Cart moving east
			{
				SetSpeedZ(-GetSpeedX()); // Diagonally move northeast
			}
			break;
		}
		case E_META_RAIL_CURVED_ZM_XP: // Curved NORTH EAST
		{
			SetRotation(225);
			SetPosY(floor(GetPosY()) + 0.3);

			if (GetSpeedZ() > 0)
			{
				SetSpeedX(GetSpeedZ());
			}
			else if (GetSpeedX() < 0)
			{
				SetSpeedZ(GetSpeedX());
			}
			break;
		}
		case E_META_RAIL_CURVED_ZP_XM: // Curved SOUTH WEST
		{
			SetRotation(135);
			SetPosY(floor(GetPosY()) + 0.3);

			if (GetSpeedZ() < 0)
			{
				SetSpeedX(GetSpeedZ());
			}
			else if (GetSpeedX() > 0)
			{
				SetSpeedZ(GetSpeedX());
			}
			break;
		}
		case E_META_RAIL_CURVED_ZP_XP: // Curved SOUTH EAST
		{
			SetRotation(45);
			SetPosY(floor(GetPosY()) + 0.3);

			if (GetSpeedZ() < 0)
			{
				SetSpeedX(-GetSpeedZ());
			}
			else if (GetSpeedX() < 0)
			{
				SetSpeedZ(-GetSpeedX());
			}
			break;
		}
		default:
		{
			ASSERT(!"Unhandled rail meta!"); // Dun dun DUN!
			break;
		}
	}
}




void cMinecart::HandlePoweredRailPhysics(NIBBLETYPE a_RailMeta)
{
	if ((a_RailMeta & 0x8) == 0x8)
	{
		switch (a_RailMeta & 0x07)
		{
			case E_META_RAIL_ZM_ZP: // NORTHSOUTH
			{
				SetRotation(270);
				SetPosY(floor(GetPosY()) + 0.55);
				SetSpeedY(0); // Don't move vertically as on ground
				SetSpeedX(0); // Correct diagonal movement from curved rails
			
				if (GetSpeedZ() != 0) // Don't do anything if cart is stationary
				{
					if (GetSpeedZ() > 0)
					{
						// Going SOUTH, slow down
						AddSpeedZ(1);
					}
					else
					{
						// Going NORTH, slow down
						AddSpeedZ(-1);
					}
				}
				break;
			}
			case E_META_RAIL_XM_XP: // EASTWEST
			{
				SetRotation(180);
				SetPosY(floor(GetPosY()) + 0.55);
				SetSpeedY(0);
				SetSpeedZ(0);

				if (GetSpeedX() != 0)
				{
					if (GetSpeedX() > 0)
					{
						AddSpeedX(-1);
					}
					else
					{
						AddSpeedX(1);
					}
				}
				break;
			}
		}
	}
}





void cMinecart::SnapToRail(NIBBLETYPE a_RailMeta)
{
	switch (a_RailMeta)
	{
		case E_META_RAIL_ASCEND_XM:
		case E_META_RAIL_ASCEND_XP:
		case E_META_RAIL_XM_XP:
		{
			SetSpeedZ(0);
			SetPosZ(floor(GetPosZ()) + 0.3);
			break;
		}
		case E_META_RAIL_ASCEND_ZM:
		case E_META_RAIL_ASCEND_ZP:
		case E_META_RAIL_ZM_ZP:
		{
			SetSpeedX(0);
			SetPosX(floor(GetPosX()) + 0.3);
			break;
		}
		default: break;
	}
}





void cMinecart::DoTakeDamage(TakeDamageInfo & TDI)
{
	if (TDI.Attacker->IsPlayer() && ((cPlayer *)TDI.Attacker)->IsGameModeCreative())
	{
		Destroy();
		TDI.FinalDamage = GetMaxHealth(); // Instant hit for creative
		super::DoTakeDamage(TDI);
		return; // No drops for creative
	}

	m_LastDamage = TDI.FinalDamage;
	super::DoTakeDamage(TDI);

	m_World->BroadcastEntityMetadata(*this);

	if (GetHealth() <= 0)
	{
		Destroy();
		
		cItems Drops;
		switch (m_Payload)
		{
			case mpNone:
			{
				Drops.push_back(cItem(E_ITEM_MINECART, 1, 0));
				break;
			}
			case mpChest:
			{
				Drops.push_back(cItem(E_ITEM_CHEST_MINECART, 1, 0));
				break;
			}
			case mpFurnace:
			{
				Drops.push_back(cItem(E_ITEM_FURNACE_MINECART, 1, 0));
				break;
			}
			case mpTNT:
			{
				Drops.push_back(cItem(E_ITEM_MINECART_WITH_TNT, 1, 0));
				break;
			}
			case mpHopper:
			{
				Drops.push_back(cItem(E_ITEM_MINECART_WITH_HOPPER, 1, 0));
				break;
			}
			default:
			{
				ASSERT(!"Unhandled minecart type when spawning pickup!");
				return;
			}
		}
		
		m_World->SpawnItemPickups(Drops, GetPosX(), GetPosY(), GetPosZ());
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
	super(mpFurnace, a_X, a_Y, a_Z),
	m_IsFueled(false)
{
}





void cMinecartWithFurnace::OnRightClicked(cPlayer & a_Player)
{
	if (a_Player.GetEquippedItem().m_ItemType == E_ITEM_COAL)
	{
		if (!a_Player.IsGameModeCreative())
		{
			a_Player.GetInventory().RemoveOneEquippedItem();
		}

		m_IsFueled = true;
		m_World->BroadcastEntityMetadata(*this);
	}
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