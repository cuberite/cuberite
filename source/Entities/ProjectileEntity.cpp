
// ProjectileEntity.cpp

// Implements the cProjectileEntity class representing the common base class for projectiles, as well as individual projectile types

#include "Globals.h"
#include "ProjectileEntity.h"
#include "../ClientHandle.h"
#include "Player.h"
#include "../LineBlockTracer.h"





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cProjectileTracerCallback:

class cProjectileTracerCallback :
	public cBlockTracer::cCallbacks
{
public:
	cProjectileTracerCallback(cProjectileEntity * a_Projectile) :
		m_Projectile(a_Projectile)
	{
	}
	
protected:
	cProjectileEntity * m_Projectile;
	
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
				break;
			}
			case E_BLOCK_WATER:
			case E_BLOCK_STATIONARY_WATER:
			{
				m_Projectile->StopBurning();
				break;
			}
		}  // switch (a_BlockType)
		
		// Continue tracing
		return false;
	}
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
		case pkArrow: return new cArrowEntity(a_Creator, a_X, a_Y, a_Z, Speed);
		// TODO: the rest
	}
	
	LOGWARNING("%s: Unknown kind: %d", __FUNCTION__, a_Kind);
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
	if (cLineBlockTracer::Trace(*m_World, TracerCallback, Pos, NextPos))
	{
		// Nothing in the way, update the position
		SetPosition(NextPos);
	}
	
	// Add gravity effect to the vertical speed component:
	SetSpeedY(GetSpeedY() + m_Gravity / 20);

	// DEBUG:
	LOGD("Arrow %d: pos {%.02f, %.02f, %.02f}, speed {%.02f, %.02f, %.02f}",
		m_UniqueID,
		GetPosX(), GetPosY(), GetPosZ(),
		GetSpeedX(), GetSpeedY(), GetSpeedZ()
	);
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cArrowEntity:

cArrowEntity::cArrowEntity(cEntity * a_Creator, double a_X, double a_Y, double a_Z, const Vector3d a_Speed) :
	super(pkArrow, a_Creator, a_X, a_Y, a_Z, 0.5, 0.5),
	m_PickupState(psNoPickup),
	m_DamageCoeff(2)
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
	m_DamageCoeff(2)
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




