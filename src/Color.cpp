#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Color.h"





#define COLOR_RED_BITS     0x00FF0000
#define COLOR_GREEN_BITS   0x0000FF00
#define COLOR_BLUE_BITS    0x000000FF
#define COLOR_RED_OFFSET   16
#define COLOR_GREEN_OFFSET 8





void cColor::SetColor(unsigned char a_Red, unsigned char a_Green, unsigned char a_Blue)
{
	m_Color = (static_cast<unsigned int>(a_Red) << COLOR_RED_OFFSET) + (static_cast<unsigned int>(a_Green) << COLOR_GREEN_OFFSET) + (static_cast<unsigned int>(a_Blue));
}





void cColor::SetRed(unsigned char a_Red)
{
	m_Color = (static_cast<unsigned int>(a_Red) << COLOR_RED_OFFSET) + ((COLOR_GREEN_BITS | COLOR_BLUE_BITS) & m_Color);
}





void cColor::SetGreen(unsigned char a_Green)
{
	m_Color = (static_cast<unsigned int>(a_Green) << COLOR_GREEN_OFFSET) + ((COLOR_RED_BITS | COLOR_BLUE_BITS) & m_Color);
}





void cColor::SetBlue(unsigned char a_Blue)
{
	m_Color = static_cast<unsigned int>(a_Blue) + ((COLOR_RED_BITS | COLOR_GREEN_BITS) & m_Color);
}





unsigned char cColor::GetRed() const
{
	return (m_Color & COLOR_RED_BITS) >> COLOR_RED_OFFSET;
}





unsigned char cColor::GetGreen() const
{
	return (m_Color & COLOR_GREEN_BITS) >> COLOR_GREEN_OFFSET;
}





unsigned char cColor::GetBlue() const
{
	return m_Color & COLOR_BLUE_BITS;
}
