
// Protocol110x.cpp

/*
Implements the 1.10.x protocol classes:
	- cProtocol1100
		- release 1.10.0 protocol (#210)
(others may be added later in the future for the 1.10 release series)
*/

#include "Globals.h"
#include "Protocol110x.h"
#include "Packetizer.h"

#include "../Root.h"
#include "../Server.h"

#include "../Entities/Boat.h"
#include "../Entities/ExpOrb.h"
#include "../Entities/Minecart.h"
#include "../Entities/FallingBlock.h"
#include "../Entities/Painting.h"
#include "../Entities/Pickup.h"
#include "../Entities/Player.h"
#include "../Entities/ItemFrame.h"
#include "../Entities/ArrowEntity.h"
#include "../Entities/FireworkEntity.h"
#include "../Entities/SplashPotionEntity.h"

#include "../Mobs/IncludeAllMonsters.h"

#include "Bindings/PluginManager.h"





cProtocol1100::cProtocol1100(cClientHandle * a_Client, UInt32 a_ProtocolVersion, const AString &a_ServerAddress, UInt16 a_ServerPort, UInt32 a_State) :
	super(a_Client, a_ProtocolVersion, a_ServerAddress, a_ServerPort, a_State)
{
}





void cProtocol1100::SendSoundEffect(const AString & a_SoundName, double a_X, double a_Y, double a_Z, float a_Volume, float a_Pitch)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x19);  // Named sound effect packet
	Pkt.WriteString(a_SoundName);
	Pkt.WriteVarInt32(0);  // Master sound category (may want to be changed to a parameter later)
	Pkt.WriteBEInt32(FloorC(a_X * 8.0));
	Pkt.WriteBEInt32(FloorC(a_Y * 8.0));
	Pkt.WriteBEInt32(FloorC(a_Z * 8.0));
	Pkt.WriteBEFloat(a_Volume);
	Pkt.WriteBEFloat(a_Pitch);
}





void cProtocol1100::HandlePacketStatusRequest(cByteBuffer & a_ByteBuffer)
{
	cServer * Server = cRoot::Get()->GetServer();
	AString ServerDescription = Server->GetDescription();
	int NumPlayers = Server->GetNumPlayers();
	int MaxPlayers = Server->GetMaxPlayers();
	AString Favicon = Server->GetFaviconData();
	cRoot::Get()->GetPluginManager()->CallHookServerPing(*m_Client, ServerDescription, NumPlayers, MaxPlayers, Favicon);

	// Version:
	Json::Value Version;
	Version["name"] = "Cuberite 1.10";
	Version["protocol"] = 210;

	// Players:
	Json::Value Players;
	Players["online"] = NumPlayers;
	Players["max"] = MaxPlayers;
	// TODO: Add "sample"

	// Description:
	Json::Value Description;
	Description["text"] = ServerDescription.c_str();

	// Create the response:
	Json::Value ResponseValue;
	ResponseValue["version"] = Version;
	ResponseValue["players"] = Players;
	ResponseValue["description"] = Description;
	if (!Favicon.empty())
	{
		ResponseValue["favicon"] = Printf("data:image/png;base64,%s", Favicon.c_str());
	}

	Json::StyledWriter Writer;
	AString Response = Writer.write(ResponseValue);

	cPacketizer Pkt(*this, 0x00);  // Response packet
	Pkt.WriteString(Response);
}
