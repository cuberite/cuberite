// MobHeadEntity.h

// Declares the cMobHeadEntity class representing a single skull / head in the world





#pragma once

#include "BlockEntity.h"
#include "Defines.h"





// tolua_begin

class cMobHeadEntity :
	public cBlockEntity
{
	typedef cBlockEntity super;
	
public:

	// tolua_end
	
	BLOCKENTITY_PROTODEF(cMobHeadEntity)
	
	/** Creates a new mob head entity at the specified block coords. a_World may be nullptr */
	cMobHeadEntity(int a_BlockX, int a_BlockY, int a_BlockZ, cWorld * a_World);

	// tolua_begin
	
	/** Set the type of the mob head */
	void SetType(const eMobHeadType & a_SkullType);
	
	/** Set the rotation of the mob head */
	void SetRotation(eMobHeadRotation a_Rotation);
	
	/** Set the player name for mob heads with player type */
	void SetOwner(const AString & a_Owner);
	
	/** Returns the type of the mob head */
	eMobHeadType GetType(void) const { return m_Type; }
	
	/** Returns the rotation of the mob head */
	eMobHeadRotation GetRotation(void) const { return m_Rotation; }
	
	/** Returns the player name of the mob head */
	AString GetOwner(void) const { return m_Owner; }
	
	// tolua_end
	
	virtual bool UsedBy(cPlayer * a_Player) override;
	virtual void SendTo(cClientHandle & a_Client) override;

private:

	eMobHeadType m_Type;
	eMobHeadRotation m_Rotation;
	AString m_Owner;
} ;  // tolua_export




