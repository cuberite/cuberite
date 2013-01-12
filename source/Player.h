
#pragma once

#include "Pawn.h"
#include "Inventory.h"
#include "Defines.h"





class cGroup;
class cWindow;
class cClientHandle;





// tolua_begin
class cPlayer :
	public cPawn
{
	typedef cPawn super;
	
public:
	enum
	{
		MAX_HEALTH = 20,
	} ;
	// tolua_end
	
	CLASS_PROTODEF(cPlayer)

	cPlayer(cClientHandle * a_Client, const AString & a_PlayerName);
	virtual ~cPlayer();

	virtual void Initialize( cWorld* a_World );								// tolua_export

	virtual void SpawnOn(cClientHandle & a_Client) override;
	
	virtual void Tick(float a_Dt, MTRand & a_TickRandom) override;

	/// Returns the curently equipped weapon; empty item if none
	virtual cItem GetEquippedWeapon(void) const override { return m_Inventory.GetEquippedItem(); }
	
	/// Returns the currently equipped helmet; empty item if nonte
	virtual cItem GetEquippedHelmet(void) const override { return m_Inventory.GetEquippedHelmet(); }
	
	/// Returns the currently equipped chestplate; empty item if none
	virtual cItem GetEquippedChestplate(void) const override { return m_Inventory.GetEquippedChestplate(); }

	/// Returns the currently equipped leggings; empty item if none
	virtual cItem GetEquippedLeggings(void) const override { return m_Inventory.GetEquippedLeggings(); }
	
	/// Returns the currently equipped boots; empty item if none
	virtual cItem GetEquippedBoots(void) const override { return m_Inventory.GetEquippedBoots(); }

	void SetTouchGround( bool a_bTouchGround );
	inline void SetStance( const double a_Stance ) { m_Stance = a_Stance; }
	double GetEyeHeight();													// tolua_export
	Vector3d GetEyePosition();												// tolua_export
	inline bool IsOnGround(void) const {return m_bTouchGround; }  // tolua_export
	inline const double GetStance(void) const { return m_Pos.y + 1.62; }					// tolua_export  // TODO: Proper stance when crouching etc.
	inline cInventory &       GetInventory(void)       { return m_Inventory; }	// tolua_export
	inline const cInventory & GetInventory(void) const { return m_Inventory; }
	
	inline cItem &       GetEquippedItem(void)       { return GetInventory().GetEquippedItem(); }  // tolua_export
	inline const cItem & GetEquippedItem(void) const { return GetInventory().GetEquippedItem(); }

	virtual void TeleportTo(double a_PosX, double a_PosY, double a_PosZ) override;

	eGameMode GetGameMode(void) const { return m_GameMode; }														// tolua_export
	std::string GetIP() { return m_IP; }																// tolua_export
	float GetLastBlockActionTime() { return m_LastBlockActionTime; }									// tolua_export
	int GetLastBlockActionCnt() { return m_LastBlockActionCnt; }										// tolua_export
	void SetLastBlockActionCnt( int );																	// tolua_export
	void SetLastBlockActionTime();																		// tolua_export
	void SetGameMode( eGameMode a_GameMode );															// tolua_export
	void LoginSetGameMode( eGameMode a_GameMode );
	void SetIP(const AString & a_IP);

	// Tries to move to a new position, with collision checks and stuff
	virtual void MoveTo( const Vector3d & a_NewPos );													// tolua_export

	cWindow* GetWindow() { return m_CurrentWindow; }
	void OpenWindow( cWindow* a_Window );
	void CloseWindow(char a_WindowType);

	cClientHandle * GetClientHandle(void) const { return m_ClientHandle; }			// tolua_export

	void SendMessage(const AString & a_Message);								// tolua_export

	const AString & GetName(void) const { return m_PlayerName; }			// tolua_export
	void SetName(const AString & a_Name) { m_PlayerName = a_Name; }			// tolua_export

	typedef std::list< cGroup* > GroupList;
	typedef std::list< std::string > StringList;

	/// Adds a player to existing group or creates a new group when it doesn't exist
	void AddToGroup( const AString & a_GroupName );							// tolua_export
	/// Removes a player from the group, resolves permissions and group inheritance (case sensitive)
	void RemoveFromGroup( const AString & a_GroupName );					// tolua_export
	bool CanUseCommand( const AString & a_Command );						// tolua_export
	bool HasPermission( const AString & a_Permission );						// tolua_export
	const GroupList & GetGroups() { return m_Groups; }						// >> EXPORTED IN MANUALBINDINGS <<
	StringList GetResolvedPermissions();									// >> EXPORTED IN MANUALBINDINGS <<
	bool IsInGroup( const AString & a_Group );								// tolua_export

	AString GetColor(void) const;											// tolua_export

	void TossItem(bool a_bDraggingItem, char a_Amount = 1, short a_CreateType = 0, short a_CreateHealth = 0);				// tolua_export

	void Heal( int a_Health );												// tolua_export
	
	/// Returns true if any food has been consumed, false if player "full"
	bool Feed(short a_Food, float a_Saturation);							// tolua_export

	short GetMaxFoodLevel() { return m_MaxFoodLevel; }						// tolua_export
	short GetFoodLevel() { return m_FoodLevel; }							// tolua_export

	float GetMaxFoodSaturationLevel() { return m_MaxFoodSaturationLevel; }	// tolua_export
	float GetFoodSaturationLevel() { return m_FoodSaturationLevel; }		// tolua_export

	void AddFoodExhaustion(float a_Exhaustion) { m_FoodExhaustionLevel += a_Exhaustion; }	// tolua_export
	
	virtual void KilledBy(cPawn * a_Killer) override;
	
	void Respawn(void);															// tolua_export

	void SetVisible( bool a_bVisible );										// tolua_export
	bool IsVisible(void) const { return m_bVisible; }									// tolua_export

	bool MoveToWorld(const char * a_WorldName );							// tolua_export

	bool SaveToDisk(void);
	bool LoadFromDisk(void);
	void LoadPermissionsFromDisk(void);											// tolua_export

	const AString & GetLoadedWorldName() { return m_LoadedWorldName; }

	void UseEquippedItem(void);
	
	void SendHealth();
	
	// In UI windows, the item that the player is dragging:
	bool IsDraggingItem(void) const { return !m_DraggingItem.IsEmpty(); }
	cItem & GetDraggingItem(void) {return m_DraggingItem; }

protected:
	virtual void Destroyed();

	typedef std::map< std::string, bool > PermissionMap;
	PermissionMap m_ResolvedPermissions;
	PermissionMap m_Permissions;

	GroupList m_ResolvedGroups;
	GroupList m_Groups;

	std::string m_PlayerName;
	std::string m_LoadedWorldName;

	bool m_bVisible;

	short m_FoodLevel;
	short m_MaxFoodLevel;
	float m_FoodSaturationLevel;
	float m_MaxFoodSaturationLevel;
	float m_FoodExhaustionLevel;
	char m_FoodTickTimer;

	float m_LastJumpHeight;
	float m_LastGroundHeight;
	bool m_bTouchGround;
	double m_Stance;
	cInventory m_Inventory;
	cWindow * m_CurrentWindow;
	cWindow * m_InventoryWindow;

	float m_TimeLastPickupCheck;

	void ResolvePermissions();

	void ResolveGroups();
	char m_Color;

	float m_LastBlockActionTime;
	int m_LastBlockActionCnt;
	eGameMode m_GameMode;
	std::string m_IP;
	
	cItem m_DraggingItem;

	long long m_LastPlayerListTime;
	static const unsigned short PLAYER_LIST_TIME_MS = 1000; // 1000 = once per second

	cClientHandle* m_ClientHandle;

	/// Filters out damage for creative mode
	virtual void DoTakeDamage(TakeDamageInfo & TDI) override;
	
} ; // tolua_export




