
// ProjectileEntity.cpp

// Implements the cProjectileEntity class representing the common base class for projectiles, as well as individual projectile types

#include "Globals.h"
#include "ProjectileEntity.h"
#include "../ClientHandle.h"
#include "Player.h"
#include "../LineBlockTracer.h"
#include "../BoundingBox.h"
#include "../ChunkMap.h"
#include "../Chunk.h"

#include <math.h>





/// Converts an angle in radians into a byte representation used by the network protocol
#define ANGLE_TO_PROTO(X) (Byte)(X * 255 / 360)





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cProjectileTracerCallback:

class cProjectileTracerCallback :
	public cBlockTracer::cCallbacks
{
public:
	cProjectileTracerCallback(cProjectileEntity * a_Projectile) :
		m_Projectile(a_Projectile),
		m_SlowdownCoeff(0.99)  // Default slowdown when not in water
	{
	}
	
	double GetSlowdownCoeff(void) const { return m_SlowdownCoeff; }
	
protected:
	cProjectileEntity * m_Projectile;
	double m_SlowdownCoeff;
	
	// cCallbacks overrides:
	virtual bool OnNextBlock(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, char a_EntryFace) override
	{
		/*
		// DEBUG:
		LOGD("Hit block %d:%d at {%d, %d, %d} face %d, %s (%s)",
			a_BlockType, a_BlockMeta,
			a_BlockX, a_BlockY, a_BlockZ, a_EntryFace,
			g_BlockIsSolid[a_BlockType] ? "solid" : "non-solid",
			ItemToString(cItem(a_BlockType, 1, a_BlockMeta)).c_str()
		);
		*/
		
		if (g_BlockIsSolid[a_BlockType])
		{
			// The projectile hit a solid block
			// Calculate the exact hit coords:
			cBoundingBox bb(a_BlockX, a_BlockX + 1, a_BlockY, a_BlockY + 1, a_BlockZ, a_BlockZ + 1);
			Vector3d Line1 = m_Projectile->GetPosition();
			Vector3d Line2 = Line1 + m_Projectile->GetSpeed();
			double LineCoeff = 0;
			eBlockFace Face;
			if (bb.CalcLineIntersection(Line1, Line2, LineCoeff, Face))
			{
				Vector3d Intersection = Line1 + m_Projectile->GetSpeed() * LineCoeff;
				m_Projectile->OnHitSolidBlock(Intersection, Face);
				return true;
			}
			else
			{
				LOGD("WEIRD! block tracer reports a hit, but BBox tracer doesn't. Ignoring the hit.");
			}
		}
		
		// Convey some special effects from special blocks:
		switch (a_BlockType)
		{
			case E_BLOCK_LAVA:
			case E_BLOCK_STATIONARY_LAVA:
			{
				m_Projectile->StartBurning(30);
				m_SlowdownCoeff = std::min(m_SlowdownCoeff, 0.9);  // Slow down to 0.9* the speed each tick when moving through lava
				break;
			}
			case E_BLOCK_WATER:
			case E_BLOCK_STATIONARY_WATER:
			{
				m_Projectile->StopBurning();
				m_SlowdownCoeff = std::min(m_SlowdownCoeff, 0.8);  // Slow down to 0.8* the speed each tick when moving through water
				break;
			}
		}  // switch (a_BlockType)
		
		// Continue tracing
		return false;
	}
} ;





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cProjectileEntityCollisionCallback:

