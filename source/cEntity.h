
#pragma once




#include "Vector3d.h"
#include "Vector3f.h"





#define CLASS_PROT_ISA() virtual bool IsA( const char* a_EntityType );
#define CLASS_PROT_GETCLASS() virtual const char* GetClass();

/* Can't use this (yet) because of tolua */
#define CLASS_PROTOTYPE() \
	CLASS_PROT_ISA(); \
	CLASS_PROT_GETCLASS(); 

#define CLASS_DEF_ISA( classname, superclass ) \
	bool classname::IsA( const char* a_EntityType ) \
	{ \
		if( strcmp( a_EntityType, #classname ) == 0 ) return true; \
		return superclass::IsA( a_EntityType ); \
	}

#define CLASS_DEF_GETCLASS( classname ) \
	const char* classname::GetClass() \
	{ \
	return #classname; \
	}

#define CLASS_DEFINITION( classname, superclass ) \
	CLASS_DEF_ISA( classname, superclass ) \
	CLASS_DEF_GETCLASS( classname )





class cWorld;
class cReferenceManager;
class cClientHandle;
class cPacket;





class cEntity																				//tolua_export
{																							//tolua_export
public:																						//tolua_export
	cEntity(const double & a_X, const double & a_Y, const double & a_Z);					//tolua_export
	virtual ~cEntity();																		//tolua_export

	virtual void Initialize( cWorld* a_World );												//tolua_export

	enum ENUM_ENTITY_TYPE																	//tolua_export
	{																						//tolua_export
		E_ENTITY,																			//tolua_export
		E_PLAYER,																			//tolua_export
		E_PICKUP																			//tolua_export
	};																						//tolua_export

	virtual unsigned int GetEntityType() { return m_EntityType; }							//tolua_export
	virtual bool IsA( const char* a_EntityType );											//tolua_export
	virtual const char* GetClass();															//tolua_export

	cWorld * GetWorld(void) const { return m_World; }										//tolua_export

	const Vector3d & GetPosition(void) const {return m_Pos; }								//tolua_export
	const double &   GetPosX    (void) const {return m_Pos.x; }								//tolua_export
	const double &   GetPosY    (void) const {return m_Pos.y; }								//tolua_export
	const double &   GetPosZ    (void) const {return m_Pos.z; }								//tolua_export
	const Vector3f & GetRot     (void) const {return m_Rot; }								//tolua_export
	float GetRotation(void) const {return m_Rot.x; }										//tolua_export
	float GetPitch   (void) const {return m_Rot.y; }										//tolua_export
	float GetRoll    (void) const {return m_Rot.z; }										//tolua_export
	Vector3f GetLookVector();																//tolua_export
	
	int GetChunkX(void) const {return m_ChunkX; }
	int GetChunkY(void) const {return m_ChunkY; }
	int GetChunkZ(void) const {return m_ChunkZ; }

	void SetPosX( const double & a_PosX );													//tolua_export
	void SetPosY( const double & a_PosY );													//tolua_export
	void SetPosZ( const double & a_PosZ );													//tolua_export
	void SetPosition( const double & a_PosX, const double & a_PosY, const double & a_PosZ );	//tolua_export
	void SetPosition( const Vector3d & a_Pos );												//tolua_export
	void SetRot( const Vector3f & a_Rot );													//tolua_export
	void SetRotation( float a_Rotation );													//tolua_export
	void SetPitch( float a_Pitch );															//tolua_export
	void SetRoll( float a_Roll );															//tolua_export

	inline int  GetUniqueID(void) const { return m_UniqueID; }											//tolua_export
	inline bool IsDestroyed(void) const { return m_bDestroyed; }										//tolua_export

	void Destroy();																			//tolua_export
	void RemoveFromChunk(void); // for internal use in cChunk

	virtual void Tick(float a_Dt) = 0;														//tolua_export

	virtual cPacket * GetSpawnPacket(void) const {ASSERT(!"GetSpawnedPacket unimplemented!"); return NULL; };  // _X: Needs to be implemented due to Lua bindings
	void SpawnOn (cClientHandle * a_Client);  // tolua_export
	
	void WrapRotation();

protected:

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

	ENUM_ENTITY_TYPE m_EntityType;
	
	cWorld* m_World;
}; //tolua_export

typedef std::list<cEntity *> cEntityList;




