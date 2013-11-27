
#pragma once





class cGroup;





class cGroupManager
{
public:
	cGroup * GetGroup(const AString & a_Name);
	
private:
	friend class cRoot;
	cGroupManager();
	~cGroupManager();

	struct sGroupManagerState;
	sGroupManagerState * m_pState;
} ;




