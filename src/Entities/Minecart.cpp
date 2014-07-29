
// Minecart.cpp

// Implements the cMinecart class representing a minecart in the world
// Handles physics when a minecart is on any type of rail (overrides simulator in Entity.cpp)
// Indiana Jones!

#include "Globals.h"
#include "Minecart.h"
#include "../World.h"
#include "../ClientHandle.h"
#include "../Chunk.h"
#include "Player.h"
#include "../BoundingBox.h"

#define MAX_SPEED 8
#define MAX_SPEED_NEGATIVE -MAX_SPEED




class cMinecartCollisionCallback :
	public cEntityCallback
{
public:
	cMinecartCollisionCallback(Vector3d a_Pos, double a_Height, double a_Width, int a_UniqueID, int a_AttacheeUniqueID) :
		m_DoesInteserct(false),
		m_CollidedEntityPos(0, 0, 0),
		m_Pos(a_Pos),
		m_Height(a_Height),
		m_Width(a_Width),
		m_UniqueID(a_UniqueID),
		m_AttacheeUniqueID(a_AttacheeUniqueID)
	{
	}

	virtual bool Item(cEntity * a_Entity) override
	{
		ASSERT(a_Entity != NULL);

		if (!a_Entity->IsPlayer() && !a_Entity->IsMob() && !a_Entity->IsMinecart() && !a_Entity->IsBoat())
		{
			return false;
		}
		else if ((a_Entity->GetUniqueID() == m_UniqueID) || (a_Entity->GetUniqueID() == m_AttacheeUniqueID))
		{
			return false;
		}

		cBoundingBox bbEntity(a_Entity->GetPosition(), a_Entity->GetWidth() / 2, a_Entity->GetHeight());
		cBoundingBox bbMinecart(Vector3d(m_Pos.x, floor(m_Pos.y), m_Pos.z), m_Width / 2, m_Height);

		if (bbEntity.DoesIntersect(bbMinecart))
		{
			m_CollidedEntityPos = a_Entity->GetPosition();
			m_DoesInteserct = true;
			return true;
		}
		return false;
	}

	bool FoundIntersection(void) const
	{
		return m_DoesInteserct;
	}

	Vector3d GetCollidedEntityPosition(void) const
	{
		return m_CollidedEntityPos;
	}

protected:
	bool m_DoesInteserct;

	Vector3d m_CollidedEntityPos;

	Vector3d m_Pos;
	double m_Height, m_Width;
	int m_UniqueID;
	int m_AttacheeUniqueID;
};





cMinecart::cMinecart(ePayload a_Payload, double a_X, double a_Y, double a_Z) :
	super(etMinecart, a_X, a_Y, a_Z, 0.98, 0.7),
	m_Payload(a_Payload),
	m_LastDamage(0),
	m_DetectorRailPosition(0, 0, 0),
	m_bIsOnDetectorRail(false)
{
	SetMass(20.f);
	SetMaxHealth(6);
	SetHealth(6);
	SetWidth(1);
	SetHeight(0.9);
}




void cMinecart::SpawnOn(cClientHandle & a_ClientHandle)
{
	a_ClientHandle.SendSpawnVehicle(*this, 10, (char)m_Payload);  // 10 = Minecarts
	a_ClientHandle.SendEntityMetadata(*this);
}





