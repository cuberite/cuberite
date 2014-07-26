
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Entity.h"
#include "../World.h"
#include "../Server.h"
#include "../Root.h"
#include "../Matrix4.h"
#include "../ClientHandle.h"
#include "../Chunk.h"
#include "../Simulator/FluidSimulator.h"
#include "../Bindings/PluginManager.h"
#include "../Tracer.h"
#include "Player.h"





int cEntity::m_EntityCount = 0;
cCriticalSection cEntity::m_CSCount;





cEntity::cEntity(eEntityType a_EntityType, double a_X, double a_Y, double a_Z, double a_Width, double a_Height)
	: m_UniqueID(0)
	, m_Health(1)
	, m_MaxHealth(1)
	, m_AttachedTo(NULL)
	, m_Attachee(NULL)
	, m_bDirtyHead(true)
	, m_bDirtyOrientation(true)
	, m_bHasSentNoSpeed(true)
	, m_bOnGround(false)
	, m_Gravity(-9.81f)
	, m_LastPos(a_X, a_Y, a_Z)
	, m_IsInitialized(false)
	, m_EntityType(a_EntityType)
	, m_World(NULL)
	, m_IsFireproof(false)
	, m_TicksSinceLastBurnDamage(0)
	, m_TicksSinceLastLavaDamage(0)
	, m_TicksSinceLastFireDamage(0)
	, m_TicksLeftBurning(0)
	, m_TicksSinceLastVoidDamage(0)
	, m_IsSwimming(false)
	, m_IsSubmerged(false)
	, m_AirLevel(0)
	, m_AirTickTimer(0)
	, m_TicksAlive(0)
	, m_HeadYaw(0.0)
	, m_Rot(0.0, 0.0, 0.0)
	, m_Pos(a_X, a_Y, a_Z)
	, m_WaterSpeed(0, 0, 0)
	, m_Mass (0.001)  // Default 1g
	, m_Width(a_Width)
	, m_Height(a_Height)
	, m_InvulnerableTicks(0)
{
	cCSLock Lock(m_CSCount);
	m_EntityCount++;
	m_UniqueID = m_EntityCount;
}





cEntity::~cEntity()
{
	// Before deleting, the entity needs to have been removed from the world, if ever added
	ASSERT((m_World == NULL) || !m_World->HasEntity(m_UniqueID));
	
	/*
	// DEBUG:
	LOGD("Deleting entity %d at pos {%.2f, %.2f, %.2f} ~ [%d, %d]; ptr %p",
		m_UniqueID,
		m_Pos.x, m_Pos.y, m_Pos.z,
		(int)(m_Pos.x / cChunkDef::Width), (int)(m_Pos.z / cChunkDef::Width),
		this
		);
	*/
	
	if (m_AttachedTo != NULL)
	{
		Detach();
	}
	if (m_Attachee != NULL)
	{
		m_Attachee->Detach();
	}

	if (m_IsInitialized)
	{
		LOGWARNING("ERROR: Entity deallocated without being destroyed");
		ASSERT(!"Entity deallocated without being destroyed or unlinked");
	}
}





const char * cEntity::GetClass(void) const
{
	return "cEntity";
}





const char * cEntity::GetClassStatic(void)
{
	return "cEntity";
}





const char * cEntity::GetParentClass(void) const
{
	return "";
}





bool cEntity::Initialize(cWorld & a_World)
{
	if (cPluginManager::Get()->CallHookSpawningEntity(a_World, *this))
	{
		return false;
	}
	
	/*
	// DEBUG:
	LOGD("Initializing entity #%d (%s) at {%.02f, %.02f, %.02f}",
		m_UniqueID, GetClass(), m_Pos.x, m_Pos.y, m_Pos.z
	);
	*/
	
	m_IsInitialized = true;
	m_World = &a_World;
	m_World->AddEntity(this);
	
	cPluginManager::Get()->CallHookSpawnedEntity(a_World, *this);
	
	// Spawn the entity on the clients:
	a_World.BroadcastSpawnEntity(*this);
	
	return true;
}





void cEntity::WrapHeadYaw(void)
{
	m_HeadYaw = NormalizeAngleDegrees(m_HeadYaw);
}





void cEntity::WrapRotation(void)
{
	m_Rot.x = NormalizeAngleDegrees(m_Rot.x);
	m_Rot.y = NormalizeAngleDegrees(m_Rot.y);
}




void cEntity::WrapSpeed(void)
{
	m_Speed.x = Clamp(m_Speed.x, -78.0, 78.0);
	m_Speed.y = Clamp(m_Speed.y, -78.0, 78.0);
	m_Speed.z = Clamp(m_Speed.z, -78.0, 78.0);
}





void cEntity::Destroy(bool a_ShouldBroadcast)
{
	if (!m_IsInitialized)
	{
		return;
	}
	
	if (a_ShouldBroadcast)
	{
		m_World->BroadcastDestroyEntity(*this);
	}

	m_IsInitialized = false;

	Destroyed();
}





void cEntity::TakeDamage(cEntity & a_Attacker)
{
	int RawDamage = a_Attacker.GetRawDamageAgainst(*this);
	
	TakeDamage(dtAttack, &a_Attacker, RawDamage, a_Attacker.GetKnockbackAmountAgainst(*this));
}





void cEntity::TakeDamage(eDamageType a_DamageType, cEntity * a_Attacker, int a_RawDamage, double a_KnockbackAmount)
{
	int FinalDamage = a_RawDamage - GetArmorCoverAgainst(a_Attacker, a_DamageType, a_RawDamage);
	cEntity::TakeDamage(a_DamageType, a_Attacker, a_RawDamage, FinalDamage, a_KnockbackAmount);
}





