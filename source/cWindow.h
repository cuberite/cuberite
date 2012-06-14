#pragma once


class cPacket_WindowClick;
class cPlayer;
class cItem;
class cWindowOwner;
class cClientHandle;
class cPacket;

typedef std::list<cPlayer *> cPlayerList;





class cWindow
{
public:
	cWindow( cWindowOwner* a_Owner, bool a_bInventoryVisible );
	~cWindow();

	int GetWindowID() { return m_WindowID; }
	void SetWindowID( int a_WindowID ) { m_WindowID = a_WindowID; }
	
	enum WindowType {
		Chest,
		Workbench,
		Furnace,
		Dispenser,
		Enchantment,
		Brewery
	};

	int GetWindowType() { return m_WindowType; }
	void SetWindowType( int a_WindowType ) { m_WindowType = a_WindowType; }

	cItem* GetSlots() { return m_Slots; }
	int GetNumSlots() { return m_NumSlots; }

	cItem* GetSlot( int a_Slot );

	cItem* GetDraggingItem( cPlayer * a_Player = 0 );

	// a_Slots is an array of slots of size a_NumSlots
	void SetSlots(cItem* a_Slots, int a_NumSlots) { m_Slots = a_Slots; m_NumSlots = a_NumSlots; }

	bool IsInventoryVisible() { return m_bInventoryVisible; }
	void SetInventoryVisible( bool a_bVisible ) { m_bInventoryVisible = a_bVisible; }

	virtual void Clicked( cPacket_WindowClick* a_ClickPacket, cPlayer & a_Player );

	virtual void Open( cPlayer & a_Player );
	virtual void Close( cPlayer & a_Player );

	cWindowOwner* GetOwner() { return m_Owner; }
	void SetOwner( cWindowOwner* a_Owner ) { m_Owner = a_Owner; }

	void SendWholeWindow( cClientHandle* a_Client );
	void BroadcastWholeWindow(void);
	void Broadcast(const cPacket & a_Packet);

	const AString & GetWindowTitle() const { return m_WindowTitle; }
	void SetWindowTitle( const std::string & a_WindowTitle ) { m_WindowTitle = a_WindowTitle; }

	void OwnerDestroyed();
	
private:

	void Destroy();

	int m_WindowID;
	int m_WindowType;
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