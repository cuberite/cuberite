
// ImageComposingCallback.cpp

// Implements the cImageComposingCallback class that implements a subset of cCallback for composing per-region images

#include "Globals.h"
#include "ImageComposingCallback.h"





cImageComposingCallback::cImageComposingCallback(const AString & a_FileNamePrefix) :
	m_FileNamePrefix(a_FileNamePrefix),
	m_CurrentRegionX(INVALID_REGION_COORD),
	m_CurrentRegionZ(INVALID_REGION_COORD),
	m_ImageData(new int[32 * 16 * 32 * 16])
{
}





cImageComposingCallback::~cImageComposingCallback()
{
	delete[] m_ImageData;
}





bool cImageComposingCallback::OnNewRegion(int a_RegionX, int a_RegionZ)
{
	ASSERT(m_CurrentRegionX == INVALID_REGION_COORD);
	ASSERT(m_CurrentRegionZ == INVALID_REGION_COORD);  // Has any previous region been finished properly?

	m_CurrentRegionX = a_RegionX;
	m_CurrentRegionZ = a_RegionZ;
	OnEraseImage();

	return CALLBACK_CONTINUE;
}





void cImageComposingCallback::OnRegionFinished(int a_RegionX, int a_RegionZ)
{
	ASSERT(m_CurrentRegionX != INVALID_REGION_COORD);
	ASSERT(m_CurrentRegionZ != INVALID_REGION_COORD);  // Has a region been started properly?
	ASSERT(m_CurrentRegionX == a_RegionX);
	ASSERT(m_CurrentRegionZ == a_RegionZ);  // Is it the same region that has been started?

	AString FileName = GetFileName(a_RegionX, a_RegionZ);
	if (!FileName.empty())
	{
		OnBeforeImageSaved(a_RegionX, a_RegionZ, FileName);
		SaveImage(FileName);
		OnAfterImageSaved(a_RegionX, a_RegionZ, FileName);
	}

	m_CurrentRegionX = INVALID_REGION_COORD;
	m_CurrentRegionZ = INVALID_REGION_COORD;
}





AString cImageComposingCallback::GetFileName(int a_RegionX, int a_RegionZ)
{
	return Printf("%s.%d.%d.bmp", m_FileNamePrefix.c_str(), a_RegionX, a_RegionZ);
}





void cImageComposingCallback::OnEraseImage(void)
{
	// By default erase the image to black:
	EraseImage(0);
}





void cImageComposingCallback::EraseImage(int a_Color)
{
	for (int i = 0; i < PIXEL_COUNT; i++)
	{
		m_ImageData[i] = a_Color;
	}
}





void cImageComposingCallback::EraseChunk(int a_Color, int a_RelChunkX, int a_RelChunkZ)
{
	int Base = a_RelChunkZ * IMAGE_HEIGHT + a_RelChunkX * 16;
	for (int v = 0; v < 16; v++)
	{
		int BaseV = Base + v * IMAGE_HEIGHT;
		for (int u = 0; u < 16; u++)
		{
			m_ImageData[BaseV + u] = a_Color;
		}
	}  // for y
}





void cImageComposingCallback::SetPixel(int a_RelU, int a_RelV, int a_Color)
{
	ASSERT((a_RelU >= 0) && (a_RelU < IMAGE_WIDTH));
	ASSERT((a_RelV >= 0) && (a_RelV < IMAGE_HEIGHT));

	m_ImageData[a_RelU + IMAGE_WIDTH * a_RelV] = a_Color;
}





int cImageComposingCallback::GetPixel(int a_RelU, int a_RelV)
{
	if ((a_RelU < 0) || (a_RelU >= IMAGE_WIDTH) || (a_RelV < 0) || (a_RelV >= IMAGE_HEIGHT))
	{
		// Outside the image data
		return -1;
	}

	return m_ImageData[a_RelU + IMAGE_WIDTH * a_RelV];
}






void cImageComposingCallback::SetPixelURow(int a_RelUStart, int a_RelV, int a_CountU, int * a_Pixels)
{
	ASSERT((a_RelUStart >= 0) && (a_RelUStart + a_CountU <= IMAGE_WIDTH));
	ASSERT((a_RelV >= 0) && (a_RelV < IMAGE_HEIGHT));
	ASSERT(a_Pixels != NULL);

	int Base = a_RelUStart + a_RelV * IMAGE_WIDTH;
	for (int u = 0; u < a_CountU; u++)
	{
		m_ImageData[Base + u] = a_Pixels[u];
	}
}





int cImageComposingCallback::ShadeColor(int a_Color, int a_Shade)
{
	if (a_Shade < 64)
	{
		return MixColor(0, a_Color, a_Shade * 4);
	}
	return MixColor(a_Color, 0xffffff, (a_Shade - 64) * 4);
}





int cImageComposingCallback::MixColor(int a_Src, int a_Dest, int a_Amount)
{
	int r = a_Src & 0xff;
	int g = (a_Src >> 8) & 0xff;
	int b = (a_Src >> 16) & 0xff;
	int rd = a_Dest & 0xff;
	int gd = (a_Dest >> 8) & 0xff;
	int bd = (a_Dest >> 16) & 0xff;
	int nr = r + (rd - r) * a_Amount / 256;
	int ng = g + (gd - g) * a_Amount / 256;
	int nb = b + (bd - b) * a_Amount / 256;
	return nr | (ng << 8) | (nb << 16);
}




void cImageComposingCallback::SaveImage(const AString & a_FileName)
{
	cFile f(a_FileName, cFile::fmWrite);
	if (!f.IsOpen())
	{
		return;
	}

	// Header for BMP files (is the same for the same-size files)
	static const unsigned char BMPHeader[] =
	{
		0x42, 0x4D, 0x36, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00,
		0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0xfe, 0xff, 0xff, 0x01, 0x00, 0x20, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x13, 0x0B, 0x00, 0x00, 0x13, 0x0B, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	} ;

	f.Write(BMPHeader, sizeof(BMPHeader));
	f.Write(m_ImageData, PIXEL_COUNT * 4);
}
