
// Map.cpp

#include "Globals.h"

#include "Map.h"

#include "ClientHandle.h"
#include "World.h"
#include "Chunk.h"





cMap::cMap(unsigned int a_ID, cWorld * a_World)
	: m_ID(a_ID)
	, m_Width(128)
	, m_Height(128)
	, m_Scale(3)
	, m_CenterX(0)
	, m_CenterZ(0)
	, m_World(a_World)
{
	m_Data.assign(m_Width * m_Height, 0);
}





cMap::cMap(unsigned int a_ID, int a_CenterX, int a_CenterZ, cWorld * a_World, unsigned int a_Scale)
	: m_ID(a_ID)
	, m_Width(128)
	, m_Height(128)
	, m_Scale(a_Scale)
	, m_CenterX(a_CenterX)
	, m_CenterZ(a_CenterZ)
	, m_World(a_World)
{
	m_Data.assign(m_Width * m_Height, 0);

	for (unsigned int X = 0; X < m_Width; ++X)
	{
		for (unsigned int Y = 0; Y < m_Height; ++Y)
		{
			// Debug
			m_Data[Y + X * m_Height] = rand() % 100;
		}
	}
}





bool cMap::UpdatePixel(unsigned int a_X, unsigned int a_Y)
{
	ASSERT(m_World != NULL);

	cChunk * Chunk = NULL;

	if (Chunk == NULL)
	{
		return false;
	}

	int Height = Chunk->GetHeight(a_X, a_Y);

	// TODO

	return true;
}





void cMap::EraseData(void)
{
	m_Data.assign(m_Width * m_Height, 0);
}





eDimension cMap::GetDimension(void) const
{
	ASSERT(m_World != NULL);
	return m_World->GetDimension();
}






void cMap::Resize(unsigned int a_Width, unsigned int a_Height)
{
	if ((m_Width == a_Width) && (m_Height == a_Height))
	{
		return;
	}

	m_Width = a_Width;
	m_Height = a_Height;

	m_Data.assign(m_Width * m_Height, 0);
}





void cMap::SetPosition(int a_CenterX, int a_CenterZ)
{
	if ((m_CenterX == a_CenterX) && (m_CenterZ == a_CenterZ))
	{
		return;
	}

	m_CenterX = a_CenterX;
	m_CenterZ = a_CenterZ;
}





void cMap::SetScale(unsigned int a_Scale)
{
	if (m_Scale == a_Scale)
	{
		return;
	}

	m_Scale = a_Scale;
}





void cMap::SendTo(cClientHandle & a_Client)
{
	a_Client.SendMapInfo(m_ID, m_Scale);

	for (unsigned int i = 0; i < m_Width; ++i)
	{
		const Byte* Colors = &m_Data[i * m_Height];

		a_Client.SendMapColumn(m_ID, i, 0, Colors, m_Height);
	}
}





unsigned int cMap::GetNumPixels(void) const
{
	return m_Width * m_Height;
}





unsigned int cMap::GetNumBlocksPerPixel(void) const
{
	return pow(2, m_Scale);
}





