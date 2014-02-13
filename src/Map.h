
// Map.h

// Implementation of in-game coloured maps





#pragma once





#include "BlockID.h"





class cClientHandle;
class cWorld;





class cMap
{
public:

	typedef Byte ColorID;

	typedef std::vector<ColorID> cColorList;


public:

	cMap(unsigned int a_ID, int a_CenterX, int a_CenterZ, cWorld * a_World, unsigned int a_Scale = 3);

	/** Update the map (Query the world) */
	void UpdateMap(void);

	/** Send this map to the specified client. */
	void SendTo(cClientHandle & a_Client);

	void Resize(unsigned int a_Width, unsigned int a_Height);

	void SetPosition(int a_CenterX, int a_CenterZ);

	void SetScale(unsigned int a_Scale);

	unsigned int GetWidth (void) const { return m_Width;  }
	unsigned int GetHeight(void) const { return m_Height; }

	unsigned int GetScale(void) const { return m_Scale; }

	int GetCenterX(void) const { return m_CenterX; }
	int GetCenterZ(void) const { return m_CenterZ; }

	unsigned int GetID(void) const { return m_ID; }

	cWorld * GetWorld(void) { return m_World; }

	eDimension GetDimension(void) const;

	const cColorList & GetData(void) const { return m_Data; }

	unsigned int GetNumPixels(void) const;

	unsigned int GetNumBlocksPerPixel(void) const;


private:

	unsigned int m_ID;

	unsigned int m_Width;
	unsigned int m_Height;

	/** The zoom level, 2^scale square blocks per pixel */
	unsigned int m_Scale;

	int m_CenterX;
	int m_CenterZ;

	/** Column-major array of colours */
	cColorList m_Data;

	cWorld * m_World;

	friend class cMapSerializer;

};
