
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "StringMap.h"





unsigned int cStringMap::size() const
{
	return m_StringMap.size();
}

void cStringMap::clear()
{
	m_StringMap.clear();
}

std::string & cStringMap::get( const std::string & index )
{
	return m_StringMap[index];
}