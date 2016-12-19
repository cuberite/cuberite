
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Entity.h"
#include "../World.h"
#include "../Root.h"
#include "../Matrix4.h"
#include "../ClientHandle.h"
#include "../Chunk.h"
#include "../Simulator/FluidSimulator.h"
#include "../Bindings/PluginManager.h"
#include "../LineBlockTracer.h"
#include "Player.h"
#include "Items/ItemHandler.h"
#include "../FastRandom.h"
#include "../NetherPortalScanner.h"





static UInt32 GetNextUniqueID(void)
{
	static std::atomic<UInt32> counter(1);
	return counter.fetch_add(1);
}





////////////////////////////////////////////////////////////////////////////////
// cEntity:

cEntity::cEntity(eEntityType a_EntityType, double a_X, double a_Y, double a_Z, double a_Width, double a_Height):
	m_UniqueID(GetNextUniqueID()),
	m_Health(1),
	m_MaxHealth(1),
	m_AttachedTo(nullptr),
	m_Attachee(nullptr),
	m_bDirtyHead(true),
	m_bDirtyOrientation(true),
	m_bHasSentNoSpeed(true),
	m_bOnGround(false),
	m_Gravity(-9.81f),
	m_AirDrag(0.02f),
	m_LastPosition(a_X, a_Y, a_Z),
	m_EntityType(a_EntityType),
	m_World(nullptr),
	m_IsWorldChangeScheduled(false),
	m_IsFireproof(false),
	m_TicksSinceLastBurnDamage(0),
	m_TicksSinceLastLavaDamage(0),
	m_TicksSinceLastFireDamage(0),
	m_TicksLeftBurning(0),
	m_TicksSinceLastVoidDamage(0),
	m_IsSwimming(false),
	m_IsSubmerged(false),
	m_AirLevel(0),
	m_AirTickTimer(0),
	m_TicksAlive(0),
	m_IsTicking(false),
	m_ParentChunk(nullptr),
	m_HeadYaw(0.0),
	m_Rot(0.0, 0.0, 0.0),
	m_Position(a_X, a_Y, a_Z),
	m_LastSentPosition(a_X, a_Y, a_Z),
	m_WaterSpeed(0, 0, 0),
	m_Mass (0.001),  // Default 1g
	m_Width(a_Width),
	m_Height(a_Height),
	m_InvulnerableTicks(0)
{
}





