
// cWindow.h

// Interfaces to the cWindow class representing a UI window for a specific block





#pragma once

#include "../cItem.h"





class cPlayer;
class cWindowOwner;
class cClientHandle;
class cChestEntity;
class cFurnaceEntity;
class cSlotArea;
class cWorld;

typedef std::list<cPlayer *> cPlayerList;
typedef std::vector<cSlotArea *> cSlotAreas;





/** 
Represents a UI window.

Each window has a list of players that are currently using it
When there's no player using a window, it is destroyed.
A window consists of several areas of slots with similar functionality - for example the crafting grid area, or 
the inventory area. Each area knows what its slots are (GetSlot() function) and can handle mouse clicks.
The window acts only as a top-level container for those areas, redirecting the click events to the correct areas.
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
	
	static const int c_NumInventorySlots = 36;

	cWindow(WindowType a_WindowType, const AString & a_WindowTitle);
	virtual ~cWindow();

	int GetWindowID(void) const { return m_WindowID; }
	int GetWindowType(void) const { return m_WindowType; }

	cWindowOwner * GetOwner() { return m_Owner; }
	void SetOwner( cWindowOwner * a_Owner ) { m_Owner = a_Owner; }
	
	int GetNumSlots(void) const;
	
	/// Fills a_Slots with the slots read from m_SlotAreas[], for the specified player
	void GetSlots(cPlayer & a_Player, cItems & a_Slots) const;

	void Clicked(
		cPlayer & a_Player, int a_WindowID, 
		short a_SlotNum, bool a_IsRightClick, bool a_IsShiftPressed,
		const cItem & a_ClickedItem
	);

	void OpenedByPlayer(cPlayer & a_Player);
	void ClosedByPlayer(cPlayer & a_Player);

	void SendWholeWindow(cClientHandle & a_Client);
	void BroadcastWholeWindow(void);
	void BroadcastInventoryProgress(short a_Progressbar, short a_Value);

	const AString & GetWindowTitle() const { return m_WindowTitle; }
	void SetWindowTitle(const AString & a_WindowTitle ) { m_WindowTitle = a_WindowTitle; }

	void OwnerDestroyed(void);
	
	/// Calls the callback safely for each player that has this window open; returns true if all players have been enumerated
	bool ForEachPlayer(cItemCallback<cPlayer> & a_Callback);

	/// Calls the callback safely for each client that has this window open; returns true if all clients have been enumerated
	bool ForEachClient(cItemCallback<cClientHandle> & a_Callback);
	
	/** Called on shift-clicking to distribute the stack into other areas; Modifies a_ItemStack as it is distributed!
	if a_ShouldApply is true, the changes are written into the slots;
	if a_ShouldApply is false, only a_ItemStack is modified to reflect the number of fits (for fit-testing purposes)
	*/
	void DistributeStack(cItem & a_ItemStack, cPlayer & a_Player, cSlotArea * a_ExcludeArea, bool a_ShouldApply);
	
protected:
	cSlotAreas m_SlotAreas;
	
private:
	char    m_WindowID;
	int     m_WindowType;
	AString m_WindowTitle;

	cCriticalSection m_CS;
	cPlayerList      m_OpenedBy;
	
	bool    m_IsDestroyed;
	
	cWindowOwner * m_Owner;
	
	static char m_WindowIDCounter;

	void Destroy(void);
} ;





class cCraftingWindow :
	public cWindow
{
public:
	cCraftingWindow(int a_BlockX, int a_BlockY, int a_BlockZ);
} ;





class cFurnaceWindow :
	public cWindow
{
public:
	cFurnaceWindow(int a_BlockX, int a_BlockY, int a_BlockZ, cFurnaceEntity * a_Furnace);
} ;





class cChestWindow :
	public cWindow
{
public:
	cChestWindow(int a_BlockX, int a_BlockY, int a_BlockZ, cChestEntity * a_Chest);
	~cChestWindow();
	
protected:
	cWorld * m_World;
	int m_BlockX, m_BlockY, m_BlockZ;  // Position of the chest, for the window-close packet
} ;





class cInventoryWindow :
	public cWindow
{
public:
	cInventoryWindow(cPlayer & a_Player);
	
protected:
	cPlayer & m_Player;

} ;





