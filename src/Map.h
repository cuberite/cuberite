
// Map.h

// Implementation of in-game coloured maps





#pragma once

#include "Defines.h"
#include "ChunkDef.h"




class cClientHandle;
class cWorld;
class cPlayer;
class cMap;





/** Encapsulates a map decorator.
A map decorator represents an object drawn on the map that can move freely.
(e.g. player trackers and item frame pointers)

Excluding manually placed decorators,
decorators are automatically managed (allocated and freed) by their parent cMap instance.
*/
struct cMapDecorator
{
public:

	enum class eType
	{
		E_TYPE_PLAYER         = 0x00,
		E_TYPE_ITEM_FRAME     = 0x01,

		/** Player outside of the boundaries of the map. */
		E_TYPE_PLAYER_OUTSIDE = 0x06
	};

	cMapDecorator(eType a_Type, unsigned int a_X, unsigned int a_Z, int a_Rot) :
		m_Type(a_Type),
		m_PixelX(a_X),
		m_PixelZ(a_Z),
		m_Rot(a_Rot)
	{
	}

public:

	unsigned int GetPixelX(void) const { return m_PixelX; }
	unsigned int GetPixelZ(void) const { return m_PixelZ; }

	int GetRot(void) const { return m_Rot; }

	eType GetType(void) const { return m_Type; }

private:

	eType m_Type;

	unsigned int m_PixelX;
	unsigned int m_PixelZ;

	int m_Rot;

};





// tolua_begin

/** Encapsulates an in-game world map. */
class cMap
{
public:

	enum eBaseColor
	{
		E_BASE_COLOR_TRANSPARENT = 0,  /* Air     */
		E_BASE_COLOR_LIGHT_GREEN = 4,  /* Grass   */
		E_BASE_COLOR_LIGHT_BLUE = 5,
		E_BASE_COLOR_LIGHT_BROWN = 8,  /* Sand    */
		E_BASE_COLOR_GRAY_1      = 12, /* Cloth   */
		E_BASE_COLOR_RED         = 16, /* TNT     */
		E_BASE_COLOR_PALE_BLUE   = 20, /* Ice     */
		E_BASE_COLOR_GRAY_2      = 24, /* Iron    */
		E_BASE_COLOR_DARK_GREEN  = 28, /* Foliage */
		E_BASE_COLOR_WHITE       = 32, /* Snow    */
		E_BASE_COLOR_LIGHT_GRAY  = 36, /* Clay    */
		E_BASE_COLOR_BROWN       = 40, /* Dirt    */
		E_BASE_COLOR_DARK_GRAY   = 44, /* Stone   */
		E_BASE_COLOR_BLUE        = 48, /* Water   */
		E_BASE_COLOR_DARK_BROWN  = 52  /* Wood    */
	};

	typedef Byte ColorID;

	// tolua_end

	typedef std::vector<ColorID> cColorList;
	typedef std::vector<cClientHandle *> cMapClientList;
	typedef std::vector<cMapDecorator> cMapDecoratorList;

	/** Construct an empty map. */
	cMap(unsigned int a_ID, cWorld * a_World);

	/** Construct an empty map at the specified coordinates. */
	cMap(unsigned int a_ID, int a_CenterX, int a_CenterZ, cWorld * a_World, unsigned int a_Scale = 3);

	/** Sends a map update to all registered clients
	Clears the list holding registered clients and decorators */
	void Tick();

	/** Update a circular region with the specified radius and center (in pixels). */
	void UpdateRadius(int a_PixelX, int a_PixelZ, unsigned int a_Radius);

	/** Update a circular region around the specified player. */
	void UpdateRadius(cPlayer & a_Player, unsigned int a_Radius);

	/** Send next update packet to the specified player and remove invalid decorators / clients. */
	void UpdateClient(cPlayer * a_Player);

	// tolua_begin

	void Resize(unsigned int a_Width, unsigned int a_Height);

	void SetPosition(int a_CenterX, int a_CenterZ);

	void SetScale(unsigned int a_Scale) { m_Scale = a_Scale; }

	bool SetPixel(unsigned int a_X, unsigned int a_Z, ColorID a_Data);

	ColorID GetPixel(unsigned int a_X, unsigned int a_Z);

	unsigned int GetWidth (void) const { return m_Width;  }
	unsigned int GetHeight(void) const { return m_Height; }

	unsigned int GetScale(void) const { return m_Scale; }

	int GetCenterX(void) const { return m_CenterX; }
	int GetCenterZ(void) const { return m_CenterZ; }

	unsigned int GetID(void) const { return m_ID; }

	cWorld * GetWorld(void) { return m_World; }

	AString GetName(void) { return m_Name; }

	eDimension GetDimension(void) const;

	unsigned int GetNumPixels(void) const;

	unsigned int GetPixelWidth(void) const;

	// tolua_end

	const cMapDecorator CreateDecorator(const cEntity * a_TrackedEntity);

	const cMapDecoratorList GetDecorators(void) const { return m_Decorators; }

	const cColorList & GetData(void) const { return m_Data; }

	static const char * GetClassStatic(void)  // Needed for ManualBindings's DoWith templates
	{
		return "cMap";
	}

	const char * GetClass(void)  // Needed for ManualBindings' DoWith templates
	{
		return "cMap";
	}


private:

	/** Update the specified pixel. */
	bool UpdatePixel(unsigned int a_X, unsigned int a_Z);

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

	cMapClientList m_ClientsInCurrentTick;

	cMapDecoratorList m_Decorators;

	AString m_Name;

	friend class cMapSerializer;

};  // tolua_export



