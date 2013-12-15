
#pragma once

#include "Pawn.h"
#include "../Inventory.h"
#include "../Defines.h"
#include "../World.h"





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
		MAX_AIR_LEVEL = 300,
		DROWNING_TICKS = 10, //number of ticks per heart of damage
	} ;
	// tolua_end
	
	CLASS_PROTODEF(cPlayer)
	

	cPlayer(cClientHandle * a_Client, const AString & a_PlayerName);
	virtual ~cPlayer();

	virtual bool Initialize(cWorld * a_World) override;

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


	// tolua_begin

	/** Sets the experience total
	Returns true on success
	"should" really only be called at init or player death, plugins excepted
	*/
	bool SetCurrentExperience(short a_XpTotal);

	/* changes Xp by Xp_delta, you "shouldn't" inc more than MAX_EXPERIENCE_ORB_SIZE
	Wont't allow xp to go negative
	Returns the new current experience, -1 on error
	*/
	short DeltaExperience(short a_Xp_delta);

	/// Gets the experience total - XpTotal for score on death
	inline short GetXpLifetimeTotal(void) { return m_LifetimeTotalXp; }

	/// Gets the currrent experience
	inline short GetCurrentXp(void) { return m_CurrentXp; }

	/// Gets the current level - XpLevel
	short GetXpLevel(void);

	/// Gets the experience bar percentage - XpP
	float GetXpPercentage(void);

	/// Caculates the amount of XP needed for a given level, ref: http://minecraft.gamepedia.com/XP
	static short XpForLevel(short int a_Level);

	/// inverse of XpForLevel, ref: http://minecraft.gamepedia.com/XP values are as per this with pre-calculations
	static short CalcLevelFromXp(short int a_CurrentXp);

	// tolua_end
	
	/// Starts charging the equipped bow
	void StartChargingBow(void);
	
	/// Finishes charging the current bow. Returns the number of ticks for which the bow has been charged
	int FinishChargingBow(void);
	
	/// Cancels the current bow charging
	void CancelChargingBow(void);
	
	/// Returns true if the player is currently charging the bow
	bool IsChargingBow(void) const { return m_IsChargingBow; }

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
	
	/// Returns the position where projectiles thrown by this player should start, player eye position + adjustment
	Vector3d GetThrowStartPos(void) const;
	
	/// Returns the initial speed vector of a throw, with a 3D length of a_SpeedCoeff.
	Vector3d GetThrowSpeed(double a_SpeedCoeff) const;
	
	/// Returns the current gamemode. Partly OBSOLETE, you should use IsGameModeXXX() functions wherever applicable
	eGameMode GetGameMode(void) const { return m_GameMode; }
	
	/// Returns the current effective gamemode (inherited gamemode is resolved before returning)
	eGameMode GetEffectiveGameMode(void) const { return (m_GameMode == gmNotSet) ? m_World->GetGameMode() : m_GameMode; }
	
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

	/// "Shoots" the player in the given direction.
	void ShootTo(Vector3d a_Direction); // tolua_export

	/// Tries to move to a new position, with attachment-related checks (y == -999)
	void MoveTo(const Vector3d & a_NewPos);  // tolua_export

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

	// tolua_begin
	
	/// Returns the full color code to use for this player, based on their primary group or set in m_Color
	AString GetColor(void) const;

	void TossItem(bool a_bDraggingItem, char a_Amount = 1, short a_CreateType = 0, short a_CreateHealth = 0);

	/// Heals the player by the specified amount of HPs (positive only); sends health update
	void Heal(int a_Health);
	
	int    GetFoodLevel                 (void) const { return m_FoodLevel; }
	double GetFoodSaturationLevel       (void) const { return m_FoodSaturationLevel; }
	int    GetFoodTickTimer             (void) const { return m_FoodTickTimer; }
	double GetFoodExhaustionLevel       (void) const { return m_FoodExhaustionLevel; }
	int    GetFoodPoisonedTicksRemaining(void) const { return m_FoodPoisonedTicksRemaining; }

	int GetAirLevel                     (void) const { return m_AirLevel; }
	
	/// Returns true if the player is satiated, i. e. their foodlevel is at the max and they cannot eat anymore
	bool IsSatiated(void) const { return (m_FoodLevel >= MAX_FOOD_LEVEL); }
	
	void SetFoodLevel                 (int a_FoodLevel);
	void SetFoodSaturationLevel       (double a_FoodSaturationLevel);
	void SetFoodTickTimer             (int a_FoodTickTimer);
	void SetFoodExhaustionLevel       (double a_FoodExhaustionLevel);
	void SetFoodPoisonedTicksRemaining(int a_FoodPoisonedTicksRemaining);

	/// Adds to FoodLevel and FoodSaturationLevel, returns true if any food has been consumed, false if player "full"
	bool Feed(int a_Food, double a_Saturation);

	/// Adds the specified exhaustion to m_FoodExhaustion. Expects only positive values.
	void AddFoodExhaustion(double a_Exhaustion)
	{
		m_FoodExhaustionLevel += a_Exhaustion;
	}
	
	/// Starts the food poisoning for the specified amount of ticks; if already foodpoisoned, sets FoodPoisonedTicksRemaining to the larger of the two
	void FoodPoison(int a_NumTicks);
	
	/// Returns true if the player is currently in the process of eating the currently equipped item
	bool IsEating(void) const { return (m_EatingFinishTick >= 0); }
	
	/// Returns true if the player is currently flying.
	bool IsFlying(void) const { return m_IsFlying; }
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

	void SendExperience(void);
	
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
	
	/// Flags the player as flying
	void SetFlying(bool a_IsFlying);

	/// If true the player can fly even when he's not in creative.
	void SetCanFly(bool a_CanFly);

	/// Returns whether the player is swimming or not
	virtual bool IsSwimming(void) const{ return m_IsSwimming; }

	/// Return whether the player is under water or not
	virtual bool IsSubmerged(void) const{ return m_IsSubmerged; }

	/// Returns wheter the player can fly or not.
	virtual bool CanFly(void) const { return m_CanFly; }
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

	/// Xp Level stuff
	enum 
	{
		XP_TO_LEVEL15 = 255,
		XP_PER_LEVEL_TO15 = 17,
		XP_TO_LEVEL30 = 825
	} ;

	/// Player's air level (for swimming)
	int m_AirLevel;

	/// used to time ticks between damage taken via drowning/suffocation
	int m_AirTickTimer;

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
	bool m_IsFlying;
	bool m_IsSwimming;
	bool m_IsSubmerged;

	bool m_CanFly;  // If this is true the player can fly. Even if he is not in creative.

	/// The world tick in which eating will be finished. -1 if not eating
	Int64 m_EatingFinishTick;

	/// Player Xp level
	short int m_LifetimeTotalXp;
	short int m_CurrentXp;

	// flag saying we need to send a xp update to client
	bool m_bDirtyExperience;

	bool m_IsChargingBow;
	int  m_BowCharge;


	virtual void Destroyed(void);

	/// Filters out damage for creative mode
	virtual void DoTakeDamage(TakeDamageInfo & TDI) override;
	
	/// Called in each tick to handle food-related processing
	void HandleFood(void);
	
	/// Called in each tick to handle air-related processing i.e. drowning
	void HandleAir();

	/// Called once per tick to set IsSwimming and IsSubmerged
	void SetSwimState(cChunk & a_Chunk);

	/// Adds food exhaustion based on the difference between Pos and LastPos, sprinting status and swimming (in water block)
	void ApplyFoodExhaustionFromMovement();
} ; // tolua_export




