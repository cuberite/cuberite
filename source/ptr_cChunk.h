#pragma once

#include "cChunk.h"

class ptr_cChunk
{
public:
	ptr_cChunk( cChunk* a_Ptr )
		: m_Ptr( a_Ptr )
	{
		if( m_Ptr )	m_Ptr->AddReference();
	}

	ptr_cChunk( const ptr_cChunk& a_Clone )
		: m_Ptr( a_Clone.m_Ptr )
	{
		if( m_Ptr )	m_Ptr->AddReference();
	}

	~ptr_cChunk()
	{
		if( m_Ptr ) m_Ptr->RemoveReference();
	}

	cChunk* operator-> ()
	{    
		return m_Ptr;
	}

	cChunk& operator* () { return *m_Ptr; }
	bool operator!() { return !m_Ptr; }
	bool operator==( const ptr_cChunk& a_Other ) { return m_Ptr == a_Other.m_Ptr; }
	operator bool() { return m_Ptr != 0; }
	operator cChunk*() { return m_Ptr; }
private:
	cChunk* m_Ptr;
};