
#pragma once

#include "../Item.h"
#include "../Vector3d.h"
#include "../Vector3f.h"





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





class cWorld;
class cReferenceManager;
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
	enum
	{
		ENTITY_STATUS_HURT            = 2,
		ENTITY_STATUS_DEAD            = 3,
		ENTITY_STATUS_WOLF_TAMING     = 6,
		ENTITY_STATUS_WOLF_TAMED      = 7,
		ENTITY_STATUS_WOLF_SHAKING    = 8,
		ENTITY_STATUS_EATING_ACCEPTED = 9,
		ENTITY_STATUS_SHEEP_EATING    = 10,
	} ;
	
	enum
	{
		FIRE_TICKS_PER_DAMAGE = 10,  ///< How many ticks to wait between damaging an entity when it stands in fire
		FIRE_DAMAGE = 1,             ///< How much damage to deal when standing in fire
		LAVA_TICKS_PER_DAMAGE = 10,  ///< How many ticks to wait between damaging an entity when it stands in lava
		LAVA_DAMAGE = 5,             ///< How much damage to deal when standing in lava
		BURN_TICKS_PER_DAMAGE = 20,  ///< How many ticks to wait between damaging an entity when it is burning
		BURN_DAMAGE = 1,             ///< How much damage to deal when the entity is burning
		BURN_TICKS = 200,            ///< How long to keep an entity burning after it has stood in lava / fire
	} ;
	
	enum eEntityType
	{
		etEntity,  // For all other types
		etPlayer,
		etPickup,
		etMonster,
		etMob = etMonster,  // DEPRECATED, use etMonster instead!
		etFallingBlock,
		etMinecart,
		etTNT,
		
		// DEPRECATED older constants, left over for compatibility reasons (plugins)
		eEntityType_Entity = etEntity,
		eEntityType_Player = etPlayer,
		eEntityType_Pickup = etPickup,
		eEntityType_Mob    = etMob,
	} ;
	
	// tolua_end

	cEntity(eEntityType a_EntityType, double a_X, double a_Y, double a_Z, double a_Width, double a_Height);
	virtual ~cEntity();

	/// Spawns the entity in the world; returns true if spawned, false if not (plugin disallowed)
	virtual bool Initialize(cWorld * a_World);

	// tolua_begin
	
	eEntityType GetEntityType(void) const { return m_EntityType; }
	
	bool IsPlayer  (void) const { return (m_EntityType == etPlayer); }
	bool IsPickup  (void) const { return (m_EntityType == etPickup); }
	bool IsMob     (void) const { return (m_EntityType == etMob); }
	bool IsMinecart(void) const { return (m_EntityType == etMinecart); }
	bool IsTNT     (void) const { return (m_EntityType == etTNT); }
	
	/// Returns true if the entity is of the specified class or a subclass (cPawn's IsA("cEntity") returns true)
	virtual bool IsA(const char * a_ClassName) const;
	
	/// Returns the topmost class name for the object
	virtual const char * GetClass(void) const;
	
	// Returns the class name of this class
	static const char * GetClassStatic(void);
	
	/// Returns the topmost class's parent class name for the object. cEntity returns an empty string (no parent).
	virtual const char * GetParentClass(void) const;

	cWorld * GetWorld(void) const { return m_World; }

	double           GetHeadYaw   (void) const { return m_HeadYaw; }
	double           GetHeight    (void) const { return m_Height;  }
	double           GetMass      (void) const { return m_Mass;    }
	const Vector3d & GetPosition  (void) const { return m_Pos;     }
	double           GetPosX      (void) const { return m_Pos.x;   }
	double           GetPosY      (void) const { return m_Pos.y;   }
	double           GetPosZ      (void) const { return m_Pos.z;   }
	const Vector3d & GetRot       (void) const { return m_Rot;     }
	double           GetRotation  (void) const { return m_Rot.x;   }
	double           GetPitch     (void) const { return m_Rot.y;   }
	double           GetRoll      (void) const { return m_Rot.z;   }
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
	void SetPosition(const Vector3d & a_Pos) { SetPosition(a_Pos.x,a_Pos.y,a_Pos.z);}
	void SetRot     (const Vector3f & a_Rot);
	void SetRotation(double a_Rotation);
	void SetPitch   (double a_Pitch);
	void SetRoll    (double a_Roll);
	void SetSpeed   (double a_SpeedX, double a_SpeedY, double a_SpeedZ);
	void SetSpeed   (const Vector3d & a_Speed) { SetSpeed(a_Speed.x, a_Speed.y, a_Speed.z); }
	void SetSpeedX  (double a_SpeedX);
	void SetSpeedY  (double a_SpeedY);
	void SetSpeedZ  (double a_SpeedZ);
	void SetWidth   (double a_Width);
	
	void AddPosX    (double a_AddPosX);
	void AddPosY    (double a_AddPosY);
	void AddPosZ    (double a_AddPosZ);
	void AddPosition(double a_AddPosX, double a_AddPosY, double a_AddPosZ);
	void AddPosition(const Vector3d & a_AddPos) { AddPosition(a_AddPos.x,a_AddPos.y,a_AddPos.z);}
	void AddSpeed   (double a_AddSpeedX, double a_AddSpeedY, double a_AddSpeedZ);
	void AddSpeed   (const Vector3d & a_AddSpeed) { AddSpeed(a_AddSpeed.x,a_AddSpeed.y,a_AddSpeed.z);}
	void AddSpeedX  (double a_AddSpeedX);
	void AddSpeedY  (double a_AddSpeedY);
	void AddSpeedZ  (double a_AddSpeedZ);

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
	
	// tolua_end
	
	/// Makes this entity take damage specified in the a_TDI. The TDI is sent through plugins first, then applied
	virtual void DoTakeDamage(TakeDamageInfo & a_TDI);
	
	// tolua_begin

	/// Returns the hitpoints that this pawn can deal to a_Receiver using its equipped items
	virtual int GetRawDamageAgainst(const cEntity & a_Receiver);
	
	/// Returns the hitpoints out of a_RawDamage that the currently equipped armor would cover
	virtual int GetArmorCoverAgainst(const cEntity * a_Attacker, eDamageType a_DamageType, int a_RawDamage);
	
	/// Returns the knockback amount that the currently equipped items would cause to a_Receiver on a hit
	virtual double GetKnockbackAmountAgainst(const cEntity & a_Receiver);
	
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

	/// Called when the health drops below zero. a_Killer may be NULL (environmental damage)
	virtual void KilledBy(cEntity * a_Killer);

	/// Heals the specified amount of HPs
	void Heal(int a_HitPoints);
	
	/// Returns the health of this entity
	int GetHealth(void) const { return m_Health; }
	
	/// Sets the health of this entity; doesn't broadcast any hurt animation
	void SetHealth(int a_Health);
	
	// tolua_end

	virtual void Tick(float a_Dt, cChunk & a_Chunk);
	
	/// Handles the physics of the entity - updates position based on speed, updates speed based on environment
	virtual void HandlePhysics(float a_Dt, cChunk & a_Chunk);
	
	/// Updates the state related to this entity being on fire
	virtual void TickBurning(cChunk & a_Chunk);

	/// Called when the entity starts burning
	virtual void OnStartedBurning(void);
	
	/// Called when the entity finishes burning
	virtual void OnFinishedBurning(void);
	
	// tolua_begin
	
	/// Sets the maximum value for the health
	void SetMaxHealth(int a_MaxHealth);

	int GetMaxHealth(void) const { return m_MaxHealth; }
	
	/// Puts the entity on fire for the specified amount of ticks
	void StartBurning(int a_TicksLeftBurning);
	
	/// Stops the entity from burning, resets all burning timers
	void StopBurning(void);
	
	// tolua_end

	/** Descendants override this function to send a command to the specified client to spawn the entity on the client.
	To spawn on all eligible clients, use cChunkMap::BroadcastSpawnEntity()
	Needs to have a default implementation due to Lua bindings.
	*/
	virtual void SpawnOn(cClientHandle & a_Client) {ASSERT(!"SpawnOn() unimplemented!"); }

	// tolua_begin
	
	/// Teleports to the entity specified
	virtual void TeleportToEntity(cEntity & a_Entity);
	
	/// Teleports to the coordinates specified
	virtual void TeleportToCoords(double a_PosX, double a_PosY, double a_PosZ);
	
	// tolua_end
	
	/// Updates clients of changes in the entity.
	virtual void BroadcastMovementUpdate(const cClientHandle * a_Exclude = NULL);
	
	/// Attaches to the specified entity; detaches from any previous one first
	void AttachTo(cEntity * a_AttachTo);
	
	/// Detaches from the currently attached entity, if any
	void Detach(void);
	
	/// Makes sure head yaw is not over the specified range.
	void WrapHeadYaw();

	/// Makes sure rotation is not over the specified range.
	void WrapRotation();

	/// Makes speed is not over 20. Max speed is 20 blocks / second
	void WrapSpeed();
	
	// tolua_begin
	
	// Metadata flags; descendants may override the defaults:
	virtual bool IsOnFire   (void) const {return (m_TicksLeftBurning > 0); }
	virtual bool IsCrouched (void) const {return false; }
	virtual bool IsRiding   (void) const {return false; }
	virtual bool IsSprinting(void) const {return false; }
	virtual bool IsRclking  (void) const {return false; }
	
	// tolua_end
	
	/// Called when the specified player right-clicks this entity
	virtual void OnRightClicked(cPlayer & a_Player) {};

	/// Returns the list of drops for this pawn when it is killed. May check a_Killer for special handling (sword of looting etc.). Called from KilledBy().
	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) {}

