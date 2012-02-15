
#pragma once

#include "cBlockEntity.h"
#include "cWindowOwner.h"
#include "packets/cPacket_BlockAction.h"





namespace Json
{
	class Value;
};

class cClientHandle;
class cServer;
class cItem;
class cNBTData;





class cChestEntity :
	public cBlockEntity,
	public cWindowOwner
{
public:
	cChestEntity(int a_X, int a_Y, int a_Z, cWorld * a_World);
	virtual ~cChestEntity();
	virtual void Destroy();

	void HandleData( cNBTData* a_NBTData );

	cItem *	GetSlot( int a_Slot );
	void	SetSlot( int a_Slot, cItem & a_Item );

	bool LoadFromFile(cFile & a_File);  // deprecated format

	bool LoadFromJson( const Json::Value& a_Value );
	void SaveToJson( Json::Value& a_Value );

	void SendTo( cClientHandle* a_Client, cServer* a_Server );

	virtual void UsedBy( cPlayer * a_Player ) override;

	cChestEntity *GetJoinedChest() { return m_JoinedChest; }
	void SetJoinedChest(cChestEntity *a_Chest) { m_JoinedChest = a_Chest; }
	void RemoveJoinedChest(cChestEntity *a_Chest) { if (m_JoinedChest && m_JoinedChest == a_Chest) { m_JoinedChest = NULL; m_TopChest = false; } }

	int GetChestHeight() { return ((m_JoinedChest) ? c_ChestHeight * 2 : c_ChestHeight); }
	cItem * GetContents(bool a_OnlyThis = false);

	static const int c_ChestWidth = 9;
	static const int c_ChestHeight = 3;
	
private:

	cItem *        m_Content;
	bool           m_TopChest;
	cChestEntity * m_JoinedChest;
};