void cEntity::TakeDamage(eDamageType a_DamageType, cEntity * a_Attacker, int a_RawDamage, int a_FinalDamage, double a_KnockbackAmount)
{
	TakeDamageInfo TDI;
	TDI.DamageType = a_DamageType;
	TDI.Attacker = a_Attacker;
	TDI.RawDamage = a_RawDamage;
	TDI.FinalDamage = a_FinalDamage;
	
	Vector3d Heading(0, 0, 0);
	if (a_Attacker != NULL)
	{
		Heading = a_Attacker->GetLookVector() * (a_Attacker->IsSprinting() ? 10 : 8);
	}
	Heading.y = 2;

	TDI.Knockback = Heading * a_KnockbackAmount;
	DoTakeDamage(TDI);
}





void cEntity::SetYawFromSpeed(void)
{
	const double EPS = 0.0000001;
	if ((abs(m_Speed.x) < EPS) && (abs(m_Speed.z) < EPS))
	{
		// atan2() may overflow or is undefined, pick any number
		SetYaw(0);
		return;
	}
	SetYaw(atan2(m_Speed.x, m_Speed.z) * 180 / PI);
}





void cEntity::SetPitchFromSpeed(void)
{
	const double EPS = 0.0000001;
	double xz = sqrt(m_Speed.x * m_Speed.x + m_Speed.z * m_Speed.z);  // Speed XZ-plane component
	if ((abs(xz) < EPS) && (abs(m_Speed.y) < EPS))
	{
		// atan2() may overflow or is undefined, pick any number
		SetPitch(0);
		return;
	}
	SetPitch(atan2(m_Speed.y, xz) * 180 / PI);
}





bool cEntity::DoTakeDamage(TakeDamageInfo & a_TDI)
{
	if (cRoot::Get()->GetPluginManager()->CallHookTakeDamage(*this, a_TDI))
	{
		return false;
	}

	if (m_Health <= 0)
	{
		// Can't take damage if already dead
		return false;
	}

	if (m_InvulnerableTicks > 0)
	{
		// Entity is invulnerable
		return false;
	}

	if ((a_TDI.Attacker != NULL) && (a_TDI.Attacker->IsPlayer()))
	{
		cPlayer * Player = (cPlayer *)a_TDI.Attacker;

		// IsOnGround() only is false if the player is moving downwards
		// TODO: Better damage increase, and check for enchantments (and use magic critical instead of plain)
		if (!Player->IsOnGround())
		{
			if ((a_TDI.DamageType == dtAttack) || (a_TDI.DamageType == dtArrowAttack))
			{
				a_TDI.FinalDamage += 2;
				m_World->BroadcastEntityAnimation(*this, 4);  // Critical hit
			}
		}

		Player->GetStatManager().AddValue(statDamageDealt, (StatValue)floor(a_TDI.FinalDamage * 10 + 0.5));
	}

	m_Health -= (short)a_TDI.FinalDamage;
	
	// TODO: Apply damage to armor
	
	m_Health = std::max(m_Health, 0);

	if ((IsMob() || IsPlayer()) && (a_TDI.Attacker != NULL))  // Knockback for only players and mobs
	{
		int KnockbackLevel = a_TDI.Attacker->GetEquippedWeapon().m_Enchantments.GetLevel(cEnchantments::enchKnockback);  // More common enchantment
		if (KnockbackLevel < 1)
		{
			// We support punch on swords and vice versa! :)
			KnockbackLevel = a_TDI.Attacker->GetEquippedWeapon().m_Enchantments.GetLevel(cEnchantments::enchPunch);
		}

		Vector3d AdditionalSpeed(0, 0, 0);
		switch (KnockbackLevel)
		{
			case 1: AdditionalSpeed.Set(5, 0.3, 5); break;
			case 2: AdditionalSpeed.Set(8, 0.3, 8); break;
			default: break;
		}
		AddSpeed(a_TDI.Knockback + AdditionalSpeed);
	}

	m_World->BroadcastEntityStatus(*this, esGenericHurt);

	m_InvulnerableTicks = 10;

	if (m_Health <= 0)
	{
		KilledBy(a_TDI);

		if (a_TDI.Attacker != NULL)
		{
			a_TDI.Attacker->Killed(this);
		}
	}
	return true;
}





int cEntity::GetRawDamageAgainst(const cEntity & a_Receiver)
{
	// Returns the hitpoints that this pawn can deal to a_Receiver using its equipped items
	// Ref: http://www.minecraftwiki.net/wiki/Damage#Dealing_damage as of 2012_12_20
	switch (this->GetEquippedWeapon().m_ItemType)
	{
		case E_ITEM_WOODEN_SWORD:    return 4;
		case E_ITEM_GOLD_SWORD:      return 4;
		case E_ITEM_STONE_SWORD:     return 5;
		case E_ITEM_IRON_SWORD:      return 6;
		case E_ITEM_DIAMOND_SWORD:   return 7;

		case E_ITEM_WOODEN_AXE:      return 3;
		case E_ITEM_GOLD_AXE:        return 3;
		case E_ITEM_STONE_AXE:       return 4;
		case E_ITEM_IRON_AXE:        return 5;
		case E_ITEM_DIAMOND_AXE:     return 6;

		case E_ITEM_WOODEN_PICKAXE:  return 2;
		case E_ITEM_GOLD_PICKAXE:    return 2;
		case E_ITEM_STONE_PICKAXE:   return 3;
		case E_ITEM_IRON_PICKAXE:    return 4;
		case E_ITEM_DIAMOND_PICKAXE: return 5;

		case E_ITEM_WOODEN_SHOVEL:   return 1;
		case E_ITEM_GOLD_SHOVEL:     return 1;
		case E_ITEM_STONE_SHOVEL:    return 2;
		case E_ITEM_IRON_SHOVEL:     return 3;
		case E_ITEM_DIAMOND_SHOVEL:  return 4;
	}
	// All other equipped items give a damage of 1:
	return 1;
}





