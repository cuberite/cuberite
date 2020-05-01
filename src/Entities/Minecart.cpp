
// Minecart.cpp

// Implements the cMinecart class representing a minecart in the world
// Handles physics when a minecart is on any type of rail (overrides simulator in Entity.cpp)
// Indiana Jones!

#include "Globals.h"
#include "Minecart.h"
#include "../BlockInfo.h"
#include "../ClientHandle.h"
#include "../Chunk.h"
#include "Player.h"
#include "../BoundingBox.h"
#include "../UI/MinecartWithChestWindow.h"

#define NO_SPEED 0.0
#define MAX_SPEED 8
#define MAX_SPEED_NEGATIVE -MAX_SPEED




class cMinecartCollisionCallback
{
public:
	cMinecartCollisionCallback(Vector3d a_Pos, double a_Height, double a_Width, UInt32 a_UniqueID, UInt32 a_AttacheeUniqueID) :
		m_DoesIntersect(false),
		m_CollidedEntityPos(0, 0, 0),
		m_Pos(a_Pos),
		m_Height(a_Height),
		m_Width(a_Width),
		m_UniqueID(a_UniqueID),
		m_AttacheeUniqueID(a_AttacheeUniqueID)
	{
	}

	bool operator () (cEntity & a_Entity)
	{
		if (
			(
				!a_Entity.IsPlayer() ||
				static_cast<cPlayer &>(a_Entity).IsGameModeSpectator()  // Spectators doesn't collide with anything
			) &&
			!a_Entity.IsMob() &&
			!a_Entity.IsMinecart() &&
			!a_Entity.IsBoat()
		)
		{
			return false;
		}
		else if ((a_Entity.GetUniqueID() == m_UniqueID) || (a_Entity.GetUniqueID() == m_AttacheeUniqueID))
		{
			return false;
		}

		cBoundingBox bbEntity(a_Entity.GetPosition(), a_Entity.GetWidth() / 2, a_Entity.GetHeight());
		cBoundingBox bbMinecart(Vector3d(m_Pos.x, floor(m_Pos.y), m_Pos.z), m_Width / 2, m_Height);

		if (bbEntity.DoesIntersect(bbMinecart))
		{
			m_CollidedEntityPos = a_Entity.GetPosition();
			m_DoesIntersect = true;
			return true;
		}
		return false;
	}

	bool FoundIntersection(void) const
	{
		return m_DoesIntersect;
	}

	Vector3d GetCollidedEntityPosition(void) const
	{
		return m_CollidedEntityPos;
	}

protected:
	bool m_DoesIntersect;

	Vector3d m_CollidedEntityPos;

	Vector3d m_Pos;
	double m_Height, m_Width;
	UInt32 m_UniqueID;
	UInt32 m_AttacheeUniqueID;
};





////////////////////////////////////////////////////////////////////////////////
// cMinecart:

cMinecart::cMinecart(ePayload a_Payload, Vector3d a_Pos):
	Super(etMinecart, a_Pos, 0.98, 0.7),
	m_Payload(a_Payload),
	m_LastDamage(0),
	m_DetectorRailPosition(0, 0, 0),
	m_bIsOnDetectorRail(false)
{
	SetMass(20.0f);
	SetGravity(-16.0f);
	SetAirDrag(0.05f);
	SetMaxHealth(6);
	SetHealth(6);
	SetWidth(1);
	SetHeight(0.9);
}





void cMinecart::SpawnOn(cClientHandle & a_ClientHandle)
{
	a_ClientHandle.SendSpawnEntity(*this);
	a_ClientHandle.SendEntityMetadata(*this);
}





