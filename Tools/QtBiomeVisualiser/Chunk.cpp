#include "Globals.h"
#include "Globals.h"
#include "Chunk.h"





Chunk::Chunk() :
	m_IsValid(false)
{
}





const uchar * Chunk::getImage(void) const
{
	ASSERT(m_IsValid);
	return m_Image;
}





void Chunk::setImage(const Image & a_Image)
{
	memcpy(m_Image, a_Image, sizeof(a_Image));
	m_IsValid = true;
}