void cMinecart::HandlePhysics(float a_Dt, cChunk & a_Chunk)
{
	if (IsDestroyed())  // Mainly to stop detector rails triggering again after minecart is dead
	{
		return;
	}

	int PosY = POSY_TOINT;
	if ((PosY <= 0) || (PosY >= cChunkDef::Height))
	{
		// Outside the world, just process normal falling physics
		super::HandlePhysics(a_Dt, a_Chunk);
		BroadcastMovementUpdate();
		return;
	}
	
	int RelPosX = POSX_TOINT - a_Chunk.GetPosX() * cChunkDef::Width;
	int RelPosZ = POSZ_TOINT - a_Chunk.GetPosZ() * cChunkDef::Width;
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
		Chunk->GetBlockTypeMeta(RelPosX, PosY + 1, RelPosZ, InsideType, InsideMeta);  // When an descending minecart hits a flat rail, it goes through the ground; check for this
		if (IsBlockRail(InsideType)) AddPosY(1);  // Push cart upwards
	}

	bool WasDetectorRail = false;
	if (IsBlockRail(InsideType))
	{
		if (InsideType == E_BLOCK_RAIL)
		{
			SnapToRail(InsideMeta);
		}
		else
		{
			SnapToRail(InsideMeta & 0x07);
		}

		switch (InsideType)
		{
			case E_BLOCK_RAIL: HandleRailPhysics(InsideMeta, a_Dt); break;
			case E_BLOCK_ACTIVATOR_RAIL: break;
			case E_BLOCK_POWERED_RAIL: HandlePoweredRailPhysics(InsideMeta); break;
			case E_BLOCK_DETECTOR_RAIL:
			{
				HandleDetectorRailPhysics(InsideMeta, a_Dt);
				WasDetectorRail = true;
				break;
			}
			default: VERIFY(!"Unhandled rail type despite checking if block was rail!"); break;
		}

		AddPosition(GetSpeed() * (a_Dt / 1000));  // Commit changes; as we use our own engine when on rails, this needs to be done, whereas it is normally in Entity.cpp
	}
	else
	{
		// Not on rail, default physics
		SetPosY(floor(GetPosY()) + 0.35);  // HandlePhysics overrides this if minecart can fall, else, it is to stop ground clipping minecart bottom when off-rail
		super::HandlePhysics(a_Dt, *Chunk);
	}

	if (m_bIsOnDetectorRail && !Vector3i(POSX_TOINT, POSY_TOINT, POSZ_TOINT).Equals(m_DetectorRailPosition))
	{
		m_World->SetBlock(m_DetectorRailPosition.x, m_DetectorRailPosition.y, m_DetectorRailPosition.z, E_BLOCK_DETECTOR_RAIL, m_World->GetBlockMeta(m_DetectorRailPosition) & 0x07);
		m_bIsOnDetectorRail = false;
	}
	else if (WasDetectorRail)
	{
		m_bIsOnDetectorRail = true;
		m_DetectorRailPosition = Vector3i(POSX_TOINT, POSY_TOINT, POSZ_TOINT);
	}
	
	// Broadcast positioning changes to client
	BroadcastMovementUpdate();
}