class cProjectileEntityCollisionCallback :
	public cEntityCallback
{
public:
	cProjectileEntityCollisionCallback(cProjectileEntity * a_Projectile, const Vector3d & a_Pos, const Vector3d & a_NextPos) :
		m_Projectile(a_Projectile),
		m_Pos(a_Pos),
		m_NextPos(a_NextPos),
		m_MinCoeff(1),
		m_HitEntity(NULL)
	{
	}
	
	
	virtual bool Item(cEntity * a_Entity) override
	{
		if (
			(a_Entity == m_Projectile) ||          // Do not check collisions with self
			(a_Entity == m_Projectile->GetCreator())  // Do not check whoever shot the projectile
		)
		{
			// TODO: Don't check creator only for the first 5 ticks
			// so that arrows stuck in ground and dug up can hurt the player
			return false;
		}
		
		cBoundingBox EntBox(a_Entity->GetPosition(), a_Entity->GetWidth() / 2, a_Entity->GetHeight());
		
		// Instead of colliding the bounding box with another bounding box in motion, we collide an enlarged bounding box with a hairline.
		// The results should be good enough for our purposes
		double LineCoeff;
		eBlockFace Face;
		EntBox.Expand(m_Projectile->GetWidth() / 2, m_Projectile->GetHeight() / 2, m_Projectile->GetWidth() / 2);
		if (!EntBox.CalcLineIntersection(m_Pos, m_NextPos, LineCoeff, Face))
		{
			// No intersection whatsoever
			return false;
		}

		// TODO: Some entities don't interact with the projectiles (pickups, falling blocks)
		// TODO: Allow plugins to interfere about which entities can be hit
		
		if (LineCoeff < m_MinCoeff)
		{
			// The entity is closer than anything we've stored so far, replace it as the potential victim
			m_MinCoeff = LineCoeff;
			m_HitEntity = a_Entity;
		}
		
		// Don't break the enumeration, we want all the entities
		return false;
	}
	
	/// Returns the nearest entity that was hit, after the enumeration has been completed
	cEntity * GetHitEntity(void) const { return m_HitEntity; }
	
	/// Returns the line coeff where the hit was encountered, after the enumeration has been completed
	double GetMinCoeff(void) const { return m_MinCoeff; }
	
	/// Returns true if the callback has encountered a true hit
	bool HasHit(void) const { return (m_MinCoeff < 1); }
	
protected:
	cProjectileEntity * m_Projectile;
	const Vector3d & m_Pos;
	const Vector3d & m_NextPos;
	double m_MinCoeff;  // The coefficient of the nearest hit on the Pos line

	// Although it's bad(tm) to store entity ptrs from a callback, we can afford it here, because the entire callback
	// is processed inside the tick thread, so the entities won't be removed in between the calls and the final processing
	cEntity * m_HitEntity;  // The nearest hit entity
} ;





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cProjectileEntity:

cProjectileEntity::cProjectileEntity(eKind a_Kind, cEntity * a_Creator, double a_X, double a_Y, double a_Z, double a_Width, double a_Height) :
	super(etProjectile, a_X, a_Y, a_Z, a_Width, a_Height),
	m_ProjectileKind(a_Kind),
	m_Creator(a_Creator),
	m_IsInGround(false)
{
}





cProjectileEntity::cProjectileEntity(eKind a_Kind, cEntity * a_Creator, const Vector3d & a_Pos, const Vector3d & a_Speed, double a_Width, double a_Height) :
	super(etProjectile, a_Pos.x, a_Pos.y, a_Pos.z, a_Width, a_Height),
	m_ProjectileKind(a_Kind),
	m_Creator(a_Creator),
	m_IsInGround(false)
{
	SetSpeed(a_Speed);
	SetYawFromSpeed();
	SetPitchFromSpeed();
}





cProjectileEntity * cProjectileEntity::Create(eKind a_Kind, cEntity * a_Creator, double a_X, double a_Y, double a_Z, const Vector3d * a_Speed)
{
	Vector3d Speed;
	if (a_Speed != NULL)
	{
		Speed = *a_Speed;
	}

	switch (a_Kind)
	{
		case pkArrow:         return new cArrowEntity           (a_Creator, a_X, a_Y, a_Z, Speed);
		case pkEgg:           return new cThrownEggEntity       (a_Creator, a_X, a_Y, a_Z, Speed);
		case pkEnderPearl:    return new cThrownEnderPearlEntity(a_Creator, a_X, a_Y, a_Z, Speed);
		case pkSnowball:      return new cThrownSnowballEntity  (a_Creator, a_X, a_Y, a_Z, Speed);
		case pkGhastFireball: return new cGhastFireballEntity   (a_Creator, a_X, a_Y, a_Z, Speed);
		case pkFireCharge:    return new cFireChargeEntity      (a_Creator, a_X, a_Y, a_Z, Speed);
		case pkExpBottle:     return new cExpBottleEntity       (a_Creator, a_X, a_Y, a_Z, Speed);
		case pkFirework:      return new cFireworkEntity        (a_Creator, a_X, a_Y, a_Z       );
		// TODO: the rest
	}
	
	LOGWARNING("%s: Unknown projectile kind: %d", __FUNCTION__, a_Kind);
	return NULL;
}





