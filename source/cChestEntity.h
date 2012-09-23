
#pragma once

#include "cBlockEntity.h"
#include "UI/WindowOwner.h"





namespace Json
{
	class Value;
};

class cClientHandle;
class cServer;
class cItem;
class cNBTData;





class cChestEntity :              // tolua_export
	public cBlockEntity,            // tolua_export
	public cBlockEntityWindowOwner  // tolua_export
{  // tolua_export
public:
	cChestEntity(int a_X, int a_Y, int a_Z, cWorld * a_World);
	virtual ~cChestEntity();
	virtual void Destroy();

	void HandleData( cNBTData* a_NBTData );

	const cItem * GetSlot( int a_Slot ) const;		//tolua_export
	void	SetSlot(int a_Slot, const cItem & a_Item );	//tolua_export

	bool LoadFromJson( const Json::Value& a_Value );
	virtual void SaveToJson(Json::Value& a_Value ) override;

	virtual void SendTo(cClientHandle & a_Client) override;

	virtual void UsedBy( cPlayer * a_Player );	//tolua_export

	cChestEntity * GetJoinedChest() { return m_JoinedChest; } // NOTE: Is this a safe function? Should it be exported to Lua?
	void SetJoinedChest(cChestEntity *a_Chest) { m_JoinedChest = a_Chest; }
	void RemoveJoinedChest(cChestEntity *a_Chest) { if (m_JoinedChest && m_JoinedChest == a_Chest) { m_JoinedChest = NULL; m_TopChest = false; } }

	int GetChestHeight() { return ((m_JoinedChest) ? c_ChestHeight * 2 : c_ChestHeight); }	//tolua_export
	cItem * GetContents(bool a_OnlyThis = false);

	static const int c_ChestWidth = 9;
	static const int c_ChestHeight = 3;
	
private:

	cItem *        m_Content;
	bool           m_TopChest;
	cChestEntity * m_JoinedChest;
};	//tolua_export




