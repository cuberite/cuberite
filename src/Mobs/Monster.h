
#pragma once

#include "../Entities/Pawn.h"
#include "../Defines.h"
#include "../BlockID.h"
#include "../Item.h"
#include "../Enchantments.h"
#include "MonsterTypes.h"



class cClientHandle;
class cWorld;

// Fwd: cPath
enum class ePathFinderStatus;
class cPath;



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

	CLASS_PROTODEF(cMonster)

	virtual void SpawnOn(cClientHandle & a_ClientHandle) override;

	virtual void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;

	virtual bool DoTakeDamage(TakeDamageInfo & a_TDI) override;

	virtual void KilledBy(TakeDamageInfo & a_TDI) override;

	virtual void OnRightClicked(cPlayer & a_Player) override;

	/** Engage pathfinder and tell it to calculate a path to a given position, and move the mobile accordingly
	Currently, the mob will only start moving to a new position after the position it is currently going to is reached. */
	virtual void MoveToPosition(const Vector3d & a_Position);  // tolua_export

	// tolua_begin
	eMonsterType GetMobType(void) const { return m_MobType; }
	eFamily GetMobFamily(void) const;
	// tolua_end

	virtual void CheckEventSeePlayer(void);
	virtual void EventSeePlayer(cEntity * a_Player);

	/** Reads the monster configuration for the specified monster name and assigns it to this object. */
	void GetMonsterConfig(const AString & a_Name);

	/** Returns whether this mob is undead (skeleton, zombie, etc.) */
	virtual bool IsUndead(void);

	virtual void EventLosePlayer(void);
	virtual void CheckEventLostPlayer(void);

	virtual void InStateIdle    (std::chrono::milliseconds a_Dt);
	virtual void InStateChasing (std::chrono::milliseconds a_Dt);
	virtual void InStateEscaping(std::chrono::milliseconds a_Dt);

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

	/** Sets whether the mob burns in daylight. Only evaluated at next burn-decision tick */
	void SetBurnsInDaylight(bool a_BurnsInDaylight) { m_BurnsInDaylight = a_BurnsInDaylight; }

	double GetRelativeWalkSpeed(void) const { return m_RelativeWalkSpeed; }  // tolua_export
	void SetRelativeWalkSpeed(double a_WalkSpeed) { m_RelativeWalkSpeed = a_WalkSpeed; }  // tolua_export

	// Overridables to handle ageable mobs
	virtual bool IsTame    (void) const { return false; }
	virtual bool IsSitting (void) const { return false; }

	// tolua_begin
	bool IsBaby (void) const { return m_Age < 0; }
	char GetAge (void) const { return m_Age; }
	void SetAge(char a_Age)  { m_Age = a_Age; }
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

	/** Creates a new object of the specified mob.
	a_MobType is the type of the mob to be created
	Asserts and returns null if mob type is not specified
	*/
	static cMonster * NewMonsterFromType(eMonsterType a_MobType);

protected:

	/** A pointer to the entity this mobile is aiming to reach */
	cEntity * m_Target;
	cPath *  m_Path;  // TODO unique ptr

	/** Stores if mobile is currently moving towards the ultimate, final destination */
	bool m_IsFollowingPath;

	/** Stores if pathfinder is being used - set when final destination is set, and unset when stopped moving to final destination */
	bool m_PathfinderActivated;

	/* If 0, will give up reaching the next m_NextWayPointPosition and will re-compute path. */
	int m_GiveUpCounter;
	int m_TicksSinceLastPathReset;

	/** Coordinates of the next position that should be reached */
	Vector3d m_NextWayPointPosition;

	/** Coordinates for the ultimate, final destination. */
	Vector3d m_FinalDestination;

	/** Coordinates for the ultimate, final destination last given to the pathfinder. */
	Vector3d m_PathFinderDestination;

	/** True if there's no path to target and we're walking to an approximated location. */
	bool m_NoPathToTarget;

	/** Whether The mob has finished their path, note that this does not imply reaching the destination,
	the destination may sometimes differ from the current path. */
	bool m_NoMoreWayPoints;

	/** Finds the lowest non-air block position (not the highest, as cWorld::GetHeight does)
	If current Y is nonsolid, goes down to try to find a solid block, then returns that + 1
	If current Y is solid, goes up to find first nonsolid block, and returns that.
	If no suitable position is found, returns cChunkDef::Height. */
	int FindFirstNonAirBlockPosition(double a_PosX, double a_PosZ);

	/** Returns if the ultimate, final destination has been reached. */
	bool ReachedFinalDestination(void) { return ((m_FinalDestination - GetPosition()).Length() < GetWidth()/2); }

	/** Returns whether or not the target is close enough for attack. */
	bool TargetIsInRange(void) { return ((m_FinalDestination - GetPosition()).SqrLength() < (m_AttackRange * m_AttackRange)); }

	/** Returns if the intermediate waypoint of m_NextWayPointPosition has been reached */
	bool ReachedNextWaypoint(void) { return ((m_NextWayPointPosition - GetPosition()).SqrLength() < 0.25); }

	/** Returns if a monster can reach a given height by jumping. */
	inline bool DoesPosYRequireJump(int a_PosY)
	{
		return ((a_PosY > POSY_TOINT) && (a_PosY == POSY_TOINT + 1));
	}

	/** Finds the next place to go by calculating a path and setting the m_NextWayPointPosition variable for the next block to head to
	This is based on the ultimate, final destination and the current position, as well as the A* algorithm, and any environmental hazards
	Returns if a path is ready, and therefore if the mob should move to m_NextWayPointPosition
	*/
	bool TickPathFinding(cChunk & a_Chunk);

	/** Move in a straight line to the next waypoint in the path, will jump if needed. */
	void MoveToWayPoint(cChunk & a_Chunk);

	/** Ensures the destination is not buried underground or under water. Also ensures the destination is not in the air.
	Only the Y coordinate of m_FinalDestination might be changed.
	1. If m_FinalDestination is the position of a water block, m_FinalDestination's Y will be modified to point to the heighest water block in the pool in the current column.
	2. If m_FinalDestination is the position of a solid, m_FinalDestination's Y will be modified to point to the first airblock above the solid in the current column.
	3. If m_FinalDestination is the position of an air block, Y will keep decreasing until hitting either a solid or water.
	Now either 1 or 2 is performed. */
	bool EnsureProperDestination(cChunk & a_Chunk);

	/** Resets a pathfinding task, be it due to failure or something else
	Resets the pathfinder. If m_IsFollowingPath is true, TickPathFinding starts a brand new path.
	Should only be called by the pathfinder, cMonster::Tick or StopMovingToPosition. */
	void ResetPathFinding(void);

	/** Stops pathfinding
	Calls ResetPathFinding and sets m_IsFollowingPath to false */
	void StopMovingToPosition();

	/** Sets the body yaw and head yaw / pitch based on next / ultimate destinations */
	void SetPitchAndYawFromDestination(void);

	virtual void HandleFalling(void);
	int m_LastGroundHeight;
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
	float m_AttackInterval;
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

	char m_Age;

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


} ;  // tolua_export