void cMinecart::HandlePhysics(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	ASSERT(IsTicking());

	int PosY = POSY_TOINT;
	if ((PosY <= 0) || (PosY >= cChunkDef::Height))
	{
		// Outside the world, just process normal falling physics
		Super::HandlePhysics(a_Dt, a_Chunk);
		BroadcastMovementUpdate();
		return;
	}

	auto relPos = a_Chunk.AbsoluteToRelative(GetPosition());
	auto chunk = a_Chunk.GetRelNeighborChunkAdjustCoords(relPos);
	if (chunk == nullptr)
	{
		// Inside an unloaded chunk, bail out all processing
		return;
	}

	BLOCKTYPE InsideType;
	NIBBLETYPE InsideMeta;
	chunk->GetBlockTypeMeta(relPos, InsideType, InsideMeta);

	if (!IsBlockRail(InsideType))
	{
		// When a descending minecart hits a flat rail, it goes through the ground; check for this
		chunk->GetBlockTypeMeta(relPos.addedY(1), InsideType, InsideMeta);
		if (IsBlockRail(InsideType))
		{
			// Push cart upwards
			AddPosY(1);
		}
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

		AddPosition(GetSpeed() * (static_cast<double>(a_Dt.count()) / 1000));  // Commit changes; as we use our own engine when on rails, this needs to be done, whereas it is normally in Entity.cpp
	}
	else
	{
		// Not on rail, default physics
		SetPosY(floor(GetPosY()) + 0.35);  // HandlePhysics overrides this if minecart can fall, else, it is to stop ground clipping minecart bottom when off-rail
		Super::HandlePhysics(a_Dt, *chunk);
	}

	if (m_bIsOnDetectorRail && !Vector3i(POSX_TOINT, POSY_TOINT, POSZ_TOINT).Equals(m_DetectorRailPosition))
	{
		m_World->SetBlock(m_DetectorRailPosition, E_BLOCK_DETECTOR_RAIL, m_World->GetBlockMeta(m_DetectorRailPosition) & 0x07);
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





void cMinecart::HandleRailPhysics(NIBBLETYPE a_RailMeta, std::chrono::milliseconds a_Dt)
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
			if (EntCol || BlckCol)
			{
				return;
			}

			if (GetSpeedZ() != NO_SPEED)  // Don't do anything if cart is stationary
			{
				if (GetSpeedZ() > 0)
				{
					// Going SOUTH, slow down
					ApplyAcceleration({ 0.0, 0.0, 1.0 }, -0.1);
				}
				else
				{
					// Going NORTH, slow down
					ApplyAcceleration({ 0.0, 0.0, -1.0 }, -0.1);
				}
			}
			break;
		}
		case E_META_RAIL_XM_XP:  // EASTWEST
		{
			SetYaw(180);
			SetPosY(floor(GetPosY()) + 0.55);
			SetSpeedY(NO_SPEED);
			SetSpeedZ(NO_SPEED);

			bool BlckCol = TestBlockCollision(a_RailMeta), EntCol = TestEntityCollision(a_RailMeta);
			if (EntCol || BlckCol)
			{
				return;
			}

			if (GetSpeedX() != NO_SPEED)
			{
				if (GetSpeedX() > 0)
				{
					ApplyAcceleration({ 1.0, 0.0, 0.0 }, -0.1);
				}
				else
				{
					ApplyAcceleration({ -1.0, 0.0, 0.0 }, -0.1);
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
				AddSpeedZ(0.5);  // Speed up
				SetSpeedY(-GetSpeedZ());  // Downward movement is negative (0 minus positive numbers is negative)
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
				// SpeedZ NEGATIVE, going NORTH
				AddSpeedZ(-0.5);  // Speed up
				SetSpeedY(GetSpeedZ());  // Downward movement negative
			}
			break;
		}
		case E_META_RAIL_ASCEND_XM:  // ASCEND EAST
		{
			SetYaw(180);
			SetSpeedZ(NO_SPEED);

			if (GetSpeedX() >= NO_SPEED)
			{
				AddSpeedX(0.5);
				SetSpeedY(-GetSpeedX());
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
				AddSpeedX(-0.5);
				SetSpeedY(GetSpeedX());
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
	// If the rail is powered set to speed up else slow down.
	const bool IsRailPowered = ((a_RailMeta & 0x8) == 0x8);
	const double Acceleration = IsRailPowered ? 1.0 : -2.0;

	switch (a_RailMeta & 0x07)
	{
		case E_META_RAIL_ZM_ZP:  // NORTHSOUTH
		{
			SetYaw(270);
			SetPosY(floor(GetPosY()) + 0.55);
			SetSpeedY(0);
			SetSpeedX(0);

			bool BlckCol = TestBlockCollision(a_RailMeta), EntCol = TestEntityCollision(a_RailMeta);
			if (EntCol || BlckCol)
			{
				return;
			}

			if (GetSpeedZ() != NO_SPEED)
			{
				if (GetSpeedZ() > NO_SPEED)
				{
					ApplyAcceleration({ 0.0, 0.0, 1.0 }, Acceleration);
				}
				else
				{
					ApplyAcceleration({ 0.0, 0.0, -1.0 }, Acceleration);
				}
			}
			// If rail is powered check for nearby blocks that could kick-start the minecart
			else if (IsRailPowered)
			{
				bool IsBlockZP = IsSolidBlockAtOffset(0, 0, 1);
				bool IsBlockZM = IsSolidBlockAtOffset(0, 0, -1);
				// Only kick-start the minecart if a block is on one side, but not both
				if (IsBlockZM && !IsBlockZP)
				{
					ApplyAcceleration({ 0.0, 0.0, 1.0 }, Acceleration);
				}
				else if (!IsBlockZM && IsBlockZP)
				{
					ApplyAcceleration({ 0.0, 0.0, -1.0 }, Acceleration);
				}
			}
			break;
		}
		case E_META_RAIL_XM_XP:  // EASTWEST
		{
			SetYaw(180);
			SetPosY(floor(GetPosY()) + 0.55);
			SetSpeedY(NO_SPEED);
			SetSpeedZ(NO_SPEED);

			bool BlckCol = TestBlockCollision(a_RailMeta), EntCol = TestEntityCollision(a_RailMeta);
			if (EntCol || BlckCol)
			{
				return;
			}

			if (GetSpeedX() != NO_SPEED)
			{
				if (GetSpeedX() > NO_SPEED)
				{
					ApplyAcceleration({ 1.0, 0.0, 0.0 }, Acceleration);
				}
				else
				{
					ApplyAcceleration({ -1.0, 0.0, 0.0 }, Acceleration);
				}
			}
			// If rail is powered check for nearby blocks that could kick-start the minecart
			else if (IsRailPowered)
			{
				bool IsBlockXP = IsSolidBlockAtOffset(1, 0, 0);
				bool IsBlockXM = IsSolidBlockAtOffset(-1, 0, 0);
				// Only kick-start the minecart if a block is on one side, but not both
				if (IsBlockXM && !IsBlockXP)
				{
					ApplyAcceleration({ 1.0, 0.0, 0.0 }, Acceleration);
				}
				else if (!IsBlockXM && IsBlockXP)
				{
					ApplyAcceleration({ -1.0, 0.0, 0.0 }, Acceleration);
				}
			}
			break;
		}
		case E_META_RAIL_ASCEND_XM:  // ASCEND EAST
		{
			SetYaw(180);
			SetSpeedZ(NO_SPEED);

			if (GetSpeedX() >= NO_SPEED)
			{
				ApplyAcceleration({ 1.0, 0.0, 0.0 }, Acceleration);
				SetSpeedY(-GetSpeedX());
			}
			else
			{
				ApplyAcceleration({ -1.0, 0.0, 0.0 }, Acceleration);
				SetSpeedY(-GetSpeedX());
			}
			break;
		}
		case E_META_RAIL_ASCEND_XP:  // ASCEND WEST
		{
			SetYaw(180);
			SetSpeedZ(NO_SPEED);

			if (GetSpeedX() > NO_SPEED)
			{
				ApplyAcceleration({ 1.0, 0.0, 0.0 }, Acceleration);
				SetSpeedY(GetSpeedX());
			}
			else
			{
				ApplyAcceleration({ -1.0, 0.0, 0.0 }, Acceleration);
				SetSpeedY(GetSpeedX());
			}
			break;
		}
		case E_META_RAIL_ASCEND_ZM:  // ASCEND NORTH
		{
			SetYaw(270);
			SetSpeedX(NO_SPEED);

			if (GetSpeedZ() >= NO_SPEED)
			{
				ApplyAcceleration({ 0.0, 0.0, 1.0 }, Acceleration);
				SetSpeedY(-GetSpeedZ());
			}
			else
			{
				ApplyAcceleration({ 0.0, 0.0, -1.0 }, Acceleration);
				SetSpeedY(-GetSpeedZ());
			}
			break;
		}
		case E_META_RAIL_ASCEND_ZP:  // ASCEND SOUTH
		{
			SetYaw(270);
			SetSpeedX(NO_SPEED);

			if (GetSpeedZ() > NO_SPEED)
			{
				ApplyAcceleration({ 0.0, 0.0, 1.0 }, Acceleration);
				SetSpeedY(GetSpeedZ());
			}
			else
			{
				ApplyAcceleration({ 0.0, 0.0, -1.0 }, Acceleration);
				SetSpeedY(GetSpeedZ());
			}
			break;
		}
		default: ASSERT(!"Unhandled powered rail metadata!"); break;
	}
}





void cMinecart::HandleDetectorRailPhysics(NIBBLETYPE a_RailMeta, std::chrono::milliseconds a_Dt)
{
	m_World->SetBlockMeta(m_DetectorRailPosition, a_RailMeta | 0x08);

	// No special handling
	HandleRailPhysics(a_RailMeta & 0x07, a_Dt);
}





void cMinecart::HandleActivatorRailPhysics(NIBBLETYPE a_RailMeta, std::chrono::milliseconds a_Dt)
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
			SetSpeedZ(NO_SPEED);
			SetPosZ(floor(GetPosZ()) + 0.5);
			break;
		}
		case E_META_RAIL_ASCEND_ZM:
		case E_META_RAIL_ASCEND_ZP:
		case E_META_RAIL_ZM_ZP:
		{
			SetSpeedX(NO_SPEED);
			SetPosX(floor(GetPosX()) + 0.5);
			break;
		}
		// Curved rail physics: once minecart has reached more than half of the block in the direction that it is travelling in, jerk it in the direction of curvature
		case E_META_RAIL_CURVED_ZM_XM:
		{
			if (GetPosZ() > floor(GetPosZ()) + 0.5)
			{
				if (GetSpeedZ() > NO_SPEED)
				{
					SetSpeedX(-GetSpeedZ() * 0.7);
				}

				SetSpeedZ(NO_SPEED);
				SetPosZ(floor(GetPosZ()) + 0.5);
			}
			else if (GetPosX() > floor(GetPosX()) + 0.5)
			{
				if (GetSpeedX() > 0)
				{
					SetSpeedZ(-GetSpeedX() * 0.7);
				}

				SetSpeedX(NO_SPEED);
				SetPosX(floor(GetPosX()) + 0.5);
			}
			SetSpeedY(NO_SPEED);
			break;
		}
		case E_META_RAIL_CURVED_ZM_XP:
		{
			if (GetPosZ() > floor(GetPosZ()) + 0.5)
			{
				if (GetSpeedZ() > NO_SPEED)
				{
					SetSpeedX(GetSpeedZ() * 0.7);
				}

				SetSpeedZ(NO_SPEED);
				SetPosZ(floor(GetPosZ()) + 0.5);
			}
			else if (GetPosX() < floor(GetPosX()) + 0.5)
			{
				if (GetSpeedX() < NO_SPEED)
				{
					SetSpeedZ(GetSpeedX() * 0.7);
				}

				SetSpeedX(NO_SPEED);
				SetPosX(floor(GetPosX()) + 0.5);
			}
			SetSpeedY(NO_SPEED);
			break;
		}
		case E_META_RAIL_CURVED_ZP_XM:
		{
			if (GetPosZ() < floor(GetPosZ()) + 0.5)
			{
				if (GetSpeedZ() < NO_SPEED)
				{
					SetSpeedX(GetSpeedZ() * 0.7);
				}

				SetSpeedZ(NO_SPEED);
				SetPosZ(floor(GetPosZ()) + 0.5);
			}
			else if (GetPosX() > floor(GetPosX()) + 0.5)
			{
				if (GetSpeedX() > NO_SPEED)
				{
					SetSpeedZ(GetSpeedX() * 0.7);
				}

				SetSpeedX(NO_SPEED);
				SetPosX(floor(GetPosX()) + 0.5);
			}
			SetSpeedY(NO_SPEED);
			break;
		}
		case E_META_RAIL_CURVED_ZP_XP:
		{
			if (GetPosZ() < floor(GetPosZ()) + 0.5)
			{
				if (GetSpeedZ() < NO_SPEED)
				{
					SetSpeedX(-GetSpeedZ() * 0.7);
				}

				SetSpeedZ(NO_SPEED);
				SetPosZ(floor(GetPosZ()) + 0.5);
			}
			else if (GetPosX() < floor(GetPosX()) + 0.5)
			{
				if (GetSpeedX() < NO_SPEED)
				{
					SetSpeedZ(-GetSpeedX() * 0.7);
				}

				SetSpeedX(NO_SPEED);
				SetPosX(floor(GetPosX()) + 0.5);
			}
			SetSpeedY(0);
			break;
		}
		default: break;
	}
}





