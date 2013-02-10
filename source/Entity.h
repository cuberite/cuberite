
#pragma once




#include "Vector3d.h"
#include "Vector3f.h"





// Place this macro in the header of each cEntity descendant class and you're done :)
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
class MTRand;




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
	
	enum eEntityType
	{
		etEntity,  // For all other types
		etPlayer,
		etPickup,
		etMob,
		etFallingBlock,
		
		// Older constants, left over for compatibility reasons (plugins)
		eEntityType_Entity = etEntity,
		eEntityType_Player = etPlayer,
		eEntityType_Pickup = etPickup,
		eEntityType_Mob    = etMob,
	} ;

	cEntity(eEntityType a_EntityType, double a_X, double a_Y, double a_Z);
	virtual ~cEntity();

	virtual void Initialize(cWorld * a_World);

	eEntityType GetEntityType(void) const { return m_EntityType; }
	
	bool IsPlayer(void) const { return (m_EntityType == etPlayer); }
	bool IsPickup(void) const { return (m_EntityType == etPickup); }
	bool IsMob   (void) const { return (m_EntityType == etMob); }
	
	/// Returns true if the entity is of the specified class or a subclass (cPawn's IsA("cEntity") returns true)
	virtual bool IsA(const char * a_ClassName) const;
	
	/// Returns the topmost class name for the object
	virtual const char * GetClass(void) const;

	// Returns the class name of this class
	static const char * GetClassStatic(void);
	
	/// Returns the topmost class's parent class name for the object. cEntity returns an empty string (no parent).
	virtual const char * GetParentClass(void) const;

	cWorld * GetWorld(void) const { return m_World; }

	const Vector3d & GetPosition  (void) const {return m_Pos; }
	double           GetPosX      (void) const {return m_Pos.x; }
	double           GetPosY      (void) const {return m_Pos.y; }
	double           GetPosZ      (void) const {return m_Pos.z; }
	const Vector3f & GetRot       (void) const {return m_Rot; }
	float            GetRotation  (void) const {return m_Rot.x; }
	float            GetPitch     (void) const {return m_Rot.y; }
	float            GetRoll      (void) const {return m_Rot.z; }
	Vector3f         GetLookVector(void) const;
	
	int GetChunkX(void) const {return m_ChunkX; }
	int GetChunkY(void) const {return m_ChunkY; }
	int GetChunkZ(void) const {return m_ChunkZ; }

	void SetPosX    (double a_PosX);
	void SetPosY    (double a_PosY);
	void SetPosZ    (double a_PosZ);
	void SetPosition(double a_PosX, double a_PosY, double a_PosZ);
	void SetPosition(const Vector3d & a_Pos);
	void SetRot     (const Vector3f & a_Rot);
	void SetRotation(float a_Rotation);
	void SetPitch   (float a_Pitch);
	void SetRoll    (float a_Roll);
	// tolua_end

	inline int  GetUniqueID(void) const { return m_UniqueID; }								// tolua_export
	inline bool IsDestroyed(void) const { return m_bDestroyed; }							// tolua_export

	void Destroy();																			// tolua_export
	void RemoveFromChunk(void); // for internal use in cChunk

	virtual void Tick(float a_Dt, MTRand & a_TickRandom);														// tolua_export
	virtual void HandlePhysics(float a_Dt) {}  // tolua_export

	/** Descendants override this function to send a command to the specified client to spawn the entity on the client.
	To spawn on all eligible clients, use cChunkMap::BroadcastSpawnEntity()
	Needs to have a default implementation due to Lua bindings.
	*/
	virtual void SpawnOn(cClientHandle & a_Client) {ASSERT(!"SpawnOn() unimplemented!"); }  // tolua_export
	
	void WrapRotation();
	
	// Metadata flags; descendants may override the defaults:
	virtual bool IsOnFire   (void) const {return (m_BurnPeriod > 0); }
	virtual bool IsCrouched (void) const {return false; }
	virtual bool IsRiding   (void) const {return false; }
	virtual bool IsSprinting(void) const {return false; }
	virtual bool IsRclking  (void) const {return false; }

protected:
	virtual void Destroyed() {} // Called after the entity has been destroyed

	void SetWorld( cWorld* a_World ) { m_World = a_World; }
	void MoveToCorrectChunk(bool a_bIgnoreOldChunk = false);

	friend class cReferenceManager;
	void AddReference( cEntity*& a_EntityPtr );
	void ReferencedBy( cEntity*& a_EntityPtr );
	void Dereference( cEntity*& a_EntityPtr );

	static cCriticalSection m_CSCount;
	static int m_EntityCount;
	
	int m_UniqueID;

	cReferenceManager* m_Referencers;
	cReferenceManager* m_References;

	int m_ChunkX, m_ChunkY, m_ChunkZ;
	Vector3d m_Pos;
	bool m_bDirtyPosition;

	Vector3f m_Rot;
	bool m_bDirtyOrientation;

	bool m_bDestroyed;
	bool m_bRemovedFromChunk;

	eEntityType m_EntityType;
	
	cWorld* m_World;
	
	float m_FireDamageInterval;
	float m_BurnPeriod;
}; // tolua_export

typedef std::list<cEntity *> cEntityList;




