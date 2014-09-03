
// Protocol18x.h

/*
Declares the 1.8.x protocol classes:
	- cProtocol180
		- release 1.8 protocol (#47)
(others may be added later in the future for the 1.8 release series)
*/





#pragma once

#include "Protocol.h"
#include "Protocol17x.h"
#include "../ByteBuffer.h"

#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable:4127)
	#pragma warning(disable:4244)
	#pragma warning(disable:4231)
	#pragma warning(disable:4189)
	#pragma warning(disable:4702)
#endif

#ifdef _MSC_VER
	#pragma warning(pop)
#endif





// fwd:
namespace Json
{
	class Value;
}





class cProtocol180 :
	public cProtocol176
{
	typedef cProtocol176 super;
	
public:

	cProtocol180(cClientHandle * a_Client, const AString & a_ServerAddress, UInt16 a_ServerPort, UInt32 a_State);

	virtual void SendBlockChange(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta) override;
	virtual void SendBlockChanges(int a_ChunkX, int a_ChunkZ, const sSetBlockVector & a_Changes) override;
	virtual void SendChat(const AString & a_Message) override;
	virtual void SendChat(const cCompositeChat & a_Message) override;
	virtual void SendPlayerListItem(const cPlayer & a_Player, bool a_IsOnline) override {}
	virtual void SendEntityEquipment(const cEntity & a_Entity, short a_SlotNum, const cItem & a_Item) override;
	virtual void SendLogin(const cPlayer & a_Player, const cWorld & a_World) override;

protected:

	virtual void HandlePacketStatusRequest(cByteBuffer & a_ByteBuffer) override;

	// Packet handlers while in the Login state (m_State == 2):
	virtual void HandlePacketLoginStart(cByteBuffer & a_ByteBuffer) override;
	virtual void HandlePacketLoginEncryptionResponse(cByteBuffer & a_ByteBuffer) override;
} ;




