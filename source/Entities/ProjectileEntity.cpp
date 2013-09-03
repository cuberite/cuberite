
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
		if (g_BlockIsSolid[a_BlockType])
		{
			// The projectile hit a solid block
			m_Projectile->OnHitSolidBlock(a_BlockX, a_BlockY, a_BlockZ, a_EntryFace);
			return true;
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
		char Face;
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
		case pkArrow:      return new cArrowEntity           (a_Creator, a_X, a_Y, a_Z, Speed);
		case pkEgg:        return new cThrownEggEntity       (a_Creator, a_X, a_Y, a_Z, Speed);
		case pkEnderPearl: return new cThrownEnderPearlEntity(a_Creator, a_X, a_Y, a_Z, Speed);
		case pkSnowball:   return new cThrownSnowballEntity  (a_Creator, a_X, a_Y, a_Z, Speed);
		// TODO: the rest
	}
	
	LOGWARNING("%s: Unknown projectile kind: %d", __FUNCTION__, a_Kind);
	return NULL;
}





void cProjectileEntity::OnHitSolidBlock(int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace)
{
	// TODO: Set proper position based on what face was hit
	switch (a_BlockFace)
	{
		case BLOCK_FACE_TOP:    SetPosition(0.5 + a_BlockX, 1.0 + a_BlockY, 0.5 + a_BlockZ); break;
		case BLOCK_FACE_BOTTOM: SetPosition(0.5 + a_BlockX,       a_BlockY, 0.5 + a_BlockZ); break;
		case BLOCK_FACE_EAST:   SetPosition(      a_BlockX, 0.5 + a_BlockY, 0.5 + a_BlockZ); break;
		case BLOCK_FACE_WEST:   SetPosition(1.0 + a_BlockX, 0.5 + a_BlockY, 0.5 + a_BlockZ); break;
		case BLOCK_FACE_NORTH:  SetPosition(0.5 + a_BlockX, 0.5 + a_BlockY, 1.0 + a_BlockZ); break;
		case BLOCK_FACE_SOUTH:  SetPosition(0.5 + a_BlockX, 0.5 + a_BlockY,       a_BlockZ); break;
		case BLOCK_FACE_NONE:   SetPosition(0.5 + a_BlockX, 0.5 + a_BlockY, 0.5 + a_BlockZ); break;
	}
	SetSpeed(0, 0, 0);

	// DEBUG:
	LOGD("Projectile %d: pos {%.02f, %.02f, %.02f}, hit solid block at face %d",
		m_UniqueID,
		GetPosX(), GetPosY(), GetPosZ(),
		a_BlockFace
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
	BroadcastMovementUpdate();
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
	// The tracer also checks the blocks for slowdown blocks - water and lava - and stores it for later
	
	// Test for entity collisions:
	cProjectileEntityCollisionCallback EntityCollisionCallback(this, Pos, NextPos);
	a_Chunk.ForEachEntity(EntityCollisionCallback);
	if (EntityCollisionCallback.HasHit())
	{
		// An entity was hit:
		// DEBUG:
		Vector3d HitPos = Pos + (NextPos - Pos) * EntityCollisionCallback.GetMinCoeff();
		LOGD("Projectile %d has hit an entity %d (%s) at {%.02f, %.02f, %.02f} (coeff %.03f)",
			m_UniqueID,
			EntityCollisionCallback.GetHitEntity()->GetUniqueID(),
			EntityCollisionCallback.GetHitEntity()->GetClass(),
			HitPos.x, HitPos.y, HitPos.z,
			EntityCollisionCallback.GetMinCoeff()
		);
		OnHitEntity(*(EntityCollisionCallback.GetHitEntity()));
	}
	// TODO: Test the entities in the neighboring chunks, too

	// Update the position:
	SetPosition(NextPos);
	
	// Add slowdown and gravity effect to the speed:
	Vector3d NewSpeed(GetSpeed());
	NewSpeed.y += m_Gravity / 20;
	NewSpeed *= TracerCallback.GetSlowdownCoeff();
	SetSpeed(NewSpeed);

	// DEBUG:
	LOGD("Projectile %d: pos {%.02f, %.02f, %.02f}, speed {%.02f, %.02f, %.02f}",
		m_UniqueID,
		GetPosX(), GetPosY(), GetPosZ(),
		GetSpeedX(), GetSpeedY(), GetSpeedZ()
	);
}





void cProjectileEntity::SpawnOn(cClientHandle & a_Client)
{
	// Default spawning - use the projectile kind to spawn an object:
	a_Client.SendSpawnObject(*this, m_ProjectileKind, 0, 0, 0);
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cArrowEntity:

cArrowEntity::cArrowEntity(cEntity * a_Creator, double a_X, double a_Y, double a_Z, const Vector3d & a_Speed) :
	super(pkArrow, a_Creator, a_X, a_Y, a_Z, 0.5, 0.5),
	m_PickupState(psNoPickup),
	m_DamageCoeff(2),
	m_IsCritical(false)
{
	SetSpeed(a_Speed);
	SetMass(0.1);
	LOGD("Created arrow %d with speed {%.02f, %.02f, %.02f}",
		m_UniqueID, GetSpeedX(), GetSpeedY(), GetSpeedZ()
	);
}





cArrowEntity::cArrowEntity(cPlayer & a_Player, double a_Force) :
	super(pkArrow, &a_Player, a_Player.GetThrowStartPos(), a_Player.GetThrowSpeed(a_Force * 1.5 * 20), 0.5, 0.5),
	m_PickupState(psInSurvivalOrCreative),
	m_DamageCoeff(2),
	m_IsCritical((a_Force >= 1))
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





void cArrowEntity::SpawnOn(cClientHandle & a_Client)
{
	a_Client.SendSpawnObject(*this, pkArrow, 0, 0, 0);
}




void cArrowEntity::OnHitEntity(cEntity & a_EntityHit)
{
	if (!a_EntityHit.IsMob() && !a_EntityHit.IsMinecart() && !a_EntityHit.IsPlayer())
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
	
	Destroy();
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cThrownEggEntity:

cThrownEggEntity::cThrownEggEntity(cEntity * a_Creator, double a_X, double a_Y, double a_Z, const Vector3d & a_Speed) :
	super(pkEgg, a_Creator, a_X, a_Y, a_Z, 0.25, 0.25)
{
	SetSpeed(a_Speed);
}





void cThrownEggEntity::OnHitSolidBlock(int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace)
{
	// TODO: Random-spawn a chicken or four
	
	Destroy();
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cThrownEnderPearlEntity :

cThrownEnderPearlEntity::cThrownEnderPearlEntity(cEntity * a_Creator, double a_X, double a_Y, double a_Z, const Vector3d & a_Speed) :
	super(pkEnderPearl, a_Creator, a_X, a_Y, a_Z, 0.25, 0.25)
{
	SetSpeed(a_Speed);
}





void cThrownEnderPearlEntity::OnHitSolidBlock(int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace)
{
	// Teleport the creator here, make them take 5 damage:
	if (m_Creator != NULL)
	{
		// TODO: The coords might need some tweaking based on the block face
		m_Creator->TeleportToCoords(a_BlockX + 0.5, a_BlockY + 1.7, a_BlockZ + 0.5);
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





void cThrownSnowballEntity::OnHitSolidBlock(int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace)
{
	// TODO: Apply damage to certain mobs (blaze etc.) and anger all mobs
	
	Destroy();
}





