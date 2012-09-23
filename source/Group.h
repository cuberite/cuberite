
#pragma once





class cGroup	//tolua_export
{				//tolua_export
public:			//tolua_export
	cGroup() {}
	~cGroup() {}

	void SetName( std::string a_Name ) { m_Name = a_Name; }				//tolua_export
	const std::string & GetName() const { return m_Name; }				//tolua_export
	void SetColor( std::string a_Color ) { m_Color = a_Color; }			//tolua_export
	void AddCommand( std::string a_Command );							//tolua_export
	void AddPermission( std::string a_Permission );						//tolua_export
	void InheritFrom( cGroup* a_Group );								//tolua_export

	bool HasCommand( std::string a_Command );							//tolua_export

	typedef std::map< std::string, bool > PermissionMap;
	const PermissionMap & GetPermissions() const { return m_Permissions; }

	typedef std::map< std::string, bool > CommandMap;
	const CommandMap & GetCommands() const { return m_Commands; }

	const AString & GetColor() const { return m_Color; }					//tolua_export

	typedef std::list< cGroup* > GroupList;
	const GroupList & GetInherits() const { return m_Inherits; }
private:
	std::string m_Name;
	std::string m_Color;

	PermissionMap m_Permissions;
	CommandMap m_Commands;
	GroupList m_Inherits;
};//tolua_export