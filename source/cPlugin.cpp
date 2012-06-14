
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPlugin.h"





cPlugin::cPlugin()
	: m_Version( 0 )
	, m_Language( E_CPP )
	, m_bCanBindCommands( false )
{
}

cPlugin::~cPlugin()
{
}

// bool cPlugin::Initialize()
// {
// 	LOG("cPlugin::Initialize()");
// 	return false;
// }

void cPlugin::Tick(float a_Dt)
{
	(void)a_Dt;
}

bool cPlugin::OnBlockPlace( cPacket_BlockPlace* a_PacketData, cPlayer* a_Player )
{
	(void)a_PacketData;
	(void)a_Player;
	return false;
}

bool cPlugin::OnCollectItem( cPickup* a_Pickup, cPlayer* a_Player )
{
	(void)a_Pickup;
	(void)a_Player;
	return false;
}

bool cPlugin::OnDisconnect(const AString & a_Reason, cPlayer* a_Player )
{
	(void)a_Reason;
	(void)a_Player;
	return false;
}

bool cPlugin::OnChat( const char* a_Chat, cPlayer* a_Player )
{
	(void)a_Chat;
	(void)a_Player;
	return false;
}

bool cPlugin::OnLogin( cPacket_Login* a_PacketData )
{
	(void)a_PacketData;
	return false;
}

void cPlugin::OnPlayerSpawn( cPlayer* a_Player )
{
	(void)a_Player;
}

bool cPlugin::OnPlayerJoin( cPlayer* a_Player )
{
	(void)a_Player;
	return false;
}

void cPlugin::AddCommand( std::string & a_Command, std::string & a_Description, std::string & a_Permission )
{
	CommandStruct Command;
	Command.Command = a_Command;
	Command.Description = a_Description;
	Command.Permission = a_Permission;
	m_Commands.push_back( Command );
}
