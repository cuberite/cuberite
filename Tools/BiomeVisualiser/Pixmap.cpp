
// Pixmap.cpp

// Implements the cPixmap class that represents a RGB pixmap and allows simple operations on it

#include "Globals.h"
#include "Pixmap.h"





cPixmap::cPixmap(void) :
	m_Width(0),
	m_Height(0),
	m_Stride(0),
	m_Pixels(NULL)
{
}





cPixmap::cPixmap(int a_Width, int a_Height) :
	m_Width(0),
	m_Height(0),
	m_Stride(0),
	m_Pixels(NULL)
{
	SetSize(a_Width, a_Height);
}





cPixmap::~cPixmap()
{
	delete m_Pixels;
}





void cPixmap::SetSize(int a_Width, int a_Height)
{
	delete m_Pixels;
	m_Pixels = new int[a_Width * a_Height];
	m_Width = a_Width;
	m_Height = a_Height;
	m_Stride = m_Width;  // Currently we don't need a special stride value, but let's support it for the future :)
}





void cPixmap::SetPixel(int a_X, int a_Y, int a_Color)
{
	ASSERT(a_X >= 0);
	ASSERT(a_X < m_Width);
	ASSERT(a_Y >= 0);
	ASSERT(a_Y < m_Height);
	
	m_Pixels[a_X + a_Y * m_Stride] = a_Color;
}





int cPixmap::GetPixel(int a_X, int a_Y) const
{
	ASSERT(a_X >= 0);
	ASSERT(a_X < m_Width);
	ASSERT(a_Y >= 0);
	ASSERT(a_Y < m_Height);
	
	return m_Pixels[a_X + a_Y * m_Stride];
}





void cPixmap::Fill(int a_Color)
{
	int NumElements = m_Height * m_Stride;
	for (int i = 0; i < NumElements; i++)
	{
		m_Pixels[i] = a_Color;
	}
}





void cPixmap::DrawToDC(HDC a_DC, int a_OriginX, int a_OriginY)
{
	BITMAPINFO bmi;
	bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
	bmi.bmiHeader.biWidth = m_Width;
	bmi.bmiHeader.biHeight = -m_Height;  // Negative, we are top-down, unlike BMPs
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biSizeImage = m_Stride * m_Height * 4;
	bmi.bmiHeader.biXPelsPerMeter = 1440;
	bmi.bmiHeader.biYPelsPerMeter = 1440;
	bmi.bmiHeader.biClrUsed = 0;
	bmi.bmiHeader.biClrImportant = 0;
	SetDIBitsToDevice(a_DC, a_OriginX, a_OriginY, m_Width, m_Height, 0, 0, 0, m_Height, m_Pixels, &bmi, DIB_RGB_COLORS);
}




