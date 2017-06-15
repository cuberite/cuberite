// MobHeadEntity.h

// Declares the cMobHeadEntity class representing a single skull / head in the world





#pragma once

#include "BlockEntity.h"
#include "Defines.h"





// tolua_begin

class cMobHeadEntity :
	public cBlockEntity
{
	typedef cBlockEntity Super;

public:

	// tolua_end

	BLOCKENTITY_PROTODEF(cMobHeadEntity)

	/** Creates a new mob head entity at the specified block coords. a_World may be nullptr */
	cMobHeadEntity(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, int a_BlockX, int a_BlockY, int a_BlockZ, cWorld * a_World);

	// tolua_begin

	/** Set the type of the mob head */
	void SetType(const eMobHeadType & a_SkullType);

	/** Set the rotation of the mob head */
	void SetRotation(eMobHeadRotation a_Rotation);

	/** Set the player for mob heads with player type */
	void SetOwner(const cPlayer & a_Owner);

	/** Sets the player components for the mob heads with player type. */
	void SetOwner(const AString & a_OwnerUUID, const AString & a_OwnerName, const AString & a_OwnerTexture, const AString & a_OwnerTextureSignature);

	/** Returns the type of the mob head */
	eMobHeadType GetType(void) const { return m_Type; }

	/** Returns the rotation of the mob head */
	eMobHeadRotation GetRotation(void) const { return m_Rotation; }

	/** Returns the player name of the mob head */
	AString GetOwnerName(void) const { return m_OwnerName; }

	/** Returns the player UUID of the mob head */
	AString GetOwnerUUID(void) const { return m_OwnerUUID; }

	/** Returns the texture of the mob head */
	AString GetOwnerTexture(void) const { return m_OwnerTexture; }

	/** Returns the texture signature of the mob head */
	AString GetOwnerTextureSignature(void) const { return m_OwnerTextureSignature; }

	// tolua_end

	// cBlockEntity overrides:
	virtual void CopyFrom(const cBlockEntity & a_Src) override;
	virtual bool UsedBy(cPlayer * a_Player) override;
	virtual void SendTo(cClientHandle & a_Client) override;

private:

	eMobHeadType m_Type;
	eMobHeadRotation m_Rotation;

	AString m_OwnerName;
	AString m_OwnerUUID;
	AString m_OwnerTexture;
	AString m_OwnerTextureSignature;
} ;  // tolua_export