bool cEntity::ArmorCoversAgainst(eDamageType a_DamageType)
{
	// Ref.: http://www.minecraftwiki.net/wiki/Armor#Effects as of 2012_12_20
	switch (a_DamageType)
	{
		case dtOnFire:
		case dtSuffocating:
		case dtDrowning:  // TODO: This one could be a special case - in various MC versions (PC vs XBox) it is and isn't armor-protected
		case dtStarving:
		case dtInVoid:
		case dtPoisoning:
		case dtWithering:
		case dtPotionOfHarming:
		case dtFalling:
		case dtLightning:
		case dtPlugin:
		{
			return false;
		}
			
		case dtAttack:
		case dtArrowAttack:
		case dtCactusContact:
		case dtLavaContact:
		case dtFireContact:
		case dtEnderPearl:
		case dtExplosion:
		{
			return true;
		}
	}
	ASSERT(!"Invalid damage type!");
	return false;
}





int cEntity::GetArmorCoverAgainst(const cEntity * a_Attacker, eDamageType a_DamageType, int a_Damage)
{
	// Returns the hitpoints out of a_RawDamage that the currently equipped armor would cover
	
	// Filter out damage types that are not protected by armor:
	if (!ArmorCoversAgainst(a_DamageType)) return 0;
	
	// Add up all armor points:
	// Ref.: http://www.minecraftwiki.net/wiki/Armor#Defense_points as of 2012_12_20
	int ArmorValue = 0;
	switch (GetEquippedHelmet().m_ItemType)
	{
		case E_ITEM_LEATHER_CAP:    ArmorValue += 1; break;
		case E_ITEM_GOLD_HELMET:    ArmorValue += 2; break;
		case E_ITEM_CHAIN_HELMET:   ArmorValue += 2; break;
		case E_ITEM_IRON_HELMET:    ArmorValue += 2; break;
		case E_ITEM_DIAMOND_HELMET: ArmorValue += 3; break;
	}
	switch (GetEquippedChestplate().m_ItemType)
	{
		case E_ITEM_LEATHER_TUNIC:      ArmorValue += 3; break;
		case E_ITEM_GOLD_CHESTPLATE:    ArmorValue += 5; break;
		case E_ITEM_CHAIN_CHESTPLATE:   ArmorValue += 5; break;
		case E_ITEM_IRON_CHESTPLATE:    ArmorValue += 6; break;
		case E_ITEM_DIAMOND_CHESTPLATE: ArmorValue += 8; break;
	}
	switch (GetEquippedLeggings().m_ItemType)
	{
		case E_ITEM_LEATHER_PANTS:    ArmorValue += 2; break;
		case E_ITEM_GOLD_LEGGINGS:    ArmorValue += 3; break;
		case E_ITEM_CHAIN_LEGGINGS:   ArmorValue += 4; break;
		case E_ITEM_IRON_LEGGINGS:    ArmorValue += 5; break;
		case E_ITEM_DIAMOND_LEGGINGS: ArmorValue += 6; break;
	}
	switch (GetEquippedBoots().m_ItemType)
	{
		case E_ITEM_LEATHER_BOOTS: ArmorValue += 1; break;
		case E_ITEM_GOLD_BOOTS:    ArmorValue += 1; break;
		case E_ITEM_CHAIN_BOOTS:   ArmorValue += 1; break;
		case E_ITEM_IRON_BOOTS:    ArmorValue += 2; break;
		case E_ITEM_DIAMOND_BOOTS: ArmorValue += 3; break;
	}
	
	// TODO: Special armor cases, such as wool, saddles, dog's collar
	// Ref.: http://www.minecraftwiki.net/wiki/Armor#Mob_armor as of 2012_12_20
	
	// Now ArmorValue is in [0, 20] range, which corresponds to [0, 80%] protection. Calculate the hitpoints from that:
	return a_Damage * (ArmorValue * 4) / 100;
}





double cEntity::GetKnockbackAmountAgainst(const cEntity & a_Receiver)
{
	// Returns the knockback amount that the currently equipped items would cause to a_Receiver on a hit
	
	// TODO: Enchantments
	return 1;
}





void cEntity::KilledBy(TakeDamageInfo & a_TDI)
{
	m_Health = 0;

	cRoot::Get()->GetPluginManager()->CallHookKilling(*this, a_TDI.Attacker, a_TDI);
	
	if (m_Health > 0)
	{
		// Plugin wants to 'unkill' the pawn. Abort
		return;
	}

	// Drop loot:
	cItems Drops;
	GetDrops(Drops, a_TDI.Attacker);
	m_World->SpawnItemPickups(Drops, GetPosX(), GetPosY(), GetPosZ());

	m_World->BroadcastEntityStatus(*this, esGenericDead);
}





void cEntity::Heal(int a_HitPoints)
{
	m_Health += a_HitPoints;
	if (m_Health > m_MaxHealth)
	{
		m_Health = m_MaxHealth;
	}
}





void cEntity::SetHealth(int a_Health)
{
	m_Health = std::max(0, std::min(m_MaxHealth, a_Health));
}