cEntity::~cEntity()
{

	// Before deleting, the entity needs to have been removed from the world, if ever added
	ASSERT((m_World == nullptr) || !m_World->HasEntity(m_UniqueID));
	ASSERT(!IsTicking());

	/*
	// DEBUG:
	LOGD("Deleting entity %d at pos {%.2f, %.2f, %.2f} ~ [%d, %d]; ptr %p",
		m_UniqueID,
		m_Pos.x, m_Pos.y, m_Pos.z,
		(int)(m_Pos.x / cChunkDef::Width), (int)(m_Pos.z / cChunkDef::Width),
		this
		);
	*/

	if (m_AttachedTo != nullptr)
	{
		Detach();
	}
	if (m_Attachee != nullptr)
	{
		m_Attachee->Detach();
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





bool cEntity::Initialize(OwnedEntity a_Self, cWorld & a_EntityWorld)
{
	if (cPluginManager::Get()->CallHookSpawningEntity(a_EntityWorld, *this))
	{
		return false;
	}

	/*
	// DEBUG:
	LOGD("Initializing entity #%d (%s) at {%.02f, %.02f, %.02f}",
		m_UniqueID, GetClass(), m_Pos.x, m_Pos.y, m_Pos.z
	);
	*/

	ASSERT(m_World == nullptr);
	ASSERT(GetParentChunk() == nullptr);
	a_EntityWorld.AddEntity(std::move(a_Self));
	ASSERT(m_World != nullptr);

	cPluginManager::Get()->CallHookSpawnedEntity(a_EntityWorld, *this);

	// Spawn the entity on the clients:
	a_EntityWorld.BroadcastSpawnEntity(*this);

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





void cEntity::SetParentChunk(cChunk * a_Chunk)
{
	m_ParentChunk = a_Chunk;
}





cChunk * cEntity::GetParentChunk()
{
	return m_ParentChunk;
}





void cEntity::Destroy(bool a_ShouldBroadcast)
{
	ASSERT(IsTicking());
	ASSERT(GetParentChunk() != nullptr);
	SetIsTicking(false);

	if (a_ShouldBroadcast)
	{
		m_World->BroadcastDestroyEntity(*this);
	}

	cChunk * ParentChunk = GetParentChunk();
	m_World->QueueTask([this, ParentChunk](cWorld & a_World)
	{
		LOGD("Destroying entity #%i (%s) from chunk (%d, %d)",
			this->GetUniqueID(), this->GetClass(),
			ParentChunk->GetPosX(), ParentChunk->GetPosZ()
		);

		// Make sure that RemoveEntity returned a valid smart pointer
		// Also, not storing the returned pointer means automatic destruction
		VERIFY(ParentChunk->RemoveEntity(*this));
	});
	Destroyed();
}





void cEntity::DestroyNoScheduling(bool a_ShouldBroadcast)
{
	SetIsTicking(false);
	if (a_ShouldBroadcast)
	{
		m_World->BroadcastDestroyEntity(*this);
	}

	Destroyed();
}






void cEntity::TakeDamage(cEntity & a_Attacker)
{
	int RawDamage = a_Attacker.GetRawDamageAgainst(*this);
	TakeDamage(dtAttack, &a_Attacker, RawDamage, a_Attacker.GetKnockbackAmountAgainst(*this));
}





void cEntity::TakeDamage(eDamageType a_DamageType, cEntity * a_Attacker, int a_RawDamage, double a_KnockbackAmount)
{
	int ArmorCover = GetArmorCoverAgainst(a_Attacker, a_DamageType, a_RawDamage);
	int EnchantmentCover = GetEnchantmentCoverAgainst(a_Attacker, a_DamageType, a_RawDamage);
	int FinalDamage = a_RawDamage - ArmorCover - EnchantmentCover;
	if ((FinalDamage == 0) && (a_RawDamage > 0))
	{
		// Nobody's invincible
		FinalDamage = 1;
	}
	ApplyArmorDamage(ArmorCover);

	cEntity::TakeDamage(a_DamageType, a_Attacker, a_RawDamage, FinalDamage, a_KnockbackAmount);
}





void cEntity::TakeDamage(eDamageType a_DamageType, UInt32 a_AttackerID, int a_RawDamage, double a_KnockbackAmount)
{
	class cFindEntity : public cEntityCallback
	{
	public:

		cEntity * m_Entity;
		eDamageType m_DamageType;
		int m_RawDamage;
		double m_KnockbackAmount;

		virtual bool Item(cEntity * a_Attacker) override
		{
			cPawn * Attacker;
			if (a_Attacker->IsPawn())
			{
				Attacker = static_cast<cPawn*>(a_Attacker);
			}
			else
			{
				Attacker = nullptr;
			}


			m_Entity->TakeDamage(m_DamageType, Attacker, m_RawDamage, m_KnockbackAmount);
			return true;
		}
	} Callback;

	Callback.m_Entity = this;
	Callback.m_DamageType = a_DamageType;
	Callback.m_RawDamage = a_RawDamage;
	Callback.m_KnockbackAmount = a_KnockbackAmount;
	m_World->DoWithEntityByID(a_AttackerID, Callback);
}





void cEntity::TakeDamage(eDamageType a_DamageType, cEntity * a_Attacker, int a_RawDamage, int a_FinalDamage, double a_KnockbackAmount)
{
	TakeDamageInfo TDI;
	TDI.DamageType = a_DamageType;
	if ((a_Attacker != nullptr) && a_Attacker->IsPawn())
	{
		TDI.Attacker = a_Attacker;
	}
	else
	{
		TDI.Attacker = nullptr;
	}
	TDI.RawDamage = a_RawDamage;
	TDI.FinalDamage = a_FinalDamage;

	Vector3d Heading(0, 0, 0);
	if (a_Attacker != nullptr)
	{
		Heading = a_Attacker->GetLookVector() * (a_Attacker->IsSprinting() ? 16 : 11);
	}

	TDI.Knockback = Heading * a_KnockbackAmount;
	DoTakeDamage(TDI);
}





void cEntity::SetYawFromSpeed(void)
{
	const double EPS = 0.0000001;
	if ((std::abs(m_Speed.x) < EPS) && (std::abs(m_Speed.z) < EPS))
	{
		// atan2() may overflow or is undefined, pick any number
		SetYaw(0);
		return;
	}
	SetYaw(atan2(m_Speed.x, m_Speed.z) * 180 / M_PI);
}





void cEntity::SetPitchFromSpeed(void)
{
	const double EPS = 0.0000001;
	double xz = sqrt(m_Speed.x * m_Speed.x + m_Speed.z * m_Speed.z);  // Speed XZ-plane component
	if ((std::abs(xz) < EPS) && (std::abs(m_Speed.y) < EPS))
	{
		// atan2() may overflow or is undefined, pick any number
		SetPitch(0);
		return;
	}
	SetPitch(atan2(m_Speed.y, xz) * 180 / M_PI);
}





bool cEntity::DoTakeDamage(TakeDamageInfo & a_TDI)
{
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

	if (cRoot::Get()->GetPluginManager()->CallHookTakeDamage(*this, a_TDI))
	{
		return false;
	}

	if ((a_TDI.Attacker != nullptr) && (a_TDI.Attacker->IsPlayer()))
	{
		cPlayer * Player = reinterpret_cast<cPlayer *>(a_TDI.Attacker);

		Player->GetEquippedItem().GetHandler()->OnEntityAttack(Player, this);

		// IsOnGround() only is false if the player is moving downwards
		// TODO: Better damage increase, and check for enchantments (and use magic critical instead of plain)
		const cEnchantments & Enchantments = Player->GetEquippedItem().m_Enchantments;

		int SharpnessLevel = static_cast<int>(Enchantments.GetLevel(cEnchantments::enchSharpness));
		int SmiteLevel = static_cast<int>(Enchantments.GetLevel(cEnchantments::enchSmite));
		int BaneOfArthropodsLevel = static_cast<int>(Enchantments.GetLevel(cEnchantments::enchBaneOfArthropods));

		if (SharpnessLevel > 0)
		{
			a_TDI.FinalDamage += static_cast<int>(ceil(1.25 * SharpnessLevel));
		}
		else if (SmiteLevel > 0)
		{
			if (IsMob())
			{
				cMonster * Monster = reinterpret_cast<cMonster *>(this);
				switch (Monster->GetMobType())
				{
					case mtSkeleton:
					case mtZombie:
					case mtWither:
					case mtZombiePigman:
					{
						a_TDI.FinalDamage += static_cast<int>(ceil(2.5 * SmiteLevel));
						break;
					}
					default: break;
				}
			}
		}
		else if (BaneOfArthropodsLevel > 0)
		{
			if (IsMob())
			{
				cMonster * Monster = reinterpret_cast<cMonster *>(this);
				switch (Monster->GetMobType())
				{
					case mtSpider:
					case mtCaveSpider:
					case mtSilverfish:
					{
						a_TDI.RawDamage += static_cast<int>(ceil(2.5 * BaneOfArthropodsLevel));
						// TODO: Add slowness effect

						break;
					};
					default: break;
				}
			}
		}

		int FireAspectLevel = static_cast<int>(Enchantments.GetLevel(cEnchantments::enchFireAspect));
		if (FireAspectLevel > 0)
		{
			int BurnTicks = 3;

			if (FireAspectLevel > 1)
			{
				BurnTicks += 4 * (FireAspectLevel - 1);
			}
			if (!IsMob() && !IsSubmerged() && !IsSwimming())
			{
				StartBurning(BurnTicks * 20);
			}
			else if (IsMob() && !IsSubmerged() && !IsSwimming())
			{
				cMonster * Monster = reinterpret_cast<cMonster *>(this);
				switch (Monster->GetMobType())
				{
					case mtGhast:
					case mtZombiePigman:
					case mtMagmaCube:
					{
						break;
					};
					default: StartBurning(BurnTicks * 20);
				}
			}
		}

		unsigned int ThornsLevel = 0;
		const cItem ArmorItems[] = { GetEquippedHelmet(), GetEquippedChestplate(), GetEquippedLeggings(), GetEquippedBoots() };
		for (size_t i = 0; i < ARRAYCOUNT(ArmorItems); i++)
		{
			const cItem & Item = ArmorItems[i];
			ThornsLevel = std::max(ThornsLevel, Item.m_Enchantments.GetLevel(cEnchantments::enchThorns));
		}

		if (ThornsLevel > 0)
		{
			int Chance = static_cast<int>(ThornsLevel * 15);

			auto & Random = GetRandomProvider();

			if (Random.RandBool(Chance / 100.0))
			{
				a_TDI.Attacker->TakeDamage(dtAttack, this, 0, Random.RandInt(1, 4), 0);
			}
		}

		if (!Player->IsOnGround())
		{
			if ((a_TDI.DamageType == dtAttack) || (a_TDI.DamageType == dtArrowAttack))
			{
				a_TDI.FinalDamage += 2;
				m_World->BroadcastEntityAnimation(*this, 4);  // Critical hit
			}
		}

		Player->GetStatManager().AddValue(statDamageDealt, static_cast<StatValue>(floor(a_TDI.FinalDamage * 10 + 0.5)));
	}

	m_Health -= static_cast<short>(a_TDI.FinalDamage);

	m_Health = std::max(m_Health, 0);

	// Add knockback:
	if ((IsMob() || IsPlayer()) && (a_TDI.Attacker != nullptr))
	{
		int KnockbackLevel = static_cast<int>(a_TDI.Attacker->GetEquippedWeapon().m_Enchantments.GetLevel(cEnchantments::enchKnockback));  // More common enchantment
		if (KnockbackLevel < 1)
		{
			// We support punch on swords and vice versa! :)
			KnockbackLevel = static_cast<int>(a_TDI.Attacker->GetEquippedWeapon().m_Enchantments.GetLevel(cEnchantments::enchPunch));
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

		if (a_TDI.Attacker != nullptr)
		{
			a_TDI.Attacker->Killed(this);
		}
	}
	return true;
}





int cEntity::GetRawDamageAgainst(const cEntity & a_Receiver)
{
	// Returns the hitpoints that this pawn can deal to a_Receiver using its equipped items
	// Ref: http://minecraft.gamepedia.com/Damage#Dealing_damage as of 2012_12_20
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




void cEntity::ApplyArmorDamage(int DamageBlocked)
{
	// cEntities don't necessarily have armor to damage.
	return;
}





bool cEntity::ArmorCoversAgainst(eDamageType a_DamageType)
{
	// Ref.: http://minecraft.gamepedia.com/Armor#Effects as of 2012_12_20
	switch (a_DamageType)
	{
		case dtOnFire:
		case dtSuffocating:
		case dtDrowning:  // TODO: This one could be a special case - in various MC versions (PC vs XBox) it is and isn't armor-protected
		case dtEnderPearl:
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
		case dtExplosion:
		{
			return true;
		}
	}
	ASSERT(!"Invalid damage type!");
	#ifndef __clang__
		return false;
	#endif
}





int cEntity::GetEnchantmentCoverAgainst(const cEntity * a_Attacker, eDamageType a_DamageType, int a_Damage)
{
	int TotalEPF = 0.0;

	const cItem ArmorItems[] = { GetEquippedHelmet(), GetEquippedChestplate(), GetEquippedLeggings(), GetEquippedBoots() };
	for (size_t i = 0; i < ARRAYCOUNT(ArmorItems); i++)
	{
		const cItem & Item = ArmorItems[i];

		if ((a_DamageType != dtInVoid) && (a_DamageType != dtAdmin) && (a_DamageType != dtStarving))
		{
			TotalEPF += static_cast<int>(Item.m_Enchantments.GetLevel(cEnchantments::enchProtection)) * 1;
		}

		if ((a_DamageType == dtBurning) || (a_DamageType == dtFireContact) || (a_DamageType == dtLavaContact))
		{
			TotalEPF += static_cast<int>(Item.m_Enchantments.GetLevel(cEnchantments::enchFireProtection)) * 2;
		}

		if ((a_DamageType == dtFalling) || (a_DamageType == dtEnderPearl))
		{
			TotalEPF += static_cast<int>(Item.m_Enchantments.GetLevel(cEnchantments::enchFeatherFalling)) * 3;
		}

		if (a_DamageType == dtExplosion)
		{
			TotalEPF += static_cast<int>(Item.m_Enchantments.GetLevel(cEnchantments::enchBlastProtection)) * 2;
		}

		// Note: Also blocks against fire charges, etc.
		if (a_DamageType == dtProjectile)
		{
			TotalEPF += static_cast<int>(Item.m_Enchantments.GetLevel(cEnchantments::enchProjectileProtection)) * 2;
		}
	}
	int CappedEPF = std::min(20, TotalEPF);
	return static_cast<int>(a_Damage * CappedEPF / 25.0);
}





int cEntity::GetArmorCoverAgainst(const cEntity * a_Attacker, eDamageType a_DamageType, int a_Damage)
{
	// Returns the hitpoints out of a_RawDamage that the currently equipped armor would cover

	// Filter out damage types that are not protected by armor:
	if (!ArmorCoversAgainst(a_DamageType))
	{
		return 0;
	}

	// Add up all armor points:
	// Ref.: http://minecraft.gamepedia.com/Armor#Defense_points
	int ArmorValue = 0;
	int Toughness = 0;
	switch (GetEquippedHelmet().m_ItemType)
	{
		case E_ITEM_LEATHER_CAP:    ArmorValue += 1; break;
		case E_ITEM_GOLD_HELMET:    ArmorValue += 2; break;
		case E_ITEM_CHAIN_HELMET:   ArmorValue += 2; break;
		case E_ITEM_IRON_HELMET:    ArmorValue += 2; break;
		case E_ITEM_DIAMOND_HELMET: ArmorValue += 3; Toughness += 2; break;
	}
	switch (GetEquippedChestplate().m_ItemType)
	{
		case E_ITEM_LEATHER_TUNIC:      ArmorValue += 3; break;
		case E_ITEM_GOLD_CHESTPLATE:    ArmorValue += 5; break;
		case E_ITEM_CHAIN_CHESTPLATE:   ArmorValue += 5; break;
		case E_ITEM_IRON_CHESTPLATE:    ArmorValue += 6; break;
		case E_ITEM_DIAMOND_CHESTPLATE: ArmorValue += 8; Toughness += 2; break;
	}
	switch (GetEquippedLeggings().m_ItemType)
	{
		case E_ITEM_LEATHER_PANTS:    ArmorValue += 2; break;
		case E_ITEM_GOLD_LEGGINGS:    ArmorValue += 3; break;
		case E_ITEM_CHAIN_LEGGINGS:   ArmorValue += 4; break;
		case E_ITEM_IRON_LEGGINGS:    ArmorValue += 5; break;
		case E_ITEM_DIAMOND_LEGGINGS: ArmorValue += 6; Toughness += 2; break;
	}
	switch (GetEquippedBoots().m_ItemType)
	{
		case E_ITEM_LEATHER_BOOTS: ArmorValue += 1; break;
		case E_ITEM_GOLD_BOOTS:    ArmorValue += 1; break;
		case E_ITEM_CHAIN_BOOTS:   ArmorValue += 1; break;
		case E_ITEM_IRON_BOOTS:    ArmorValue += 2; break;
		case E_ITEM_DIAMOND_BOOTS: ArmorValue += 3; Toughness += 2; break;
	}

	// TODO: Special armor cases, such as wool, saddles, dog's collar
	// Ref.: http://minecraft.gamepedia.com/Armor#Mob_armor as of 2012_12_20

	double Reduction = std::max(ArmorValue / 5.0, ArmorValue - a_Damage / (2 + Toughness / 4.0));
	return static_cast<int>(a_Damage * std::min(20.0, Reduction) / 25.0);
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

	// If the victim is a player the hook is handled by the cPlayer class
	if (!IsPlayer())
	{
		AString emptystring = AString("");
		cRoot::Get()->GetPluginManager()->CallHookKilled(*this, a_TDI, emptystring);
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
	m_Health = std::min(m_Health, m_MaxHealth);
}





void cEntity::SetHealth(int a_Health)
{
	m_Health = Clamp(a_Health, 0, m_MaxHealth);
}





void cEntity::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	ASSERT(IsTicking());
	ASSERT(GetWorld() != nullptr);
	m_TicksAlive++;

	if (m_InvulnerableTicks > 0)
	{
		m_InvulnerableTicks--;
	}

	if ((GetPosY() < 0) && (!IsPlayer()))
	{
		Destroy();
		return;
	}

	if (m_AttachedTo != nullptr)
	{
		SetPosition(m_AttachedTo->GetPosition());
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

		if (
			IsMob() || IsPickup() || IsExpOrb() ||
			(IsPlayer() && !((reinterpret_cast<cPlayer *>(this))->IsGameModeCreative() || (reinterpret_cast<cPlayer *>(this))->IsGameModeSpectator()))
		)
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

		if (!DetectPortal())  // Our chunk is invalid if we have moved to another world
		{
			// None of the above functions changed position, we remain in the chunk of NextChunk
			HandlePhysics(a_Dt, *NextChunk);
		}
	}
}





void cEntity::HandlePhysics(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	int BlockX = POSX_TOINT;
	int BlockY = POSY_TOINT;
	int BlockZ = POSZ_TOINT;

	// Position changed -> super::HandlePhysics() called
	GET_AND_VERIFY_CURRENT_CHUNK(NextChunk, BlockX, BlockZ)

	// TODO Add collision detection with entities.
	auto DtSec = std::chrono::duration_cast<std::chrono::duration<double>>(a_Dt);
	Vector3d NextPos = Vector3d(GetPosX(), GetPosY(), GetPosZ());
	Vector3d NextSpeed = Vector3d(GetSpeedX(), GetSpeedY(), GetSpeedZ());

	if ((BlockY >= cChunkDef::Height) || (BlockY < 0))
	{
		// Outside of the world
		AddSpeedY(m_Gravity * DtSec.count());
		AddPosition(GetSpeed() * DtSec.count());
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

		m_bHasSentNoSpeed = false;  // this unlocks movement sending to client in BroadcastMovementUpdate function
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
		double fallspeed;
		if (IsBlockWater(BlockIn))
		{
			fallspeed = m_Gravity * DtSec.count() / 3;  // Fall 3x slower in water
			ApplyFriction(NextSpeed, 0.7, static_cast<float>(DtSec.count()));
		}
		else if (BlockIn == E_BLOCK_COBWEB)
		{
			NextSpeed.y *= 0.05;  // Reduce overall falling speed
			fallspeed = 0;  // No falling
		}
		else
		{
			// Normal gravity
			fallspeed = m_Gravity * DtSec.count();
			NextSpeed -= NextSpeed * (m_AirDrag * 20.0f) * DtSec.count();
		}
		NextSpeed.y += static_cast<float>(fallspeed);

		// A real boat floats
		if (IsBoat())
		{
			// Find top water block and sit there
			int NextBlockY = BlockY;
			BLOCKTYPE NextBlock = NextChunk->GetBlock(RelBlockX, NextBlockY, RelBlockZ);
			while (IsBlockWater(NextBlock))
			{
				NextBlock = NextChunk->GetBlock(RelBlockX, ++NextBlockY, RelBlockZ);
			}
			NextPos.y = NextBlockY - 0.5;
			NextSpeed.y = 0;
		}
	}
	else
	{
		ApplyFriction(NextSpeed, 0.7, static_cast<float>(DtSec.count()));
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

	m_WaterSpeed *= 0.9;  // Reduce speed each tick

	switch (WaterDir)
	{
		case X_PLUS:
		{
			m_WaterSpeed.x = 0.2f;
			m_bOnGround = false;
			break;
		}
		case X_MINUS:
		{
			m_WaterSpeed.x = -0.2f;
			m_bOnGround = false;
			break;
		}
		case Z_PLUS:
		{
			m_WaterSpeed.z = 0.2f;
			m_bOnGround = false;
			break;
		}
		case Z_MINUS:
		{
			m_WaterSpeed.z = -0.2f;
			m_bOnGround = false;
			break;
		}
		default:
		{
			break;
		}
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

	if (NextSpeed.SqrLength() > 0.0f)
	{
		Vector3d HitCoords;
		Vector3i HitBlockCoords;
		eBlockFace HitBlockFace;
		Vector3d wantNextPos = NextPos + NextSpeed * DtSec.count();
		auto isHit = cLineBlockTracer::FirstSolidHitTrace(*GetWorld(), NextPos, wantNextPos, HitCoords, HitBlockCoords, HitBlockFace);
		if (isHit)
		{
			// Set our position to where the block was hit, minus a bit:
			// TODO: The real entity's m_Width should be taken into account here
			NextPos = HitCoords - NextSpeed.NormalizeCopy() * 0.1;
			if (HitBlockFace == BLOCK_FACE_YP)
			{
				// We hit the ground, adjust the position to the top of the block:
				m_bOnGround = true;
				NextPos.y = HitBlockCoords.y + 1;
			}

			// Avoid movement in the direction of the blockface that has been hit:
			switch (HitBlockFace)
			{
				case BLOCK_FACE_XM:
				case BLOCK_FACE_XP:
				{
					NextSpeed.x = 0;
					break;
				}
				case BLOCK_FACE_YM:
				case BLOCK_FACE_YP:
				{
					NextSpeed.y = 0;
					break;
				}
				case BLOCK_FACE_ZM:
				case BLOCK_FACE_ZP:
				{
					NextSpeed.z = 0;
					break;
				}
				default:
				{
					break;
				}
			}
		}
		else
		{
			// We didn't hit anything, so move:
			NextPos += (NextSpeed * DtSec.count());
		}
	}

	SetPosition(NextPos);
	SetSpeed(NextSpeed);
}





void cEntity::ApplyFriction(Vector3d & a_Speed, double a_SlowdownMultiplier, float a_Dt)
{
	if (a_Speed.SqrLength() > 0.0004f)
	{
		a_Speed.x *= a_SlowdownMultiplier / (1 + a_Dt);
		if (fabs(a_Speed.x) < 0.05)
		{
			a_Speed.x = 0;
		}
		a_Speed.z *= a_SlowdownMultiplier / (1 + a_Dt);
		if (fabs(a_Speed.z) < 0.05)
		{
			a_Speed.z = 0;
		}
	}
}





void cEntity::TickBurning(cChunk & a_Chunk)
{
	// Remember the current burning state:
	bool HasBeenBurning = (m_TicksLeftBurning > 0);

	if (GetWorld()->IsWeatherWetAt(POSX_TOINT, POSZ_TOINT))
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
			if (!IsFireproof())
			{
				TakeDamage(dtOnFire, nullptr, BURN_DAMAGE, 0);
			}
			m_TicksSinceLastBurnDamage = 0;
		}
		m_TicksLeftBurning--;
	}

	// Update the burning times, based on surroundings:
	int MinRelX = FloorC(GetPosX() - m_Width / 2) - a_Chunk.GetPosX() * cChunkDef::Width;
	int MaxRelX = FloorC(GetPosX() + m_Width / 2) - a_Chunk.GetPosX() * cChunkDef::Width;
	int MinRelZ = FloorC(GetPosZ() - m_Width / 2) - a_Chunk.GetPosZ() * cChunkDef::Width;
	int MaxRelZ = FloorC(GetPosZ() + m_Width / 2) - a_Chunk.GetPosZ() * cChunkDef::Width;
	int MinY = Clamp(POSY_TOINT, 0, cChunkDef::Height - 1);
	int MaxY = Clamp(FloorC(GetPosY() + m_Height), 0, cChunkDef::Height - 1);
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
			if (!IsFireproof())
			{
				TakeDamage(dtLavaContact, nullptr, LAVA_DAMAGE, 0);
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
			if (!IsFireproof())
			{
				TakeDamage(dtFireContact, nullptr, FIRE_DAMAGE, 0);
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
		TakeDamage(dtInVoid, nullptr, 2, 0);
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
		(((Y + 1) - GetPosY() < w) && (GetWorld()->GetBlock(X, Y + 1, Z) == E_BLOCK_CACTUS)) ||
		((GetPosY() - Y < 1) && (GetWorld()->GetBlock(X, Y - 1, Z) == E_BLOCK_CACTUS)))
		)
	{
		TakeDamage(dtCactusContact, nullptr, 1, 0);
	}
}




void cEntity::ScheduleMoveToWorld(cWorld * a_World, Vector3d a_NewPosition, bool a_SetPortalCooldown)
{
	m_NewWorld = a_World;
	m_NewWorldPosition = a_NewPosition;
	m_IsWorldChangeScheduled = true;
	m_WorldChangeSetPortalCooldown = a_SetPortalCooldown;
}




bool cEntity::DetectPortal()
{
	// If somebody scheduled a world change with ScheduleMoveToWorld, change worlds now.
	if (m_IsWorldChangeScheduled)
	{
		m_IsWorldChangeScheduled = false;

		if (m_WorldChangeSetPortalCooldown)
		{
			// Delay the portal check.
			m_PortalCooldownData.m_TicksDelayed = 0;
			m_PortalCooldownData.m_ShouldPreventTeleportation = true;
		}

		MoveToWorld(m_NewWorld, false, m_NewWorldPosition);
		return true;
	}

	if (GetWorld()->GetDimension() == dimOverworld)
	{
		if (GetWorld()->GetLinkedNetherWorldName().empty() && GetWorld()->GetLinkedEndWorldName().empty())
		{
			// Teleportation to either dimension not enabled, don't bother proceeding
			return false;
		}
	}
	else if (GetWorld()->GetLinkedOverworldName().empty())
	{
		// Overworld teleportation disabled, abort
		return false;
	}

	int X = POSX_TOINT, Y = POSY_TOINT, Z = POSZ_TOINT;
	if ((Y > 0) && (Y < cChunkDef::Height))
	{
		switch (GetWorld()->GetBlock(X, Y, Z))
		{
			case E_BLOCK_NETHER_PORTAL:
			{
				if (m_PortalCooldownData.m_ShouldPreventTeleportation)
				{
					// Just exited a portal, don't teleport again
					return false;
				}

				if (IsPlayer() && !(reinterpret_cast<cPlayer *>(this))->IsGameModeCreative() && (m_PortalCooldownData.m_TicksDelayed != 80))
				{
					// Delay teleportation for four seconds if the entity is a non-creative player
					m_PortalCooldownData.m_TicksDelayed++;
					return false;
				}
				m_PortalCooldownData.m_TicksDelayed = 0;

				// Nether portal in the nether
				if (GetWorld()->GetDimension() == dimNether)
				{
					if (GetWorld()->GetLinkedOverworldName().empty())
					{
						return false;
					}
					cWorld * DestinationWorld = cRoot::Get()->GetWorld(GetWorld()->GetLinkedOverworldName());
					eDimension DestionationDim = DestinationWorld->GetDimension();

					m_PortalCooldownData.m_ShouldPreventTeleportation = true;  // Stop portals from working on respawn

					if (IsPlayer())
					{
						// Send a respawn packet before world is loaded / generated so the client isn't left in limbo
						(reinterpret_cast<cPlayer *>(this))->GetClientHandle()->SendRespawn(DestionationDim);
					}

					Vector3d TargetPos = GetPosition();
					TargetPos.x *= 8.0;
					TargetPos.z *= 8.0;

					cWorld * TargetWorld = cRoot::Get()->GetWorld(GetWorld()->GetLinkedOverworldName());
					ASSERT(TargetWorld != nullptr);  // The linkage checker should have prevented this at startup. See cWorld::start()
					LOGD("Jumping %s -> %s", DimensionToString(dimNether).c_str(), DimensionToString(DestionationDim).c_str());
					new cNetherPortalScanner(this, TargetWorld, TargetPos, cChunkDef::Height);
					return true;
				}
				// Nether portal in the overworld
				else
				{
					if (GetWorld()->GetLinkedNetherWorldName().empty())
					{
						return false;
					}
					cWorld * DestinationWorld = cRoot::Get()->GetWorld(GetWorld()->GetLinkedNetherWorldName());
					eDimension DestionationDim = DestinationWorld->GetDimension();

					m_PortalCooldownData.m_ShouldPreventTeleportation = true;

					if (IsPlayer())
					{
						if (DestionationDim == dimNether)
						{
							reinterpret_cast<cPlayer *>(this)->AwardAchievement(achEnterPortal);
						}

						reinterpret_cast<cPlayer *>(this)->GetClientHandle()->SendRespawn(DestionationDim);
					}

					Vector3d TargetPos = GetPosition();
					TargetPos.x /= 8.0;
					TargetPos.z /= 8.0;

					cWorld * TargetWorld = cRoot::Get()->GetWorld(GetWorld()->GetLinkedNetherWorldName());
					ASSERT(TargetWorld != nullptr);  // The linkage checker should have prevented this at startup. See cWorld::start()
					LOGD("Jumping %s -> %s", DimensionToString(dimOverworld).c_str(), DimensionToString(DestionationDim).c_str());
					new cNetherPortalScanner(this, TargetWorld, TargetPos, (cChunkDef::Height / 2));
					return true;
				}
			}
			case E_BLOCK_END_PORTAL:
			{
				if (m_PortalCooldownData.m_ShouldPreventTeleportation)
				{
					return false;
				}

				// End portal in the end
				if (GetWorld()->GetDimension() == dimEnd)
				{

					if (GetWorld()->GetLinkedOverworldName().empty())
					{
						return false;
					}
					cWorld * DestinationWorld = cRoot::Get()->GetWorld(GetWorld()->GetLinkedOverworldName());
					eDimension DestionationDim = DestinationWorld->GetDimension();


					m_PortalCooldownData.m_ShouldPreventTeleportation = true;

					if (IsPlayer())
					{
						cPlayer * Player = reinterpret_cast<cPlayer *>(this);
						if (Player->GetBedWorld() == DestinationWorld)
						{
							Player->TeleportToCoords(Player->GetLastBedPos().x, Player->GetLastBedPos().y, Player->GetLastBedPos().z);
						}
						else
						{
							Player->TeleportToCoords(DestinationWorld->GetSpawnX(), DestinationWorld->GetSpawnY(), DestinationWorld->GetSpawnZ());
						}
						Player->GetClientHandle()->SendRespawn(DestionationDim);
					}

					cWorld * TargetWorld = cRoot::Get()->GetWorld(GetWorld()->GetLinkedOverworldName());
					ASSERT(TargetWorld != nullptr);  // The linkage checker should have prevented this at startup. See cWorld::start()
					LOGD("Jumping %s -> %s", DimensionToString(dimEnd).c_str(), DimensionToString(DestionationDim).c_str());
					return MoveToWorld(TargetWorld, false);
				}
				// End portal in the overworld
				else
				{
					if (GetWorld()->GetLinkedEndWorldName().empty())
					{
						return false;
					}
					cWorld * DestinationWorld = cRoot::Get()->GetWorld(GetWorld()->GetLinkedEndWorldName());
					eDimension DestionationDim = DestinationWorld->GetDimension();

					m_PortalCooldownData.m_ShouldPreventTeleportation = true;

					if (IsPlayer())
					{
						if (DestionationDim == dimEnd)
						{
							reinterpret_cast<cPlayer *>(this)->AwardAchievement(achEnterTheEnd);
						}
						reinterpret_cast<cPlayer *>(this)->GetClientHandle()->SendRespawn(DestionationDim);
					}

					cWorld * TargetWorld = cRoot::Get()->GetWorld(GetWorld()->GetLinkedEndWorldName());
					ASSERT(TargetWorld != nullptr);  // The linkage checker should have prevented this at startup. See cWorld::start()
					LOGD("Jumping %s -> %s", DimensionToString(dimOverworld).c_str(), DimensionToString(DestionationDim).c_str());
					return MoveToWorld(TargetWorld, false);
				}

			}
			default: break;
		}
	}

	// Allow portals to work again
	m_PortalCooldownData.m_ShouldPreventTeleportation = false;
	m_PortalCooldownData.m_TicksDelayed = 0;
	return false;
}





bool cEntity::DoMoveToWorld(cWorld * a_World, bool a_ShouldSendRespawn, Vector3d a_NewPosition)
{
	UNUSED(a_ShouldSendRespawn);
	ASSERT(a_World != nullptr);
	ASSERT(IsTicking());

	if (GetWorld() == a_World)
	{
		// Don't move to same world
		return false;
	}

	//  Ask the plugins if the entity is allowed to changing the world
	if (cRoot::Get()->GetPluginManager()->CallHookEntityChangingWorld(*this, *a_World))
	{
		// A Plugin doesn't allow the entity to changing the world
		return false;
	}

	// Stop ticking, in preperation for detaching from this world.
	SetIsTicking(false);

	// Tell others we are gone
	GetWorld()->BroadcastDestroyEntity(*this);

	// Set position to the new position
	SetPosition(a_NewPosition);

	// Stop all mobs from targeting this entity
	// Stop this entity from targeting other mobs
	if (this->IsMob())
	{
		cMonster * Monster = static_cast<cMonster*>(this);
		Monster->SetTarget(nullptr);
		Monster->StopEveryoneFromTargetingMe();
	}

	// Queue add to new world and removal from the old one
	cWorld * OldWorld = GetWorld();
	cChunk * ParentChunk = GetParentChunk();
	SetWorld(a_World);  // Chunks may be streamed before cWorld::AddPlayer() sets the world to the new value
	OldWorld->QueueTask([this, ParentChunk, a_World](cWorld & a_OldWorld)
	{
		LOGD("Warping entity #%i (%s) from world \"%s\" to \"%s\". Source chunk: (%d, %d) ",
			this->GetUniqueID(), this->GetClass(),
			a_OldWorld.GetName().c_str(), a_World->GetName().c_str(),
			ParentChunk->GetPosX(), ParentChunk->GetPosZ()
		);
		a_World->AddEntity(ParentChunk->RemoveEntity(*this));
		cRoot::Get()->GetPluginManager()->CallHookEntityChangedWorld(*this, a_OldWorld);
	});
	return true;
}





bool cEntity::MoveToWorld(cWorld * a_World, bool a_ShouldSendRespawn, Vector3d a_NewPosition)
{
	return DoMoveToWorld(a_World, a_ShouldSendRespawn, a_NewPosition);
}





bool cEntity::MoveToWorld(cWorld * a_World, bool a_ShouldSendRespawn)
{
	return MoveToWorld(a_World, a_ShouldSendRespawn, Vector3d(a_World->GetSpawnX(), a_World->GetSpawnY(), a_World->GetSpawnZ()));
}





bool cEntity::MoveToWorld(const AString & a_WorldName, bool a_ShouldSendRespawn)
{
	cWorld * World = cRoot::Get()->GetWorld(a_WorldName);
	if (World == nullptr)
	{
		LOG("%s: Couldn't find world \"%s\".", __FUNCTION__, a_WorldName.c_str());
		return false;
	}

	return DoMoveToWorld(World, a_ShouldSendRespawn, Vector3d(World->GetSpawnX(), World->GetSpawnY(), World->GetSpawnZ()));
}





void cEntity::SetSwimState(cChunk & a_Chunk)
{
	int RelY = FloorC(GetPosY() + 0.1);
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
		// Ref.: https://forum.cuberite.org/thread-1244.html
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





void cEntity::SetIsTicking(bool a_IsTicking)
{
	m_IsTicking = a_IsTicking;
	ASSERT(!(m_IsTicking && (m_ParentChunk == nullptr)));  // We shouldn't be ticking if we have no parent chunk
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

	int RespirationLevel = static_cast<int>(GetEquippedHelmet().m_Enchantments.GetLevel(cEnchantments::enchRespiration));

	if (IsSubmerged())
	{
		if (!IsPlayer())  // Players control themselves
		{
			SetSpeedY(1);  // Float in the water
		}

		if (RespirationLevel > 0)
		{
			reinterpret_cast<cPawn *>(this)->AddEntityEffect(cEntityEffect::effNightVision, 200, 5, 0);
		}

		if (m_AirLevel <= 0)
		{
			// Runs the air tick timer to check whether the player should be damaged
			if (m_AirTickTimer <= 0)
			{
				// Damage player
				TakeDamage(dtDrowning, nullptr, 1, 1, 0);
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

		if (RespirationLevel > 0)
		{
			m_AirTickTimer = DROWNING_TICKS + (RespirationLevel * 15 * 20);
		}

	}
}





void cEntity::OnStartedBurning(void)
{
	// Broadcast the change:
	m_World->BroadcastEntityMetadata(*this);
}





void cEntity::OnFinishedBurning(void)
{
	// Broadcast the change:
	m_World->BroadcastEntityMetadata(*this);
}





void cEntity::SetMaxHealth(int a_MaxHealth)
{
	m_MaxHealth = a_MaxHealth;

	// Reset health, if too high:
	m_Health = std::min(m_Health, a_MaxHealth);
}





void cEntity::SetIsFireproof(bool a_IsFireproof)
{
	m_IsFireproof = a_IsFireproof;
}





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
	//  ask the plugins to allow teleport to the new position.
	if (!cRoot::Get()->GetPluginManager()->CallHookEntityTeleport(*this, m_LastPosition, Vector3d(a_PosX, a_PosY, a_PosZ)))
	{
		SetPosition(a_PosX, a_PosY, a_PosZ);
		m_World->BroadcastTeleportEntity(*this);
	}
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

		// Only send movement if speed is not 0 and 'no speed' was sent to client
		if (!m_bHasSentNoSpeed || IsPlayer())
		{
			// TODO: Pickups move disgracefully if relative move packets are sent as opposed to just velocity. Have a system to send relmove only when SetPosXXX() is called with a large difference in position
			int DiffX = FloorC(GetPosX() * 32.0) - FloorC(m_LastSentPosition.x * 32.0);
			int DiffY = FloorC(GetPosY() * 32.0) - FloorC(m_LastSentPosition.y * 32.0);
			int DiffZ = FloorC(GetPosZ() * 32.0) - FloorC(m_LastSentPosition.z * 32.0);

			if ((DiffX != 0) || (DiffY != 0) || (DiffZ != 0))  // Have we moved?
			{
				if ((abs(DiffX) <= 127) && (abs(DiffY) <= 127) && (abs(DiffZ) <= 127))  // Limitations of a Byte
				{
					// Difference within Byte limitations, use a relative move packet
					if (m_bDirtyOrientation)
					{
						m_World->BroadcastEntityRelMoveLook(*this, static_cast<char>(DiffX), static_cast<char>(DiffY), static_cast<char>(DiffZ), a_Exclude);
						m_bDirtyOrientation = false;
					}
					else
					{
						m_World->BroadcastEntityRelMove(*this, static_cast<char>(DiffX), static_cast<char>(DiffY), static_cast<char>(DiffZ), a_Exclude);
					}
					// Clients seem to store two positions, one for the velocity packet and one for the teleport / relmove packet
					// The latter is only changed with a relmove / teleport, and m_LastSentPosition stores this position
					m_LastSentPosition = GetPosition();
				}
				else
				{
					// Too big a movement, do a teleport
					m_World->BroadcastTeleportEntity(*this, a_Exclude);
					m_LastSentPosition = GetPosition();  // See above
					m_bDirtyOrientation = false;
				}
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




cEntity * cEntity::GetAttached()
{
	return m_AttachedTo;
}





void cEntity::AttachTo(cEntity * a_AttachTo)
{
	if (m_AttachedTo == a_AttachTo)
	{
		// Already attached to that entity, nothing to do here
		return;
	}
	if (m_AttachedTo != nullptr)
	{
		// Detach from any previous entity:
		Detach();
	}

	// Update state information
	m_AttachedTo = a_AttachTo;
	a_AttachTo->m_Attachee = this;
	if (a_AttachTo != nullptr)
	{
		m_World->BroadcastAttachEntity(*this, *a_AttachTo);
	}
}





void cEntity::Detach(void)
{
	if (m_AttachedTo == nullptr)
	{
		// Already not attached to any entity, our work is done
		return;
	}
	m_World->BroadcastDetachEntity(*this, *m_AttachedTo);

	m_AttachedTo->m_Attachee = nullptr;
	m_AttachedTo = nullptr;
}





bool cEntity::IsA(const char * a_ClassName) const
{
	return ((a_ClassName != nullptr) && (strcmp(a_ClassName, "cEntity") == 0));
}





bool cEntity::IsAttachedTo(const cEntity * a_Entity) const
{
	if ((m_AttachedTo != nullptr) && (a_Entity->GetUniqueID() == m_AttachedTo->GetUniqueID()))
	{
		return true;
	}
	return false;
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
	// Make sure that mass is not zero. 1g is the default because we
	// have to choose a number. It's perfectly legal to have a mass
	// less than 1g as long as is NOT equal or less than zero.
	m_Mass = std::max(a_Mass, 0.001);
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




void cEntity::AddSpeed(double a_AddSpeedX, double a_AddSpeedY, double a_AddSpeedZ)
{
	DoSetSpeed(m_Speed.x + a_AddSpeedX, m_Speed.y + a_AddSpeedY, m_Speed.z + a_AddSpeedZ);
}





void cEntity::AddSpeedX(double a_AddSpeedX)
{
	AddSpeed(a_AddSpeedX, 0, 0);
}





void cEntity::AddSpeedY(double a_AddSpeedY)
{
	AddSpeed(0, a_AddSpeedY, 0);
}





void cEntity::AddSpeedZ(double a_AddSpeedZ)
{
	AddSpeed(0, 0, a_AddSpeedZ);
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
	if (m_AttachedTo == nullptr)
	{
		return;
	}
	if ((a_Forward != 0.0f) || (a_Sideways != 0.0f))
	{
		m_AttachedTo->HandleSpeedFromAttachee(a_Forward, a_Sideways);
	}
}





bool cEntity::IsTicking(void) const
{
	ASSERT(!(m_IsTicking && (m_ParentChunk == nullptr)));  // We shouldn't be ticking if we have no parent chunk
	return m_IsTicking;
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
void cEntity::SetPosition(const Vector3d & a_Position)
{
	m_LastPosition = m_Position;
	m_Position = a_Position;
}




