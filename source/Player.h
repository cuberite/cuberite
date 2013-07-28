
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
		MAX_FOOD_LEVEL = 20,
		EATING_TICKS = 30,  ///< Number of ticks it takes to eat an item
	} ;
	// tolua_end
	
	CLASS_PROTODEF(cPlayer)
	

	cPlayer(cClientHandle * a_Client, const AString & a_PlayerName);
	virtual ~cPlayer();

	virtual void Initialize(cWorld * a_World);  // tolua_export

	virtual void SpawnOn(cClientHandle & a_Client) override;
	
	virtual void Tick(float a_Dt, cChunk & a_Chunk) override;

	virtual void HandlePhysics(float a_Dt, cChunk & a_Chunk) override { };

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
	double GetEyeHeight(void) const;													// tolua_export
	Vector3d GetEyePosition(void) const;												// tolua_export
	inline bool IsOnGround(void) const {return m_bTouchGround; }  // tolua_export
	inline const double GetStance(void) const { return GetPosY() + 1.62; }  // tolua_export  // TODO: Proper stance when crouching etc.
	inline cInventory &       GetInventory(void)       { return m_Inventory; }	// tolua_export
	inline const cInventory & GetInventory(void) const { return m_Inventory; }
	
	inline const cItem & GetEquippedItem(void) const { return GetInventory().GetEquippedItem(); }  // tolua_export

	virtual void TeleportToCoords(double a_PosX, double a_PosY, double a_PosZ) override;

	// tolua_begin
	
	/// Returns the current gamemode. Partly OBSOLETE, you should use IsGameModeXXX() functions wherever applicable
	eGameMode GetGameMode(void) const { return m_GameMode; }
	
	/** Sets the gamemode for the player.
	The gamemode may be gmNotSet, in that case the player inherits the world's gamemode.
	Updates the gamemode on the client (sends the packet)
	*/
	void SetGameMode(eGameMode a_GameMode);

	/// Returns true if the player is in Creative mode, either explicitly, or by inheriting from current world
	bool IsGameModeCreative(void) const;
	
	/// Returns true if the player is in Survival mode, either explicitly, or by inheriting from current world
	bool IsGameModeSurvival(void) const;
	
	/// Returns true if the player is in Adventure mode, either explicitly, or by inheriting from current world
	bool IsGameModeAdventure(void) const;
	
	AString GetIP(void) const { return m_IP; }  // tolua_export

	// tolua_end
	
	void SetIP(const AString & a_IP);

	float GetLastBlockActionTime() { return m_LastBlockActionTime; }
	int GetLastBlockActionCnt() { return m_LastBlockActionCnt; }
	void SetLastBlockActionCnt( int );
	void SetLastBlockActionTime();
	
	// Sets the current gamemode, doesn't check validity, doesn't send update packets to client
	void LoginSetGameMode(eGameMode a_GameMode);

	/// Tries to move to a new position, with collision checks and stuff
	virtual void MoveTo( const Vector3d & a_NewPos );  // tolua_export

	cWindow * GetWindow(void) { return m_CurrentWindow; }  // tolua_export
	const cWindow * GetWindow(void) const { return m_CurrentWindow; }
	
	/// Opens the specified window; closes the current one first using CloseWindow()
	void OpenWindow(cWindow * a_Window);  // Exported in ManualBindings.cpp
	
	// tolua_begin
	
	/// Closes the current window, resets current window to m_InventoryWindow. A plugin may refuse the closing if a_CanRefuse is true
	void CloseWindow(bool a_CanRefuse = true);
	
	/// Closes the current window if it matches the specified ID, resets current window to m_InventoryWindow
	void CloseWindowIfID(char a_WindowID, bool a_CanRefuse = true);

	cClientHandle * GetClientHandle(void) const { return m_ClientHandle; }

	void SendMessage(const AString & a_Message);

	const AString & GetName(void) const { return m_PlayerName; }
	void SetName(const AString & a_Name) { m_PlayerName = a_Name; }
	
	// tolua_end

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
	
	// tolua_begin
	
	int    GetFoodLevel                 (void) const { return m_FoodLevel; }
	double GetFoodSaturationLevel       (void) const { return m_FoodSaturationLevel; }
	int    GetFoodTickTimer             (void) const { return m_FoodTickTimer; }
	double GetFoodExhaustionLevel       (void) const { return m_FoodExhaustionLevel; }
	int    GetFoodPoisonedTicksRemaining(void) const { return m_FoodPoisonedTicksRemaining; }
	
	/// Returns true if the player is satiated, i. e. their foodlevel is at the max and they cannot eat anymore
	bool IsSatiated(void) const { return (m_FoodLevel >= MAX_FOOD_LEVEL); }
	
	void SetFoodLevel                 (int a_FoodLevel);
	void SetFoodSaturationLevel       (double a_FoodSaturationLevel);
	void SetFoodTickTimer             (int a_FoodTickTimer);
	void SetFoodExhaustionLevel       (double a_FoodSaturationLevel);
	void SetFoodPoisonedTicksRemaining(int a_FoodPoisonedTicksRemaining);

	/// Adds to FoodLevel and FoodSaturationLevel, returns true if any food has been consumed, false if player "full"
	bool Feed(int a_Food, double a_Saturation);

	/// Adds the specified exhaustion to m_FoodExhaustion. Expects only positive values.
	void AddFoodExhaustion(double a_Exhaustion) { m_FoodExhaustionLevel += a_Exhaustion; }
	
	/// Starts the food poisoning for the specified amount of ticks; if already foodpoisoned, sets FoodPoisonedTicksRemaining to the larger of the two
	void FoodPoison(int a_NumTicks);
	
	/// Returns true if the player is currently in the process of eating the currently equipped item
	bool IsEating(void) const { return (m_EatingFinishTick >= 0); }
	
	// tolua_end
	
	/// Starts eating the currently equipped item. Resets the eating timer and sends the proper animation packet
	void StartEating(void);
	
	/// Finishes eating the currently equipped item. Consumes the item, updates health and broadcasts the packets
	void FinishEating(void);
	
	/// Aborts the current eating operation
	void AbortEating(void);
	
	virtual void KilledBy(cEntity * a_Killer) override;
	
	void Respawn(void);															// tolua_export

	void SetVisible( bool a_bVisible );										// tolua_export
	bool IsVisible(void) const { return m_bVisible; }									// tolua_export

	bool MoveToWorld(const char * a_WorldName);  // tolua_export

	bool SaveToDisk(void);
	bool LoadFromDisk(void);
	void LoadPermissionsFromDisk(void);											// tolua_export

	const AString & GetLoadedWorldName() { return m_LoadedWorldName; }

	void UseEquippedItem(void);
	
	void SendHealth(void);
	
	// In UI windows, the item that the player is dragging:
	bool IsDraggingItem(void) const { return !m_DraggingItem.IsEmpty(); }
	cItem & GetDraggingItem(void) {return m_DraggingItem; }
	
	// In UI windows, when inventory-painting:
	/// Clears the list of slots that are being inventory-painted. To be used by cWindow only
	void ClearInventoryPaintSlots(void);
	
	/// Adds a slot to the list for inventory painting. To be used by cWindow only
	void AddInventoryPaintSlot(int a_SlotNum);
	
	/// Returns the list of slots currently stored for inventory painting. To be used by cWindow only
	const cSlotNums & GetInventoryPaintSlots(void) const;
	
	// tolua_begin
	
	/// Returns the current maximum speed, as reported in the 1.6.1+ protocol (takes current sprinting state into account)
	double GetMaxSpeed(void) const;
	
	/// Gets the normal maximum speed, as reported in the 1.6.1+ protocol, in the protocol units
	double GetNormalMaxSpeed(void) const { return m_NormalMaxSpeed; }
	
	/// Gets the sprinting maximum speed, as reported in the 1.6.1+ protocol, in the protocol units
	double GetSprintingMaxSpeed(void) const { return m_SprintingMaxSpeed; }
	
	/// Sets the normal maximum speed, as reported in the 1.6.1+ protocol. Sends the update to player, if needed.
	void SetNormalMaxSpeed(double a_Speed);
	
	/// Sets the sprinting maximum speed, as reported in the 1.6.1+ protocol. Sends the update to player, if needed.
	void SetSprintingMaxSpeed(double a_Speed);
	
	/// Sets the crouch status, broadcasts to all visible players
	void SetCrouch(bool a_IsCrouched);
	
	/// Starts or stops sprinting, sends the max speed update to the client, if needed
	void SetSprint(bool a_IsSprinting);
	
	// tolua_end
	
	// cEntity overrides:
	virtual bool IsCrouched (void) const { return m_IsCrouched; }
	virtual bool IsSprinting(void) const { return m_IsSprinting; }
	virtual bool IsRclking  (void) const { return IsEating(); }