protected:
	static cCriticalSection m_CSCount;
	static int m_EntityCount;
	
	int m_UniqueID;
	
	int m_Health;
	int m_MaxHealth;
	
	/// The entity to which this entity is attached (vehicle), NULL if none
	cEntity * m_AttachedTo;
	
	/// The entity which is attached to this entity (rider), NULL if none
	cEntity * m_Attachee;

	cReferenceManager* m_Referencers;
	cReferenceManager* m_References;

	// Flags that signal that we haven't updated the clients with the latest.
	bool     m_bDirtyHead;
	bool     m_bDirtyOrientation;
	bool     m_bDirtyPosition;
	bool     m_bDirtySpeed;

	bool     m_bOnGround;
	float    m_Gravity;

	// Last Position.
	double m_LastPosX, m_LastPosY, m_LastPosZ;

	// This variables keep track of the last time a packet was sent
	Int64 m_TimeLastTeleportPacket,m_TimeLastMoveReltPacket,m_TimeLastSpeedPacket;  // In ticks

	bool m_IsInitialized;  // Is set to true when it's initialized, until it's destroyed (Initialize() till Destroy() )

	eEntityType m_EntityType;
	
	cWorld * m_World;
	
	/// Time, in ticks, since the last damage dealt by being on fire. Valid only if on fire (IsOnFire())
	int m_TicksSinceLastBurnDamage;
	
	/// Time, in ticks, since the last damage dealt by standing in lava. Reset to zero when moving out of lava.
	int m_TicksSinceLastLavaDamage;
	
	/// Time, in ticks, since the last damage dealt by standing in fire. Reset to zero when moving out of fire.
	int m_TicksSinceLastFireDamage;
	
	/// Time, in ticks, until the entity extinguishes its fire
	int m_TicksLeftBurning;

	virtual void Destroyed(void) {} // Called after the entity has been destroyed

	void SetWorld(cWorld * a_World) { m_World = a_World; }
	
	friend class cReferenceManager;
	void AddReference( cEntity*& a_EntityPtr );
	void ReferencedBy( cEntity*& a_EntityPtr );
	void Dereference( cEntity*& a_EntityPtr );
	
private:
	// Measured in degrees (MAX 360°)
	double   m_HeadYaw;
	// Measured in meter/second (m/s)
	Vector3d m_Speed;
	// Measured in degrees (MAX 360°)
	Vector3d m_Rot;
	
	/// Position of the entity's XZ center and Y bottom
	Vector3d m_Pos;
	
	// Measured in meter / second
	Vector3d m_WaterSpeed;
	
	// Measured in Kilograms (Kg)
	double m_Mass;
	
	/// Width of the entity, in the XZ plane. Since entities are represented as cylinders, this is more of a diameter.
	double m_Width;
	
	/// Height of the entity (Y axis)
	double m_Height;
} ;  // tolua_export

typedef std::list<cEntity *> cEntityList;




