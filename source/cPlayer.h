#pragma once

#include "cPawn.h"
#include <list>
#include <string> // TODO - use const char*

class cGroup;
class cWindow;
class cInventory;
class cClientHandle;
class cPlayer :	public cPawn												//tolua_export
{																			//tolua_export
public:
	CLASS_PROTOTYPE();

	cPlayer(cClientHandle* a_Client, const char* a_PlayerName);
	virtual ~cPlayer();

	virtual void Initialize( cWorld* a_World );								//tolua_export

	virtual void SpawnOn( cClientHandle* a_Target );
	virtual void Tick(float a_Dt);

	void SetTouchGround( bool a_bTouchGround );
	inline void SetStance( const double & a_Stance ) { m_Stance = a_Stance; }
	double GetEyeHeight();													//tolua_export
	Vector3d GetEyePosition();												//tolua_export
	inline bool GetFlying() { return m_bTouchGround; }						//tolua_export
	inline const double & GetStance() { return m_Stance; }					//tolua_export
	cInventory & GetInventory() { return *m_Inventory; }					//tolua_export
	virtual void TeleportTo( cEntity* a_Entity );													//tolua_export
	virtual void TeleportTo( const double & a_PosX, const double & a_PosY, const double & a_PosZ );	//tolua_export

	int GetGameMode() { return m_GameMode; } //return GameMode for player.
	float GetLastBlockActionTime() { return m_LastBlockActionTime; } //return LastBlockActionTime for player.
	void SetLastBlockActionTime();
	void SetGameMode( int a_GameMode );

	// Tries to move to a new position, with collision checks and stuff
	virtual void MoveTo( const Vector3d & a_NewPos );												//tolua_export

	cWindow* GetWindow() { return m_CurrentWindow; }
	void OpenWindow( cWindow* a_Window );
	void CloseWindow();

	cClientHandle* GetClientHandle() { return m_ClientHandle; }				//tolua_export
	void SetClientHandle( cClientHandle* a_Client ) { m_ClientHandle = a_Client; }

	void SendMessage( const char* a_Message );								//tolua_export

	const char* GetName();													//tolua_export
	void SetName( const char* a_Name );										//tolua_export

	typedef std::list< cGroup* > GroupList;
	void AddToGroup( const char* a_GroupName );								//tolua_export
	bool CanUseCommand( const char* a_Command );							//tolua_export
	bool HasPermission( const char* a_Permission );							//tolua_export
	const GroupList & GetGroups();											// >> EXPORTED IN MANUALBINDINGS <<
	bool IsInGroup( const char* a_Group );								//tolua_export

	std::string GetColor();													//tolua_export

	void TossItem( bool a_bDraggingItem, int a_Amount = 1 );				//tolua_export

	void Heal( int a_Health );												//tolua_export
	void TakeDamage( int a_Damage, cEntity* a_Instigator );					//tolua_export
	void KilledBy( cEntity* a_Killer );										//tolua_export
	void Respawn();															//tolua_export

	void SetVisible( bool a_bVisible );										//tolua_export
	bool IsVisible() { return m_bVisible; }									//tolua_export

	bool SaveToDisk();
	bool LoadFromDisk();

	//Burning logic
	bool m_bBurnable;
	enum PMetaState{NORMAL,BURNING,CROUCHED,RIDING} e_EPMetaState;
	virtual void CheckMetaDataBurn();
	virtual void InStateBurning(float a_Dt);

protected:
	struct sPlayerState;
	sPlayerState* m_pState;

	bool m_bVisible;

	float m_LastGroundHeight;
	bool m_bTouchGround;
	double m_Stance;
	cInventory* m_Inventory;
	cWindow* m_CurrentWindow;

	float m_TimeLastPickupCheck;

	void ResolvePermissions();

	void ResolveGroups();
	char m_Color;
	float m_FireDamageInterval;
	float m_BurnPeriod;

	float m_LastBlockActionTime;
	int m_GameMode;

	cClientHandle* m_ClientHandle;
}; //tolua_export