bool cMinecart::IsSolidBlockAtOffset(int a_XOffset, int a_YOffset, int a_ZOffset)
{
	BLOCKTYPE Block = m_World->GetBlock(POSX_TOINT + a_XOffset, POSY_TOINT + a_YOffset, POSZ_TOINT + a_ZOffset);
	if (IsBlockRail(Block) || !cBlockInfo::IsSolid(Block))
	{
		return false;
	}
	return true;
}





bool cMinecart::TestBlockCollision(NIBBLETYPE a_RailMeta)
{
	switch (a_RailMeta)
	{
		case E_META_RAIL_ZM_ZP:
		{
			if (GetSpeedZ() > 0)
			{
				if (IsSolidBlockAtOffset(0, 0, 1))
				{
					// We could try to detect a block in front based purely on coordinates, but xoft made a bounding box system - why not use? :P
					cBoundingBox bbBlock(Vector3d(POSX_TOINT, POSY_TOINT, static_cast<int>(ceil(GetPosZ()))), 0.5, 1);
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
				if (IsSolidBlockAtOffset(0, 0, -1))
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
				if (IsSolidBlockAtOffset(1, 0, 0))
				{
					cBoundingBox bbBlock(Vector3d(static_cast<int>(ceil(GetPosX())), POSY_TOINT, POSZ_TOINT), 0.5, 1);
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
				if (IsSolidBlockAtOffset(-1, 0, 0))
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
		{
			bool IsBlockXM = IsSolidBlockAtOffset(-1, 0, 0);
			bool IsBlockZM = IsSolidBlockAtOffset(0, 0, -1);

			if (((GetSpeedZ() < 0) && IsBlockZM) || ((GetSpeedX() < 0) && IsBlockXM))
			{
				SetSpeed(0, 0, 0);
				SetPosition(POSX_TOINT + 0.5, GetPosY(), POSZ_TOINT + 0.5);
				return true;
			}

			break;
		}
		case E_META_RAIL_CURVED_ZM_XP:
		{
			bool IsBlockXP = IsSolidBlockAtOffset(1, 0, 0);
			bool IsBlockZM = IsSolidBlockAtOffset(0, 0, -1);

			if (((GetSpeedZ() < 0) &&  IsBlockZM) || ((GetSpeedX() > 0) && IsBlockXP))
			{
				SetSpeed(0, 0, 0);
				SetPosition(POSX_TOINT + 0.5, GetPosY(), POSZ_TOINT + 0.5);
				return true;
			}

			break;
		}
		case E_META_RAIL_CURVED_ZP_XM:
		{
			bool IsBlockXM = IsSolidBlockAtOffset(-1, 0, 0);
			bool IsBlockZP = IsSolidBlockAtOffset(0, 0, +1);

			if (((GetSpeedZ() > 0) && IsBlockZP) || ((GetSpeedX() < 0) && IsBlockXM))
			{
				SetSpeed(0, 0, 0);
				SetPosition(POSX_TOINT + 0.5, GetPosY(), POSZ_TOINT + 0.5);
				return true;
			}

			break;
		}
		case E_META_RAIL_CURVED_ZP_XP:
		{
			bool IsBlockXP = IsSolidBlockAtOffset(1, 0, 0);
			bool IsBlockZP = IsSolidBlockAtOffset(0, 0, 1);

			if (((GetSpeedZ() > 0) && IsBlockZP) || ((GetSpeedX() > 0) && IsBlockXP))
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
	cMinecartCollisionCallback MinecartCollisionCallback(
		GetPosition(), GetHeight(), GetWidth(), GetUniqueID(),
		((m_Attachee == nullptr) ? cEntity::INVALID_ID : m_Attachee->GetUniqueID())
	);
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
				if (GetSpeedZ() > 0)  // True if minecart is moving into the direction of the entity
				{
					SetSpeedZ(0);  // Entity handles the pushing
				}
			}
			else  // if (MinecartCollisionCallback.GetCollidedEntityPosition().z < GetPosZ())
			{
				if (GetSpeedZ() < 0)  // True if minecart is moving into the direction of the entity
				{
					SetSpeedZ(0);  // Entity handles the pushing
				}
			}
			return true;
		}
		case E_META_RAIL_XM_XP:
		{
			if (MinecartCollisionCallback.GetCollidedEntityPosition().x >= GetPosX())
			{
				if (GetSpeedX() > 0)  // True if minecart is moving into the direction of the entity
				{
					SetSpeedX(0);  // Entity handles the pushing
				}
			}
			else  // if (MinecartCollisionCallback.GetCollidedEntityPosition().x < GetPosX())
			{
				if (GetSpeedX() < 0)  // True if minecart is moving into the direction of the entity
				{
					SetSpeedX(0);  // Entity handles the pushing
				}
			}
			return true;
		}
		case E_META_RAIL_CURVED_ZM_XM:
		case E_META_RAIL_CURVED_ZP_XP:
		{
			Vector3d Distance = MinecartCollisionCallback.GetCollidedEntityPosition() - Vector3d(GetPosX(), 0, GetPosZ());

			// Prevent division by small numbers
			if (std::abs(Distance.z) < 0.001)
			{
				Distance.z = 0.001;
			}

			/* Check to which side the minecart is to be pushed.
			Let's consider a z-x-coordinate system where the minecart is the center (0, 0).
			The minecart moves along the line x = -z, the perpendicular line to this is x = z.
			In order to decide to which side the minecart is to be pushed, it must be checked on what side of the perpendicular line the pushing entity is located. */
			if (
				((Distance.z > 0) && ((Distance.x / Distance.z) >= 1)) ||
				((Distance.z < 0) && ((Distance.x / Distance.z) <= 1))
			)
			{
				// Moving -X +Z
				if ((-GetSpeedX() * 0.4 / sqrt(2.0)) < 0.01)
				{
					// ~ SpeedX >= 0 Immobile or not moving in the "right" direction. Give it a bump!
					AddSpeedX(-4 / sqrt(2.0));
					AddSpeedZ(4 / sqrt(2.0));
				}
				else
				{
					// ~ SpeedX < 0 Moving in the "right" direction. Only accelerate it a bit.
					SetSpeedX(GetSpeedX() * 0.4 / sqrt(2.0));
					SetSpeedZ(GetSpeedZ() * 0.4 / sqrt(2.0));
				}
			}
			else if ((GetSpeedX() * 0.4 / sqrt(2.0)) < 0.01)
			{
				// Moving +X -Z
				// ~ SpeedX <= 0 Immobile or not moving in the "right" direction
				AddSpeedX(4 / sqrt(2.0));
				AddSpeedZ(-4 / sqrt(2.0));
			}
			else
			{
				// ~ SpeedX > 0 Moving in the "right" direction
				SetSpeedX(GetSpeedX() * 0.4 / sqrt(2.0));
				SetSpeedZ(GetSpeedZ() * 0.4 / sqrt(2.0));
			}
			break;
		}
		case E_META_RAIL_CURVED_ZM_XP:
		case E_META_RAIL_CURVED_ZP_XM:
		{
			Vector3d Distance = MinecartCollisionCallback.GetCollidedEntityPosition() - Vector3d(GetPosX(), 0, GetPosZ());

			// Prevent division by small numbers
			if (std::abs(Distance.z) < 0.001)
			{
				Distance.z = 0.001;
			}

			/* Check to which side the minecart is to be pushed.
			Let's consider a z-x-coordinate system where the minecart is the center (0, 0).
			The minecart moves along the line x = z, the perpendicular line to this is x = -z.
			In order to decide to which side the minecart is to be pushed, it must be checked on what side of the perpendicular line the pushing entity is located. */
			if (
				((Distance.z > 0) && ((Distance.x / Distance.z) <= -1)) ||
				((Distance.z < 0) && ((Distance.x / Distance.z) >= -1))
			)
			{
				// Moving +X +Z
				if ((GetSpeedX() * 0.4) < 0.01)
				{
					// ~ SpeedX <= 0 Immobile or not moving in the "right" direction
					AddSpeedX(4 / sqrt(2.0));
					AddSpeedZ(4 / sqrt(2.0));
				}
				else
				{
					// ~ SpeedX > 0 Moving in the "right" direction
					SetSpeedX(GetSpeedX() * 0.4 / sqrt(2.0));
					SetSpeedZ(GetSpeedZ() * 0.4 / sqrt(2.0));
				}
			}
			else if ((-GetSpeedX() * 0.4) < 0.01)
			{
				// Moving -X -Z
				// ~ SpeedX >= 0 Immobile or not moving in the "right" direction
				AddSpeedX(-4 / sqrt(2.0));
				AddSpeedZ(-4 / sqrt(2.0));
			}
			else
			{
				// ~ SpeedX < 0 Moving in the "right" direction
				SetSpeedX(GetSpeedX() * 0.4 / sqrt(2.0));
				SetSpeedZ(GetSpeedZ() * 0.4 / sqrt(2.0));
			}
			break;
		}
		default: break;
	}

	return false;
}





bool cMinecart::DoTakeDamage(TakeDamageInfo & TDI)
{
	if ((TDI.Attacker != nullptr) && TDI.Attacker->IsPlayer() && static_cast<cPlayer *>(TDI.Attacker)->IsGameModeCreative())
	{
		Destroy();
		TDI.FinalDamage = GetMaxHealth();  // Instant hit for creative
		SetInvulnerableTicks(0);
		return Super::DoTakeDamage(TDI);  // No drops for creative
	}

	m_LastDamage = static_cast<int>(TDI.FinalDamage);
	if (!Super::DoTakeDamage(TDI))
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
		}

		m_World->SpawnItemPickups(Drops, GetPosX(), GetPosY(), GetPosZ());
	}
	return true;
}





void cMinecart::ApplyAcceleration(Vector3d a_ForwardDirection, double a_Acceleration)
{
	double CurSpeed = GetSpeed().Dot(a_ForwardDirection);
	double NewSpeed = CurSpeed + a_Acceleration;

	if (NewSpeed < 0.0)
	{
		// Prevent deceleration from turning the minecart around.
		NewSpeed = 0.0;
	}

	auto Acceleration = a_ForwardDirection * (NewSpeed - CurSpeed);

	AddSpeed(Acceleration);
}





void cMinecart::DoSetSpeed(double a_SpeedX, double a_SpeedY, double a_SpeedZ)
{
	if (a_SpeedX > MAX_SPEED)
	{
		a_SpeedX = MAX_SPEED;
	}
	else if (a_SpeedX < MAX_SPEED_NEGATIVE)
	{
		a_SpeedX = MAX_SPEED_NEGATIVE;
	}
	if (a_SpeedY > MAX_SPEED)
	{
		a_SpeedY = MAX_SPEED;
	}
	else if (a_SpeedY < MAX_SPEED_NEGATIVE)
	{
		a_SpeedY = MAX_SPEED_NEGATIVE;
	}
	if (a_SpeedZ > MAX_SPEED)
	{
		a_SpeedZ = MAX_SPEED;
	}
	else if (a_SpeedZ < MAX_SPEED_NEGATIVE)
	{
		a_SpeedZ = MAX_SPEED_NEGATIVE;
	}

	Super::DoSetSpeed(a_SpeedX, a_SpeedY, a_SpeedZ);
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

cRideableMinecart::cRideableMinecart(Vector3d a_Pos, const cItem & a_Content, int a_Height):
	Super(mpNone, a_Pos),
	m_Content(a_Content),
	m_Height(a_Height)
{
}





void cRideableMinecart::OnRightClicked(cPlayer & a_Player)
{
	Super::OnRightClicked(a_Player);

	if (m_Attachee != nullptr)
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

cMinecartWithChest::cMinecartWithChest(Vector3d a_Pos):
	Super(mpChest, a_Pos),
	cEntityWindowOwner(this),
	m_Contents(ContentsWidth, ContentsHeight)
{
	m_Contents.AddListener(*this);
}





void cMinecartWithChest::OnRightClicked(cPlayer & a_Player)
{
	// If the window is not created, open it anew:
	cWindow * Window = GetWindow();
	if (Window == nullptr)
	{
		OpenNewWindow();
		Window = GetWindow();
	}

	// Open the window for the player:
	if (Window != nullptr)
	{
		if (a_Player.GetWindow() != Window)
		{
			a_Player.OpenWindow(*Window);
		}
	}
}





void cMinecartWithChest::OpenNewWindow()
{
	OpenWindow(new cMinecartWithChestWindow(this));
}





void cMinecartWithChest::Destroyed()
{
	if (GetWindow() != nullptr)
	{
		GetWindow()->OwnerDestroyed();
	}
	cItems Pickups;
	m_Contents.CopyToItems(Pickups);


	// Schedule the pickups creation for the next world tick
	// This avoids a deadlock when terminating the world
	// Note that the scheduled task may be run when this object is no longer valid, we need to store everything in the task's captured variables
	auto posX = GetPosX();
	auto posY = GetPosY() + 1;
	auto posZ = GetPosZ();
	GetWorld()->ScheduleTask(1, [Pickups, posX, posY, posZ](cWorld & World)
	{
		World.SpawnItemPickups(Pickups, posX, posY, posZ, 4);
	});
}





////////////////////////////////////////////////////////////////////////////////
// cMinecartWithFurnace:

cMinecartWithFurnace::cMinecartWithFurnace(Vector3d a_Pos):
	Super(mpFurnace, a_Pos),
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





void cMinecartWithFurnace::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	Super::Tick(a_Dt, a_Chunk);
	if (!IsTicking())
	{
		// The base class tick destroyed us
		return;
	}

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

cMinecartWithTNT::cMinecartWithTNT(Vector3d a_Pos):
	Super(mpTNT, a_Pos)
{
}

// TODO: Make it activate when passing over activator rail





////////////////////////////////////////////////////////////////////////////////
// cMinecartWithHopper:

cMinecartWithHopper::cMinecartWithHopper(Vector3d a_Pos):
	Super(mpHopper, a_Pos)
{
}

// TODO: Make it suck up blocks and travel further than any other cart and physics and put and take blocks
// AND AVARYTHING!!
