#pragma once

#include "MemoryLeak.h"

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

class cChunk;
class cWorld;
class cReferenceManager;
class Vector3d;
class Vector3f;
class cClientHandle;
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

	cWorld* GetWorld() { return m_World; }													//tolua_export

	const Vector3d & GetPosition();															//tolua_export
	const double & GetPosX();																//tolua_export
	const double & GetPosY();																//tolua_export
	const double & GetPosZ();																//tolua_export
	const Vector3f & GetRot();																//tolua_export
	float GetRotation();																	//tolua_export
	float GetPitch();																		//tolua_export
	float GetRoll();																		//tolua_export
	Vector3f GetLookVector();																//tolua_export

	void SetPosX( const double & a_PosX );													//tolua_export
	void SetPosY( const double & a_PosY );													//tolua_export
	void SetPosZ( const double & a_PosZ );													//tolua_export
	void SetPosition( const double & a_PosX, const double & a_PosY, const double & a_PosZ );	//tolua_export
	void SetPosition( const Vector3d & a_Pos );												//tolua_export
	void SetRot( const Vector3f & a_Rot );													//tolua_export
	void SetRotation( float a_Rotation );													//tolua_export
	void SetPitch( float a_Pitch );															//tolua_export
	void SetRoll( float a_Roll );															//tolua_export

	inline int GetUniqueID() { return m_UniqueID; }											//tolua_export
	inline bool IsDestroyed() { return m_bDestroyed; }										//tolua_export

	void Destroy();																			//tolua_export
	void RemoveFromChunk( cChunk* a_Chunk ); // for internal use in cChunk

	virtual void Tick(float a_Dt) = 0;														//tolua_export

	virtual void SpawnOn( cClientHandle* a_Target ) = 0;									//tolua_export
	void WrapRotation();

protected:
	void MoveToCorrectChunk(bool a_bIgnoreOldChunk = false);

	friend class cReferenceManager;
	void AddReference( cEntity*& a_EntityPtr );
	void ReferencedBy( cEntity*& a_EntityPtr );
	void Dereference( cEntity*& a_EntityPtr );

	static int m_EntityCount;
	int m_UniqueID;

	cReferenceManager* m_Referencers;
	cReferenceManager* m_References;

	int m_ChunkX, m_ChunkY, m_ChunkZ;
	Vector3d* m_Pos;
	bool m_bDirtyPosition;

	Vector3f* m_Rot;
	bool m_bDirtyOrientation;

	bool m_bDestroyed;
	bool m_bRemovedFromChunk;

	ENUM_ENTITY_TYPE m_EntityType;
private:
	cWorld* m_World;
}; //tolua_export
