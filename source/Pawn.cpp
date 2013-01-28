
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Pawn.h"
#include "Root.h"
#include "Server.h"
#include "World.h"
#include "Player.h"
#include "PluginManager.h"
#include "Vector3d.h"
#include "BlockID.h"
#include "Defines.h"





cPawn::cPawn(eEntityType a_EntityType)
	: cEntity(a_EntityType, 0, 0, 0)
	, m_Health(1)
	, m_MaxHealth(1)
	, m_LastPosX( 0.0 )
	, m_LastPosY( 0.0 )
	, m_LastPosZ( 0.0 )
	, m_TimeLastTeleportPacket(0)
	, m_bBurnable(true)
	, m_MetaData(NORMAL)
{
}





void cPawn::Heal(int a_HitPoints)
{
	m_Health += a_HitPoints;
	if (m_Health > m_MaxHealth)
	{
		m_Health = m_MaxHealth;
	}
}





void cPawn::TakeDamage(cPawn & a_Attacker)
{
	int RawDamage = a_Attacker.GetRawDamageAgainst(*this);
	
	TakeDamage(dtAttack, &a_Attacker, RawDamage, a_Attacker.GetKnockbackAmountAgainst(*this));
}





void cPawn::TakeDamage(eDamageType a_DamageType, cPawn * a_Attacker, int a_RawDamage, double a_KnockbackAmount)
{
	int FinalDamage = a_RawDamage - GetArmorCoverAgainst(a_Attacker, a_DamageType, a_RawDamage);
	TakeDamage(a_DamageType, a_Attacker, a_RawDamage, FinalDamage, a_KnockbackAmount);
}





void cPawn::TakeDamage(eDamageType a_DamageType, cPawn * a_Attacker, int a_RawDamage, int a_FinalDamage, double a_KnockbackAmount)
{
	TakeDamageInfo TDI;
	TDI.DamageType = a_DamageType;
	TDI.Attacker = a_Attacker;
	TDI.RawDamage = a_RawDamage;
	TDI.FinalDamage = a_FinalDamage;
	Vector3d Heading;
	Heading.x = sin(m_Rot.x);
	Heading.y = 0.4;  // TODO: adjust the amount of "up" knockback when testing
	Heading.z = cos(m_Rot.x);
	TDI.Knockback = Heading * a_KnockbackAmount;
	DoTakeDamage(TDI);
}





void cPawn::DoTakeDamage(TakeDamageInfo & a_TDI)
{
	if (cRoot::Get()->GetPluginManager()->CallHookTakeDamage(*this, a_TDI))
	{
		return;
	}

	if (m_Health <= 0)
	{
		// Can't take damage if already dead
		return;
	}

	m_Health -= (short)a_TDI.FinalDamage;
	
	// TODO: Apply damage to armor
	
	if (m_Health < 0)
	{
		m_Health = 0;
	}

	m_World->BroadcastEntityStatus(*this, ENTITY_STATUS_HURT);

	if (m_Health <= 0)
	{
		KilledBy(a_TDI.Attacker);
	}
}





void cPawn::KilledBy(cPawn * a_Killer)
{
	m_Health = 0;

	cRoot::Get()->GetPluginManager()->CallHookKilling(*this, a_Killer);
	
	if (m_Health > 0)
	{
		// Plugin wants to 'unkill' the pawn. Abort
		return;
	}

	// Drop loot:	
	cItems Drops;
	GetDrops(Drops, a_Killer);
	m_World->SpawnItemPickups(Drops, m_Pos.x, m_Pos.y, m_Pos.z);

	m_World->BroadcastEntityStatus(*this, ENTITY_STATUS_DEAD);
}





int cPawn::GetRawDamageAgainst(const cPawn & a_Receiver)
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





