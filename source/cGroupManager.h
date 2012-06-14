#pragma once

class cGroup;
class cGroupManager
{
public:
	static cGroupManager * GetGroupManager();	//tolua_export

	cGroup* GetGroup( const char* a_Name );
private:
	friend class cRoot;
	cGroupManager();
	~cGroupManager();

	struct sGroupManagerState;
	sGroupManagerState* m_pState;
};