void cProjectileEntity::OnHitSolidBlock(const Vector3d & a_HitPos, eBlockFace a_HitFace)
{
	// Set the position based on what face was hit:
	SetPosition(a_HitPos);
	SetSpeed(0, 0, 0);

	// DEBUG:
	LOGD("Projectile %d: pos {%.02f, %.02f, %.02f}, hit solid block at face %d",
		m_UniqueID,
		a_HitPos.x, a_HitPos.y, a_HitPos.z,
		a_HitFace
	);

	m_IsInGround = true;
}





AString cProjectileEntity::GetMCAClassName(void) const
{
	switch (m_ProjectileKind)
	{
		case pkArrow:         return "Arrow";
		case pkSnowball:      return "Snowball";
		case pkEgg:           return "Egg";
		case pkGhastFireball: return "Fireball";
		case pkFireCharge:    return "SmallFireball";
		case pkEnderPearl:    return "ThrownEnderPearl";
		case pkExpBottle:     return "ThrownExpBottle";
		case pkSplashPotion:  return "ThrownPotion";
		case pkWitherSkull:   return "WitherSkull";
		case pkFishingFloat:  return "";  // Unknown, perhaps MC doesn't save this?
	}
	ASSERT(!"Unhandled projectile entity kind!");
	return "";
}





void cProjectileEntity::Tick(float a_Dt, cChunk & a_Chunk)
{
	super::Tick(a_Dt, a_Chunk);

	if (GetProjectileKind() != pkArrow) // See cArrow::Tick
	{
		BroadcastMovementUpdate();
	}
}





void cProjectileEntity::HandlePhysics(float a_Dt, cChunk & a_Chunk)
{
	if (m_IsInGround)
	{
		// Already-grounded projectiles don't move at all
		return;
	}
	
	Vector3d PerTickSpeed = GetSpeed() / 20;
	Vector3d Pos = GetPosition();
	
	// Trace the tick's worth of movement as a line:
	Vector3d NextPos = Pos + PerTickSpeed;
	cProjectileTracerCallback TracerCallback(this);
	if (!cLineBlockTracer::Trace(*m_World, TracerCallback, Pos, NextPos))
	{
		// Something has been hit, abort all other processing
		return;
	}
	// The tracer also checks the blocks for slowdown blocks - water and lava - and stores it for later in its SlowdownCoeff
	
	// Test for entity collisions:
	cProjectileEntityCollisionCallback EntityCollisionCallback(this, Pos, NextPos);
	a_Chunk.ForEachEntity(EntityCollisionCallback);
	if (EntityCollisionCallback.HasHit())
	{
		// An entity was hit:
		Vector3d HitPos = Pos + (NextPos - Pos) * EntityCollisionCallback.GetMinCoeff();

		// DEBUG:
		LOGD("Projectile %d has hit an entity %d (%s) at {%.02f, %.02f, %.02f} (coeff %.03f)",
			m_UniqueID,
			EntityCollisionCallback.GetHitEntity()->GetUniqueID(),
			EntityCollisionCallback.GetHitEntity()->GetClass(),
			HitPos.x, HitPos.y, HitPos.z,
			EntityCollisionCallback.GetMinCoeff()
		);
		
		OnHitEntity(*(EntityCollisionCallback.GetHitEntity()), HitPos);
	}
	// TODO: Test the entities in the neighboring chunks, too

	// Update the position:
	SetPosition(NextPos);
	
	// Add slowdown and gravity effect to the speed:
	Vector3d NewSpeed(GetSpeed());
	NewSpeed.y += m_Gravity / 20;
	NewSpeed *= TracerCallback.GetSlowdownCoeff();
	SetSpeed(NewSpeed);
	SetYawFromSpeed();
	SetPitchFromSpeed();

	// DEBUG:
	LOGD("Projectile %d: pos {%.02f, %.02f, %.02f}, speed {%.02f, %.02f, %.02f}, rot {%.02f, %.02f}",
		m_UniqueID,
		GetPosX(), GetPosY(), GetPosZ(),
		GetSpeedX(), GetSpeedY(), GetSpeedZ(),
		GetYaw(), GetPitch()
	);
}





