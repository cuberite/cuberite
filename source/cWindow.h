
// cWindow.h

// Interfaces to the cWindow class representing a UI window for a specific block





#pragma once


class cPlayer;
class cItem;
class cWindowOwner;
class cClientHandle;

typedef std::list<cPlayer *> cPlayerList;





/** 
Represents a UI window (base class) for a specific block entity.

There is up to one instance of the class for each block entity
Each window has a list of players that are currently using it
When there's no player using a window, it is destroyed
*/
class cWindow
{
public:
	enum WindowType
	{
		Inventory   = -1,  // This value is never actually sent to a client
		Chest       = 0,
		Workbench   = 1,
		Furnace     = 2,
		Dispenser   = 3,
		Enchantment = 4,
		Brewery     = 5
	};

	cWindow(cWindowOwner * a_Owner, bool a_bInventoryVisible, WindowType a_WindowType, int a_WindowID);
	~cWindow();

	int GetWindowID(void) const { return m_WindowID; }
	int GetWindowType(void) const { return m_WindowType; }

	cItem* GetSlots(void) const { return m_Slots; }
	int GetNumSlots(void) const { return m_NumSlots; }

	cItem* GetSlot( int a_Slot );

	cItem* GetDraggingItem( cPlayer * a_Player = 0 );

	// a_Slots is an array of slots of size a_NumSlots
	void SetSlots(cItem* a_Slots, int a_NumSlots) { m_Slots = a_Slots; m_NumSlots = a_NumSlots; }

	bool IsInventoryVisible() { return m_bInventoryVisible; }
	void SetInventoryVisible( bool a_bVisible ) { m_bInventoryVisible = a_bVisible; }

	virtual void Clicked(
		cPlayer & a_Player, int a_WindowID, 
		short a_SlotNum, bool a_IsRightClick, bool a_IsShiftPressed,
		const cItem & a_HeldItem
	);

	virtual void Open( cPlayer & a_Player );
	virtual void Close( cPlayer & a_Player );

	cWindowOwner* GetOwner() { return m_Owner; }
	void SetOwner( cWindowOwner* a_Owner ) { m_Owner = a_Owner; }

	void SendWholeWindow(cClientHandle * a_Client);
	void BroadcastWholeWindow(void);
	void BroadcastInventoryProgress(short a_Progressbar, short a_Value);

	const AString & GetWindowTitle() const { return m_WindowTitle; }
	void SetWindowTitle( const std::string & a_WindowTitle ) { m_WindowTitle = a_WindowTitle; }

	void OwnerDestroyed(void);
	
	/// Calls the callback safely for each player that has this window open; returns true if all players have been enumerated
	bool ForEachPlayer(cItemCallback<cPlayer> & a_Callback);

	/// Calls the callback safely for each client that has this window open; returns true if all clients have been enumerated
	bool ForEachClient(cItemCallback<cClientHandle> & a_Callback);
	
private:

	void Destroy();

	char    m_WindowID;
	int     m_WindowType;
	AString m_WindowTitle;

	cWindowOwner * m_Owner;

	cCriticalSection m_CS;
	cPlayerList m_OpenedBy;
	
	bool    m_bInventoryVisible;
	int     m_NumSlots;
	cItem * m_Slots;
	cItem * m_DraggingItem;
	bool    m_IsDestroyed;
};