int cPawn::GetArmorCoverAgainst(const cPawn * a_Attacker, eDamageType a_DamageType, int a_Damage)
{
	// Returns the hitpoints out of a_RawDamage that the currently equipped armor would cover
	
	// Filter out damage types that are not protected by armor:
	// Ref.: http://www.minecraftwiki.net/wiki/Armor#Effects as of 2012_12_20
	switch (a_DamageType)
	{
		case dtOnFire:
		case dtSuffocating:
		case dtDrowning:  // TODO: This one could be a special case - in various MC versions (PC vs XBox) it is and isn't armor-protected
		case dtStarving:
		case dtInVoid:
		case dtPoisoning:
		case dtPotionOfHarming:
		case dtFalling:
		case dtLightning:
		{
			return 0;
		}
	}
	
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





double cPawn::GetKnockbackAmountAgainst(const cPawn & a_Receiver)
{
	// Returns the knockback amount that the currently equipped items would cause to a_Receiver on a hit
	
	// TODO: Enchantments
	return 1;
}





void cPawn::GetDrops(cItems & a_Drops, cPawn * a_Killer)
{
	UNUSED(a_Drops);
	UNUSED(a_Killer);
}





void cPawn::TeleportToEntity(cEntity & a_Entity)
{
	TeleportTo(a_Entity.GetPosX(), a_Entity.GetPosY(), a_Entity.GetPosZ());
}





void cPawn::TeleportTo(double a_PosX, double a_PosY, double a_PosZ)
{
	SetPosition(a_PosX, a_PosY, a_PosZ);

	GetWorld()->BroadcastTeleportEntity(*this);
}





void cPawn::Tick(float a_Dt, MTRand & a_TickRandom)
{
	CheckMetaDataBurn();  // Check to see if pawn should burn based on block they are on
	
	if (GetMetaData() == BURNING)
	{
		InStateBurning(a_Dt);
	}
}






void cPawn::SetMetaData(MetaData a_MetaData)
{
	//Broadcast new status to clients in the chunk
	m_MetaData = a_MetaData;
	m_World->BroadcastMetadata(*this);
}





//----Change Entity MetaData
void cPawn::CheckMetaDataBurn()
{
	BLOCKTYPE Block      = GetWorld()->GetBlock((int) m_Pos.x, (int) m_Pos.y, (int) m_Pos.z);
	BLOCKTYPE BlockAbove = GetWorld()->GetBlock((int) m_Pos.x, (int) m_Pos.y + 1, (int) m_Pos.z);
	BLOCKTYPE BlockBelow = GetWorld()->GetBlock((int) m_Pos.x, (int) m_Pos.y - 1, (int) m_Pos.z);
	
	if (
		(GetMetaData() == BURNING) &&
		(IsBlockWater(Block) || IsBlockWater(BlockAbove) || IsBlockWater(BlockBelow))
	)
	{
		SetMetaData(NORMAL);
 	}
 	else if (
 		m_bBurnable && 
 		(GetMetaData() != BURNING) &&
		(
			IsBlockLava(Block) || (Block == E_BLOCK_FIRE) ||
			IsBlockLava(BlockAbove) || (BlockAbove == E_BLOCK_FIRE) ||
			IsBlockLava(BlockBelow) || (BlockBelow == E_BLOCK_FIRE)
		)
	)
	{
		SetMetaData(BURNING);  
	}
}





// What to do if On fire
void cPawn::InStateBurning(float a_Dt)
{
	m_FireDamageInterval += a_Dt;
	if (m_FireDamageInterval < 800)
	{
		return;
	}

	BLOCKTYPE Block      = GetWorld()->GetBlock((int)m_Pos.x, (int)m_Pos.y,     (int)m_Pos.z);
	BLOCKTYPE BlockAbove = GetWorld()->GetBlock((int)m_Pos.x, (int)m_Pos.y + 1, (int)m_Pos.z);	
	m_FireDamageInterval = 0;
	TakeDamage(dtOnFire, NULL, 1, 0);

	m_BurnPeriod++;
	if (
		IsBlockLava(Block) || 
		(Block == E_BLOCK_FIRE) ||
		IsBlockLava(BlockAbove) || 
		(BlockAbove == E_BLOCK_FIRE)
	)
	{
		m_BurnPeriod = 0;
		TakeDamage(dtLavaContact, NULL, 6, 0);
	}
	
	if (m_BurnPeriod > 7)
	{
		SetMetaData(NORMAL);
		m_BurnPeriod = 0;
	}
}





void cPawn::SetMaxHealth(short a_MaxHealth)
{
	this->m_MaxHealth = a_MaxHealth;

	// Reset health
	m_Health = a_MaxHealth;
}