void cEntity::Tick(float a_Dt, cChunk & a_Chunk)
{
	m_TicksAlive++;
	
	if (m_InvulnerableTicks > 0)
	{
		m_InvulnerableTicks--;
	}

	if (m_AttachedTo != NULL)
	{
		Vector3d DeltaPos = m_Pos - m_AttachedTo->GetPosition();
		if (DeltaPos.Length() > 0.5)
		{
			SetPosition(m_AttachedTo->GetPosition());

			if (IsPlayer())
			{
				cPlayer * Player = (cPlayer *)this;
				Player->UpdateMovementStats(DeltaPos);
			}
		}
	}
	else
	{
		if (!a_Chunk.IsValid())
		{
			return;
		}

		// Position changed -> super::Tick() called
		GET_AND_VERIFY_CURRENT_CHUNK(NextChunk, POSX_TOINT, POSZ_TOINT)

		TickBurning(*NextChunk);

		if (GetPosY() < VOID_BOUNDARY)
		{
			TickInVoid(*NextChunk);
		}
		else
		{
			m_TicksSinceLastVoidDamage = 0;
		}

		if (IsMob() || IsPlayer() || IsPickup() || IsExpOrb())
		{
			DetectCacti();
		}
		if (IsMob() || IsPlayer())
		{
			// Set swimming state
			SetSwimState(*NextChunk);

			// Handle drowning
			HandleAir();
		}

		// None of the above functions change position, we remain in the chunk of NextChunk
		HandlePhysics(a_Dt, *NextChunk);
	}
}





void cEntity::HandlePhysics(float a_Dt, cChunk & a_Chunk)
{
	int BlockX = POSX_TOINT;
	int BlockY = POSY_TOINT;
	int BlockZ = POSZ_TOINT;

	// Position changed -> super::HandlePhysics() called
	GET_AND_VERIFY_CURRENT_CHUNK(NextChunk, BlockX, BlockZ)

	// TODO Add collision detection with entities.
	a_Dt /= 1000;  // Convert from msec to sec
	Vector3d NextPos = Vector3d(GetPosX(), GetPosY(), GetPosZ());
	Vector3d NextSpeed = Vector3d(GetSpeedX(), GetSpeedY(), GetSpeedZ());
	
	if ((BlockY >= cChunkDef::Height) || (BlockY < 0))
	{
		// Outside of the world
		AddSpeedY(m_Gravity * a_Dt);
		AddPosition(GetSpeed() * a_Dt);
		return;
	}
	
	int RelBlockX = BlockX - (NextChunk->GetPosX() * cChunkDef::Width);
	int RelBlockZ = BlockZ - (NextChunk->GetPosZ() * cChunkDef::Width);
	BLOCKTYPE BlockIn = NextChunk->GetBlock( RelBlockX, BlockY, RelBlockZ);
	BLOCKTYPE BlockBelow = (BlockY > 0) ? NextChunk->GetBlock(RelBlockX, BlockY - 1, RelBlockZ) : E_BLOCK_AIR;
	if (!cBlockInfo::IsSolid(BlockIn))  // Making sure we are not inside a solid block
	{
		if (m_bOnGround)  // check if it's still on the ground
		{
			if (!cBlockInfo::IsSolid(BlockBelow))  // Check if block below is air or water.
			{
				m_bOnGround = false;
			}
		}
	}
	else
	{
		// Push out entity.
		BLOCKTYPE GotBlock;

		static const struct
		{
			int x, y, z;
		} gCrossCoords[] =
		{
			{ 1, 0,  0},
			{-1, 0,  0},
			{ 0, 0,  1},
			{ 0, 0, -1},
		} ;

		bool IsNoAirSurrounding = true;
		for (size_t i = 0; i < ARRAYCOUNT(gCrossCoords); i++)
		{
			if (!NextChunk->UnboundedRelGetBlockType(RelBlockX + gCrossCoords[i].x, BlockY, RelBlockZ + gCrossCoords[i].z, GotBlock))
			{
				// The pickup is too close to an unloaded chunk, bail out of any physics handling
				return;
			}
			if (!cBlockInfo::IsSolid(GotBlock))
			{
				NextPos.x += gCrossCoords[i].x;
				NextPos.z += gCrossCoords[i].z;
				IsNoAirSurrounding = false;
				break;
			}
		}  // for i - gCrossCoords[]
			
		if (IsNoAirSurrounding)
		{
			NextPos.y += 0.5;
		}

		m_bOnGround = true;

		/*
		// DEBUG:
		LOGD("Entity #%d (%s) is inside a block at {%d, %d, %d}",
			m_UniqueID, GetClass(), BlockX, BlockY, BlockZ
		);
		*/
	}

	if (!m_bOnGround)
	{
		float fallspeed;
		if (IsBlockWater(BlockIn))
		{
			fallspeed = m_Gravity * a_Dt / 3;  // Fall 3x slower in water.
		}
		else if (BlockIn == E_BLOCK_COBWEB)
		{
			NextSpeed.y *= 0.05;  // Reduce overall falling speed
			fallspeed = 0;  // No falling.
		}
		else
		{
			// Normal gravity
			fallspeed = m_Gravity * a_Dt;
		}
		NextSpeed.y += fallspeed;
	}
	else
	{
		// Friction
		if (NextSpeed.SqrLength() > 0.0004f)
		{
			NextSpeed.x *= 0.7f / (1 + a_Dt);
			if (fabs(NextSpeed.x) < 0.05)
			{
				NextSpeed.x = 0;
			}
			NextSpeed.z *= 0.7f / (1 + a_Dt);
			if (fabs(NextSpeed.z) < 0.05)
			{
				NextSpeed.z = 0;
			}
		}
	}

	// Adjust X and Z speed for COBWEB temporary. This speed modification should be handled inside block handlers since we
	// might have different speed modifiers according to terrain.
	if (BlockIn == E_BLOCK_COBWEB)
	{
		NextSpeed.x *= 0.25;
		NextSpeed.z *= 0.25;
	}

	// Get water direction
	Direction WaterDir = m_World->GetWaterSimulator()->GetFlowingDirection(BlockX, BlockY, BlockZ);

	m_WaterSpeed *= 0.9f;  // Reduce speed each tick

	switch (WaterDir)
	{
		case X_PLUS:
			m_WaterSpeed.x = 0.2f;
			m_bOnGround = false;
			break;
		case X_MINUS:
			m_WaterSpeed.x = -0.2f;
			m_bOnGround = false;
			break;
		case Z_PLUS:
			m_WaterSpeed.z = 0.2f;
			m_bOnGround = false;
			break;
		case Z_MINUS:
			m_WaterSpeed.z = -0.2f;
			m_bOnGround = false;
			break;
			
	default:
		break;
	}

	if (fabs(m_WaterSpeed.x) < 0.05)
	{
		m_WaterSpeed.x = 0;
	}

	if (fabs(m_WaterSpeed.z) < 0.05)
	{
		m_WaterSpeed.z = 0;
	}

	NextSpeed += m_WaterSpeed;

	if (NextSpeed.SqrLength() > 0.f)
	{
		cTracer Tracer(GetWorld());
		// Distance traced is an integer, so we round up from the distance we should go (Speed * Delta), else we will encounter collision detection failurse
		int DistanceToTrace = (int)(ceil((NextSpeed * a_Dt).SqrLength()) * 2);
		bool HasHit = Tracer.Trace(NextPos, NextSpeed, DistanceToTrace);

		if (HasHit)
		{
			// Oh noez! We hit something: verify that the (hit position - current) was smaller or equal to the (position that we should travel without obstacles - current)
			// This is because previously, we traced with a length that was rounded up (due to integer limitations), and in the case that something was hit, we don't want to overshoot our projected movement
			if ((Tracer.RealHit - NextPos).SqrLength() <= (NextSpeed * a_Dt).SqrLength())
			{
				// Block hit was within our projected path
				// Begin by stopping movement in the direction that we hit something. The Normal is the line perpendicular to a 2D face and in this case, stores what block face was hit through either -1 or 1.
				// For example: HitNormal.y = -1 : BLOCK_FACE_YM; HitNormal.y = 1 : BLOCK_FACE_YP
				if (Tracer.HitNormal.x != 0.f) NextSpeed.x = 0.f;
				if (Tracer.HitNormal.y != 0.f) NextSpeed.y = 0.f;
				if (Tracer.HitNormal.z != 0.f) NextSpeed.z = 0.f;

				if (Tracer.HitNormal.y == 1)  // Hit BLOCK_FACE_YP, we are on the ground
				{
					m_bOnGround = true;
				}

				// Now, set our position to the hit block (i.e. move part way along our intended trajectory)
				NextPos.Set(Tracer.RealHit.x, Tracer.RealHit.y, Tracer.RealHit.z);
				NextPos.x += Tracer.HitNormal.x * 0.1;
				NextPos.y += Tracer.HitNormal.y * 0.05;
				NextPos.z += Tracer.HitNormal.z * 0.1;
			}
			else
			{
				// We have hit a block but overshot our intended trajectory, move normally, safe in the warm cocoon of knowledge that we won't appear to teleport forwards on clients,
				// and that this piece of software will come to be hailed as the epitome of performance and functionality in C++, never before seen, and of such a like that will never
				// be henceforth seen again in the time of programmers and man alike
				// </&sensationalist>
				NextPos += (NextSpeed * a_Dt);
			}
		}
		else
		{
			// We didn't hit anything, so move =]
			NextPos += (NextSpeed * a_Dt);
		}
	}

	SetPosition(NextPos);
	SetSpeed(NextSpeed);
}





