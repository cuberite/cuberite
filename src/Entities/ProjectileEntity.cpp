
// ProjectileEntity.cpp

// Implements the cProjectileEntity class representing the common base class for projectiles, as well as individual projectile types

#include "Globals.h"

#include "../Bindings/PluginManager.h"
#include "ProjectileEntity.h"
#include "../ClientHandle.h"
#include "../LineBlockTracer.h"
#include "../BoundingBox.h"
#include "../ChunkMap.h"
#include "../Chunk.h"

#include "ArrowEntity.h"
#include "ThrownEggEntity.h"
#include "ThrownEnderPearlEntity.h"
#include "ExpBottleEntity.h"
#include "ThrownSnowballEntity.h"
#include "FireChargeEntity.h"
#include "FireworkEntity.h"
#include "GhastFireballEntity.h"
#include "WitherSkullEntity.h"
#include "SplashPotionEntity.h"
#include "Player.h"





/** Converts an angle in radians into a byte representation used by the network protocol */
#define ANGLE_TO_PROTO(X) static_cast<Byte>(X * 255 / 360)





////////////////////////////////////////////////////////////////////////////////
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
	virtual bool OnNextBlock(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, eBlockFace a_EntryFace) override
	{
		/*
		// DEBUG:
		LOGD("Hit block %d:%d at {%d, %d, %d} face %d, %s (%s)",
			a_BlockType, a_BlockMeta,
			a_BlockX, a_BlockY, a_BlockZ, a_EntryFace,
			cBlockInfo::IsSolid(a_BlockType) ? "solid" : "non-solid",
			ItemToString(cItem(a_BlockType, 1, a_BlockMeta)).c_str()
		);
		*/

		if (cBlockInfo::IsSolid(a_BlockType))
		{
			// The projectile hit a solid block, calculate the exact hit coords:
			cBoundingBox bb(a_BlockX, a_BlockX + 1, a_BlockY, a_BlockY + 1, a_BlockZ, a_BlockZ + 1);  // Bounding box of the block hit
			const Vector3d LineStart = m_Projectile->GetPosition();  // Start point for the imaginary line that goes through the block hit
			const Vector3d LineEnd = LineStart + m_Projectile->GetSpeed();  // End point for the imaginary line that goes through the block hit
			double LineCoeff = 0;  // Used to calculate where along the line an intersection with the bounding box occurs
			eBlockFace Face;  // Face hit

			if (bb.CalcLineIntersection(LineStart, LineEnd, LineCoeff, Face))
			{
				Vector3d Intersection = LineStart + m_Projectile->GetSpeed() * LineCoeff;  // Point where projectile goes into the hit block

				if (cPluginManager::Get()->CallHookProjectileHitBlock(*m_Projectile, a_BlockX, a_BlockY, a_BlockZ, Face, &Intersection))
				{
					return false;
				}

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





////////////////////////////////////////////////////////////////////////////////
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
		m_HitEntity(nullptr)
	{
	}


	virtual bool Item(cEntity * a_Entity) override
	{
		if (
			(a_Entity == m_Projectile) ||          // Do not check collisions with self
			(a_Entity->GetUniqueID() == m_Projectile->GetCreatorUniqueID())  // Do not check whoever shot the projectile
		)
		{
			// Don't check creator only for the first 5 ticks so that projectiles can collide with the creator
			if (m_Projectile->GetTicksAlive() <= 5)
			{
				return false;
			}
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

		if (
			!a_Entity->IsMob() &&
			!a_Entity->IsMinecart() &&
			(
				!a_Entity->IsPlayer() ||
				static_cast<cPlayer *>(a_Entity)->IsGameModeSpectator()
			) &&
			!a_Entity->IsBoat()
		)
		{
			// Not an entity that interacts with a projectile
			return false;
		}

		if (cPluginManager::Get()->CallHookProjectileHitEntity(*m_Projectile, *a_Entity))
		{
			// A plugin disagreed.
			return false;
		}

		if (LineCoeff < m_MinCoeff)
		{
			// The entity is closer than anything we've stored so far, replace it as the potential victim
			m_MinCoeff = LineCoeff;
			m_HitEntity = a_Entity;
		}

		// Don't break the enumeration, we want all the entities
		return false;
	}

	/** Returns the nearest entity that was hit, after the enumeration has been completed */
	cEntity * GetHitEntity(void) const { return m_HitEntity; }

	/** Returns the line coeff where the hit was encountered, after the enumeration has been completed */
	double GetMinCoeff(void) const { return m_MinCoeff; }

	/** Returns true if the callback has encountered a true hit */
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





////////////////////////////////////////////////////////////////////////////////
// cProjectileEntity:

cProjectileEntity::cProjectileEntity(eKind a_Kind, cEntity * a_Creator, double a_X, double a_Y, double a_Z, double a_Width, double a_Height) :
	super(etProjectile, a_X, a_Y, a_Z, a_Width, a_Height),
	m_ProjectileKind(a_Kind),
	m_CreatorData(
		((a_Creator != nullptr) ? a_Creator->GetUniqueID() : cEntity::INVALID_ID),
		((a_Creator != nullptr) ? (a_Creator->IsPlayer() ? static_cast<cPlayer *>(a_Creator)->GetName() : "") : ""),
		((a_Creator != nullptr) ? a_Creator->GetEquippedWeapon().m_Enchantments : cEnchantments())
	),
	m_IsInGround(false)
{
	SetGravity(-12.0f);
	SetAirDrag(0.01f);
}





cProjectileEntity::cProjectileEntity(eKind a_Kind, cEntity * a_Creator, const Vector3d & a_Pos, const Vector3d & a_Speed, double a_Width, double a_Height) :
	super(etProjectile, a_Pos.x, a_Pos.y, a_Pos.z, a_Width, a_Height),
	m_ProjectileKind(a_Kind),
	m_CreatorData(a_Creator->GetUniqueID(), a_Creator->IsPlayer() ? static_cast<cPlayer *>(a_Creator)->GetName() : "", a_Creator->GetEquippedWeapon().m_Enchantments),
	m_IsInGround(false)
{
	SetSpeed(a_Speed);
	SetYawFromSpeed();
	SetPitchFromSpeed();
	SetGravity(-12.0f);
	SetAirDrag(0.01f);
}





std::unique_ptr<cProjectileEntity> cProjectileEntity::Create(eKind a_Kind, cEntity * a_Creator, double a_X, double a_Y, double a_Z, const cItem * a_Item, const Vector3d * a_Speed)
{
	Vector3d Speed;
	if (a_Speed != nullptr)
	{
		Speed = *a_Speed;
	}

	switch (a_Kind)
	{
		case pkArrow:         return cpp14::make_unique<cArrowEntity>           (a_Creator, a_X, a_Y, a_Z, Speed);
		case pkEgg:           return cpp14::make_unique<cThrownEggEntity>       (a_Creator, a_X, a_Y, a_Z, Speed);
		case pkEnderPearl:    return cpp14::make_unique<cThrownEnderPearlEntity>(a_Creator, a_X, a_Y, a_Z, Speed);
		case pkSnowball:      return cpp14::make_unique<cThrownSnowballEntity>  (a_Creator, a_X, a_Y, a_Z, Speed);
		case pkGhastFireball: return cpp14::make_unique<cGhastFireballEntity>   (a_Creator, a_X, a_Y, a_Z, Speed);
		case pkFireCharge:    return cpp14::make_unique<cFireChargeEntity>      (a_Creator, a_X, a_Y, a_Z, Speed);
		case pkExpBottle:     return cpp14::make_unique<cExpBottleEntity>       (a_Creator, a_X, a_Y, a_Z, Speed);
		case pkSplashPotion:  return cpp14::make_unique<cSplashPotionEntity>    (a_Creator, a_X, a_Y, a_Z, Speed, *a_Item);
		case pkWitherSkull:   return cpp14::make_unique<cWitherSkullEntity>     (a_Creator, a_X, a_Y, a_Z, Speed);
		case pkFirework:
		{
			ASSERT(a_Item != nullptr);
			if (a_Item->m_FireworkItem.m_Colours.empty())
			{
				return nullptr;
			}

			return cpp14::make_unique<cFireworkEntity>(a_Creator, a_X, a_Y, a_Z, *a_Item);
		}
		case pkFishingFloat: break;
	}

	LOGWARNING("%s: Unknown projectile kind: %d", __FUNCTION__, a_Kind);
	return nullptr;
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





void cProjectileEntity::OnHitEntity(cEntity & a_EntityHit, const Vector3d & a_HitPos)
{
	UNUSED(a_HitPos);

	// If we were created by a player and we hit a pawn, notify attacking player's wolves
	if (a_EntityHit.IsPawn() && (GetCreatorName() != ""))
	{
		class cNotifyWolves : public cEntityCallback
		{
		public:
			cPawn * m_EntityHit;

			virtual bool Item(cEntity * a_Hitter) override
			{
				static_cast<cPlayer*>(a_Hitter)->NotifyNearbyWolves(m_EntityHit, true);
				return true;
			}
		} Callback;

		Callback.m_EntityHit = static_cast<cPawn*>(&a_EntityHit);
		m_World->DoWithEntityByID(GetCreatorUniqueID(), Callback);
	}
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
		case pkEnderPearl:    return "ThrownEnderpearl";
		case pkExpBottle:     return "ThrownExpBottle";
		case pkSplashPotion:  return "SplashPotion";
		case pkWitherSkull:   return "WitherSkull";
		case pkFirework:      return "Firework";
		case pkFishingFloat:  return "";  // Unknown, perhaps MC doesn't save this?
	}
	ASSERT(!"Unhandled projectile entity kind!");
	#ifndef __clang__
		return "";
	#endif
}





void cProjectileEntity::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	super::Tick(a_Dt, a_Chunk);
	if (!IsTicking())
	{
		// The base class tick destroyed us
		return;
	}
	BroadcastMovementUpdate();
}





void cProjectileEntity::HandlePhysics(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	if (m_IsInGround)
	{
		// Already-grounded projectiles don't move at all
		return;
	}

	auto DtSec = std::chrono::duration_cast<std::chrono::duration<double>>(a_Dt);

	const Vector3d DeltaSpeed = GetSpeed() * DtSec.count();
	const Vector3d Pos = GetPosition();
	const Vector3d NextPos = Pos + DeltaSpeed;

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
		if (!IsTicking())
		{
			return;  // We were destroyed by an override of OnHitEntity
		}
	}
	// TODO: Test the entities in the neighboring chunks, too

	// Trace the tick's worth of movement as a line:
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
	NewSpeed.y += m_Gravity * DtSec.count();
	NewSpeed -= NewSpeed * (m_AirDrag * 20.0f) * DtSec.count();
	SetSpeed(NewSpeed);
	SetYawFromSpeed();
	SetPitchFromSpeed();

	/*
	LOGD("Projectile %d: pos {%.02f, %.02f, %.02f}, speed {%.02f, %.02f, %.02f}, rot {%.02f, %.02f}",
		m_UniqueID,
		GetPosX(), GetPosY(), GetPosZ(),
		GetSpeedX(), GetSpeedY(), GetSpeedZ(),
		GetYaw(), GetPitch()
	);
	*/
}





void cProjectileEntity::SpawnOn(cClientHandle & a_Client)
{
	// Default spawning - use the projectile kind to spawn an object:
	a_Client.SendSpawnObject(*this, m_ProjectileKind, 12, ANGLE_TO_PROTO(GetYaw()), ANGLE_TO_PROTO(GetPitch()));
	a_Client.SendEntityMetadata(*this);
}





void cProjectileEntity::CollectedBy(cPlayer & a_Dest)
{
	// Overriden in arrow
	UNUSED(a_Dest);
}




