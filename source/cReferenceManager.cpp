
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cReferenceManager.h"
#include "cEntity.h"





cReferenceManager::cReferenceManager( ENUM_REFERENCE_MANAGER_TYPE a_Type )
	: m_Type( a_Type )
{
}

cReferenceManager::~cReferenceManager()
{
	if( m_Type == RFMNGR_REFERENCERS )
	{
		for( std::list< cEntity** >::iterator itr = m_References.begin(); itr != m_References.end(); ++itr )
		{
			*(*itr) = 0; // Set referenced pointer to 0
		}
	}
	else
	{
		for( std::list< cEntity** >::iterator itr = m_References.begin(); itr != m_References.end(); ++itr )
		{
			cEntity* Ptr = (*(*itr));
			if( Ptr ) Ptr->Dereference( *(*itr) );
		}
	}
}

void cReferenceManager::AddReference( cEntity*& a_EntityPtr )
{
	m_References.push_back( &a_EntityPtr );
}

void cReferenceManager::Dereference( cEntity*& a_EntityPtr )
{
	m_References.remove( &a_EntityPtr );
}