void cEntity::TickBurning(cChunk & a_Chunk)
{
	// Remember the current burning state:
	bool HasBeenBurning = (m_TicksLeftBurning > 0);

	if (m_World->IsWeatherWet())
	{
		if (POSY_TOINT > m_World->GetHeight(POSX_TOINT, POSZ_TOINT))
		{
			m_TicksLeftBurning = 0;
		}
	}
	
	// Do the burning damage:
	if (m_TicksLeftBurning > 0)
	{
		m_TicksSinceLastBurnDamage++;
		if (m_TicksSinceLastBurnDamage >= BURN_TICKS_PER_DAMAGE)
		{
			if (!m_IsFireproof)
			{
				TakeDamage(dtOnFire, NULL, BURN_DAMAGE, 0);
			}
			m_TicksSinceLastBurnDamage = 0;
		}
		m_TicksLeftBurning--;
	}
	
	// Update the burning times, based on surroundings:
	int MinRelX = (int)floor(GetPosX() - m_Width / 2) - a_Chunk.GetPosX() * cChunkDef::Width;
	int MaxRelX = (int)floor(GetPosX() + m_Width / 2) - a_Chunk.GetPosX() * cChunkDef::Width;
	int MinRelZ = (int)floor(GetPosZ() - m_Width / 2) - a_Chunk.GetPosZ() * cChunkDef::Width;
	int MaxRelZ = (int)floor(GetPosZ() + m_Width / 2) - a_Chunk.GetPosZ() * cChunkDef::Width;
	int MinY = std::max(0, std::min(cChunkDef::Height - 1, POSY_TOINT));
	int MaxY = std::max(0, std::min(cChunkDef::Height - 1, (int)ceil (GetPosY() + m_Height)));
	bool HasWater = false;
	bool HasLava = false;
	bool HasFire = false;
	
	for (int x = MinRelX; x <= MaxRelX; x++)
	{
		for (int z = MinRelZ; z <= MaxRelZ; z++)
		{
			int RelX = x;
			int RelZ = z;

			for (int y = MinY; y <= MaxY; y++)
			{
				BLOCKTYPE Block;
				a_Chunk.UnboundedRelGetBlockType(RelX, y, RelZ, Block);
				
				switch (Block)
				{
					case E_BLOCK_FIRE:
					{
						HasFire = true;
						break;
					}
					case E_BLOCK_LAVA:
					case E_BLOCK_STATIONARY_LAVA:
					{
						HasLava = true;
						break;
					}
					case E_BLOCK_STATIONARY_WATER:
					case E_BLOCK_WATER:
					{
						HasWater = true;
						break;
					}
				}  // switch (BlockType)
			}  // for y
		}  // for z
	}  // for x
	
	if (HasWater)
	{
		// Extinguish the fire
		m_TicksLeftBurning = 0;
	}
	
	if (HasLava)
	{
		// Burn:
		m_TicksLeftBurning = BURN_TICKS;
		
		// Periodically damage:
		m_TicksSinceLastLavaDamage++;
		if (m_TicksSinceLastLavaDamage >= LAVA_TICKS_PER_DAMAGE)
		{
			if (!m_IsFireproof)
			{
				TakeDamage(dtLavaContact, NULL, LAVA_DAMAGE, 0);
			}
			m_TicksSinceLastLavaDamage = 0;
		}
	}
	else
	{
		m_TicksSinceLastLavaDamage = 0;
	}
	
	if (HasFire)
	{
		// Burn:
		m_TicksLeftBurning = BURN_TICKS;
		
		// Periodically damage:
		m_TicksSinceLastFireDamage++;
		if (m_TicksSinceLastFireDamage >= FIRE_TICKS_PER_DAMAGE)
		{
			if (!m_IsFireproof)
			{
				TakeDamage(dtFireContact, NULL, FIRE_DAMAGE, 0);
			}
			m_TicksSinceLastFireDamage = 0;
		}
	}
	else
	{
		m_TicksSinceLastFireDamage = 0;
	}
	
	// If just started / finished burning, notify descendants:
	if ((m_TicksLeftBurning > 0) && !HasBeenBurning)
	{
		OnStartedBurning();
	}
	else if ((m_TicksLeftBurning <= 0) && HasBeenBurning)
	{
		OnFinishedBurning();
	}
}