void cMinecart::HandleRailPhysics(NIBBLETYPE a_RailMeta, float a_Dt)
{
	/*
	NOTE: Please bear in mind that taking away from negatives make them even more negative,
	adding to negatives make them positive, etc.
	*/
	
	switch (a_RailMeta)
	{
		case E_META_RAIL_ZM_ZP:  // NORTHSOUTH
		{
			SetYaw(270);
			SetPosY(floor(GetPosY()) + 0.55);
			SetSpeedY(0);  // Don't move vertically as on ground
			SetSpeedX(0);  // Correct diagonal movement from curved rails

			// Execute both the entity and block collision checks
			bool BlckCol = TestBlockCollision(a_RailMeta), EntCol = TestEntityCollision(a_RailMeta);
			if (EntCol || BlckCol) return;
			
			if (GetSpeedZ() != 0)  // Don't do anything if cart is stationary
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
		case E_META_RAIL_XM_XP:  // EASTWEST
		{
			SetYaw(180);
			SetPosY(floor(GetPosY()) + 0.55);
			SetSpeedY(0);
			SetSpeedZ(0);

			bool BlckCol = TestBlockCollision(a_RailMeta), EntCol = TestEntityCollision(a_RailMeta);
			if (EntCol || BlckCol) return;

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
		case E_META_RAIL_ASCEND_ZM:  // ASCEND NORTH
		{
			SetYaw(270);
			SetSpeedX(0);

			if (GetSpeedZ() >= 0)
			{
				// SpeedZ POSITIVE, going SOUTH
				if (GetSpeedZ() <= MAX_SPEED)  // Speed limit
				{
					AddSpeedZ(0.5);  // Speed up
					SetSpeedY(-GetSpeedZ());  // Downward movement is negative (0 minus positive numbers is negative)
				}
			}
			else
			{
				// SpeedZ NEGATIVE, going NORTH
				AddSpeedZ(1);  // Slow down
				SetSpeedY(-GetSpeedZ());  // Upward movement is positive (0 minus negative number is positive number)
			}
			break;
		}
		case E_META_RAIL_ASCEND_ZP:  // ASCEND SOUTH
		{
			SetYaw(270);
			SetSpeedX(0);

			if (GetSpeedZ() > 0)
			{
				// SpeedZ POSITIVE, going SOUTH
				AddSpeedZ(-1);  // Slow down
				SetSpeedY(GetSpeedZ());  // Upward movement positive
			}
			else
			{
				if (GetSpeedZ() >= MAX_SPEED_NEGATIVE)  // Speed limit
				{
					// SpeedZ NEGATIVE, going NORTH
					AddSpeedZ(-0.5);  // Speed up
					SetSpeedY(GetSpeedZ());  // Downward movement negative
				}
			}
			break;
		}
		case E_META_RAIL_ASCEND_XM:  // ASCEND EAST
		{
			SetYaw(180);
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
		case E_META_RAIL_ASCEND_XP:  // ASCEND WEST
		{
			SetYaw(180);
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
		case E_META_RAIL_CURVED_ZM_XM:  // Ends pointing NORTH and WEST
		{
			SetYaw(315);  // Set correct rotation server side
			SetPosY(floor(GetPosY()) + 0.55);  // Levitate dat cart
			SetSpeedY(0);

			TestBlockCollision(a_RailMeta);
			TestEntityCollision(a_RailMeta);

			// SnapToRail handles turning

			break;
		}
		case E_META_RAIL_CURVED_ZM_XP:  // Curved NORTH EAST
		{
			SetYaw(225);
			SetPosY(floor(GetPosY()) + 0.55);
			SetSpeedY(0);

			TestBlockCollision(a_RailMeta);
			TestEntityCollision(a_RailMeta);

			break;
		}
		case E_META_RAIL_CURVED_ZP_XM:  // Curved SOUTH WEST
		{
			SetYaw(135);
			SetPosY(floor(GetPosY()) + 0.55);
			SetSpeedY(0);

			TestBlockCollision(a_RailMeta);
			TestEntityCollision(a_RailMeta);

			break;
		}
		case E_META_RAIL_CURVED_ZP_XP:  // Curved SOUTH EAST
		{
			SetYaw(45);
			SetPosY(floor(GetPosY()) + 0.55);
			SetSpeedY(0);

			TestBlockCollision(a_RailMeta);
			TestEntityCollision(a_RailMeta);

			break;
		}
		default:
		{
			ASSERT(!"Unhandled rail meta!");  // Dun dun DUN!
			break;
		}
	}
}




void cMinecart::HandlePoweredRailPhysics(NIBBLETYPE a_RailMeta)
{
	// Initialise to 'slow down' values
	int AccelDecelSpeed = -2;
	int AccelDecelNegSpeed = 2;

	if ((a_RailMeta & 0x8) == 0x8)
	{
		// Rail powered - set variables to 'speed up' values
		AccelDecelSpeed = 1;
		AccelDecelNegSpeed = -1;
	}

	switch (a_RailMeta & 0x07)
	{
		case E_META_RAIL_ZM_ZP:  // NORTHSOUTH
		{
			SetYaw(270);
			SetPosY(floor(GetPosY()) + 0.55);
			SetSpeedY(0);
			SetSpeedX(0);

			bool BlckCol = TestBlockCollision(a_RailMeta), EntCol = TestEntityCollision(a_RailMeta);
			if (EntCol || BlckCol) return;
			
			if (GetSpeedZ() != 0)
			{
				if (GetSpeedZ() > 0)
				{
					AddSpeedZ(AccelDecelSpeed);
				}
				else
				{
					AddSpeedZ(AccelDecelNegSpeed);
				}
			}
			break;
		}
		case E_META_RAIL_XM_XP:  // EASTWEST
		{
			SetYaw(180);
			SetPosY(floor(GetPosY()) + 0.55);
			SetSpeedY(0);
			SetSpeedZ(0);

			bool BlckCol = TestBlockCollision(a_RailMeta), EntCol = TestEntityCollision(a_RailMeta);
			if (EntCol || BlckCol) return;

			if (GetSpeedX() != 0)
			{
				if (GetSpeedX() > 0)
				{
					AddSpeedX(AccelDecelSpeed);
				}
				else
				{
					AddSpeedX(AccelDecelNegSpeed);
				}
			}
			break;
		}
		case E_META_RAIL_ASCEND_XM:  // ASCEND EAST
		{
			SetYaw(180);
			SetSpeedZ(0);

			if (GetSpeedX() >= 0)
			{
				if (GetSpeedX() <= MAX_SPEED)
				{
					AddSpeedX(AccelDecelSpeed);
					SetSpeedY(-GetSpeedX());
				}
			}
			else
			{
				AddSpeedX(AccelDecelNegSpeed);
				SetSpeedY(-GetSpeedX());
			}
			break;
		}
		case E_META_RAIL_ASCEND_XP:  // ASCEND WEST
		{
			SetYaw(180);
			SetSpeedZ(0);

			if (GetSpeedX() > 0)
			{
				AddSpeedX(AccelDecelSpeed);
				SetSpeedY(GetSpeedX());
			}
			else
			{
				if (GetSpeedX() >= MAX_SPEED_NEGATIVE)
				{
					AddSpeedX(AccelDecelNegSpeed);
					SetSpeedY(GetSpeedX());
				}
			}
			break;
		}
		case E_META_RAIL_ASCEND_ZM:  // ASCEND NORTH
		{
			SetYaw(270);
			SetSpeedX(0);

			if (GetSpeedZ() >= 0)
			{
				if (GetSpeedZ() <= MAX_SPEED)
				{
					AddSpeedZ(AccelDecelSpeed);
					SetSpeedY(-GetSpeedZ());
				}
			}
			else
			{
				AddSpeedZ(AccelDecelNegSpeed);
				SetSpeedY(-GetSpeedZ());
			}
			break;
		}
		case E_META_RAIL_ASCEND_ZP:  // ASCEND SOUTH
		{
			SetYaw(270);
			SetSpeedX(0);

			if (GetSpeedZ() > 0)
			{
				AddSpeedZ(AccelDecelSpeed);
				SetSpeedY(GetSpeedZ());
			}
			else
			{
				if (GetSpeedZ() >= MAX_SPEED_NEGATIVE)
				{
					AddSpeedZ(AccelDecelNegSpeed);
					SetSpeedY(GetSpeedZ());
				}
			}
			break;
		}
		default: ASSERT(!"Unhandled powered rail metadata!"); break;
	}
}





void cMinecart::HandleDetectorRailPhysics(NIBBLETYPE a_RailMeta, float a_Dt)
{
	m_World->SetBlockMeta(m_DetectorRailPosition, a_RailMeta | 0x08);

	// No special handling
	HandleRailPhysics(a_RailMeta & 0x07, a_Dt);
}




void cMinecart::HandleActivatorRailPhysics(NIBBLETYPE a_RailMeta, float a_Dt)
{
	HandleRailPhysics(a_RailMeta & 0x07, a_Dt);
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
			SetPosZ(floor(GetPosZ()) + 0.5);
			break;
		}
		case E_META_RAIL_ASCEND_ZM:
		case E_META_RAIL_ASCEND_ZP:
		case E_META_RAIL_ZM_ZP:
		{
			SetSpeedX(0);
			SetPosX(floor(GetPosX()) + 0.5);
			break;
		}
		// Curved rail physics: once minecart has reached more than half of the block in the direction that it is travelling in, jerk it in the direction of curvature
		case E_META_RAIL_CURVED_ZM_XM:
		{
			if (GetPosZ() > floor(GetPosZ()) + 0.5)
			{
				if (GetSpeedZ() > 0)
				{
					SetSpeedX(-GetSpeedZ() * 0.7);
				}

				SetSpeedZ(0);
				SetPosZ(floor(GetPosZ()) + 0.5);
			}
			else if (GetPosX() > floor(GetPosX()) + 0.5)
			{
				if (GetSpeedX() > 0)
				{
					SetSpeedZ(-GetSpeedX() * 0.7);
				}

				SetSpeedX(0);
				SetPosX(floor(GetPosX()) + 0.5);
			}
			SetSpeedY(0);
			break;
		}
		case E_META_RAIL_CURVED_ZM_XP:
		{
			if (GetPosZ() > floor(GetPosZ()) + 0.5)
			{
				if (GetSpeedZ() > 0)
				{
					SetSpeedX(GetSpeedZ() * 0.7);
				}

				SetSpeedZ(0);
				SetPosZ(floor(GetPosZ()) + 0.5);
			}
			else if (GetPosX() < floor(GetPosX()) + 0.5)
			{
				if (GetSpeedX() < 0)
				{
					SetSpeedZ(GetSpeedX() * 0.7);
				}

				SetSpeedX(0);
				SetPosX(floor(GetPosX()) + 0.5);
			}
			SetSpeedY(0);
			break;
		}
		case E_META_RAIL_CURVED_ZP_XM:
		{
			if (GetPosZ() < floor(GetPosZ()) + 0.5)
			{
				if (GetSpeedZ() < 0)
				{
					SetSpeedX(GetSpeedZ() * 0.7);
				}

				SetSpeedZ(0);
				SetPosZ(floor(GetPosZ()) + 0.5);
			}
			else if (GetPosX() > floor(GetPosX()) + 0.5)
			{
				if (GetSpeedX() > 0)
				{
					SetSpeedZ(GetSpeedX() * 0.7);
				}

				SetSpeedX(0);
				SetPosX(floor(GetPosX()) + 0.5);
			}
			SetSpeedY(0);
			break;
		}
		case E_META_RAIL_CURVED_ZP_XP:
		{
			if (GetPosZ() < floor(GetPosZ()) + 0.5)
			{
				if (GetSpeedZ() < 0)
				{
					SetSpeedX(-GetSpeedZ() * 0.7);
				}

				SetSpeedZ(0);
				SetPosZ(floor(GetPosZ()) + 0.5);
			}
			else if (GetPosX() < floor(GetPosX()) + 0.5)
			{
				if (GetSpeedX() < 0)
				{
					SetSpeedZ(-GetSpeedX() * 0.7);
				}

				SetSpeedX(0);
				SetPosX(floor(GetPosX()) + 0.5);
			}
			SetSpeedY(0);
			break;
		}
		default: break;
	}
}





bool cMinecart::TestBlockCollision(NIBBLETYPE a_RailMeta)
{
	switch (a_RailMeta)
	{
		case E_META_RAIL_ZM_ZP:
		{
			if (GetSpeedZ() > 0)
			{
				BLOCKTYPE Block = m_World->GetBlock(POSX_TOINT, POSY_TOINT, (int)ceil(GetPosZ()));
				if (!IsBlockRail(Block) && cBlockInfo::IsSolid(Block))
				{
					// We could try to detect a block in front based purely on coordinates, but xoft made a bounding box system - why not use? :P
					cBoundingBox bbBlock(Vector3d(POSX_TOINT, POSY_TOINT, (int)ceil(GetPosZ())), 0.5, 1);
					cBoundingBox bbMinecart(Vector3d(GetPosX(), floor(GetPosY()), GetPosZ()), GetWidth() / 2, GetHeight());

					if (bbBlock.DoesIntersect(bbMinecart))
					{
						SetSpeed(0, 0, 0);
						SetPosZ(floor(GetPosZ()) + 0.4);
						return true;
					}
				}
			}
			else if (GetSpeedZ() < 0)
			{
				BLOCKTYPE Block = m_World->GetBlock(POSX_TOINT, POSY_TOINT, POSZ_TOINT - 1);
				if (!IsBlockRail(Block) && cBlockInfo::IsSolid(Block))
				{
					cBoundingBox bbBlock(Vector3d(POSX_TOINT, POSY_TOINT, POSZ_TOINT - 1), 0.5, 1);
					cBoundingBox bbMinecart(Vector3d(GetPosX(), floor(GetPosY()), GetPosZ() - 1), GetWidth() / 2, GetHeight());

					if (bbBlock.DoesIntersect(bbMinecart))
					{
						SetSpeed(0, 0, 0);
						SetPosZ(floor(GetPosZ()) + 0.65);
						return true;
					}
				}
			}
			break;
		}
		case E_META_RAIL_XM_XP:
		{
			if (GetSpeedX() > 0)
			{
				BLOCKTYPE Block = m_World->GetBlock((int)ceil(GetPosX()), POSY_TOINT, POSZ_TOINT);
				if (!IsBlockRail(Block) && cBlockInfo::IsSolid(Block))
				{
					cBoundingBox bbBlock(Vector3d((int)ceil(GetPosX()), POSY_TOINT, POSZ_TOINT), 0.5, 1);
					cBoundingBox bbMinecart(Vector3d(GetPosX(), floor(GetPosY()), GetPosZ()), GetWidth() / 2, GetHeight());

					if (bbBlock.DoesIntersect(bbMinecart))
					{
						SetSpeed(0, 0, 0);
						SetPosX(floor(GetPosX()) + 0.4);
						return true;
					}
				}
			}
			else if (GetSpeedX() < 0)
			{
				BLOCKTYPE Block = m_World->GetBlock(POSX_TOINT - 1, POSY_TOINT, POSZ_TOINT);
				if (!IsBlockRail(Block) && cBlockInfo::IsSolid(Block))
				{
					cBoundingBox bbBlock(Vector3d(POSX_TOINT - 1, POSY_TOINT, POSZ_TOINT), 0.5, 1);
					cBoundingBox bbMinecart(Vector3d(GetPosX() - 1, floor(GetPosY()), GetPosZ()), GetWidth() / 2, GetHeight());

					if (bbBlock.DoesIntersect(bbMinecart))
					{
						SetSpeed(0, 0, 0);
						SetPosX(floor(GetPosX()) + 0.65);
						return true;
					}
				}
			}
			break;
		}
		case E_META_RAIL_CURVED_ZM_XM:
		case E_META_RAIL_CURVED_ZM_XP:
		case E_META_RAIL_CURVED_ZP_XM:
		case E_META_RAIL_CURVED_ZP_XP:
		{
			BLOCKTYPE BlockXM = m_World->GetBlock(POSX_TOINT - 1, POSY_TOINT, POSZ_TOINT);
			BLOCKTYPE BlockXP = m_World->GetBlock(POSX_TOINT + 1, POSY_TOINT, POSZ_TOINT);
			BLOCKTYPE BlockZM = m_World->GetBlock(POSX_TOINT, POSY_TOINT, POSZ_TOINT - 1);
			BLOCKTYPE BlockZP = m_World->GetBlock(POSX_TOINT, POSY_TOINT, POSZ_TOINT + 1);
			if (
				(!IsBlockRail(BlockXM) && cBlockInfo::IsSolid(BlockXM)) ||
				(!IsBlockRail(BlockXP) && cBlockInfo::IsSolid(BlockXP)) ||
				(!IsBlockRail(BlockZM) && cBlockInfo::IsSolid(BlockZM)) ||
				(!IsBlockRail(BlockZP) && cBlockInfo::IsSolid(BlockZP))
				)
			{
				SetSpeed(0, 0, 0);
				SetPosition(POSX_TOINT + 0.5, GetPosY(), POSZ_TOINT + 0.5);
				return true;
			}
			break;
		}
		default: break;
	}
	return false;
}




bool cMinecart::TestEntityCollision(NIBBLETYPE a_RailMeta)
{
	cMinecartCollisionCallback MinecartCollisionCallback(GetPosition(), GetHeight(), GetWidth(), GetUniqueID(), ((m_Attachee == NULL) ? -1 : m_Attachee->GetUniqueID()));
	int ChunkX, ChunkZ;
	cChunkDef::BlockToChunk(POSX_TOINT, POSZ_TOINT, ChunkX, ChunkZ);
	m_World->ForEachEntityInChunk(ChunkX, ChunkZ, MinecartCollisionCallback);

	if (!MinecartCollisionCallback.FoundIntersection())
	{
		return false;
	}

	switch (a_RailMeta)
	{
		case E_META_RAIL_ZM_ZP:
		{
			if (MinecartCollisionCallback.GetCollidedEntityPosition().z >= GetPosZ())
			{
				if ((-GetSpeedZ() * 0.4) < 0.01)
				{
					AddSpeedZ(-4);
				}
				else
				{
					SetSpeedZ(-GetSpeedZ() * 0.4);
				}
			}
			else
			{
				if ((GetSpeedZ() * 0.4) < 0.01)
				{
					AddSpeedZ(4);
				}
				else
				{
					SetSpeedZ(GetSpeedZ() * 0.4);
				}
			}
			return true;
		}
		case E_META_RAIL_XM_XP:
		{
			if (MinecartCollisionCallback.GetCollidedEntityPosition().x >= GetPosX())
			{
				if ((-GetSpeedX() * 0.4) < 0.01)
				{
					AddSpeedX(-4);
				}
				else
				{
					SetSpeedX(-GetSpeedX() * 0.4);
				}
			}
			else
			{
				if ((GetSpeedX() * 0.4) < 0.01)
				{
					AddSpeedX(4);
				}
				else
				{
					SetSpeedX(GetSpeedX() * 0.4);
				}
			}
			return true;
		}
		case E_META_RAIL_CURVED_ZM_XM:
		case E_META_RAIL_CURVED_ZM_XP:
		case E_META_RAIL_CURVED_ZP_XM:
		case E_META_RAIL_CURVED_ZP_XP:
		{
			// TODO - simply can't be bothered right now
			break;
		}
		default: break;
	}

	return false;
}





bool cMinecart::DoTakeDamage(TakeDamageInfo & TDI)
{
	if ((TDI.Attacker != NULL) && TDI.Attacker->IsPlayer() && ((cPlayer *)TDI.Attacker)->IsGameModeCreative())
	{
		Destroy();
		TDI.FinalDamage = GetMaxHealth();  // Instant hit for creative
		SetInvulnerableTicks(0);
		return super::DoTakeDamage(TDI);  // No drops for creative
	}

	m_LastDamage = TDI.FinalDamage;
	if (!super::DoTakeDamage(TDI))
	{
		return false;
	}

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
				return true;
			}
		}
		
		m_World->SpawnItemPickups(Drops, GetPosX(), GetPosY(), GetPosZ());
	}
	return true;
}





