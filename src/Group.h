
#pragma once





class cGroup	// tolua_export
{				// tolua_export
public:			// tolua_export
	cGroup() {}
	~cGroup() {}

	void SetName( AString a_Name ) { m_Name = a_Name; }				// tolua_export
	const AString & GetName() const { return m_Name; }				// tolua_export
	void SetColor( AString a_Color ) { m_Color = a_Color; }			// tolua_export
	void AddCommand( AString a_Command );							// tolua_export
	void AddPermission( AString a_Permission );						// tolua_export
	void InheritFrom( cGroup* a_Group );							// tolua_export

	bool HasCommand( AString a_Command );							// tolua_export

	typedef std::map< AString, bool > PermissionMap;
	const PermissionMap & GetPermissions() const { return m_Permissions; }

	void ClearPermission(void);

	typedef std::map< AString, bool > CommandMap;
	const CommandMap & GetCommands() const { return m_Commands; }

	const AString & GetColor() const { return m_Color; }					// tolua_export

	typedef std::list< cGroup* > GroupList;
	const GroupList & GetInherits() const { return m_Inherits; }
private:
	AString m_Name;
	AString m_Color;

	PermissionMap m_Permissions;
	CommandMap m_Commands;
	GroupList m_Inherits;
};// tolua_export