void cEntity::TickInVoid(cChunk & a_Chunk)
{
	if (m_TicksSinceLastVoidDamage == 20)
	{
		TakeDamage(dtInVoid, NULL, 2, 0);
		m_TicksSinceLastVoidDamage = 0;
	}
	else
	{
		m_TicksSinceLastVoidDamage++;
	}
}





void cEntity::DetectCacti(void)
{
	int X = POSX_TOINT, Y = POSY_TOINT, Z = POSZ_TOINT;
	double w = m_Width / 2;
	if (
		((Y > 0) && (Y < cChunkDef::Height)) &&
		((((X + 1) - GetPosX() < w) && (GetWorld()->GetBlock(X + 1, Y, Z) == E_BLOCK_CACTUS)) ||
		((GetPosX() - X < w) && (GetWorld()->GetBlock(X - 1, Y, Z) == E_BLOCK_CACTUS)) ||
		(((Z + 1) - GetPosZ() < w) && (GetWorld()->GetBlock(X, Y, Z + 1) == E_BLOCK_CACTUS)) ||
		((GetPosZ() - Z < w) && (GetWorld()->GetBlock(X, Y, Z - 1) == E_BLOCK_CACTUS)) ||
		(((GetPosY() - Y < 1) && (GetWorld()->GetBlock(X, Y, Z) == E_BLOCK_CACTUS))))
		)
	{
		TakeDamage(dtCactusContact, NULL, 1, 0);
	}
}





void cEntity::SetSwimState(cChunk & a_Chunk)
{
	int RelY = (int)floor(GetPosY() + 0.1);
	if ((RelY < 0) || (RelY >= cChunkDef::Height - 1))
	{
		m_IsSwimming = false;
		m_IsSubmerged = false;
		return;
	}

	BLOCKTYPE BlockIn;
	int RelX = POSX_TOINT - a_Chunk.GetPosX() * cChunkDef::Width;
	int RelZ = POSZ_TOINT - a_Chunk.GetPosZ() * cChunkDef::Width;

	// Check if the player is swimming:
	if (!a_Chunk.UnboundedRelGetBlockType(RelX, RelY, RelZ, BlockIn))
	{
		// This sometimes happens on Linux machines
		// Ref.: http://forum.mc-server.org/showthread.php?tid=1244
		LOGD("SetSwimState failure: RelX = %d, RelZ = %d, Pos = %.02f, %.02f}",
			RelX, RelY, GetPosX(), GetPosZ()
		);
		m_IsSwimming = false;
		m_IsSubmerged = false;
		return;
	}
	m_IsSwimming = IsBlockWater(BlockIn);

	// Check if the player is submerged:
	VERIFY(a_Chunk.UnboundedRelGetBlockType(RelX, RelY + 1, RelZ, BlockIn));
	m_IsSubmerged = IsBlockWater(BlockIn);
}





void cEntity::DoSetSpeed(double a_SpeedX, double a_SpeedY, double a_SpeedZ)
{
	m_Speed.Set(a_SpeedX, a_SpeedY, a_SpeedZ);
	
	WrapSpeed();
}





void cEntity::HandleAir(void)
{
	// Ref.: http://www.minecraftwiki.net/wiki/Chunk_format
	// See if the entity is /submerged/ water (block above is water)
	// Get the type of block the entity is standing in:

	if (IsSubmerged())
	{
		if (!IsPlayer())  // Players control themselves
		{
			SetSpeedY(1);  // Float in the water
		}

		// Either reduce air level or damage player
		if (m_AirLevel < 1)
		{
			if (m_AirTickTimer < 1)
			{
				// Damage player
				TakeDamage(dtDrowning, NULL, 1, 1, 0);
				// Reset timer
				m_AirTickTimer = DROWNING_TICKS;
			}
			else
			{
				m_AirTickTimer--;
			}
		}
		else
		{
			// Reduce air supply
			m_AirLevel--;
		}
	}
	else
	{
		// Set the air back to maximum
		m_AirLevel = MAX_AIR_LEVEL;
		m_AirTickTimer = DROWNING_TICKS;
	}
}