void cMinecart::Destroyed()
{
	if (m_bIsOnDetectorRail)
	{
		m_World->SetBlock(m_DetectorRailPosition.x, m_DetectorRailPosition.y, m_DetectorRailPosition.z, E_BLOCK_DETECTOR_RAIL, m_World->GetBlockMeta(m_DetectorRailPosition) & 0x07);
	}
}





////////////////////////////////////////////////////////////////////////////////
// cRideableMinecart:

cRideableMinecart::cRideableMinecart(double a_X, double a_Y, double a_Z, const cItem & a_Content, int a_Height) :
	super(mpNone, a_X, a_Y, a_Z),
	m_Content(a_Content),
	m_Height(a_Height)
{
}





void cRideableMinecart::OnRightClicked(cPlayer & a_Player)
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





////////////////////////////////////////////////////////////////////////////////
// cMinecartWithChest:

cMinecartWithChest::cMinecartWithChest(double a_X, double a_Y, double a_Z) :
	super(mpChest, a_X, a_Y, a_Z)
{
}





void cMinecartWithChest::SetSlot(size_t a_Idx, const cItem & a_Item)
{
	ASSERT(a_Idx < ARRAYCOUNT(m_Items));
	
	m_Items[a_Idx] = a_Item;
}





void cMinecartWithChest::OnRightClicked(cPlayer & a_Player)
{
	// Show the chest UI window to the player
	// TODO
}





