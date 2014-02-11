// YggdrasilResponse.h

#include "json/json.h"

// tolua_begin
class cGameProfile
{
public:
	cGameProfile() :
		m_UUID(""),
		m_Name("")
	{
	}
	
	cGameProfile(AString & a_UUID, AString & a_Name) :
		m_UUID(a_UUID),
		m_Name(a_Name)
	{
	}


	bool Empty(void) const
	{
		return m_UUID.empty() && m_Name.empty();
	}


	AString GetUUID(void) const
	{
		return m_UUID;
	}


	AString GetName(void) const
	{
		return m_Name;
	}
	
	// tolua_end


	static cGameProfile * NewFromJson(const Json::Value & a_Value)
	{
		AString UUID = a_Value.get("id", "").asString();
		AString Name = a_Value.get("name", "Unknown").asString();
		return new cGameProfile(UUID, Name);
	}
	
	
protected:
	AString m_UUID;
	AString m_Name;

} ; // tolua_export