/// Called when the entity starts burning
void cEntity::OnStartedBurning(void)
{
	// Broadcast the change:
	m_World->BroadcastEntityMetadata(*this);
}





/// Called when the entity finishes burning
void cEntity::OnFinishedBurning(void)
{
	// Broadcast the change:
	m_World->BroadcastEntityMetadata(*this);
}





/// Sets the maximum value for the health
void cEntity::SetMaxHealth(int a_MaxHealth)
{
	m_MaxHealth = a_MaxHealth;

	// Reset health, if too high:
	m_Health = std::min(m_Health, a_MaxHealth);
}





/// Sets whether the entity is fireproof
void cEntity::SetIsFireproof(bool a_IsFireproof)
{
	m_IsFireproof = a_IsFireproof;
}





/// Puts the entity on fire for the specified amount of ticks
void cEntity::StartBurning(int a_TicksLeftBurning)
{
	if (m_TicksLeftBurning > 0)
	{
		// Already burning, top up the ticks left burning and bail out:
		m_TicksLeftBurning = std::max(m_TicksLeftBurning, a_TicksLeftBurning);
		return;
	}
	
	m_TicksLeftBurning = a_TicksLeftBurning;
	OnStartedBurning();
}





/// Stops the entity from burning, resets all burning timers
void cEntity::StopBurning(void)
{
	bool HasBeenBurning = (m_TicksLeftBurning > 0);
	m_TicksLeftBurning = 0;
	m_TicksSinceLastBurnDamage = 0;
	m_TicksSinceLastFireDamage = 0;
	m_TicksSinceLastLavaDamage = 0;
	
	// Notify if the entity has stopped burning
	if (HasBeenBurning)
	{
		OnFinishedBurning();
	}
}





void cEntity::TeleportToEntity(cEntity & a_Entity)
{
	TeleportToCoords(a_Entity.GetPosX(), a_Entity.GetPosY(), a_Entity.GetPosZ());
}





void cEntity::TeleportToCoords(double a_PosX, double a_PosY, double a_PosZ)
{
	SetPosition(a_PosX, a_PosY, a_PosZ);
	m_World->BroadcastTeleportEntity(*this);
}





void cEntity::BroadcastMovementUpdate(const cClientHandle * a_Exclude)
{
	// Process packet sending every two ticks
	if (GetWorld()->GetWorldAge() % 2 == 0)
	{
		double SpeedSqr = GetSpeed().SqrLength();
		if (SpeedSqr == 0.0)
		{
			// Speed is zero, send this to clients once only as well as an absolute position
			if (!m_bHasSentNoSpeed)
			{
				m_World->BroadcastEntityVelocity(*this, a_Exclude);
				m_World->BroadcastTeleportEntity(*this, a_Exclude);
				m_bHasSentNoSpeed = true;
			}
		}
		else
		{
			// Movin'
			m_World->BroadcastEntityVelocity(*this, a_Exclude);
			m_bHasSentNoSpeed = false;
		}
		
		// TODO: Pickups move disgracefully if relative move packets are sent as opposed to just velocity. Have a system to send relmove only when SetPosXXX() is called with a large difference in position
		int DiffX = (int)(floor(GetPosX() * 32.0) - floor(m_LastPos.x * 32.0));
		int DiffY = (int)(floor(GetPosY() * 32.0) - floor(m_LastPos.y * 32.0));
		int DiffZ = (int)(floor(GetPosZ() * 32.0) - floor(m_LastPos.z * 32.0));

		if ((DiffX != 0) || (DiffY != 0) || (DiffZ != 0))  // Have we moved?
		{
			if ((abs(DiffX) <= 127) && (abs(DiffY) <= 127) && (abs(DiffZ) <= 127))  // Limitations of a Byte
			{
				// Difference within Byte limitations, use a relative move packet
				if (m_bDirtyOrientation)
				{
					m_World->BroadcastEntityRelMoveLook(*this, (char)DiffX, (char)DiffY, (char)DiffZ, a_Exclude);
					m_bDirtyOrientation = false;
				}
				else
				{
					m_World->BroadcastEntityRelMove(*this, (char)DiffX, (char)DiffY, (char)DiffZ, a_Exclude);
				}
				// Clients seem to store two positions, one for the velocity packet and one for the teleport/relmove packet
				// The latter is only changed with a relmove/teleport, and m_LastPos stores this position
				m_LastPos = GetPosition();
			}
			else
			{
				// Too big a movement, do a teleport
				m_World->BroadcastTeleportEntity(*this, a_Exclude);
				m_LastPos = GetPosition();  // See above
				m_bDirtyOrientation = false;
			}
		}

		if (m_bDirtyHead)
		{
			m_World->BroadcastEntityHeadLook(*this, a_Exclude);
			m_bDirtyHead = false;
		}
		if (m_bDirtyOrientation)
		{
			// Send individual update in case above (sending with rel-move packet) wasn't done
			GetWorld()->BroadcastEntityLook(*this, a_Exclude);
			m_bDirtyOrientation = false;
		}
	}
}





void cEntity::AttachTo(cEntity * a_AttachTo)
{
	if (m_AttachedTo == a_AttachTo)
	{
		// Already attached to that entity, nothing to do here
		return;
	}
	if (m_AttachedTo != NULL)
	{
		// Detach from any previous entity:
		Detach();
	}

	// Attach to the new entity:
	m_AttachedTo = a_AttachTo;
	a_AttachTo->m_Attachee = this;
	m_World->BroadcastAttachEntity(*this, a_AttachTo);
}





