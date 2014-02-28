
#pragma once





class cGroup;





class cGroupManager
{
public:
	bool ExistsGroup(const AString & a_Name);
	cGroup * GetGroup(const AString & a_Name);
	void LoadGroups(void);
	void CheckUsers(void);
	
	/** Writes the default header to the specified ini file, and saves it as "users.ini". */
	static void GenerateDefaultUsersIni(cIniFile & a_IniFile);
	
private:
	friend class cRoot;
	cGroupManager();
	~cGroupManager();

	struct sGroupManagerState;
	sGroupManagerState * m_pState;
} ;




