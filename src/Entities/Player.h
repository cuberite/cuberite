
#pragma once

#include "Pawn.h"
#include "../Inventory.h"
#include "../Defines.h"
#include "../World.h"

#include "../Statistics.h"





class cWindow;
class cClientHandle;
class cTeam;





// tolua_begin
class cPlayer :
	public cPawn
{
	typedef cPawn super;

public:
	static const int MAX_HEALTH;

	static const int MAX_FOOD_LEVEL;

	/** Number of ticks it takes to eat an item */
	static const int EATING_TICKS;

	// tolua_end

	CLASS_PROTODEF(cPlayer)


	cPlayer(cClientHandlePtr a_Client, const AString & a_PlayerName);

	virtual bool Initialize(OwnedEntity a_Self, cWorld & a_World) override;

	virtual ~cPlayer() override;

	virtual void SpawnOn(cClientHandle & a_Client) override;

	virtual void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;

	void TickFreezeCode();

	virtual void HandlePhysics(std::chrono::milliseconds a_Dt, cChunk &) override { UNUSED(a_Dt); }

	/** Returns the currently equipped weapon; empty item if none */
	virtual cItem GetEquippedWeapon(void) const override { return m_Inventory.GetEquippedItem(); }

	/** Returns the currently equipped helmet; empty item if none */
	virtual cItem GetEquippedHelmet(void) const override { return m_Inventory.GetEquippedHelmet(); }

	/** Returns the currently equipped chestplate; empty item if none */
	virtual cItem GetEquippedChestplate(void) const override { return m_Inventory.GetEquippedChestplate(); }

	/** Returns the currently equipped leggings; empty item if none */
	virtual cItem GetEquippedLeggings(void) const override { return m_Inventory.GetEquippedLeggings(); }

	/** Returns the currently equipped boots; empty item if none */
	virtual cItem GetEquippedBoots(void) const override { return m_Inventory.GetEquippedBoots(); }

	virtual void ApplyArmorDamage(int DamageBlocked) override;

	// tolua_begin

	/** Sets the experience total
	Returns true on success
	"should" really only be called at init or player death, plugins excepted
	*/
	bool SetCurrentExperience(int a_XpTotal);

	/* changes Xp by Xp_delta, you "shouldn't" inc more than MAX_EXPERIENCE_ORB_SIZE
	Wont't allow xp to go negative
	Returns the new current experience, -1 on error
	*/
	int DeltaExperience(int a_Xp_delta);

	/** Gets the experience total - XpTotal for score on death */
	inline int GetXpLifetimeTotal(void) { return m_LifetimeTotalXp; }

	/** Gets the current experience */
	inline int GetCurrentXp(void) { return m_CurrentXp; }

	/** Gets the current level - XpLevel */
	int GetXpLevel(void);

	/** Gets the experience bar percentage - XpP */
	float GetXpPercentage(void);

	/** Calculates the amount of XP needed for a given level
	Ref: http://minecraft.gamepedia.com/XP
	*/
	static int XpForLevel(int a_Level);

	/** Inverse of XpForLevel
	Ref: http://minecraft.gamepedia.com/XP
	values are as per this with pre-calculations
	*/
	static int CalcLevelFromXp(int a_CurrentXp);

	// tolua_end

	/** Starts charging the equipped bow */
	void StartChargingBow(void);

	/** Finishes charging the current bow. Returns the number of ticks for which the bow has been charged */
	int FinishChargingBow(void);

	/** Cancels the current bow charging */
	void CancelChargingBow(void);

	/** Returns true if the player is currently charging the bow */
	bool IsChargingBow(void) const { return m_IsChargingBow; }

	void SetTouchGround(bool a_bTouchGround);
	inline void SetStance(const double a_Stance) { m_Stance = a_Stance; }
	double GetEyeHeight(void) const;  // tolua_export
	Vector3d GetEyePosition(void) const;  // tolua_export
	virtual bool IsOnGround(void) const override { return m_bTouchGround; }
	inline double GetStance(void) const { return m_Stance; }  // tolua_export
	inline cInventory &       GetInventory(void)       { return m_Inventory; }  // tolua_export
	inline const cInventory & GetInventory(void) const { return m_Inventory; }

	/** Gets the contents of the player's associated enderchest */
	cItemGrid & GetEnderChestContents(void) { return m_EnderChestContents; }

	inline const cItem & GetEquippedItem(void) const { return GetInventory().GetEquippedItem(); }  // tolua_export

	/** Returns whether the player is climbing (ladders, vines etc.) */
	bool IsClimbing(void) const;

	virtual void TeleportToCoords(double a_PosX, double a_PosY, double a_PosZ) override;

	// Sets the current gamemode, doesn't check validity, doesn't send update packets to client
	void LoginSetGameMode(eGameMode a_GameMode);

	// Updates player's capabilities - flying, visibility, etc. from their gamemode.
	void SetCapabilities();

	// tolua_begin

	/** Prevent the player from moving and lock him into a_Location. */
	void Freeze(const Vector3d & a_Location);

	/** Is the player frozen? */
	bool IsFrozen();

	/** Cancels Freeze(...) and allows the player to move naturally. */
	void Unfreeze();

	/** Sends the "look" packet to the player, forcing them to set their rotation to the specified values.
	a_YawDegrees is clipped to range [-180, +180),
	a_PitchDegrees is clipped to range [-180, +180) but the client only uses [-90, +90]
	*/
	void SendRotation(double a_YawDegrees, double a_PitchDegrees);

	/** Returns the position where projectiles thrown by this player should start, player eye position + adjustment */
	Vector3d GetThrowStartPos(void) const;

	/** Returns the initial speed vector of a throw, with a 3D length of a_SpeedCoeff. */
	Vector3d GetThrowSpeed(double a_SpeedCoeff) const;

	/** Returns the current gamemode. Partly OBSOLETE, you should use IsGameModeXXX() functions wherever applicable */
	eGameMode GetGameMode(void) const { return m_GameMode; }

	/** Returns the current effective gamemode (inherited gamemode is resolved before returning) */
	eGameMode GetEffectiveGameMode(void) const { return (m_GameMode == gmNotSet) ? m_World->GetGameMode() : m_GameMode; }

	/** Sets the gamemode for the player.
	The gamemode may be gmNotSet, in that case the player inherits the world's gamemode.
	Updates the gamemode on the client (sends the packet)
	*/
	void SetGameMode(eGameMode a_GameMode);

	/** Returns true if the player is in Creative mode, either explicitly, or by inheriting from current world */
	bool IsGameModeCreative(void) const;

	/** Returns true if the player is in Survival mode, either explicitly, or by inheriting from current world */
	bool IsGameModeSurvival(void) const;

	/** Returns true if the player is in Adventure mode, either explicitly, or by inheriting from current world */
	bool IsGameModeAdventure(void) const;

	/** Returns true if the player is in Spectator mode, either explicitly, or by inheriting from current world */
	bool IsGameModeSpectator(void) const;

	/** Returns true if the player can be targeted by Mobs */
	bool CanMobsTarget(void) const;

	AString GetIP(void) const { return m_IP; }  // tolua_export

	/** Returns the associated team, nullptr if none */
	cTeam * GetTeam(void) { return m_Team; }  // tolua_export

	/** Sets the player team, nullptr if none */
	void SetTeam(cTeam * a_Team);

	// tolua_end

	/** Forces the player to query the scoreboard for his team */
	cTeam * UpdateTeam(void);

	/** Return the associated statistic and achievement manager. */
	cStatManager & GetStatManager() { return m_Stats; }

	/** Awards the player an achievement.
	If all prerequisites are met, this method will award the achievement and will broadcast a chat message.
	If the achievement has been already awarded to the player, this method will just increment the stat counter.
	Returns the _new_ stat value. (0 = Could not award achievement) */
	unsigned int AwardAchievement(const eStatistic a_Ach);

	void SetIP(const AString & a_IP);

	/** Forces the player to move in the given direction.
	@deprecated Use SetSpeed instead. */
	void ForceSetSpeed(const Vector3d & a_Speed);  // tolua_export

	cWindow * GetWindow(void) { return m_CurrentWindow; }  // tolua_export
	const cWindow * GetWindow(void) const { return m_CurrentWindow; }

	// tolua_begin

	/** Opens the specified window; closes the current one first using CloseWindow() */
	void OpenWindow(cWindow & a_Window);

	/** Closes the current window, resets current window to m_InventoryWindow. A plugin may refuse the closing if a_CanRefuse is true */
	void CloseWindow(bool a_CanRefuse = true);

	/** Closes the current window if it matches the specified ID, resets current window to m_InventoryWindow */
	void CloseWindowIfID(char a_WindowID, bool a_CanRefuse = true);

	/** Returns the raw client handle associated with the player. */
	cClientHandle * GetClientHandle(void) const { return m_ClientHandle.get(); }

	// tolua_end

	/** Returns the SharedPtr to client handle associated with the player. */
	cClientHandlePtr GetClientHandlePtr(void) const { return m_ClientHandle; }

	// tolua_begin

	void SendMessage              (const AString & a_Message);
	void SendMessageInfo          (const AString & a_Message);
	void SendMessageFailure       (const AString & a_Message);
	void SendMessageSuccess       (const AString & a_Message);
	void SendMessageWarning       (const AString & a_Message);
	void SendMessageFatal         (const AString & a_Message);
	void SendMessagePrivateMsg    (const AString & a_Message, const AString & a_Sender);
	void SendMessage              (const cCompositeChat & a_Message);
	void SendMessageRaw           (const AString & a_MessageRaw, eChatType a_Type = eChatType::ctChatBox);
	void SendSystemMessage        (const AString & a_Message);
	void SendAboveActionBarMessage(const AString & a_Message);
	void SendSystemMessage        (const cCompositeChat & a_Message);
	void SendAboveActionBarMessage(const cCompositeChat & a_Message);

	const AString & GetName(void) const { return m_PlayerName; }
	void SetName(const AString & a_Name) { m_PlayerName = a_Name; }

	// tolua_end

	bool HasPermission(const AString & a_Permission);  // tolua_export

	/** Returns true iff a_Permission matches the a_Template.
	A match is defined by either being exactly the same, or each sub-item matches until there's a wildcard in a_Template.
	Ie. {"a", "b", "c"} matches {"a", "b", "*"} but doesn't match {"a", "b"} */
	static bool PermissionMatches(const AStringVector & a_Permission, const AStringVector & a_Template);  // Exported in ManualBindings with AString params

	/** Returns all the permissions that the player has assigned to them. */
	const AStringVector & GetPermissions(void) const { return m_Permissions; }  // Exported in ManualBindings.cpp

	/** Returns all the restrictions that the player has assigned to them. */
	const AStringVector & GetRestrictions(void) const { return m_Restrictions; }  // Exported in ManualBindings.cpp

	// tolua_begin

	/** Returns the full color code to use for this player, based on their rank.
	The returned value either is empty, or includes the cChatColor::Delimiter. */
	AString GetColor(void) const;

	/** Returns the player name prefix, may contain @ format directives */
	AString GetPrefix(void) const;

	/** Returns the player name suffix, may contain @ format directives */
	AString GetSuffix(void) const;

	/** Returns the name that is used in the playerlist. */
	AString GetPlayerListName(void) const;

	/** tosses the item in the selected hotbar slot */
	void TossEquippedItem(char a_Amount = 1);

	/** tosses the item held in hand (when in UI windows) */
	void TossHeldItem(char a_Amount = 1);

	/** tosses a pickup newly created from a_Item */
	void TossPickup(const cItem & a_Item);

	/** Heals the player by the specified amount of HPs (positive only); sends health update */
	virtual void Heal(int a_Health) override;

	int    GetFoodLevel                 (void) const { return m_FoodLevel; }
	double GetFoodSaturationLevel       (void) const { return m_FoodSaturationLevel; }
	int    GetFoodTickTimer             (void) const { return m_FoodTickTimer; }
	double GetFoodExhaustionLevel       (void) const { return m_FoodExhaustionLevel; }

	/** Returns true if the player is satiated, i. e. their foodlevel is at the max and they cannot eat anymore */
	bool IsSatiated(void) const { return (m_FoodLevel >= MAX_FOOD_LEVEL); }

	void SetFoodLevel                 (int a_FoodLevel);
	void SetFoodSaturationLevel       (double a_FoodSaturationLevel);
	void SetFoodTickTimer             (int a_FoodTickTimer);
	void SetFoodExhaustionLevel       (double a_FoodExhaustionLevel);

	/** Adds to FoodLevel and FoodSaturationLevel, returns true if any food has been consumed, false if player "full" */
	bool Feed(int a_Food, double a_Saturation);

	/** Adds the specified exhaustion to m_FoodExhaustion. Expects only positive values. */
	void AddFoodExhaustion(double a_Exhaustion);

	/** Returns true if the player is currently in the process of eating the currently equipped item */
	bool IsEating(void) const { return (m_EatingFinishTick >= 0); }

	/** Returns true if the player is currently flying. */
	bool IsFlying(void) const { return m_IsFlying; }

	/** Returns if a player is sleeping in a bed */
	bool IsInBed(void) const { return m_bIsInBed; }

	/** returns true if the player has thrown out a floater. */
	bool IsFishing(void) const { return m_IsFishing; }

	void SetIsFishing(bool a_IsFishing, UInt32 a_FloaterID = cEntity::INVALID_ID) { m_IsFishing = a_IsFishing; m_FloaterID = a_FloaterID; }

	UInt32 GetFloaterID(void) const { return m_FloaterID; }

	// tolua_end

	/** Sets a player's in-bed state
	We can't be sure plugins will keep this value updated, so no exporting
	If value is false (not in bed), will update players of the fact that they have been ejected from the bed
	*/
	void SetIsInBed(bool a_Flag)
	{
		m_bIsInBed = a_Flag;
		if (!a_Flag)
		{
			GetWorld()->BroadcastEntityAnimation(*this, 2);
		}
	}

	/** Starts eating the currently equipped item. Resets the eating timer and sends the proper animation packet */
	void StartEating(void);

	/** Finishes eating the currently equipped item. Consumes the item, updates health and broadcasts the packets */
	void FinishEating(void);

	/** Aborts the current eating operation */
	void AbortEating(void);

	virtual void KilledBy(TakeDamageInfo & a_TDI) override;

	virtual void Killed(cEntity * a_Victim) override;

	void Respawn(void);  // tolua_export

	void SetVisible( bool a_bVisible);  // tolua_export
	bool IsVisible(void) const { return m_bVisible; }  // tolua_export

	/** Moves the player to the specified world.
	Returns true if successful, false on failure (world not found). */
	virtual bool DoMoveToWorld(cWorld * a_World, bool a_ShouldSendRespawn, Vector3d a_NewPosition) override;

	/** Saves all player data, such as inventory, to JSON */
	bool SaveToDisk(void);

	typedef cWorld * cWorldPtr;

	/** Loads the player data from the disk file
	Sets a_World to the world where the player will spawn, based on the stored world name or the default world by calling LoadFromFile()
	Returns true on success, false on failure
	*/
	bool LoadFromDisk(cWorldPtr & a_World);

	/** Loads the player data from the specified file
	Sets a_World to the world where the player will spawn, based on the stored world name or the default world
	Returns true on success, false on failure
	*/
	bool LoadFromFile(const AString & a_FileName, cWorldPtr & a_World);

	const AString & GetLoadedWorldName() { return m_LoadedWorldName; }

	void UseEquippedItem(int a_Amount = 1);

	void SendHealth(void);

	void SendExperience(void);

	/** In UI windows, get the item that the player is dragging */
	cItem & GetDraggingItem(void) {return m_DraggingItem; }  // tolua_export

	/** In UI windows, set the item that the player is dragging */
	void SetDraggingItem(const cItem & a_Item);  // tolua_export

	// In UI windows, when inventory-painting:
	/** Clears the list of slots that are being inventory-painted. To be used by cWindow only */
	void ClearInventoryPaintSlots(void);

	/** Adds a slot to the list for inventory painting. To be used by cWindow only */
	void AddInventoryPaintSlot(int a_SlotNum);

	/** Returns the list of slots currently stored for inventory painting. To be used by cWindow only */
	const cSlotNums & GetInventoryPaintSlots(void) const;

	// tolua_begin

	/** Returns the current relative maximum speed (takes current sprinting / flying state into account) */
	double GetMaxSpeed(void) const;

	/** Gets the normal relative maximum speed */
	double GetNormalMaxSpeed(void) const { return m_NormalMaxSpeed; }

	/** Gets the sprinting relative maximum speed */
	double GetSprintingMaxSpeed(void) const { return m_SprintingMaxSpeed; }

	/** Gets the flying relative maximum speed */
	double GetFlyingMaxSpeed(void) const { return m_FlyingMaxSpeed; }

	/** Sets the normal relative maximum speed. Sends the update to player, if needed. */
	void SetNormalMaxSpeed(double a_Speed);

	/** Sets the sprinting relative maximum speed. Sends the update to player, if needed. */
	void SetSprintingMaxSpeed(double a_Speed);

	/** Sets the flying relative maximum speed. Sends the update to player, if needed. */
	void SetFlyingMaxSpeed(double a_Speed);

	/** Sets the crouch status, broadcasts to all visible players */
	void SetCrouch(bool a_IsCrouched);

	/** Starts or stops sprinting, sends the max speed update to the client, if needed */
	void SetSprint(bool a_IsSprinting);

	/** Flags the player as flying */
	void SetFlying(bool a_IsFlying);

	/** If true the player can fly even when he's not in creative. */
	void SetCanFly(bool a_CanFly);

	/** Returns true if the player has a custom name. */
	bool HasCustomName(void) const { return !m_CustomName.empty(); }

	/** Returns the custom name of this player. If the player hasn't a custom name, it will return an empty string. */
	const AString & GetCustomName(void) const { return m_CustomName; }

	/** Sets the custom name of this player. If you want to disable the custom name, simply set an empty string.
	The custom name will be used in the tab-list, in the player nametag and in the tab-completion. */
	void SetCustomName(const AString & a_CustomName);

	/** Gets the last position that the player slept in
	This is initialised to the world spawn point if the player has not slept in a bed as of yet
	*/
	Vector3i GetLastBedPos(void) const { return m_LastBedPos; }

	/** Sets the player's bed (home / respawn) position to the specified position.
	Sets the respawn world to the player's world. */
	void SetBedPos(const Vector3i & a_Pos);

	/** Sets the player's bed (home / respawn) position and respawn world to the specified parameters. */
	void SetBedPos(const Vector3i & a_Pos, cWorld * a_World);

	// tolua_end

	// TODO lua export GetBedPos and GetBedWorld
	cWorld * GetBedWorld();

	/** Update movement-related statistics. */
	void UpdateMovementStats(const Vector3d & a_DeltaPos, bool a_PreviousIsOnGround);

	/** Whether placing the given blocks would intersect any entitiy */
	bool DoesPlacingBlocksIntersectEntity(const sSetBlockVector & a_Blocks);

	// tolua_begin

	/** Returns wheter the player can fly or not. */
	virtual bool CanFly(void) const { return m_CanFly; }

	/** Returns the UUID (short format) that has been read from the client, or empty string if not available. */
	const AString & GetUUID(void) const { return m_UUID; }

	/** (Re)loads the rank and permissions from the cRankManager.
	Expects the m_UUID member to be valid.
	Loads the m_Rank, m_Permissions, m_MsgPrefix, m_MsgSuffix and m_MsgNameColorCode members. */
	void LoadRank(void);

	/** Calls the block-placement hook and places the block in the world, unless refused by the hook.
	If the hook prevents the placement, sends the current block at the specified coords back to the client.
	Assumes that the block is in a currently loaded chunk.
	Returns true if the block is successfully placed. */
	bool PlaceBlock(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta);

	/** Sends the block in the specified range around the specified coord to the client
	as a block change packet.
	The blocks in range (a_BlockX - a_Range, a_BlockX + a_Range) are sent (NY-metric). */
	void SendBlocksAround(int a_BlockX, int a_BlockY, int a_BlockZ, int a_Range = 1);

	bool HasSkinPart(eSkinPart a_Part) const { return (m_SkinParts & a_Part) != 0; }
	int GetSkinParts(void) const { return m_SkinParts; }
	void SetSkinParts(int a_Parts);

	eMainHand GetMainHand(void) const { return m_MainHand; }
	void SetMainHand(eMainHand a_Hand);

	// tolua_end

	/** Calls the block placement hooks and places the blocks in the world.
	First the "placing" hooks for all the blocks are called, then the blocks are placed, and finally
	the "placed" hooks are called.
	If the any of the "placing" hooks aborts, none of the blocks are placed and the function returns false.
	Returns true if all the blocks are placed.
	Assumes that all the blocks are in currently loaded chunks. */
	bool PlaceBlocks(const sSetBlockVector & a_Blocks);

	/** Notify nearby wolves that the player or one of the player's wolves took damage or did damage to an entity
	@param a_Opponent the opponent we're fighting.
	@param a_IsPlayerInvolved Should be true if the player took or did damage, and false if one of the player's wolves took or did damage.
	*/
	void NotifyNearbyWolves(cPawn * a_Opponent, bool a_IsPlayerInvolved);

	// cEntity overrides:
	virtual bool IsCrouched (void) const override { return m_IsCrouched; }
	virtual bool IsSprinting(void) const override { return m_IsSprinting; }
	virtual bool IsRclking  (void) const override { return IsEating() || IsChargingBow(); }

	virtual void AttachTo(cEntity * a_AttachTo) override;
	virtual void Detach(void) override;

	/** Called by cClientHandle when the client is being destroyed.
	The player removes its m_ClientHandle ownership so that the ClientHandle gets deleted. */
	void RemoveClientHandle(void);

	/** Returns the relative block hardness for the block a_Block.
	The bigger it is the faster the player can break the block.
	Returns zero if the block is instant breakable.
	Otherwise it returns the dig speed (float GetDigSpeed(BLOCKTYPE a_Block)) divided by the block hardness (cBlockInfo::GetHardness(BLOCKTYPE a_Block)) divided by 30 if the player can harvest the block and divided by 100 if he can't. */
	float GetPlayerRelativeBlockHardness(BLOCKTYPE a_Block);

protected:

	typedef std::vector<std::vector<AString> > AStringVectorVector;

	/** The name of the rank assigned to this player. */
	AString m_Rank;

	/** All the permissions that this player has, based on their rank. */
	AStringVector m_Permissions;

	/** All the restrictions that this player has, based on their rank. */
	AStringVector m_Restrictions;

	/** All the permissions that this player has, based on their rank, split into individual dot-delimited parts.
	This is used mainly by the HasPermission() function to optimize the lookup. */
	AStringVectorVector m_SplitPermissions;

	/** All the restrictions that this player has, based on their rank, split into individual dot-delimited parts.
	This is used mainly by the HasPermission() function to optimize the lookup. */
	AStringVectorVector m_SplitRestrictions;


	// Message visuals:
	AString m_MsgPrefix, m_MsgSuffix;
	AString m_MsgNameColorCode;

	AString m_PlayerName;
	AString m_LoadedWorldName;

	/** Xp Level stuff */
	enum
	{
		XP_TO_LEVEL15 = 255,
		XP_PER_LEVEL_TO15 = 17,
		XP_TO_LEVEL30 = 825
	} ;

	bool m_bVisible;

	// Food-related variables:
	/** Represents the food bar, one point equals half a "drumstick" */
	int m_FoodLevel;

	/** "Overcharge" for the m_FoodLevel; is depleted before m_FoodLevel */
	double m_FoodSaturationLevel;

	/** Count-up to the healing or damaging action, based on m_FoodLevel */
	int m_FoodTickTimer;

	/** A "buffer" which adds up hunger before it is substracted from m_FoodSaturationLevel or m_FoodLevel. Each action adds a little */
	double m_FoodExhaustionLevel;

	double m_Stance;

	/** Stores the player's inventory, consisting of crafting grid, hotbar, and main slots */
	cInventory m_Inventory;

	/** An item grid that stores the player specific enderchest contents */
	cItemGrid m_EnderChestContents;

	cWindow * m_CurrentWindow;
	cWindow * m_InventoryWindow;

	/** The player's last saved bed position */
	Vector3i m_LastBedPos;

	/** The world which the player respawns in upon death */
	cWorld * m_SpawnWorld;

	eGameMode m_GameMode;
	AString m_IP;

	/** The item being dragged by the cursor while in a UI window */
	cItem m_DraggingItem;

	std::chrono::steady_clock::time_point m_LastPlayerListTime;

	cClientHandlePtr m_ClientHandle;

	cSlotNums m_InventoryPaintSlots;

	/** If true, we are locking m_Position to m_FrozenPosition. */
	bool m_IsFrozen;

	/** Was the player frozen manually by a plugin or automatically by the server? */
	bool m_IsManuallyFrozen;

	/** Max speed, relative to the game default.
	1 means regular speed, 2 means twice as fast, 0.5 means half-speed.
	Default value is 1. */
	double m_NormalMaxSpeed;

	/** Max speed, relative to the game default max speed, when sprinting.
	1 means regular speed, 2 means twice as fast, 0.5 means half-speed.
	Default value is 1.3. */
	double m_SprintingMaxSpeed;

	/** Max speed, relative to the game default flying max speed, when flying.
	1 means regular speed, 2 means twice as fast, 0.5 means half-speed.
	Default value is 1. */
	double m_FlyingMaxSpeed;

	bool m_IsCrouched;
	bool m_IsSprinting;
	bool m_IsFlying;
	bool m_IsFishing;

	bool m_CanFly;  // If this is true the player can fly. Even if he is not in creative.

	/** The world tick in which eating will be finished. -1 if not eating */
	Int64 m_EatingFinishTick;

	/** Player Xp level */
	int m_LifetimeTotalXp;
	int m_CurrentXp;

	// flag saying we need to send a xp update to client
	bool m_bDirtyExperience;

	bool m_IsChargingBow;
	int  m_BowCharge;

	UInt32 m_FloaterID;

	cTeam * m_Team;

	cStatManager m_Stats;

	/** Flag representing whether the player is currently in a bed
	Set by a right click on unoccupied bed, unset by a time fast forward or teleport */
	bool m_bIsInBed;

	/** How long till the player's inventory will be saved
	Default save interval is #defined in PLAYER_INVENTORY_SAVE_INTERVAL */
	unsigned int m_TicksUntilNextSave;

	/** Flag used by food handling system to determine whether a teleport has just happened
	Will not apply food penalties if found to be true; will set to false after processing
	*/
	bool m_bIsTeleporting;

	/** The short UUID (no dashes) of the player, as read from the ClientHandle.
	If no ClientHandle is given, the UUID is initialized to empty. */
	AString m_UUID;

	AString m_CustomName;

	/** Displayed skin part bit mask */
	int m_SkinParts;

	/** The main hand of the player */
	eMainHand m_MainHand;

	/** Sets the speed and sends it to the client, so that they are forced to move so. */
	virtual void DoSetSpeed(double a_SpeedX, double a_SpeedY, double a_SpeedZ) override;

	void ResolvePermissions(void);
	void ResolveGroups(void);

	virtual void Destroyed(void) override;

	/** Filters out damage for creative mode / friendly fire */
	virtual bool DoTakeDamage(TakeDamageInfo & TDI) override;

	/** Stops players from burning in creative mode */
	virtual void TickBurning(cChunk & a_Chunk) override;

	/** Called in each tick to handle food-related processing */
	void HandleFood(void);

	/** Called in each tick if the player is fishing to make sure the floater dissapears when the player doesn't have a fishing rod as equipped item. */
	void HandleFloater(void);

	/** Tosses a list of items. */
	void TossItems(const cItems & a_Items);

	/** Returns the filename for the player data based on the UUID given.
	This can be used both for online and offline UUIDs. */
	AString GetUUIDFileName(const AString & a_UUID);

private:

	/** Pins the player to a_Location until Unfreeze() is called.
	If ManuallyFrozen is false, the player will unfreeze when the chunk is loaded. */
	void FreezeInternal(const Vector3d & a_Location, bool a_ManuallyFrozen);

	/** Returns how high the liquid is in percent. Used by IsInsideWater */
	float GetLiquidHeightPercent(NIBBLETYPE a_Meta);

	/** Checks if the player is inside of water */
	bool IsInsideWater();

	/** Returns the dig speed using the current tool on the block a_Block.
	Returns one if using hand.
	If the player is using a tool that is good to break the block the value is higher.
	If he has an enchanted tool with efficiency or he has a haste or mining fatique effect it gets multiplied by a specific factor depending on the strength of the effect or enchantment.
	In he is in water it gets divided by 5 except his tool is enchanted with aqa affinity.
	If he is not on ground it also gets divided by 5. */
	float GetDigSpeed(BLOCKTYPE a_Block);

} ;  // tolua_export
