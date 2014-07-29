
// Map.h

// Implementation of in-game coloured maps





#pragma once





#include "BlockID.h"





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
class cMapDecorator
{
public:

	enum eType
	{
		E_TYPE_PLAYER         = 0x00,
		E_TYPE_ITEM_FRAME     = 0x01,

		/** Player outside of the boundaries of the map. */
		E_TYPE_PLAYER_OUTSIDE = 0x06
	};


public:

	/** Constructs a map decorator fixed at the specified pixel coordinates. (DEBUG) */
	cMapDecorator(cMap * a_Map, eType a_Type, int a_X, int a_Z, int a_Rot);

	/** Constructs a map decorator that tracks a player. */
	cMapDecorator(cMap * a_Map, cPlayer * a_Player);

	/** Updates the decorator. */
	void Update(void);

	unsigned int GetPixelX(void) const { return m_PixelX; }
	unsigned int GetPixelZ(void) const { return m_PixelZ; }

	unsigned int GetRot(void) const { return m_Rot; }

	eType GetType(void) const { return m_Type; }

	cPlayer * GetPlayer(void) { return m_Player; }


protected:

	cMap * m_Map;

	eType m_Type;

	unsigned int m_PixelX;
	unsigned int m_PixelZ;

	unsigned int m_Rot;

	cPlayer * m_Player;

};

typedef std::list<cMapDecorator> cMapDecoratorList;





// tolua_begin

/** Encapsulates an in-game world map. */
class cMap
{
public:

	enum eBaseColor
	{
		E_BASE_COLOR_TRANSPARENT = 0,  /* Air     */
		E_BASE_COLOR_LIGHT_GREEN = 4,  /* Grass   */
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


public:

	/** Construct an empty map. */
	cMap(unsigned int a_ID, cWorld * a_World);

	/** Construct an empty map at the specified coordinates. */
	cMap(unsigned int a_ID, int a_CenterX, int a_CenterZ, cWorld * a_World, unsigned int a_Scale = 3);

	/** Send this map to the specified client. WARNING: Slow */
	void SendTo(cClientHandle & a_Client);

	/** Update a circular region with the specified radius and center (in pixels). */
	void UpdateRadius(int a_PixelX, int a_PixelZ, unsigned int a_Radius);

	/** Update a circular region around the specified player. */
	void UpdateRadius(cPlayer & a_Player, unsigned int a_Radius);

	/** Send next update packet to the specified player and remove invalid decorators/clients. */
	void UpdateClient(cPlayer * a_Player);

	// tolua_begin

	/** Erase pixel data */
	void EraseData(void);

	void Resize(unsigned int a_Width, unsigned int a_Height);

	void SetPosition(int a_CenterX, int a_CenterZ);

	void SetScale(unsigned int a_Scale);

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

	size_t GetNumDecorators(void) const;

	const cColorList & GetData(void) const { return m_Data; }

	static const char * GetClassStatic(void)  // Needed for ManualBindings's DoWith templates
	{
		return "cMap";
	}


protected:

	/** Encapsulates the state of a map client.
	In order to enhance performace, maps are streamed column-by-column to each client.
	This structure stores the state of the stream.
	*/
	struct cMapClient
	{
		cClientHandle * m_Handle;

		/** Whether the map scale was modified and needs to be resent. */
		bool m_SendInfo;

		/** Ticks since last decorator update. */
		unsigned int m_NextDecoratorUpdate;

		/** Number of pixel data updates. */
		Int64 m_DataUpdate;

		Int64 m_LastUpdate;
	};

	typedef std::list<cMapClient> cMapClientList;


private:

	/** Update the associated decorators. */
	void UpdateDecorators(void);

	/** Update the specified pixel. */
	bool UpdatePixel(unsigned int a_X, unsigned int a_Z);

	/** Add a new map client. */
	void AddPlayer(cPlayer * a_Player, Int64 a_WorldAge);

	/** Remove inactive or invalid clients. */
	void RemoveInactiveClients(Int64 a_WorldAge);

	/** Send next update packet to the specified client. */
	void StreamNext(cMapClient & a_Client);

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

	cMapDecoratorList m_Decorators;

	cMapClientList m_Clients;

	AString m_Name;

	friend class cMapSerializer;

};  // tolua_export



