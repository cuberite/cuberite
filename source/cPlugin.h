#pragma once

#include "MemoryLeak.h"

#include <vector>
#include <string>

class cPacket_BlockPlace;
class cPacket_PickupSpawn;
class cPacket_EntityEquipment;
class cPacket_Disconnect;
class cPacket_Chat;
class cPacket_BlockDig;
class cPacket_Login;
class cClientHandle;
class cPlayer;
class cPickup;
class cItem;
class cEntity;
class cPawn;
struct TakeDamageInfo;

// tolua_begin
class cPlugin
{
public:
	cPlugin();
	virtual ~cPlugin();
	
	virtual void OnDisable() {}
	virtual bool Initialize() = 0;

	// Called each tick
	virtual void Tick(float a_Dt);

	/**
	 * On all these functions, return true if you want to override default behavior
	 * You can also return false, so default behavior is used, but with changed PacketData
	 **/
	virtual bool OnCollectItem( cPickup* a_Pickup, cPlayer* a_Player );
	virtual bool OnDisconnect( std::string a_Reason, cPlayer* a_Player );
	virtual bool OnBlockPlace( cPacket_BlockPlace* a_PacketData, cPlayer* a_Player );
	virtual bool OnBlockDig( cPacket_BlockDig* a_PacketData, cPlayer* a_Player, cItem* a_PickupItem ) { (void)a_PacketData; (void)a_Player; (void)a_PickupItem; return false; }
	virtual bool OnChat( std::string a_Chat, cPlayer* a_Player );
	virtual bool OnLogin( cPacket_Login* a_PacketData );
	virtual void OnPlayerSpawn( cPlayer* a_Player );
	virtual bool OnPlayerJoin( cPlayer* a_Player );
	virtual void OnPlayerMove( cPlayer* a_Player ) { (void)a_Player; }
	virtual void OnTakeDamage( cPawn* a_Pawn, TakeDamageInfo* a_TakeDamageInfo ) { (void)a_Pawn; (void)a_TakeDamageInfo; }
	virtual bool OnKilled( cPawn* a_Killed, cEntity* a_Killer ) { (void)a_Killed; (void)a_Killer; return false; }

	// Accessors
	std::string GetName() const { return m_Name; }
	void SetName( std::string a_Name ) { m_Name = a_Name; }

	int GetVersion() const { return m_Version; }
	void SetVersion( int a_Version ) { m_Version = a_Version; }

	struct CommandStruct
	{
		std::string Command;
		std::string Description;
		std::string Permission;
	};

	void AddCommand( std::string & a_Command, std::string & a_Description, std::string & a_Permission );
	// tolua_end
	typedef bool (FuncCommandHandler)( std::string & a_Command, std::vector< std::string > & a_Split );
	void BindCommand( FuncCommandHandler* a_Function, std::string & a_Command );		// >> EXPORTED IN MANUALBINDINGS <<
	const std::vector< CommandStruct > & GetCommands() const { return m_Commands; }		// >> EXPORTED IN MANUALBINDINGS <<

private:
	std::vector< CommandStruct > m_Commands;
	std::string m_Name;
	int m_Version;
};	//tolua_export