protected:
	typedef std::map< std::string, bool > PermissionMap;
	PermissionMap m_ResolvedPermissions;
	PermissionMap m_Permissions;

	GroupList m_ResolvedGroups;
	GroupList m_Groups;

	std::string m_PlayerName;
	std::string m_LoadedWorldName;

	bool m_bVisible;

	// Food-related variables:
	/// Represents the food bar, one point equals half a "drumstick"
	int m_FoodLevel;
	
	/// "Overcharge" for the m_FoodLevel; is depleted before m_FoodLevel
	double m_FoodSaturationLevel;
	
	/// Count-up to the healing or damaging action, based on m_FoodLevel
	int   m_FoodTickTimer;
	
	/// A "buffer" which adds up hunger before it is substracted from m_FoodSaturationLevel or m_FoodLevel. Each action adds a little
	double m_FoodExhaustionLevel;
	
	/// Number of ticks remaining for the foodpoisoning effect; zero if not foodpoisoned
	int m_FoodPoisonedTicksRemaining;
	
	/// Last position that has been recorded for food-related processing:
	Vector3d m_LastFoodPos;
	
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

	cClientHandle * m_ClientHandle;
	
	cSlotNums m_InventoryPaintSlots;
	
	/// Max speed, in ENTITY_PROPERTIES packet's units, when the player is walking. 0.1 by default
	double m_NormalMaxSpeed;
	
	/// Max speed, in ENTITY_PROPERTIES packet's units, when the player is sprinting. 0.13 by default
	double m_SprintingMaxSpeed;
	
	bool m_IsCrouched;
	bool m_IsSprinting;

	/// The world tick in which eating will be finished. -1 if not eating
	Int64 m_EatingFinishTick;


	virtual void Destroyed(void);

	/// Filters out damage for creative mode
	virtual void DoTakeDamage(TakeDamageInfo & TDI) override;
	
	/// Called in each tick to handle food-related processing
	void HandleFood(void);
	
	/// Adds food exhaustion based on the difference between Pos and LastPos, sprinting status and swimming (in water block)
	void ApplyFoodExhaustionFromMovement(cChunk & a_Chunk);
} ; // tolua_export




