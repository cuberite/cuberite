
// Minecart.cpp

// Implements the cMinecart class representing a minecart in the world
// Handles physics when a minecart is on any type of rail (overrides simulator in Entity.cpp)
// Indiana Jones!

#include "Globals.h"
#include "ChunkDef.h"
#include "Defines.h"
#include "Minecart.h"
#include "../BlockInfo.h"
#include "../ClientHandle.h"
#include "../Chunk.h"
#include "Player.h"
#include "../BoundingBox.h"
#include "../Blocks/ChunkInterface.h"
#include "../Blocks/BlockRail.h"
#include "../UI/MinecartWithChestWindow.h"

#define NO_SPEED 0.0
#define MAX_SPEED 8
#define MAX_SPEED_NEGATIVE -MAX_SPEED
#define DIR_NORTH_SOUTH 270
#define DIR_EAST_WEST 180
#define DIR_NORTH_WEST 315
#define DIR_NORTH_EAST 225
#define DIR_SOUTH_WEST 135
#define DIR_SOUTH_EAST 45

class cMinecartAttachCallback
{
public:
	cMinecartAttachCallback(cMinecart & a_Minecart, cEntity * a_Attachee) :
		m_Minecart(a_Minecart), m_Attachee(a_Attachee)
	{
	}

	bool operator()(cEntity & a_Entity)
	{
		// Check if minecart is empty and if given entity is a mob:
		if ((m_Attachee == nullptr) && a_Entity.IsMob())
		{
			// If so, attach to minecart and stop iterating:
			a_Entity.AttachTo(m_Minecart);
			return true;
		}
		return false;
	}

protected:

