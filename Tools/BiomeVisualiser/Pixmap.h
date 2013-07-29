
// Pixmap.h

// Declares a cPixmap class that represents a RGB pixmap and allows simple operations on it

#pragma once





class cPixmap
{
public:
	cPixmap(void);
	cPixmap(int a_Width, int a_Height);
	~cPixmap();
	
	void SetSize(int a_Width, int a_Height);
	
	int GetWidth (void) const { return m_Width; }
	int GetHeight(void) const { return m_Height; }
	
	void SetPixel(int a_X, int a_Y, int a_Color);
	int  GetPixel(int a_X, int a_Y) const;
	void Fill(int a_Color);
	
	void DrawToDC(HDC a_DC, int a_OriginX, int a_OriginY);
	
protected:
	int   m_Width;
	int   m_Height;
	int   m_Stride;
	int * m_Pixels;
} ;




