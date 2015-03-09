
#pragma once

#include "../Item.h"
#include "../Vector3.h"





// Place this macro in the public section of each cEntity descendant class and you're done :)
#define CLASS_PROTODEF(classname) \
	virtual bool IsA(const char * a_ClassName) const override\
	{ \
		return ((strcmp(a_ClassName, #classname) == 0) || super::IsA(a_ClassName)); \
	} \
	virtual const char * GetClass(void) const override \
	{ \
		return #classname; \
	} \
	static const char * GetClassStatic(void) \
	{ \
		return #classname; \
	} \
	virtual const char * GetParentClass(void) const override \
	{ \
		return super::GetClass(); \
	}

#define POSX_TOINT FloorC(GetPosX())
#define POSY_TOINT FloorC(GetPosY())
#define POSZ_TOINT FloorC(GetPosZ())
#define POS_TOINT  Vector3i(POSXTOINT, POSYTOINT, POSZTOINT)

#define GET_AND_VERIFY_CURRENT_CHUNK(ChunkVarName, X, Z) cChunk * ChunkVarName = a_Chunk.GetNeighborChunk(X, Z); if ((ChunkVarName == nullptr) || !ChunkVarName->IsValid()) { return; }





class cWorld;
class cClientHandle;
class cPlayer;
class cChunk;





// tolua_begin
struct TakeDamageInfo
{
	eDamageType DamageType;   // Where does the damage come from? Being hit / on fire / contact with cactus / ...
	cEntity *   Attacker;     // The attacking entity; valid only for dtAttack
	int         RawDamage;    // What damage would the receiver get without any armor. Usually: attacker mob type + weapons
	int         FinalDamage;  // What actual damage will be received. Usually: m_RawDamage minus armor
	Vector3d    Knockback;    // The amount and direction of knockback received from the damage
	// TODO: Effects - list of effects that the hit is causing. Unknown representation yet
} ;
// tolua_end





// tolua_begin
class cEntity
{
public:

	enum eEntityType
	{
		etEntity,  // For all other types
		etEnderCrystal,
		etPlayer,
		etPickup,
		etMonster,
		etFallingBlock,
		etMinecart,
		etBoat,
		etTNT,
		etProjectile,
		etExpOrb,
		etFloater,
		etItemFrame,
		etPainting,
		
		// Common variations
		etMob = etMonster,  // DEPRECATED, use etMonster instead!
	} ;
	
	// tolua_end
	
	enum eEntityStatus
	{
		// TODO: Investiagate 0, 1, and 5 as Wiki.vg is not certain

		// Entity becomes coloured red
		esGenericHurt            = 2,
		// Entity plays death animation (entity falls to ground)
		esGenericDead            = 3,
		// Iron Golem plays attack animation (arms lift and fall)
		esIronGolemAttacking     = 4,
		// Wolf taming particles spawn (smoke)
		esWolfTaming             = 6,
		// Wolf tamed particles spawn (hearts)
		esWolfTamed              = 7,
		// Wolf plays water removal animation (shaking and water particles)
		esWolfDryingWater        = 8,
		// Informs client that eating was accepted
		esPlayerEatingAccepted   = 9,
		// Sheep plays eating animation (head lowers to ground)
		esSheepEating            = 10,
		// Iron Golem holds gift to villager children
		esIronGolemGivingPlant   = 11,
		// Villager spawns heart particles
		esVillagerBreeding       = 12,
		// Villager spawns thunderclound particles
		esVillagerAngry          = 13,
		// Villager spawns green crosses
		esVillagerHappy          = 14,
		// Witch spawns magic particle (TODO: investigation into what this is)
		esWitchMagicking         = 15,

		// It seems 16 (zombie conversion) is now done with metadata

		// Informs client to explode a firework based on its metadata
		esFireworkExploding      = 17,
	} ;
	
	static const int FIRE_TICKS_PER_DAMAGE = 10;   ///< Ticks to wait between damaging an entity when it stands in fire
	static const int FIRE_DAMAGE           = 1;    ///< Damage to deal when standing in fire
	static const int LAVA_TICKS_PER_DAMAGE = 10;   ///< Ticks to wait between damaging an entity when it stands in lava
	static const int LAVA_DAMAGE           = 5;    ///< Damage to deal when standing in lava
	static const int BURN_TICKS_PER_DAMAGE = 20;   ///< Ticks to wait between damaging an entity when it is burning
	static const int BURN_DAMAGE           = 1;    ///< Damage to deal when the entity is burning
	
	static const int BURN_TICKS            = 200;  ///< Ticks to keep an entity burning after it has stood in lava / fire
	
	static const int MAX_AIR_LEVEL         = 300;  ///< Maximum air an entity can have
	static const int DROWNING_TICKS        = 20;   ///< Number of ticks per heart of damage
	
	static const int VOID_BOUNDARY         = -46;  ///< Y position to begin applying void damage
	static const int FALL_DAMAGE_HEIGHT    = 4;    ///< Y difference after which fall damage is applied
	
	cEntity(eEntityType a_EntityType, double a_X, double a_Y, double a_Z, double a_Width, double a_Height);
	virtual ~cEntity();

	/** Spawns the entity in the world; returns true if spawned, false if not (plugin disallowed).
	Adds the entity to the world. */
	virtual bool Initialize(cWorld & a_World);

	// tolua_begin
	
	eEntityType GetEntityType(void) const { return m_EntityType; }
	
	bool IsEnderCrystal(void) const { return (m_EntityType == etEnderCrystal); }
	bool IsPlayer      (void) const { return (m_EntityType == etPlayer);       }
	bool IsPickup      (void) const { return (m_EntityType == etPickup);       }
	bool IsMob         (void) const { return (m_EntityType == etMonster);      }
	bool IsPawn        (void) const { return (IsMob() || IsPlayer());          }
	bool IsFallingBlock(void) const { return (m_EntityType == etFallingBlock); }
	bool IsMinecart    (void) const { return (m_EntityType == etMinecart);     }
	bool IsBoat        (void) const { return (m_EntityType == etBoat);         }
	bool IsTNT         (void) const { return (m_EntityType == etTNT);          }
	bool IsProjectile  (void) const { return (m_EntityType == etProjectile);   }
	bool IsExpOrb      (void) const { return (m_EntityType == etExpOrb);       }
	bool IsFloater     (void) const { return (m_EntityType == etFloater);      }
	bool IsItemFrame   (void) const { return (m_EntityType == etItemFrame);    }
	bool IsPainting    (void) const { return (m_EntityType == etPainting);     }
	
	/// Returns true if the entity is of the specified class or a subclass (cPawn's IsA("cEntity") returns true)
	virtual bool IsA(const char * a_ClassName) const;
	
	/** Returns the class name of this class */
	static const char * GetClassStatic(void);
	
	/** Returns the topmost class name for the object */
	virtual const char * GetClass(void) const;
	
	/** Returns the topmost class's parent class name for the object. cEntity returns an empty string (no parent). */
	virtual const char * GetParentClass(void) const;

	cWorld * GetWorld(void) const { return m_World; }

	double           GetHeadYaw   (void) const { return m_HeadYaw; }  // In degrees
	double           GetHeight    (void) const { return m_Height;  }
	double           GetMass      (void) const { return m_Mass;    }
	const Vector3d & GetPosition  (void) const { return m_Pos;     }
	double           GetPosX      (void) const { return m_Pos.x;   }
	double           GetPosY      (void) const { return m_Pos.y;   }
	double           GetPosZ      (void) const { return m_Pos.z;   }
	const Vector3d & GetRot       (void) const { return m_Rot;     }  // OBSOLETE, use individual GetYaw(), GetPitch, GetRoll() components
	double           GetYaw       (void) const { return m_Rot.x;   }  // In degrees, [-180, +180)
	double           GetPitch     (void) const { return m_Rot.y;   }  // In degrees, [-180, +180), but normal client clips to [-90, +90]
	double           GetRoll      (void) const { return m_Rot.z;   }  // In degrees, unused in current client
	Vector3d         GetLookVector(void) const;
	const Vector3d & GetSpeed     (void) const { return m_Speed;   }
	double           GetSpeedX    (void) const { return m_Speed.x; }
	double           GetSpeedY    (void) const { return m_Speed.y; }
	double           GetSpeedZ    (void) const { return m_Speed.z; }
	double           GetWidth     (void) const { return m_Width;   }
	
	int GetChunkX(void) const {return (int)floor(m_Pos.x / cChunkDef::Width); }
	int GetChunkZ(void) const {return (int)floor(m_Pos.z / cChunkDef::Width); }

	void SetHeadYaw (double a_HeadYaw);
	void SetHeight  (double a_Height);
	void SetMass    (double a_Mass);
	void SetPosX    (double a_PosX);
	void SetPosY    (double a_PosY);
	void SetPosZ    (double a_PosZ);
	void SetPosition(double a_PosX, double a_PosY, double a_PosZ);
	void SetPosition(const Vector3d & a_Pos) { SetPosition(a_Pos.x, a_Pos.y, a_Pos.z); }
	void SetRot     (const Vector3f & a_Rot);  // OBSOLETE, use individual SetYaw(), SetPitch(), SetRoll() components
	void SetYaw     (double a_Yaw);    // In degrees, normalizes to [-180, +180)
	void SetPitch   (double a_Pitch);  // In degrees, normalizes to [-180, +180)
	void SetRoll    (double a_Roll);   // In degrees, normalizes to [-180, +180)

	/** Sets the speed of the entity, measured in m / sec */
	void SetSpeed(double a_SpeedX, double a_SpeedY, double a_SpeedZ);
	
	/** Sets the speed of the entity, measured in m / sec */
	void SetSpeed(const Vector3d & a_Speed) { SetSpeed(a_Speed.x, a_Speed.y, a_Speed.z); }
	
	/** Sets the speed in the X axis, leaving the other speed components intact. Measured in m / sec. */
	void SetSpeedX(double a_SpeedX);
	
	/** Sets the speed in the Y axis, leaving the other speed components intact. Measured in m / sec. */
	void SetSpeedY(double a_SpeedY);
	
	/** Sets the speed in the Z axis, leaving the other speed components intact. Measured in m / sec. */
	void SetSpeedZ(double a_SpeedZ);
	
	void SetWidth   (double a_Width);
	
	void AddPosX    (double a_AddPosX);
	void AddPosY    (double a_AddPosY);
	void AddPosZ    (double a_AddPosZ);
	void AddPosition(double a_AddPosX, double a_AddPosY, double a_AddPosZ);
	void AddPosition(const Vector3d & a_AddPos) { AddPosition(a_AddPos.x, a_AddPos.y, a_AddPos.z); }
	void AddSpeed   (double a_AddSpeedX, double a_AddSpeedY, double a_AddSpeedZ);
	void AddSpeed   (const Vector3d & a_AddSpeed) { AddSpeed(a_AddSpeed.x, a_AddSpeed.y, a_AddSpeed.z); }
	void AddSpeedX  (double a_AddSpeedX);
	void AddSpeedY  (double a_AddSpeedY);
	void AddSpeedZ  (double a_AddSpeedZ);
	
	virtual void HandleSpeedFromAttachee(float a_Forward, float a_Sideways);
	void SteerVehicle(float a_Forward, float a_Sideways);

	inline int  GetUniqueID(void) const { return m_UniqueID; }
	inline bool IsDestroyed(void) const { return !m_IsInitialized; }

	/// Schedules the entity for destroying; if a_ShouldBroadcast is set to true, broadcasts the DestroyEntity packet
	void Destroy(bool a_ShouldBroadcast = true);

	/// Makes this pawn take damage from an attack by a_Attacker. Damage values are calculated automatically and DoTakeDamage() called
	void TakeDamage(cEntity & a_Attacker);
	
	/// Makes this entity take the specified damage. The final damage is calculated using current armor, then DoTakeDamage() called
	void TakeDamage(eDamageType a_DamageType, cEntity * a_Attacker, int a_RawDamage, double a_KnockbackAmount);

	/// Makes this entity take the specified damage. The values are packed into a TDI, knockback calculated, then sent through DoTakeDamage()
	void TakeDamage(eDamageType a_DamageType, cEntity * a_Attacker, int a_RawDamage, int a_FinalDamage, double a_KnockbackAmount);
	
	float GetGravity(void) const { return m_Gravity; }
	
	void SetGravity(float a_Gravity) { m_Gravity = a_Gravity; }
	
	/// Sets the rotation to match the speed vector (entity goes "face-forward")
	void SetYawFromSpeed(void);
	
	/// Sets the pitch to match the speed vector (entity gies "face-forward")
	void SetPitchFromSpeed(void);
	
	// tolua_end
	
	/** Makes this entity take damage specified in the a_TDI.
	The TDI is sent through plugins first, then applied.
	If it returns false, the entity hasn't receive any damage. */
	virtual bool DoTakeDamage(TakeDamageInfo & a_TDI);
	
	// tolua_begin

	/// Returns the hitpoints that this pawn can deal to a_Receiver using its equipped items
	virtual int GetRawDamageAgainst(const cEntity & a_Receiver);
	
	/** Returns whether armor will protect against the passed damage type **/
	virtual bool ArmorCoversAgainst(eDamageType a_DamageType);
	
	/// Returns the hitpoints out of a_RawDamage that the currently equipped armor would cover
	virtual int GetArmorCoverAgainst(const cEntity * a_Attacker, eDamageType a_DamageType, int a_RawDamage);
	
	/// Returns the knockback amount that the currently equipped items would cause to a_Receiver on a hit
	virtual double GetKnockbackAmountAgainst(const cEntity & a_Receiver);
	
	/// Returns the curently equipped weapon; empty item if none
	virtual cItem GetEquippedWeapon(void) const { return cItem(); }
	
	/// Returns the currently equipped helmet; empty item if none
	virtual cItem GetEquippedHelmet(void) const { return cItem(); }
	
	/// Returns the currently equipped chestplate; empty item if none
	virtual cItem GetEquippedChestplate(void) const { return cItem(); }

	/// Returns the currently equipped leggings; empty item if none
	virtual cItem GetEquippedLeggings(void) const { return cItem(); }
	
	/// Returns the currently equipped boots; empty item if none
	virtual cItem GetEquippedBoots(void) const { return cItem(); }

	/// Called when the health drops below zero. a_Killer may be nullptr (environmental damage)
	virtual void KilledBy(TakeDamageInfo & a_TDI);

	/// Called when the entity kills another entity
	virtual void Killed(cEntity * a_Victim) {}

	/// Heals the specified amount of HPs
	virtual void Heal(int a_HitPoints);
	
	/// Returns the health of this entity
	int GetHealth(void) const { return m_Health; }
	
	/// Sets the health of this entity; doesn't broadcast any hurt animation
	void SetHealth(int a_Health);
	
	// tolua_end

	virtual void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk);
	
	/// Handles the physics of the entity - updates position based on speed, updates speed based on environment
	virtual void HandlePhysics(std::chrono::milliseconds a_Dt, cChunk & a_Chunk);
	
	/// Updates the state related to this entity being on fire
	virtual void TickBurning(cChunk & a_Chunk);

	/** Detects the time for application of cacti damage */
	virtual void DetectCacti(void);

	/** Detects whether we are in a portal block and begins teleportation procedures if so
	Returns true if MoveToWorld() was called, false if not
	*/
	virtual bool DetectPortal(void);
	
	/// Handles when the entity is in the void
	virtual void TickInVoid(cChunk & a_Chunk);

	/// Called when the entity starts burning
	virtual void OnStartedBurning(void);
	
	/// Called when the entity finishes burning
	virtual void OnFinishedBurning(void);
	
	// tolua_begin
	
	/// Sets the maximum value for the health
	void SetMaxHealth(int a_MaxHealth);

	int GetMaxHealth(void) const { return m_MaxHealth; }
	
	/// Sets whether the entity is fireproof
	void SetIsFireproof(bool a_IsFireproof);
	
	bool IsFireproof(void) const { return m_IsFireproof; }
	
	/// Puts the entity on fire for the specified amount of ticks
	void StartBurning(int a_TicksLeftBurning);
	
	/// Stops the entity from burning, resets all burning timers
	void StopBurning(void);
	
	// tolua_end

	/** Descendants override this function to send a command to the specified client to spawn the entity on the client.
	To spawn on all eligible clients, use cChunkMap::BroadcastSpawnEntity()
	*/
	virtual void SpawnOn(cClientHandle & a_Client) = 0;

	// tolua_begin
	
	/// Teleports to the entity specified
	virtual void TeleportToEntity(cEntity & a_Entity);
	
	/// Teleports to the coordinates specified
	virtual void TeleportToCoords(double a_PosX, double a_PosY, double a_PosZ);

	/** Moves entity to specified world, taking a world pointer */
	bool MoveToWorld(cWorld * a_World, bool a_ShouldSendRespawn = true) { return DoMoveToWorld(a_World, a_ShouldSendRespawn); }

	/** Moves entity to specified world, taking a world name */
	bool MoveToWorld(const AString & a_WorldName, bool a_ShouldSendRespawn = true);
	
	// tolua_end

	virtual bool DoMoveToWorld(cWorld * a_World, bool a_ShouldSendRespawn);

	/** Returns if the entity is travelling away from a specified world */
	bool IsWorldTravellingFrom(cWorld * a_World) const { return (m_WorldTravellingFrom == a_World); }

	/** Sets the world the entity will be leaving */
	void SetWorldTravellingFrom(cWorld * a_World) { m_WorldTravellingFrom = a_World; }
	
	/// Updates clients of changes in the entity.
	virtual void BroadcastMovementUpdate(const cClientHandle * a_Exclude = nullptr);
	
	/// Attaches to the specified entity; detaches from any previous one first
	void AttachTo(cEntity * a_AttachTo);
	
	/// Detaches from the currently attached entity, if any
	virtual void Detach(void);
	
	/// Makes sure head yaw is not over the specified range.
	void WrapHeadYaw();

	/// Makes sure rotation is not over the specified range.
	void WrapRotation();

	/// Makes speed is not over 20. Max speed is 20 blocks / second
	void WrapSpeed();
	
	// tolua_begin
	
	// COMMON metadata flags; descendants may override the defaults:
	virtual bool IsOnFire   (void) const {return (m_TicksLeftBurning > 0); }
	virtual bool IsCrouched (void) const {return false; }
	virtual bool IsRiding   (void) const {return false; }
	virtual bool IsSprinting(void) const {return false; }
	virtual bool IsRclking  (void) const {return false; }
	virtual bool IsInvisible(void) const { return false; }

	/** Returns whether the player is swimming or not */
	virtual bool IsSwimming(void) const{ return m_IsSwimming; }
	/** Return whether the player is under water or not */
	virtual bool IsSubmerged(void) const{ return m_IsSubmerged; }
	/** Gets remaining air of a monster */
	int GetAirLevel(void) const { return m_AirLevel; }

	/** Gets number of ticks this entity has existed for */
	long int GetTicksAlive(void) const { return m_TicksAlive; }
	
	/** Gets the invulnerable ticks from the entity */
	int GetInvulnerableTicks(void) const { return m_InvulnerableTicks; }

	/** Set the invulnerable ticks from the entity */
	void SetInvulnerableTicks(int a_InvulnerableTicks) { m_InvulnerableTicks = a_InvulnerableTicks; }

	/** Returns whether the entity is on ground or not */
	virtual bool IsOnGround(void) const { return m_bOnGround; }
	
	// tolua_end
	
	/// Called when the specified player right-clicks this entity
	virtual void OnRightClicked(cPlayer & a_Player) {}

	/// Returns the list of drops for this pawn when it is killed. May check a_Killer for special handling (sword of looting etc.). Called from KilledBy().
	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = nullptr)
	{
		UNUSED(a_Drops);
		UNUSED(a_Killer);
	}

	/** Sets the internal world pointer to a new cWorld, doesn't update anything else. */
	void SetWorld(cWorld * a_World) { m_World = a_World; }

protected:
	static cCriticalSection m_CSCount;
	static int m_EntityCount;
	
	/** Measured in meter/second (m/s) */
	Vector3d m_Speed;

	int m_UniqueID;
	
	int m_Health;
	int m_MaxHealth;
	
	/// The entity to which this entity is attached (vehicle), nullptr if none
	cEntity * m_AttachedTo;
	
	/// The entity which is attached to this entity (rider), nullptr if none
	cEntity * m_Attachee;

	/** Stores whether head yaw has been set manually */
	bool m_bDirtyHead;
	
	/** Stores whether our yaw/pitch/roll (body orientation) has been set manually */
	bool m_bDirtyOrientation;
	
	/** Stores whether we have sent a Velocity packet with a speed of zero (no speed) to the client
	Ensures that said packet is sent only once */
	bool m_bHasSentNoSpeed;

	/** Stores if the entity is on the ground */
	bool m_bOnGround;
	
	/** Stores gravity that is applied to an entity every tick
	For realistic effects, this should be negative. For spaaaaaaace, this can be zero or even positive */
	float m_Gravity;
	
	/** Last position sent to client via the Relative Move or Teleport packets (not Velocity)
	Only updated if cEntity::BroadcastMovementUpdate() is called! */
	Vector3d m_LastPos;

	/** True when entity is initialised (Initialize()) and false when destroyed pending deletion (Destroy()) */
	bool m_IsInitialized;

	/** World entity is travelling from
	Set to a valid world pointer by MoveToWorld; reset to nullptr when the entity is removed from the old world
	Can't be a simple boolean as context switches between worlds may leave the new chunk processing (and therefore immediately removing) the entity before the old chunk could remove it
	*/
	cWorld * m_WorldTravellingFrom;

	eEntityType m_EntityType;
	
	cWorld * m_World;
	
	/// Whether the entity is capable of taking fire or lava damage.
	bool m_IsFireproof;

	/// Time, in ticks, since the last damage dealt by being on fire. Valid only if on fire (IsOnFire())
	int m_TicksSinceLastBurnDamage;
	
	/// Time, in ticks, since the last damage dealt by standing in lava. Reset to zero when moving out of lava.
	int m_TicksSinceLastLavaDamage;
	
	/// Time, in ticks, since the last damage dealt by standing in fire. Reset to zero when moving out of fire.
	int m_TicksSinceLastFireDamage;
	
	/// Time, in ticks, until the entity extinguishes its fire
	int m_TicksLeftBurning;
	
	/// Time, in ticks, since the last damage dealt by the void. Reset to zero when moving out of the void.
	int m_TicksSinceLastVoidDamage;
	
	/** Does the actual speed-setting. The default implementation just sets the member variable value;
	overrides can provide further processing, such as forcing players to move at the given speed. */
	virtual void DoSetSpeed(double a_SpeedX, double a_SpeedY, double a_SpeedZ);
	
	virtual void Destroyed(void) {}  // Called after the entity has been destroyed

	/** Applies friction to an entity
	@param a_Speed The speed vector to apply changes to
	@param a_SlowdownMultiplier The factor to reduce the speed by
	*/
	static void ApplyFriction(Vector3d & a_Speed, double a_SlowdownMultiplier, float a_Dt);

	/** Called in each tick to handle air-related processing i.e. drowning */
	virtual void HandleAir(void);
	
	/** Called once per tick to set IsSwimming and IsSubmerged */
	virtual void SetSwimState(cChunk & a_Chunk);

	/** If an entity is currently swimming in or submerged under water */
	bool m_IsSwimming, m_IsSubmerged;

	/** Air level of a mobile */
	int m_AirLevel;
	int m_AirTickTimer;

	/** Structure storing the portal delay timer and cooldown boolean */
	struct sPortalCooldownData
	{
		/** Ticks since entry of portal, used to delay teleportation */
		unsigned short m_TicksDelayed;

		/** Whether the entity has just exited the portal, and should therefore not be teleported again
		This prevents teleportation loops, and is reset when the entity has moved out of the portal
		*/
		bool m_ShouldPreventTeleportation;
	};

	/** Portal delay timer and cooldown boolean data */
	sPortalCooldownData m_PortalCooldownData;
	
	/** The number of ticks this entity has been alive for */
	long int m_TicksAlive;
	
private:
	/** Measured in degrees, [-180, +180) */
	double   m_HeadYaw;
	
	/** Measured in degrees, [-180, +180) */
	Vector3d m_Rot;
	
	/** Position of the entity's XZ center and Y bottom */
	Vector3d m_Pos;
	
	/** Measured in meter / second */
	Vector3d m_WaterSpeed;
	
	/** Measured in Kilograms (Kg) */
	double m_Mass;
	
	/** Width of the entity, in the XZ plane. Since entities are represented as cylinders, this is more of a diameter. */
	double m_Width;
	
	/** Height of the entity (Y axis) */
	double m_Height;

	/** If a player hit a entity, the entity receive a invulnerable of 10 ticks.
	While this ticks, a player can't hit this entity. */
	int m_InvulnerableTicks;
} ;  // tolua_export

typedef std::list<cEntity *> cEntityList;