void cProjectileEntity::SpawnOn(cClientHandle & a_Client)
{
	// Default spawning - use the projectile kind to spawn an object:
	a_Client.SendSpawnObject(*this, m_ProjectileKind, 12, ANGLE_TO_PROTO(GetYaw()), ANGLE_TO_PROTO(GetPitch()));
	a_Client.SendEntityMetadata(*this);
}





void cProjectileEntity::CollectedBy(cPlayer * a_Dest)
{
	// Overriden in arrow
	UNUSED(a_Dest);
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cArrowEntity:

cArrowEntity::cArrowEntity(cEntity * a_Creator, double a_X, double a_Y, double a_Z, const Vector3d & a_Speed) :
	super(pkArrow, a_Creator, a_X, a_Y, a_Z, 0.5, 0.5),
	m_PickupState(psNoPickup),
	m_DamageCoeff(2),
	m_IsCritical(false),
	m_Timer(0),
	m_HitGroundTimer(0),
	m_bIsCollected(false),
	m_HitBlockPos(Vector3i(0, 0, 0))
{
	SetSpeed(a_Speed);
	SetMass(0.1);
	SetYawFromSpeed();
	SetPitchFromSpeed();
	LOGD("Created arrow %d with speed {%.02f, %.02f, %.02f} and rot {%.02f, %.02f}",
		m_UniqueID, GetSpeedX(), GetSpeedY(), GetSpeedZ(),
		GetYaw(), GetPitch()
	);
}





cArrowEntity::cArrowEntity(cPlayer & a_Player, double a_Force) :
	super(pkArrow, &a_Player, a_Player.GetThrowStartPos(), a_Player.GetThrowSpeed(a_Force * 1.5 * 20), 0.5, 0.5),
	m_PickupState(psInSurvivalOrCreative),
	m_DamageCoeff(2),
	m_IsCritical((a_Force >= 1)),
	m_Timer(0),
	m_HitGroundTimer(0),
	m_bIsCollected(false),
	m_HitBlockPos(0, 0, 0)
{
}





bool cArrowEntity::CanPickup(const cPlayer & a_Player) const
{
	switch (m_PickupState)
	{
		case psNoPickup:             return false;
		case psInSurvivalOrCreative: return (a_Player.IsGameModeSurvival() || a_Player.IsGameModeCreative());
		case psInCreative:           return a_Player.IsGameModeCreative();
	}
	ASSERT(!"Unhandled pickup state");
	return false;
}





void cArrowEntity::OnHitSolidBlock(const Vector3d & a_HitPos, eBlockFace a_HitFace)
{
	if (a_HitFace == BLOCK_FACE_NONE) { return; }

	super::OnHitSolidBlock(a_HitPos, a_HitFace);
	int a_X = (int)a_HitPos.x, a_Y = (int)a_HitPos.y, a_Z = (int)a_HitPos.z;

	switch (a_HitFace)
	{
		case BLOCK_FACE_XM: // Strangely, bounding boxes / block tracers return the actual block for these two directions, so AddFace not needed
		case BLOCK_FACE_YM:
		{
			break;
		}
		default: AddFaceDirection(a_X, a_Y, a_Z, a_HitFace, true);
	}

	m_HitBlockPos = Vector3i(a_X, a_Y, a_Z);

	// Broadcast arrow hit sound
	m_World->BroadcastSoundEffect("random.bowhit", (int)GetPosX() * 8, (int)GetPosY() * 8, (int)GetPosZ() * 8, 0.5f, (float)(0.75 + ((float)((GetUniqueID() * 23) % 32)) / 64));
}





void cArrowEntity::OnHitEntity(cEntity & a_EntityHit, const Vector3d & a_HitPos)
{
	if (!a_EntityHit.IsMob() && !a_EntityHit.IsMinecart() && !a_EntityHit.IsPlayer() && !a_EntityHit.IsBoat())
	{
		// Not an entity that interacts with an arrow
		return;
	}
	
	int Damage = (int)(GetSpeed().Length() / 20 * m_DamageCoeff + 0.5);
	if (m_IsCritical)
	{
		Damage += m_World->GetTickRandomNumber(Damage / 2 + 2);
	}
	a_EntityHit.TakeDamage(dtRangedAttack, this, Damage, 1);
	
	// Broadcast successful hit sound
	m_World->BroadcastSoundEffect("random.successful_hit", (int)GetPosX() * 8, (int)GetPosY() * 8, (int)GetPosZ() * 8, 0.5, (float)(0.75 + ((float)((GetUniqueID() * 23) % 32)) / 64));

	Destroy();
}





void cArrowEntity::CollectedBy(cPlayer * a_Dest)
{
	if ((m_IsInGround) && (!m_bIsCollected) && (CanPickup(*a_Dest)))
	{
		int NumAdded = a_Dest->GetInventory().AddItem(E_ITEM_ARROW);
		if (NumAdded > 0) // Only play effects if there was space in inventory
		{
			m_World->BroadcastCollectPickup((const cPickup &)*this, *a_Dest);
			// Also send the "pop" sound effect with a somewhat random pitch (fast-random using EntityID ;)
			m_World->BroadcastSoundEffect("random.pop", (int)GetPosX() * 8, (int)GetPosY() * 8, (int)GetPosZ() * 8, 0.5, (float)(0.75 + ((float)((GetUniqueID() * 23) % 32)) / 64));
			m_bIsCollected = true;
		}
	}
}





void cArrowEntity::Tick(float a_Dt, cChunk & a_Chunk)
{
	super::Tick(a_Dt, a_Chunk);
	m_Timer += a_Dt;

	if (m_bIsCollected)
	{
		if (m_Timer > 500.f)  // 0.5 seconds
		{
			Destroy();
			return;
		}
	}
	else if (m_Timer > 1000 * 60 * 5)  // 5 minutes
	{
		Destroy();
		return;
	}

	if (m_IsInGround)
	{
		// When an arrow hits, the client doesn't think its in the ground and keeps on moving, IF BroadcastMovementUpdate() and TeleportEntity was called during flight, AT ALL
		// Fix is to simply not sync with the client and send a teleport to confirm pos after arrow has stabilised (around 1 sec after landing)
		// We can afford to do this because xoft's algorithm for trajectory is near perfect, so things are pretty close anyway without sync
		// Besides, this seems to be what the vanilla server does, note how arrows teleport half a second after they hit to the server position

		if (m_HitGroundTimer != -1) // Sent a teleport already, don't do again
		{
			if (m_HitGroundTimer > 1000.f) // Send after a second, could be less, but just in case
			{
				m_World->BroadcastTeleportEntity(*this);
				m_HitGroundTimer = -1;
			}
			else
			{
				m_HitGroundTimer += a_Dt;
			}
		}

		int RelPosX = m_HitBlockPos.x - a_Chunk.GetPosX() * cChunkDef::Width;
		int RelPosZ = m_HitBlockPos.z - a_Chunk.GetPosZ() * cChunkDef::Width;
		cChunk * Chunk = a_Chunk.GetRelNeighborChunkAdjustCoords(RelPosX, RelPosZ);

		if (Chunk == NULL)
		{
			// Inside an unloaded chunk, abort
			return;
		}

		if (Chunk->GetBlock(RelPosX, m_HitBlockPos.y, RelPosZ) == E_BLOCK_AIR) // Block attached to was destroyed?
		{
			m_IsInGround = false; // Yes, begin simulating physics again
		}
	}
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cThrownEggEntity:

cThrownEggEntity::cThrownEggEntity(cEntity * a_Creator, double a_X, double a_Y, double a_Z, const Vector3d & a_Speed) :
	super(pkEgg, a_Creator, a_X, a_Y, a_Z, 0.25, 0.25)
{
	SetSpeed(a_Speed);
}





void cThrownEggEntity::OnHitSolidBlock(const Vector3d & a_HitPos, eBlockFace a_HitFace)
{
	if (m_World->GetTickRandomNumber(7) == 1)
	{
		m_World->SpawnMob(a_HitPos.x, a_HitPos.y, a_HitPos.z, cMonster::mtChicken);
	}
	else if (m_World->GetTickRandomNumber(32) == 1)
	{
		m_World->SpawnMob(a_HitPos.x, a_HitPos.y, a_HitPos.z, cMonster::mtChicken);
		m_World->SpawnMob(a_HitPos.x, a_HitPos.y, a_HitPos.z, cMonster::mtChicken);
		m_World->SpawnMob(a_HitPos.x, a_HitPos.y, a_HitPos.z, cMonster::mtChicken);
		m_World->SpawnMob(a_HitPos.x, a_HitPos.y, a_HitPos.z, cMonster::mtChicken);
	}
	Destroy();
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cThrownEnderPearlEntity :

cThrownEnderPearlEntity::cThrownEnderPearlEntity(cEntity * a_Creator, double a_X, double a_Y, double a_Z, const Vector3d & a_Speed) :
	super(pkEnderPearl, a_Creator, a_X, a_Y, a_Z, 0.25, 0.25)
{
	SetSpeed(a_Speed);
}





void cThrownEnderPearlEntity::OnHitSolidBlock(const Vector3d & a_HitPos, eBlockFace a_HitFace)
{
	// Teleport the creator here, make them take 5 damage:
	if (m_Creator != NULL)
	{
		// TODO: The coords might need some tweaking based on the block face
		m_Creator->TeleportToCoords(a_HitPos.x + 0.5, a_HitPos.y + 1.7, a_HitPos.z + 0.5);
		m_Creator->TakeDamage(dtEnderPearl, this, 5, 0);
	}
	
	Destroy();
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cThrownSnowballEntity :

cThrownSnowballEntity::cThrownSnowballEntity(cEntity * a_Creator, double a_X, double a_Y, double a_Z, const Vector3d & a_Speed) :
	super(pkSnowball, a_Creator, a_X, a_Y, a_Z, 0.25, 0.25)
{
	SetSpeed(a_Speed);
}





void cThrownSnowballEntity::OnHitSolidBlock(const Vector3d & a_HitPos, eBlockFace a_HitFace)
{
	// TODO: Apply damage to certain mobs (blaze etc.) and anger all mobs
	
	Destroy();
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cBottleOEnchantingEntity :

cExpBottleEntity::cExpBottleEntity(cEntity * a_Creator, double a_X, double a_Y, double a_Z, const Vector3d & a_Speed) :
super(pkExpBottle, a_Creator, a_X, a_Y, a_Z, 0.25, 0.25)
{
	SetSpeed(a_Speed);
}





void cExpBottleEntity::OnHitSolidBlock(const Vector3d & a_HitPos, eBlockFace a_HitFace)
{
	// Spawn an experience orb with a reward between 3 and 11.
	m_World->BroadcastSoundParticleEffect(2002, (int) round(GetPosX()), (int) round(GetPosY()), (int) round(GetPosZ()), 0);
	m_World->SpawnExperienceOrb(GetPosX(), GetPosY(), GetPosZ(), 3 + m_World->GetTickRandomNumber(8));

	Destroy();
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cFireworkEntity :

cFireworkEntity::cFireworkEntity(cEntity * a_Creator, double a_X, double a_Y, double a_Z) :
super(pkFirework, a_Creator, a_X, a_Y, a_Z, 0.25, 0.25)
{
}





void cFireworkEntity::OnHitSolidBlock(const Vector3d & a_HitPos, eBlockFace a_HitFace)
{
	if ((a_HitFace != BLOCK_FACE_BOTTOM) && (a_HitFace != BLOCK_FACE_NONE))
	{
		return;
	}

	SetSpeed(0, 0, 0);
	SetPosition(GetPosX(), GetPosY() - 0.5, GetPosZ());

	m_IsInGround = true;

	BroadcastMovementUpdate();
}





void cFireworkEntity::HandlePhysics(float a_Dt, cChunk & a_Chunk)
{
	if (m_IsInGround)
	{
		if (a_Chunk.GetBlock((int)GetPosX(), (int)GetPosY() + 1, (int)GetPosZ()) == E_BLOCK_AIR)
		{
			m_IsInGround = false;
		}
		else
		{
			return;
		}
	}

	Vector3d PerTickSpeed = GetSpeed() / 20;
	Vector3d Pos = GetPosition();

	// Trace the tick's worth of movement as a line:
	Vector3d NextPos = Pos + PerTickSpeed;
	cProjectileTracerCallback TracerCallback(this);
	if (!cLineBlockTracer::Trace(*m_World, TracerCallback, Pos, NextPos))
	{
		// Something has been hit, abort all other processing
		return;
	}
	// The tracer also checks the blocks for slowdown blocks - water and lava - and stores it for later in its SlowdownCoeff

	// Update the position:
	SetPosition(NextPos);

	// Add slowdown and gravity effect to the speed:
	Vector3d NewSpeed(GetSpeed());
	NewSpeed.y += 2;
	NewSpeed *= TracerCallback.GetSlowdownCoeff();
	SetSpeed(NewSpeed);
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cGhastFireballEntity :

cGhastFireballEntity::cGhastFireballEntity(cEntity * a_Creator, double a_X, double a_Y, double a_Z, const Vector3d & a_Speed) :
	super(pkGhastFireball, a_Creator, a_X, a_Y, a_Z, 1, 1)
{
	SetSpeed(a_Speed);
	SetGravity(0);
}





void cGhastFireballEntity::Explode(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	m_World->DoExplosionAt(1, a_BlockX, a_BlockY, a_BlockZ, true, esGhastFireball, this);
}





void cGhastFireballEntity::OnHitSolidBlock(const Vector3d & a_HitPos, eBlockFace a_HitFace)
{
	Destroy();
	Explode((int)floor(a_HitPos.x), (int)floor(a_HitPos.y), (int)floor(a_HitPos.z));
}





void cGhastFireballEntity::OnHitEntity(cEntity & a_EntityHit, const Vector3d & a_HitPos)
{
	Destroy();
	Explode((int)floor(a_HitPos.x), (int)floor(a_HitPos.y), (int)floor(a_HitPos.z));
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cFireChargeEntity :

cFireChargeEntity::cFireChargeEntity(cEntity * a_Creator, double a_X, double a_Y, double a_Z, const Vector3d & a_Speed) :
	super(pkFireCharge, a_Creator, a_X, a_Y, a_Z, 0.3125, 0.3125)
{
	SetSpeed(a_Speed);
	SetGravity(0);
}





void cFireChargeEntity::Explode(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	if (m_World->GetBlock(a_BlockX, a_BlockY, a_BlockZ) == E_BLOCK_AIR)
	{
		m_World->SetBlock(a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_FIRE, 1);
	}
}





void cFireChargeEntity::OnHitSolidBlock(const Vector3d & a_HitPos, eBlockFace a_HitFace)
{
	Destroy();
	Explode((int)floor(a_HitPos.x), (int)floor(a_HitPos.y), (int)floor(a_HitPos.z));
}





void cFireChargeEntity::OnHitEntity(cEntity & a_EntityHit, const Vector3d & a_HitPos)
{
	Destroy();
	Explode((int)floor(a_HitPos.x), (int)floor(a_HitPos.y), (int)floor(a_HitPos.z));
	
	// TODO: Some entities are immune to hits
	a_EntityHit.StartBurning(5 * 20);  // 5 seconds of burning
}