void cEntity::Detach(void)
{
	if (m_AttachedTo == NULL)
	{
		// Attached to no entity, our work is done
		return;
	}
	m_AttachedTo->m_Attachee = NULL;
	m_AttachedTo = NULL;
	m_World->BroadcastAttachEntity(*this, NULL);
}





bool cEntity::IsA(const char * a_ClassName) const
{
	return (strcmp(a_ClassName, "cEntity") == 0);
}





void cEntity::SetRot(const Vector3f & a_Rot)
{
	m_Rot = a_Rot;
	m_bDirtyOrientation = true;
}





void cEntity::SetHeadYaw(double a_HeadYaw)
{
	m_HeadYaw = a_HeadYaw;
	m_bDirtyHead = true;
	WrapHeadYaw();
}





void cEntity::SetHeight(double a_Height)
{
	m_Height = a_Height;
}





void cEntity::SetMass(double a_Mass)
{
	if (a_Mass > 0)
	{
		m_Mass = a_Mass;
	}
	else
	{
		// Make sure that mass is not zero. 1g is the default because we
		// have to choose a number. It's perfectly legal to have a mass
		// less than 1g as long as is NOT equal or less than zero.
		m_Mass = 0.001;
	}
}





void cEntity::SetYaw(double a_Yaw)
{
	m_Rot.x = a_Yaw;
	m_bDirtyOrientation = true;
	WrapRotation();
}





void cEntity::SetPitch(double a_Pitch)
{
	m_Rot.y = a_Pitch;
	m_bDirtyOrientation = true;
	WrapRotation();
}





void cEntity::SetRoll(double a_Roll)
{
	m_Rot.z = a_Roll;
	m_bDirtyOrientation = true;
}





void cEntity::SetSpeed(double a_SpeedX, double a_SpeedY, double a_SpeedZ)
{
	DoSetSpeed(a_SpeedX, a_SpeedY, a_SpeedZ);
}




void cEntity::SetSpeedX(double a_SpeedX)
{
	SetSpeed(a_SpeedX, m_Speed.y, m_Speed.z);
}




void cEntity::SetSpeedY(double a_SpeedY)
{
	SetSpeed(m_Speed.x, a_SpeedY, m_Speed.z);
}




void cEntity::SetSpeedZ(double a_SpeedZ)
{
	SetSpeed(m_Speed.x, m_Speed.y, a_SpeedZ);
}





void cEntity::SetWidth(double a_Width)
{
	m_Width = a_Width;
}





void cEntity::AddPosX(double a_AddPosX)
{
	m_Pos.x += a_AddPosX;
}




void cEntity::AddPosY(double a_AddPosY)
{
	m_Pos.y += a_AddPosY;
}




void cEntity::AddPosZ(double a_AddPosZ)
{
	m_Pos.z += a_AddPosZ;
}




void cEntity::AddPosition(double a_AddPosX, double a_AddPosY, double a_AddPosZ)
{
	m_Pos.x += a_AddPosX;
	m_Pos.y += a_AddPosY;
	m_Pos.z += a_AddPosZ;
}




void cEntity::AddSpeed(double a_AddSpeedX, double a_AddSpeedY, double a_AddSpeedZ)
{
	m_Speed.x += a_AddSpeedX;
	m_Speed.y += a_AddSpeedY;
	m_Speed.z += a_AddSpeedZ;
	WrapSpeed();
}





void cEntity::AddSpeedX(double a_AddSpeedX)
{
	m_Speed.x += a_AddSpeedX;
	WrapSpeed();
}





void cEntity::AddSpeedY(double a_AddSpeedY)
{
	m_Speed.y += a_AddSpeedY;
	WrapSpeed();
}





void cEntity::AddSpeedZ(double a_AddSpeedZ)
{
	m_Speed.z += a_AddSpeedZ;
	WrapSpeed();
}





void cEntity::HandleSpeedFromAttachee(float a_Forward, float a_Sideways)
{
	Vector3d LookVector = m_Attachee->GetLookVector();
	double AddSpeedX = LookVector.x * a_Forward + LookVector.z * a_Sideways;
	double AddSpeedZ = LookVector.z * a_Forward - LookVector.x * a_Sideways;
	SetSpeed(AddSpeedX, 0, AddSpeedZ);
	BroadcastMovementUpdate();
}





void cEntity::SteerVehicle(float a_Forward, float a_Sideways)
{
	if (m_AttachedTo == NULL)
	{
		return;
	}
	if ((a_Forward != 0) || (a_Sideways != 0))
	{
		m_AttachedTo->HandleSpeedFromAttachee(a_Forward, a_Sideways);
	}
}





////////////////////////////////////////////////////////////////////////////////
// Get look vector (this is NOT a rotation!)
Vector3d cEntity::GetLookVector(void) const
{
	Matrix4d m;
	m.Init(Vector3d(), 0, m_Rot.x, -m_Rot.y);
	Vector3d Look = m.Transform(Vector3d(0, 0, 1));
	return Look;
}





////////////////////////////////////////////////////////////////////////////////
// Set position
void cEntity::SetPosition(double a_PosX, double a_PosY, double a_PosZ)
{
	m_Pos.Set(a_PosX, a_PosY, a_PosZ);
}





void cEntity::SetPosX(double a_PosX)
{
	m_Pos.x = a_PosX;
}





void cEntity::SetPosY(double a_PosY)
{
	m_Pos.y = a_PosY;
}





void cEntity::SetPosZ(double a_PosZ)
{
	m_Pos.z = a_PosZ;
}




