
#pragma once

#include "../Entities/Pawn.h"
#include "../Defines.h"
#include "../BlockID.h"
#include "../Item.h"
#include "../Enchantments.h"
#include "MonsterTypes.h"
#include "PathFinder.h"


class cClientHandle;
class cWorld;


// tolua_begin
class cMonster :
	public cPawn
{
	typedef cPawn super;
public:

	enum eFamily
	{
		mfHostile  = 0,  // Spider, Zombies ...
		mfPassive  = 1,  // Cows, Pigs
		mfAmbient  = 2,  // Bats
		mfWater    = 3,  // Squid, Guardian

		mfNoSpawn,
		mfUnhandled,  // Nothing. Be sure this is the last and the others are in order
	} ;

	// tolua_end

	enum MState{ATTACKING, IDLE, CHASING, ESCAPING} m_EMState;
	enum MPersonality{PASSIVE, AGGRESSIVE, COWARDLY} m_EMPersonality;

	/** Creates the mob object.
	If a_ConfigName is not empty, the configuration is loaded using GetMonsterConfig()
	a_MobType is the type of the mob (also used in the protocol ( http://wiki.vg/Entities#Mobs 2012_12_22))
	a_SoundHurt and a_SoundDeath are assigned into m_SoundHurt and m_SoundDeath, respectively
	*/
	cMonster(const AString & a_ConfigName, eMonsterType a_MobType, const AString & a_SoundHurt, const AString & a_SoundDeath, double a_Width, double a_Height);

	~cMonster();

	virtual void Destroyed() override;

	CLASS_PROTODEF(cMonster)

	virtual void SpawnOn(cClientHandle & a_ClientHandle) override;

	virtual void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;

	virtual bool DoTakeDamage(TakeDamageInfo & a_TDI) override;

	virtual void KilledBy(TakeDamageInfo & a_TDI) override;

	virtual void OnRightClicked(cPlayer & a_Player) override;

	virtual void HandleFalling(void) override;

	/** Engage pathfinder and tell it to calculate a path to a given position, and move the mobile accordingly
	Currently, the mob will only start moving to a new position after the position it is currently going to is reached. */
	virtual void MoveToPosition(const Vector3d & a_Position);  // tolua_export

	// tolua_begin
	eMonsterType GetMobType(void) const { return m_MobType; }
	eFamily GetMobFamily(void) const;
	// tolua_end

	virtual void CheckEventSeePlayer(cChunk & a_Chunk);
	virtual void EventSeePlayer(cEntity * a_Entity, cChunk & a_Chunk);

	/** Reads the monster configuration for the specified monster name and assigns it to this object. */
	void GetMonsterConfig(const AString & a_Name);

	/** Returns whether this mob is undead (skeleton, zombie, etc.) */
	virtual bool IsUndead(void);

	virtual void EventLosePlayer(void);
	virtual void CheckEventLostPlayer(void);

	virtual void InStateIdle    (std::chrono::milliseconds a_Dt, cChunk & a_Chunk);
	virtual void InStateChasing (std::chrono::milliseconds a_Dt, cChunk & a_Chunk);
	virtual void InStateEscaping(std::chrono::milliseconds a_Dt, cChunk & a_Chunk);

	int GetAttackRate() { return static_cast<int>(m_AttackRate); }
	void SetAttackRate(float a_AttackRate) { m_AttackRate = a_AttackRate; }
	void SetAttackRange(int a_AttackRange) { m_AttackRange = a_AttackRange; }
	void SetAttackDamage(int a_AttackDamage) { m_AttackDamage = a_AttackDamage; }
	void SetSightDistance(int a_SightDistance) { m_SightDistance = a_SightDistance; }

	float GetDropChanceWeapon() { return m_DropChanceWeapon; }
	float GetDropChanceHelmet() { return m_DropChanceHelmet; }
	float GetDropChanceChestplate() { return m_DropChanceChestplate; }
	float GetDropChanceLeggings() { return m_DropChanceLeggings; }
	float GetDropChanceBoots() { return m_DropChanceBoots; }
	bool CanPickUpLoot() { return m_CanPickUpLoot; }
	void SetDropChanceWeapon(float a_DropChanceWeapon) { m_DropChanceWeapon = a_DropChanceWeapon; }
	void SetDropChanceHelmet(float a_DropChanceHelmet) { m_DropChanceHelmet = a_DropChanceHelmet; }
	void SetDropChanceChestplate(float a_DropChanceChestplate) { m_DropChanceChestplate = a_DropChanceChestplate; }
	void SetDropChanceLeggings(float a_DropChanceLeggings) { m_DropChanceLeggings = a_DropChanceLeggings; }
	void SetDropChanceBoots(float a_DropChanceBoots) { m_DropChanceBoots = a_DropChanceBoots; }
	void SetCanPickUpLoot(bool a_CanPickUpLoot) { m_CanPickUpLoot = a_CanPickUpLoot; }
	void ResetAttackCooldown();

	/** Sets whether the mob burns in daylight. Only evaluated at next burn-decision tick */
	void SetBurnsInDaylight(bool a_BurnsInDaylight) { m_BurnsInDaylight = a_BurnsInDaylight; }

	double GetRelativeWalkSpeed(void) const { return m_RelativeWalkSpeed; }  // tolua_export
	void SetRelativeWalkSpeed(double a_WalkSpeed) { m_RelativeWalkSpeed = a_WalkSpeed; }  // tolua_export

	// Overridables to handle ageable mobs
	virtual bool IsTame    (void) const { return false; }
	virtual bool IsSitting (void) const { return false; }

	// tolua_begin
	bool IsBaby (void) const { return m_Age < 0; }
	int GetAge (void) const { return m_Age; }
	void SetAge(int a_Age)  { m_Age = a_Age; }
	// tolua_end


	// tolua_begin

	/** Returns true if the monster has a custom name. */
	bool HasCustomName(void) const { return !m_CustomName.empty(); }

	/** Gets the custom name of the monster. If no custom name is set, the function returns an empty string. */
	const AString & GetCustomName(void) const { return m_CustomName; }

	/** Sets the custom name of the monster. You see the name over the monster.
	If you want to disable the custom name, simply set an empty string. */
	void SetCustomName(const AString & a_CustomName);

	/** Is the custom name of this monster always visible? If not, you only see the name when you sight the mob. */
	bool IsCustomNameAlwaysVisible(void) const { return m_CustomNameAlwaysVisible; }

	/** Sets the custom name visiblity of this monster.
	If it's false, you only see the name when you sight the mob. If it's true, you always see the custom name. */
	void SetCustomNameAlwaysVisible(bool a_CustomNameAlwaysVisible);

	/** Translates MobType enum to a string, empty string if unknown */
	static AString MobTypeToString(eMonsterType a_MobType);

	/** Translates MobType enum to the vanilla name of the mob, empty string if unknown. */
	static AString MobTypeToVanillaName(eMonsterType a_MobType);

	/** Translates MobType string to the enum, mtInvalidType if not recognized */
	static eMonsterType StringToMobType(const AString & a_MobTypeName);

	/** Returns the mob family based on the type */
	static eFamily FamilyFromType(eMonsterType a_MobType);

	/** Returns the spawn delay (number of game ticks between spawn attempts) for the given mob family */
	static int GetSpawnDelay(cMonster::eFamily a_MobFamily);

	// tolua_end

	/** Sets the target that this mob will chase. Pass a nullptr to unset. */
	void SetTarget(cPawn * a_NewTarget);

	/** Returns the current target. */
	cPawn * GetTarget();

	/** Returns the current target's underlying weak pointer. */
	auto GetWeakTargetPtr() { return m_Target; }

	/** Creates a new object of the specified mob.
	a_MobType is the type of the mob to be created
	Asserts and returns null if mob type is not specified
	*/
	static std::shared_ptr<cMonster> NewMonsterFromType(eMonsterType a_MobType);

protected:

	/** The pathfinder instance handles pathfinding for this monster. */
	cPathFinder m_PathFinder;

	/** Stores if pathfinder is being used - set when final destination is set, and unset when stopped moving to final destination */
	bool m_PathfinderActivated;

	/** Coordinates of the next position that should be reached */
	Vector3d m_NextWayPointPosition;

	/** Coordinates for the ultimate, final destination. */
	Vector3d m_FinalDestination;

	/** Finds the lowest non-air block position (not the highest, as cWorld::GetHeight does)
	If current Y is nonsolid, goes down to try to find a solid block, then returns that + 1
	If current Y is solid, goes up to find first nonsolid block, and returns that.
	If no suitable position is found, returns cChunkDef::Height. */
	int FindFirstNonAirBlockPosition(double a_PosX, double a_PosZ);

	/** Returns if the ultimate, final destination has been reached. */
	bool ReachedFinalDestination(void) { return ((m_FinalDestination - GetPosition()).SqrLength() < WAYPOINT_RADIUS * WAYPOINT_RADIUS); }

	/** Returns whether or not the target is close enough for attack. */
	bool TargetIsInRange(void)
	{
		ASSERT(GetTarget() != nullptr);
		return ((GetTarget()->GetPosition() - GetPosition()).SqrLength() < (m_AttackRange * m_AttackRange));
	}

	/** Returns if a monster can reach a given height by jumping. */
	inline bool DoesPosYRequireJump(int a_PosY)
	{
		return ((a_PosY > POSY_TOINT));
	}

	/** Move in a straight line to the next waypoint in the path, will jump if needed. */
	void MoveToWayPoint(cChunk & a_Chunk);

	/** Stops pathfinding. Calls ResetPathFinding and sets m_IsFollowingPath to false */
	void StopMovingToPosition();

	/** Sets the body yaw and head yaw */
	void SetPitchAndYawFromDestination(bool a_IsFollowingPath);

	int m_JumpCoolDown;

	std::chrono::milliseconds m_IdleInterval;
	std::chrono::milliseconds m_DestroyTimer;

	eMonsterType m_MobType;
	AString m_CustomName;
	bool m_CustomNameAlwaysVisible;

	AString m_SoundHurt;
	AString m_SoundDeath;

	float m_AttackRate;
	int m_AttackDamage;
	int m_AttackRange;
	int m_AttackCoolDownTicksLeft;
	int m_SightDistance;

	float m_DropChanceWeapon;
	float m_DropChanceHelmet;
	float m_DropChanceChestplate;
	float m_DropChanceLeggings;
	float m_DropChanceBoots;
	bool m_CanPickUpLoot;
	int m_TicksSinceLastDamaged;  // How many ticks ago we were last damaged by a player?

	void HandleDaylightBurning(cChunk & a_Chunk, bool WouldBurn);
	bool WouldBurnAt(Vector3d a_Location, cChunk & a_Chunk);
	bool m_BurnsInDaylight;
	double m_RelativeWalkSpeed;

	int m_Age;
	int m_AgingTimer;

	/** Adds a random number of a_Item between a_Min and a_Max to itemdrops a_Drops */
	void AddRandomDropItem(cItems & a_Drops, unsigned int a_Min, unsigned int a_Max, short a_Item, short a_ItemHealth = 0);

	/** Adds a item a_Item with the chance of a_Chance (in percent) to itemdrops a_Drops */
	void AddRandomUncommonDropItem(cItems & a_Drops, float a_Chance, short a_Item, short a_ItemHealth = 0);

	/** Adds one rare item out of the list of rare items a_Items modified by the looting level a_LootingLevel(I-III or custom) to the itemdrop a_Drops */
	void AddRandomRareDropItem(cItems & a_Drops, cItems & a_Items, unsigned int a_LootingLevel);

	/** Adds armor that is equipped with the chance saved in m_DropChance[...] (this will be greter than 1 if picked up or 0.085 + (0.01 per LootingLevel) if born with) to the drop */
	void AddRandomArmorDropItem(cItems & a_Drops, unsigned int a_LootingLevel);

	/** Adds weapon that is equipped with the chance saved in m_DropChance[...] (this will be greter than 1 if picked up or 0.085 + (0.01 per LootingLevel) if born with) to the drop */
	void AddRandomWeaponDropItem(cItems & a_Drops, unsigned int a_LootingLevel);

private:
	/** A pointer to the entity this mobile is aiming to reach */
	std::weak_ptr<cPawn> m_Target;

} ;  // tolua_export