	cMinecart & m_Minecart;
	cEntity * m_Attachee;
};





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
	Super(etMinecart, a_Pos, 0.98f, 0.7f),
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

	auto RelPos = cChunkDef::AbsoluteToRelative(GetPosition());
	auto Chunk = a_Chunk.GetRelNeighborChunkAdjustCoords(RelPos);
	if (Chunk == nullptr)
	{
		// Inside an unloaded Chunk, bail out all processing
		return;
	}

	if (m_bIsOnDetectorRail && !Vector3i(POSX_TOINT, POSY_TOINT, POSZ_TOINT).Equals(m_DetectorRailPosition))
	{
		// Check if the rail is still there
		if (m_World->GetBlock(m_DetectorRailPosition) == E_BLOCK_DETECTOR_RAIL)
		{
			m_World->SetBlock(m_DetectorRailPosition, m_World->GetBlock(m_DetectorRailPosition));
		}

		m_bIsOnDetectorRail = false;
	}

	BlockState InsideBlock = a_Chunk.GetBlock(RelPos);

	auto ContainedBlock = Chunk->GetBlock(RelPos);
	if (!cBlockRailHandler::IsBlockRail(ContainedBlock))
	{
		// When a descending minecart hits a flat rail, it goes through the ground; check for this
		auto BlockAbove = Chunk->GetBlock(RelPos.addedY(1));
		if (cBlockRailHandler::IsBlockRail(BlockAbove))
		{
			// Push cart upwards
			AddPosY(1);
		}
		else
		{
			// When a minecart gets to a descending rail, it should go down.
			auto BlockBelow = Chunk->GetBlock(RelPos.addedY(-1));
			if (cBlockRailHandler::IsBlockRail(BlockBelow))
			{
				// Push cart downwards
				AddPosY(-1);
			}
		}
	}

	if (cBlockRailHandler::IsBlockRail(InsideBlock))
	{
		if (ContainedBlock.Type() == BlockType::Rail)
		{
			SnapToRail(ContainedBlock);
		}
		else
		{
			SnapToRail(ContainedBlock);
		}

		switch (ContainedBlock.Type())
		{
			case BlockType::Rail: HandleRailPhysics(ContainedBlock, a_Dt); break;
			case BlockType::ActivatorRail: HandleActivatorRailPhysics(InsideBlock, a_Dt); break;
			case BlockType::PoweredRail: HandlePoweredRailPhysics(ContainedBlock); break;
			case BlockType::DetectorRail:
			{
				m_DetectorRailPosition = Vector3i(POSX_TOINT, POSY_TOINT, POSZ_TOINT);
				m_bIsOnDetectorRail = true;
				HandleDetectorRailPhysics(ContainedBlock, a_Dt);
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
		Super::HandlePhysics(a_Dt, *Chunk);
	}

	// Enforce speed limit:
	m_Speed.Clamp(MAX_SPEED_NEGATIVE, MAX_SPEED);

	// Broadcast positioning changes to client
	BroadcastMovementUpdate();
}





void cMinecart::HandleRailPhysics(BlockState a_Rail, std::chrono::milliseconds a_Dt)
{
	/*
	NOTE: Please bear in mind that taking away from negatives make them even more negative,
	adding to negatives make them positive, etc.
	*/

	using namespace Block;
	switch (cBlockRailHandler::GetShapeFromRail(a_Rail))
	{
		case cBlockRailHandler::Shape::NorthSouth:
		{
			SetYaw(DIR_NORTH_SOUTH);
			SetPosY(floor(GetPosY()) + 0.55);
			SetSpeedY(NO_SPEED);  // Don't move vertically as on ground
			SetSpeedX(NO_SPEED);  // Correct diagonal movement from curved rails

			// Execute both the entity and block collision checks
			auto BlockCollision = TestBlockCollision(a_Rail);
			auto EntityCollision = TestEntityCollision(a_Rail);
			if (EntityCollision || BlockCollision)
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

			return;
		}
		case cBlockRailHandler::Shape::EastWest:
		{
			SetYaw(DIR_EAST_WEST);
			SetPosY(floor(GetPosY()) + 0.55);
			SetSpeedY(NO_SPEED);
			SetSpeedZ(NO_SPEED);

			auto BlockCollision = TestBlockCollision(a_Rail);
			auto EntityCollision = TestEntityCollision(a_Rail);
			if (EntityCollision || BlockCollision)
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

			return;
		}
		case cBlockRailHandler::Shape::AscendingNorth:
		{
			SetYaw(DIR_NORTH_SOUTH);
			SetSpeedX(NO_SPEED);

			auto BlockCollision = TestBlockCollision(a_Rail);
			auto EntityCollision = TestEntityCollision(a_Rail);
			if (EntityCollision || BlockCollision)
			{
				return;
			}

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

			return;
		}
		case cBlockRailHandler::Shape::AscendingSouth:
		{
			SetYaw(DIR_NORTH_SOUTH);
			SetSpeedX(NO_SPEED);

			auto BlockCollision = TestBlockCollision(a_Rail);
			auto EntityCollision = TestEntityCollision(a_Rail);
			if (EntityCollision || BlockCollision)
			{
				return;
			}

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

			return;
		}
		case cBlockRailHandler::Shape::AscendingEast:
		{
			SetYaw(DIR_EAST_WEST);
			SetSpeedZ(NO_SPEED);

			auto BlockCollision = TestBlockCollision(a_Rail);
			auto EntityCollision = TestEntityCollision(a_Rail);
			if (EntityCollision || BlockCollision)
			{
				return;
			}

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

			return;
		}
		case cBlockRailHandler::Shape::AscendingWest:
		{
			SetYaw(DIR_EAST_WEST);
			SetSpeedZ(NO_SPEED);

			auto BlockCollision = TestBlockCollision(a_Rail);
			auto EntityCollision = TestEntityCollision(a_Rail);
			if (EntityCollision || BlockCollision)
			{
				return;
			}

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

			return;
		}
		case cBlockRailHandler::Shape::NorthWest:
		{
			SetYaw(DIR_NORTH_WEST);  // Set correct rotation server side
			SetPosY(floor(GetPosY()) + 0.55);  // Levitate dat cart
			SetSpeedY(NO_SPEED);

			auto BlockCollision = TestBlockCollision(a_Rail);
			auto EntityCollision = TestEntityCollision(a_Rail);
			if (EntityCollision || BlockCollision)
			{
				return;
			}

			// SnapToRail handles turning

			return;
		}
		case cBlockRailHandler::Shape::NorthEast:
		{
			SetYaw(DIR_NORTH_EAST);
			SetPosY(floor(GetPosY()) + 0.55);
			SetSpeedY(NO_SPEED);

			auto BlockCollision = TestBlockCollision(a_Rail);
			auto EntityCollision = TestEntityCollision(a_Rail);
			if (EntityCollision || BlockCollision)
			{
				return;
			}

			return;
		}
		case cBlockRailHandler::Shape::SouthWest:
		{
			SetYaw(DIR_SOUTH_WEST);
			SetPosY(floor(GetPosY()) + 0.55);
			SetSpeedY(NO_SPEED);

			auto BlockCollision = TestBlockCollision(a_Rail);
			auto EntityCollision = TestEntityCollision(a_Rail);
			if (EntityCollision || BlockCollision)
			{
				return;
			}

			return;
		}
		case cBlockRailHandler::Shape::SouthEast:
		{
			SetYaw(DIR_SOUTH_EAST);
			SetPosY(floor(GetPosY()) + 0.55);
			SetSpeedY(NO_SPEED);

			auto BlockCollision = TestBlockCollision(a_Rail);
			auto EntityCollision = TestEntityCollision(a_Rail);
			if (EntityCollision || BlockCollision)
			{
				return;
			}

			return;
		}
		default: return;
	}
	UNREACHABLE("Unsupported rail meta type");
}





void cMinecart::HandlePoweredRailPhysics(BlockState a_Rail)
{
	if (a_Rail.Type() != BlockType::PoweredRail)
	{
		return;
	}

	using namespace Block;

	// If the rail is powered set to speed up else slow down.
	const bool IsRailPowered = PoweredRail::Powered(a_Rail);
	const double Acceleration = IsRailPowered ? 1.0 : -2.0;

	switch (PoweredRail::Shape(a_Rail))
	{
		case PoweredRail::Shape::NorthSouth:
		{
			SetYaw(DIR_NORTH_SOUTH);
			SetPosY(floor(GetPosY()) + 0.55);
			SetSpeedY(0);
			SetSpeedX(0);

			bool BlockCollision = TestBlockCollision(a_Rail), EntityCollision = TestEntityCollision(a_Rail);
			if (EntityCollision || BlockCollision)
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
		case PoweredRail::Shape::EastWest:
		{
			SetYaw(DIR_EAST_WEST);
			SetPosY(floor(GetPosY()) + 0.55);
			SetSpeedY(NO_SPEED);
			SetSpeedZ(NO_SPEED);

			bool BlockCollision = TestBlockCollision(a_Rail), EntityCollision = TestEntityCollision(a_Rail);
			if (EntityCollision || BlockCollision)
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
		case PoweredRail::Shape::AscendingEast:
		{
			SetYaw(DIR_EAST_WEST);
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
		case PoweredRail::Shape::AscendingWest:
		{
			SetYaw(DIR_EAST_WEST);
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
		case PoweredRail::Shape::AscendingNorth:
		{
			SetYaw(DIR_NORTH_SOUTH);
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
		case PoweredRail::Shape::AscendingSouth:
		{
			SetYaw(DIR_NORTH_SOUTH);
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
	}
}





void cMinecart::HandleDetectorRailPhysics(BlockState a_Rail, std::chrono::milliseconds a_Dt)
{
	using namespace Block;
	m_World->SetBlock(m_DetectorRailPosition, DetectorRail::DetectorRail(true, DetectorRail::Shape(a_Rail), DetectorRail::Waterlogged(a_Rail)));

	// No special handling
	HandleRailPhysics(a_Rail, a_Dt);
}





void cMinecart::HandleActivatorRailPhysics(BlockState a_Rail, std::chrono::milliseconds a_Dt)
{
	m_World->SetBlock(m_DetectorRailPosition, Block::ActivatorRail::ActivatorRail(true, Block::ActivatorRail::Shape(a_Rail), Block::ActivatorRail::Waterlogged(a_Rail)));
	// TODO - shake minecart, throw entities out
}





void cMinecart::SnapToRail(BlockState a_Rail)
{
	switch (cBlockRailHandler::GetShapeFromRail(a_Rail))
	{
		case cBlockRailHandler::Shape::AscendingWest:
		case cBlockRailHandler::Shape::AscendingEast:
		case cBlockRailHandler::Shape::EastWest:
		{
			SetSpeedZ(NO_SPEED);
			SetPosZ(floor(GetPosZ()) + 0.5);
			break;
		}
		case cBlockRailHandler::Shape::AscendingNorth:
		case cBlockRailHandler::Shape::AscendingSouth:
		case cBlockRailHandler::Shape::NorthSouth:
		{
			SetSpeedX(NO_SPEED);
			SetPosX(floor(GetPosX()) + 0.5);
			break;
		}
		// Curved rail physics: once minecart has reached more than half of the block in the direction that it is travelling in, jerk it in the direction of curvature
		case cBlockRailHandler::Shape::NorthWest:
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
		case cBlockRailHandler::Shape::NorthEast:
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
		case cBlockRailHandler::Shape::SouthWest:
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
		case cBlockRailHandler::Shape::SouthEast:
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





bool cMinecart::IsSolidBlockAtPosition(Vector3i a_Pos)
{
	auto Block = m_World->GetBlock(a_Pos);
	return !cBlockRailHandler::IsBlockRail(Block) && cBlockInfo::IsSolid(Block);
}





bool cMinecart::IsSolidBlockAtOffset(int a_XOffset, int a_YOffset, int a_ZOffset)
{
	return IsSolidBlockAtPosition({POSX_TOINT + a_XOffset, POSY_TOINT + a_YOffset, POSZ_TOINT + a_ZOffset});
}





bool cMinecart::IsBlockCollisionAtOffset(Vector3i a_Offset)
{
	auto BlockPosition = GetPosition().Floor() + a_Offset;
	if (!IsSolidBlockAtPosition(BlockPosition))
	{
		return false;
	}

	auto bbBlock = cBoundingBox(
		static_cast<Vector3d>(BlockPosition),
		static_cast<Vector3d>(BlockPosition + Vector3i(1, 1, 1))
	);

	return GetBoundingBox().DoesIntersect(bbBlock);
}





bool cMinecart::TestBlockCollision(BlockState a_Rail)
{
	auto SpeedX = GetSpeedX();
	auto SpeedZ = GetSpeedZ();

	// Don't do anything if minecarts aren't moving.
	#ifdef __clang__
		#pragma clang diagnostic push
		#pragma clang diagnostic ignored "-Wfloat-equal"
	#endif

	if ((SpeedX == 0) && (SpeedZ == 0))
	{
		return false;
	}

	#ifdef __clang__
		#pragma clang diagnostic pop
	#endif

	auto StopTheCart = true;
	auto StopOffset = Vector3d();

	switch (cBlockRailHandler::GetShapeFromRail(a_Rail))
	{
		case cBlockRailHandler::Shape::NorthSouth:
		{
			if (SpeedZ > 0)
			{
				StopOffset = Vector3d(0, 0, 0.4);
				StopTheCart = IsBlockCollisionAtOffset({0, 0, 1});
			}
			else  // SpeedZ < 0
			{
				StopTheCart = IsBlockCollisionAtOffset({0, 0, -1});
				StopOffset = Vector3d(0, 0, 0.65);
			}
			break;
		}
		case cBlockRailHandler::Shape::EastWest:
		{
			if (SpeedX > 0)
			{
				StopTheCart = IsBlockCollisionAtOffset({1, 0, 0});
				StopOffset = Vector3d(0.4, 0, 0);
			}
			else  // SpeedX < 0
			{
				StopTheCart = IsBlockCollisionAtOffset({-1, 0, 0});
				StopOffset = Vector3d(0.65, 0, 0);
			}
			break;
		}

		// Ascending rails check for one block on the way up, two on the way down.
		case cBlockRailHandler::Shape::AscendingWest:
		{
			StopOffset = Vector3d(0.5, 0, 0);

			if (SpeedX < 0)
			{
				StopTheCart = IsBlockCollisionAtOffset({-1, 1, 0});
			}
			else  // SpeedX > 0
			{
				StopTheCart = IsBlockCollisionAtOffset({1, 0, 0}) || IsBlockCollisionAtOffset({1, 1, 0});
			}
			break;
		}
		case cBlockRailHandler::Shape::AscendingEast:
		{
			StopOffset = Vector3d(0.5, 0, 0);

			if (SpeedX > 0)
			{
				StopTheCart = IsBlockCollisionAtOffset({1, 1, 0});
			}
			else  // SpeedX < 0
			{
				StopTheCart = IsBlockCollisionAtOffset({-1, 0, 0}) || IsBlockCollisionAtOffset({-1, 1, 0});
			}
			break;
		}
		case cBlockRailHandler::Shape::AscendingNorth:
		{
			StopOffset = Vector3d(0, 0, 0.5);

			if (SpeedZ < 0)
			{
				StopTheCart = IsBlockCollisionAtOffset({0, 1, -1});
			}
			else  // SpeedZ > 0
			{
				StopTheCart = IsBlockCollisionAtOffset({0, 0, 1}) || IsBlockCollisionAtOffset({0, 1, 1});
			}
			break;
		}
		case cBlockRailHandler::Shape::AscendingSouth:
		{
			StopOffset = Vector3d(0, 0, 0.5);

			if (SpeedZ > 0)
			{
				StopTheCart = IsBlockCollisionAtOffset({0, 1, 1});
			}
			else  // SpeedZ < 0
			{
				StopTheCart = IsBlockCollisionAtOffset({0, 0, -1}) || IsBlockCollisionAtOffset({0, 1, -1});
			}
			break;
		}

		// Curved rails allow movement across both the x and z axes. But when the cart is
		// moving towards one of the rail endpoints, it will always have velocity towards
		// the direction of that endpoint in the same axis.
		case cBlockRailHandler::Shape::SouthEast:
		{
			StopOffset = Vector3d(0.5, 0, 0.5);

			if (SpeedZ > 0)
			{
				StopTheCart = IsBlockCollisionAtOffset({0, 0, 1});
				break;
			}
			if (SpeedX > 0)
			{
				StopTheCart = IsBlockCollisionAtOffset({1, 0, 0});
				break;
			}

			break;
		}
		case cBlockRailHandler::Shape::SouthWest:
		{
			StopOffset = Vector3d(0.5, 0, 0.5);

			if (SpeedZ > 0)
			{
				StopTheCart = IsBlockCollisionAtOffset({0, 0, 1});
				break;
			}
			if (SpeedX < 0)
			{
				StopTheCart = IsBlockCollisionAtOffset({-1, 0, 0});
				break;
			}

			break;
		}
		case cBlockRailHandler::Shape::NorthWest:
		{
			StopOffset = Vector3d(0.5, 0, 0.5);

			if (SpeedZ < 0)
			{
				StopTheCart = IsBlockCollisionAtOffset({0, 0, -1});
				break;
			}
			if (SpeedX < 0)
			{
				StopTheCart = IsBlockCollisionAtOffset({-1, 0, 0});
				break;
			}

			break;
		}
		case cBlockRailHandler::Shape::NorthEast:
		{
			StopOffset = Vector3d(0.5, 0, 0.5);

			if (SpeedZ < 0)
			{
				StopTheCart = IsBlockCollisionAtOffset({0, 0, -1});
				break;
			}
			if (SpeedX > 0)
			{
				StopTheCart = IsBlockCollisionAtOffset({1, 0, 0});
				break;
			}
			break;
		}
		default: break;
	}

	if (StopTheCart)
	{
		SetSpeed(0, 0, 0);

		#ifdef __clang__
			#pragma clang diagnostic push
			#pragma clang diagnostic ignored "-Wfloat-equal"
		#endif

		if (StopOffset.x != 0)
		{
			SetPosX(POSX_TOINT + StopOffset.x);
		}
		if (StopOffset.z != 0)
		{
			SetPosZ(POSZ_TOINT + StopOffset.z);
		}

		#ifdef __clang__
			#pragma clang diagnostic pop
		#endif

		return true;
	}

	return false;
}





bool cMinecart::TestEntityCollision(BlockState a_Rail)
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

	// Collision was true, create bounding box for minecart, call attach callback for all entities within that box
	cMinecartAttachCallback MinecartAttachCallback(*this, m_Attachee);
	Vector3d MinecartPosition = GetPosition();
	cBoundingBox bbMinecart(Vector3d(MinecartPosition.x, floor(MinecartPosition.y), MinecartPosition.z), GetWidth() / 2, GetHeight());
	m_World->ForEachEntityInBox(bbMinecart, MinecartAttachCallback);

	switch (cBlockRailHandler::GetShapeFromRail(a_Rail))
	{
		case cBlockRailHandler::Shape::NorthSouth:
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
		case cBlockRailHandler::Shape::EastWest:
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
		case cBlockRailHandler::Shape::NorthWest:
		case cBlockRailHandler::Shape::SouthEast:
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
		case cBlockRailHandler::Shape::NorthEast:
		case cBlockRailHandler::Shape::SouthWest:
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

	return true;
}





void cMinecart::KilledBy(TakeDamageInfo & a_TDI)
{
	Super::KilledBy(a_TDI);

	Destroy();
}





void cMinecart::OnRemoveFromWorld(cWorld & a_World)
{
	if (m_bIsOnDetectorRail)
	{
		m_World->SetBlock(m_DetectorRailPosition, Block::DetectorRail::DetectorRail(false, Block::DetectorRail::Shape(m_World->GetBlock(m_DetectorRailPosition)), Block::DetectorRail::Waterlogged(m_World->GetBlock(m_DetectorRailPosition))));
	}

	Super::OnRemoveFromWorld(a_World);
}





void cMinecart::HandleSpeedFromAttachee(float a_Forward, float a_Sideways)
{
	// limit normal minecart speed max lower than 4
	// once speed is higher than 4, no more add speed.
	if (GetSpeed().Length() > 4)
	{
		return;
	}
	Vector3d LookVector = m_Attachee->GetLookVector();
	Vector3d ToAddSpeed = LookVector * (a_Forward * 0.4) ;
	ToAddSpeed.y = 0;
	AddSpeed(ToAddSpeed);
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





////////////////////////////////////////////////////////////////////////////////
// cRideableMinecart:

cRideableMinecart::cRideableMinecart(Vector3d a_Pos, const cItem & a_Content, int a_ContentHeight):
	Super(mpNone, a_Pos),
	m_Content(a_Content),
	m_ContentHeight(a_ContentHeight)
{
}





void cRideableMinecart::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	a_Drops.emplace_back(Item::Minecart);
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

	// Attach the player to this minecart:
	a_Player.AttachTo(*this);
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





void cMinecartWithChest::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	m_Contents.CopyToItems(a_Drops);
	a_Drops.emplace_back(Item::ChestMinecart);
}





void cMinecartWithChest::OnRemoveFromWorld(cWorld & a_World)
{
	const auto Window = GetWindow();
	if (Window != nullptr)
	{
		Window->OwnerDestroyed();
	}

	Super::OnRemoveFromWorld(a_World);
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





////////////////////////////////////////////////////////////////////////////////
// cMinecartWithFurnace:

cMinecartWithFurnace::cMinecartWithFurnace(Vector3d a_Pos):
	Super(mpFurnace, a_Pos),
	m_FueledTimeLeft(-1),
	m_IsFueled(false)
{
}





void cMinecartWithFurnace::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	a_Drops.emplace_back(Item::FurnaceMinecart);
}





void cMinecartWithFurnace::OnRightClicked(cPlayer & a_Player)
{
	if (a_Player.GetEquippedItem().m_ItemType == Item::Coal)
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





void cMinecartWithTNT::HandleActivatorRailPhysics(BlockState a_Rail, std::chrono::milliseconds a_Dt)
{
	Super::HandleActivatorRailPhysics(a_Rail, a_Dt);

	if (Block::ActivatorRail::Powered(a_Rail) && !m_isTNTFused)
	{
		m_isTNTFused = true;
		m_TNTFuseTicksLeft = 80;
		m_World->BroadcastSoundEffect("entity.tnt.primed", GetPosition(), 1.0f, 1.0f);
		m_World->BroadcastEntityAnimation(*this, EntityAnimation::MinecartTNTIgnites);
	}
}





void cMinecartWithTNT::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	a_Drops.emplace_back(Item::TNTMinecart);
}





void cMinecartWithTNT::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	Super::Tick(a_Dt, a_Chunk);
	if (!IsTicking())
	{
		return;
	}

	if (m_isTNTFused)
	{
		if (m_TNTFuseTicksLeft > 0)
		{
			--m_TNTFuseTicksLeft;
		}
		else
		{
			Destroy();
			m_World->DoExplosionAt(4.0, GetPosX(), GetPosY() + GetHeight() / 2, GetPosZ(), true, esTNTMinecart, this);
		}
	}
}





////////////////////////////////////////////////////////////////////////////////
// cMinecartWithHopper:

cMinecartWithHopper::cMinecartWithHopper(Vector3d a_Pos):
	Super(mpHopper, a_Pos)
{
}

// TODO: Make it suck up blocks and travel further than any other cart and physics and put and take blocks
// AND AVARYTHING!!





void cMinecartWithHopper::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	a_Drops.emplace_back(Item::HopperMinecart);
}
