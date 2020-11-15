
#pragma once

#include "ChunkDef.h"





class cChunk;
class cItems;
class cPlayer;
class cWorld;
class cBlockEntity;

using OwnedBlockEntity = std::unique_ptr<cBlockEntity>;
using cBlockEntities = std::unordered_map<size_t, OwnedBlockEntity>;





// tolua_begin
class cBlockEntity
{
protected:
	cBlockEntity(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World) :
		m_Pos(a_Pos),
		m_RelX(a_Pos.x - cChunkDef::Width * FAST_FLOOR_DIV(a_Pos.x, cChunkDef::Width)),
		m_RelZ(a_Pos.z - cChunkDef::Width * FAST_FLOOR_DIV(a_Pos.z, cChunkDef::Width)),
		m_BlockType(a_BlockType),
		m_BlockMeta(a_BlockMeta),
		m_World(a_World)
	{
	}

public:

	// tolua_end

	virtual ~cBlockEntity() {}  // force a virtual destructor in all descendants

	virtual void Destroy() {}

	void SetWorld(cWorld * a_World)
	{
		m_World = a_World;
	}

	/** Updates the internally stored position.
	Note that this should not ever be used for world-contained block entities, it is meant only for when BEs in a cBlockArea are manipulated.
	Asserts that the block entity is not assigned to a world. */
	void SetPos(Vector3i a_NewPos);

	/** Returns true if the specified blocktype is supposed to have an associated block entity. */
	static bool IsBlockEntityBlockType(BLOCKTYPE a_BlockType);

	/** Creates a new block entity for the specified block type at the specified absolute pos.
	If a_World is valid, then the entity is created bound to that world
	Returns nullptr for unknown block types. */
	static OwnedBlockEntity CreateByBlockType(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World = nullptr);

	/** Makes an exact copy of this block entity, except for its m_World (set to nullptr), and at a new position.
	Uses CopyFrom() to copy the properties. */
	OwnedBlockEntity Clone(Vector3i a_Pos);

	/** Returns the contents of this block entity that it would drop if broken.
	Note that the block handler will usually handle pickups for the block itself, in addition to any items returned here. */
	virtual cItems ConvertToPickups() const;

	/** Copies all properties of a_Src into this entity, except for its m_World and location.
	Each non-abstract descendant should override to copy its specific properties, and call
	Super::CopyFrom(a_Src) to copy the common ones. */
	virtual void CopyFrom(const cBlockEntity & a_Src);

	// tolua_begin

	// Position, in absolute block coordinates:
	Vector3i GetPos() const { return m_Pos; }
	int GetPosX() const { return m_Pos.x; }
	int GetPosY() const { return m_Pos.y; }
	int GetPosZ() const { return m_Pos.z; }

	Vector3i GetRelPos() const { return Vector3i(m_RelX, m_Pos.y, m_RelZ); }

	BLOCKTYPE GetBlockType() const { return m_BlockType; }

	cWorld * GetWorld() const { return m_World; }

	int GetChunkX() const { return FAST_FLOOR_DIV(m_Pos.x, cChunkDef::Width); }
	int GetChunkZ() const { return FAST_FLOOR_DIV(m_Pos.z, cChunkDef::Width); }

	int GetRelX() const { return m_RelX; }
	int GetRelZ() const { return m_RelZ; }

	// tolua_end

	/** Called when a player uses this entity; should open the UI window.
	returns true if the use was successful, return false to use the block as a "normal" block */
	virtual bool UsedBy( cPlayer * a_Player) = 0;

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
	Vector3i m_Pos;

	/** Position relative to the chunk, used to speed up ticking */
	int m_RelX, m_RelZ;

	/** The blocktype representing this particular instance in the world.
	Mainly used for multi-block-type entities, such as furnaces / lit furnaces. */
	BLOCKTYPE m_BlockType;

	/** The block meta representing this particular instance in the world
	Mainly used for directional entities, such as dispensers. */
	NIBBLETYPE m_BlockMeta;

	cWorld * m_World;
} ;  // tolua_export
