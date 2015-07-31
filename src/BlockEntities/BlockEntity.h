
#pragma once





/** Place this macro in the declaration of each cBlockEntity descendant. */
#define BLOCKENTITY_PROTODEF(classname) \
	virtual bool IsA(const char * a_ClassName) const override \
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





class cChunk;
class cPlayer;
class cWorld;





// tolua_begin
class cBlockEntity
{
protected:
	cBlockEntity(BLOCKTYPE a_BlockType, int a_BlockX, int a_BlockY, int a_BlockZ, cWorld * a_World) :
		m_PosX(a_BlockX),
		m_PosY(a_BlockY),
		m_PosZ(a_BlockZ),
		m_RelX(a_BlockX - cChunkDef::Width * FAST_FLOOR_DIV(a_BlockX, cChunkDef::Width)),
		m_RelZ(a_BlockZ - cChunkDef::Width * FAST_FLOOR_DIV(a_BlockZ, cChunkDef::Width)),
		m_BlockType(a_BlockType),
		m_World(a_World)
	{
	}

public:
	// tolua_end
	
	virtual ~cBlockEntity() {}  // force a virtual destructor in all descendants
	
	virtual void Destroy(void) {}
	
	void SetWorld(cWorld * a_World)
	{
		m_World = a_World;
	}
	
	/** Creates a new block entity for the specified block type
	If a_World is valid, then the entity is created bound to that world
	Returns nullptr for unknown block types. */
	static cBlockEntity * CreateByBlockType(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, int a_BlockX, int a_BlockY, int a_BlockZ, cWorld * a_World = nullptr);
	
	static const char * GetClassStatic(void)  // Needed for ManualBindings's ForEach templates
	{
		return "cBlockEntity";
	}

	/** Returns true if the object is the specified class, or its descendant. */
	virtual bool IsA(const char * a_ClassName) const { return (strcmp(a_ClassName, "cBlockEntity") == 0); }

	/** Returns the name of the tompost class (the most descendant). Used for Lua bindings to push the correct object type. */
	virtual const char * GetClass(void) const { return GetClassStatic(); }

	/** Returns the name of the parent class, or empty string if no parent class. */
	virtual const char * GetParentClass(void) const { return ""; }
	
	// tolua_begin
	
	// Position, in absolute block coordinates:
	Vector3i GetPos(void) const { return Vector3i{m_PosX, m_PosY, m_PosZ}; }
	int GetPosX(void) const { return m_PosX; }
	int GetPosY(void) const { return m_PosY; }
	int GetPosZ(void) const { return m_PosZ; }

	BLOCKTYPE GetBlockType(void) const { return m_BlockType; }
	
	cWorld * GetWorld(void) const { return m_World; }
	
	int GetChunkX(void) const { return FAST_FLOOR_DIV(m_PosX, cChunkDef::Width); }
	int GetChunkZ(void) const { return FAST_FLOOR_DIV(m_PosZ, cChunkDef::Width); }
	
	int GetRelX(void) const { return m_RelX; }
	int GetRelZ(void) const { return m_RelZ; }
	
	// tolua_end
	
	/** Called when a player uses this entity; should open the UI window */
	virtual void UsedBy( cPlayer * a_Player) = 0;
	
	/** Sends the packet defining the block entity to the client specified.
	To send to all eligible clients, use cWorld::BroadcastBlockEntity() */
	virtual void SendTo(cClientHandle & a_Client) = 0;
	
	/** Ticks the entity; returns true if the chunk should be marked as dirty as a result of this ticking. By default does nothing. */
	virtual bool Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
	{
		UNUSED(a_Dt);
		return false;
	}

protected:
	/** Position in absolute block coordinates */
	int m_PosX, m_PosY, m_PosZ;
	
	/** Position relative to the chunk, used to speed up ticking */
	int m_RelX, m_RelZ;

	BLOCKTYPE m_BlockType;
	
	cWorld * m_World;
} ;  // tolua_export




