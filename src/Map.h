
// Map.h

// Implementation of in-game coloured maps





#pragma once





#include "BlockID.h"





class cClientHandle;
class cWorld;





// tolua_begin
class cMap
{
public:

	typedef Byte ColorID;

	// tolua_end

	typedef std::vector<ColorID> cColorList;


public:

	/** Construct an empty map. */
	cMap(unsigned int a_ID, cWorld * a_World);

	cMap(unsigned int a_ID, int a_CenterX, int a_CenterZ, cWorld * a_World, unsigned int a_Scale = 3);

	/** Send this map to the specified client. */
	void SendTo(cClientHandle & a_Client);

	// tolua_begin

	/** Erase pixel data */
	void EraseData(void);

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

	// tolua_end


private:

	/** Update the specified pixel. */
	bool UpdatePixel(unsigned int a_X, unsigned int a_Y);

	void PixelToWorldCoords(unsigned int a_X, unsigned int a_Y, int & a_WorldX, int & a_WorldY);

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
