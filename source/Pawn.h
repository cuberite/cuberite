
#pragma once

#include "Entity.h"
#include "Item.h"





// fwd:
class cPawn;





// tolua_begin
enum eDamageType
{
	// Canonical names for the types (as documented in the plugin wiki):
	dtAttack,           // Being attacked by a mob
	dtLightning,        // Hit by a lightning strike
	dtFalling,          // Falling down; dealt when hitting the ground
	dtDrowning,         // Drowning in water / lava
	dtSuffocating,      // Suffocating inside a block
	dtStarving,         // Hunger
	dtCactusContact,    // Contact with a cactus block
	dtLavaContact,      // Contact with a lava block
	dtPoisoning,        // Having the poison effect
	dtOnFire,           // Being on fire
	dtFireContact,      // Standing inside a fire block
	dtInVoid,           // Falling into the Void (Y < 0)
	dtPotionOfHarming,
	
	// Some common synonyms:
	dtPawnAttack   = dtAttack,
	dtEntityAttack = dtAttack,
	dtMob          = dtAttack,
	dtMobAttack    = dtAttack,
	dtFall         = dtFalling,
	dtDrown        = dtDrowning,
	dtSuffocation  = dtSuffocating,
	dtStarvation   = dtStarving,
	dtHunger       = dtStarving,
	dtCactus       = dtCactusContact,
	dtCactuses     = dtCactusContact,
	dtCacti        = dtCactusContact,
	dtLava         = dtLavaContact,
	dtPoison       = dtPoisoning,
	dtBurning      = dtOnFire,
	dtInFire       = dtFireContact,
} ;





struct TakeDamageInfo
{
	eDamageType DamageType;   // Where does the damage come from? Being hit / on fire / contact with cactus / ...
	cPawn *     Attacker;     // The attacking pawn; valid only for dtAttack
	int         RawDamage;    // What damage would the receiver get without any armor. Usually: attacker mob type + weapons
	int         FinalDamage;  // What actual damage will be received. Usually: m_RawDamage minus armor
	Vector3d    Knockback;    // The amount and direction of knockback received from the damage
	// TODO: Effects - list of effects that the hit is causing. Unknown representation yet
} ;
// tolua_end





// tolua_begin
class cPawn :
	public cEntity
{
	// tolua_end
	typedef cEntity super;
	
public:
	CLASS_PROTODEF(cPawn);

	cPawn(eEntityType a_EntityType);

	virtual void Tick(float a_Dt) override;

	// tolua_begin
	
	/// Teleports to the entity specified
	virtual void TeleportToEntity(cEntity & a_Entity);
	
	/// Teleports to the coordinates specified
	virtual void TeleportTo(double a_PosX, double a_PosY, double a_PosZ);
	
	/// Heals the specified amount of HPs
	void Heal(int a_HitPoints);
	
	/// Returns the health of this pawn
	int GetHealth(void) const { return m_Health; }
	
	/// Makes this pawn take damage from an attack by a_Attacker. Damage values are calculated automatically and DoTakeDamage() called
	void TakeDamage(cPawn & a_Attacker);
	
	/// Makes this pawn take the specified damage. The final damage is calculated using current armor, then DoTakeDamage() called
	void TakeDamage(eDamageType a_DamageType, cPawn * a_Attacker, int a_RawDamage, double a_KnockbackAmount);

	/// Makes this pawn take the specified damage. The values are packed into a TDI, knockback calculated, then sent through DoTakeDamage()
	void TakeDamage(eDamageType a_DamageType, cPawn * a_Attacker, int a_RawDamage, int a_FinalDamage, double a_KnockbackAmount);
	
	/// Makes this pawn take damage specified in the a_TDI. The TDI is sent through plugins first, then applied
	virtual void DoTakeDamage(TakeDamageInfo & a_TDI);

	/// Called when the health drops below zero. a_Killer may be NULL (environmental damage)
	virtual void KilledBy(cPawn * a_Killer);
	
	/// Returns the hitpoints that this pawn can deal to a_Receiver using its equipped items
	virtual int GetRawDamageAgainst(const cPawn & a_Receiver);
	
	/// Returns the hitpoints out of a_RawDamage that the currently equipped armor would cover
	virtual int GetArmorCoverAgainst(const cPawn * a_Attacker, eDamageType a_DamageType, int a_RawDamage);
	
	/// Returns the knockback amount that the currently equipped items would cause to a_Receiver on a hit
	virtual double GetKnockbackAmountAgainst(const cPawn & a_Receiver);
	
	/// Returns the curently equipped weapon; empty item if none
	virtual cItem GetEquippedWeapon(void) const { return cItem(); }
	
	/// Returns the currently equipped helmet; empty item if nonte
	virtual cItem GetEquippedHelmet(void) const { return cItem(); }
	
	/// Returns the currently equipped chestplate; empty item if nonte
	virtual cItem GetEquippedChestplate(void) const { return cItem(); }

	/// Returns the currently equipped leggings; empty item if nonte
	virtual cItem GetEquippedLeggings(void) const { return cItem(); }
	
	/// Returns the currently equipped boots; empty item if nonte
	virtual cItem GetEquippedBoots(void) const { return cItem(); }

	// tolua_end
	
	/// Returns the list of drops for this pawn when it is killed. May check a_Killer for special handling (sword of looting etc.). Called from KilledBy().
	virtual void GetDrops(cItems & a_Drops, cPawn * a_Killer = NULL);

	enum MetaData {NORMAL, BURNING, CROUCHED, RIDING, SPRINTING, EATING, BLOCKING};

	virtual void SetMetaData(MetaData a_MetaData);
	virtual MetaData GetMetaData(void) const { return m_MetaData; }

	virtual void InStateBurning(float a_Dt);

	virtual void CheckMetaDataBurn();

	virtual void SetMaxHealth(short a_MaxHealth);
	virtual short GetMaxHealth() { return m_MaxHealth; }

protected:
	short m_Health;
	short m_MaxHealth;

	bool m_bBurnable;

	MetaData m_MetaData;

	double m_LastPosX, m_LastPosY, m_LastPosZ;
	Int64 m_TimeLastTeleportPacket;  // In ticks
}; //tolua_export