////////////////////////////////////////////////////////////////////////////////
// cMinecartWithFurnace:

cMinecartWithFurnace::cMinecartWithFurnace(double a_X, double a_Y, double a_Z) :
	super(mpFurnace, a_X, a_Y, a_Z),
	m_FueledTimeLeft(-1),
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
		if (!m_IsFueled)  // We don't want to change the direction by right clicking it.
		{
			AddSpeed(a_Player.GetLookVector().x, 0, a_Player.GetLookVector().z);
		}
		m_IsFueled = true;
		m_FueledTimeLeft = m_FueledTimeLeft + 600;  // The minecart will be active 600 more ticks.
		m_World->BroadcastEntityMetadata(*this);
	}
}





void cMinecartWithFurnace::Tick(float a_Dt, cChunk & a_Chunk)
{
	super::Tick(a_Dt, a_Chunk);

	if (m_IsFueled)
	{
		m_FueledTimeLeft--;
		if (m_FueledTimeLeft < 0)
		{
			m_IsFueled = false;
			m_World->BroadcastEntityMetadata(*this);
			return;
		}

		if (GetSpeed().Length() > 6)
		{
			return;
		}
		AddSpeed(GetSpeed() / 4);
	}
}





////////////////////////////////////////////////////////////////////////////////
// cMinecartWithTNT:

cMinecartWithTNT::cMinecartWithTNT(double a_X, double a_Y, double a_Z) :
	super(mpTNT, a_X, a_Y, a_Z)
{
}

// TODO: Make it activate when passing over activator rail





////////////////////////////////////////////////////////////////////////////////
// cMinecartWithHopper:

cMinecartWithHopper::cMinecartWithHopper(double a_X, double a_Y, double a_Z) :
	super(mpHopper, a_X, a_Y, a_Z)
{
}

// TODO: Make it suck up blocks and travel further than any other cart and physics and put and take blocks
// AND AVARYTHING!!
