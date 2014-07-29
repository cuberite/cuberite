
#pragma once





// tolua_begin
class cGroup
{
public:
	// tolua_end
	cGroup() {}
	~cGroup() {}

	// tolua_begin
	void SetName( const AString & a_Name) { m_Name = a_Name; }
	const AString & GetName() const { return m_Name; }
	void SetColor( const AString & a_Color) { m_Color = a_Color; }
	void AddCommand( const AString & a_Command);
	void AddPermission( const AString & a_Permission);
	void InheritFrom( cGroup* a_Group);
	// tolua_end

	typedef std::map< AString, bool > PermissionMap;
	const PermissionMap & GetPermissions() const { return m_Permissions; }

	void ClearPermission(void);

	typedef std::map< AString, bool > CommandMap;
	const CommandMap & GetCommands() const { return m_Commands; }

	const AString & GetColor() const { return m_Color; }  // tolua_export

	typedef std::list< cGroup* > GroupList;
	const GroupList & GetInherits() const { return m_Inherits; }
private:
	AString m_Name;
	AString m_Color;

	PermissionMap m_Permissions;
	CommandMap m_Commands;
	GroupList m_Inherits;
};  // tolua_export
