#pragma once

#include <list>

class cEntity;
class cReferenceManager
{
public:
	enum ENUM_REFERENCE_MANAGER_TYPE
	{	
		RFMNGR_REFERENCERS,
		RFMNGR_REFERENCES,
	};
	cReferenceManager( ENUM_REFERENCE_MANAGER_TYPE a_Type );
	~cReferenceManager();

	void AddReference( cEntity*& a_EntityPtr );
	void Dereference( cEntity*& a_EntityPtr );
private:
	ENUM_REFERENCE_MANAGER_TYPE m_Type;
	std::list< cEntity** > m_References;
};