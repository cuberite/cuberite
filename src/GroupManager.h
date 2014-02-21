
#pragma once





class cGroup;





class cGroupManager
{
public:
	bool ExistsGroup(const AString & a_Name);
	cGroup * GetGroup(const AString & a_Name);
	void LoadGroups(void);
	void CheckUsers(void);
	
private:
	friend class cRoot;
	cGroupManager();
	~cGroupManager();

	struct sGroupManagerState;
	sGroupManagerState * m_pState;
} ;




