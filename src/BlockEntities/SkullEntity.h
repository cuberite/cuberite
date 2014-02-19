// SkullEntity.h

// Declares the cSkullEntity class representing a single skull/head in the world





#pragma once

#include "BlockEntity.h"





namespace Json
{
	class Value;
}





// tolua_begin

class cSkullEntity : 
	public cBlockEntity
{
	typedef cBlockEntity super;
	
public:

	// tolua_end
	
	/** Creates a new skull entity at the specified block coords. a_World may be NULL */
	cSkullEntity(int a_BlockX, int a_BlockY, int a_BlockZ, cWorld * a_World);

	bool LoadFromJson( const Json::Value& a_Value );
	virtual void SaveToJson(Json::Value& a_Value ) override;

	// tolua_begin
	
	/** Set the Skull Type */
	void SetSkullType(const eSkullType & a_SkullType);
	
	/** Set the Rotation */
	void SetRotation(eSkullRotation a_Rotation);
	
	/** Set the Player Name for Player Skull */
	void SetOwner(const AString & a_Owner);
	
	/** Get the Skull Type */
	eSkullType GetSkullType(void) const { return m_SkullType; }
	
	/** Get the Rotation */
	eSkullRotation GetRotation(void) const { return m_Rotation; }
	
	/** Get the setted Player Name */
	AString GetOwner(void) const { return m_Owner; }
	
	// tolua_end
	
	virtual void UsedBy(cPlayer * a_Player) override;
	virtual void SendTo(cClientHandle & a_Client) override;

	static const char * GetClassStatic(void) { return "cSkullEntity"; }
	
private:

	eSkullType m_SkullType;
	eSkullRotation m_Rotation;
	AString m_Owner;
} ;  // tolua_export




