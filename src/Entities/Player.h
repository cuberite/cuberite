
#pragma once

#include "Pawn.h"
#include "../Inventory.h"
#include "../Defines.h"
#include "../World.h"
#include "../Items/ItemHandler.h"

#include "../StatisticsManager.h"

#include "../UUID.h"





class cWindow;
class cClientHandle;
class cTeam;





// tolua_begin
class cPlayer:
	public cPawn
{
	// tolua_end

	using Super = cPawn;

	/** Tag representing a sneaking pose. */
	struct BodyStanceCrouching
	{
		BodyStanceCrouching(cPlayer & a_Player);
	};

	/** Tag representing a sleeping pose.
	Set by a right click on unoccupied bed, unset by a time fast forward or teleport. */
	struct BodyStanceSleeping
	{
		BodyStanceSleeping(cPlayer & a_Player);
	};

	/** Tag representing a sprinting pose. */
	struct BodyStanceSprinting
	{
	};

	/** Tag representing the neutral stance. */
	struct BodyStanceStanding
	{
		BodyStanceStanding(cPlayer & a_Player);
	};

	/** Tag representing a swimming or elytra flying pose. */
	struct BodyStanceGliding
	{
		BodyStanceGliding(cPlayer & a_Player);

		cTickTime TicksElytraFlying;
	};

	/*
	struct HandStanceNeutral
	{
	};

	struct HandStandChargingBow
	{
		int m_BowCharge;
	};

	struct HandStanceEating
	{
	};
	*/

	// tolua_begin

public:

	static const int MAX_HEALTH;

	static const int MAX_FOOD_LEVEL;

	// tolua_end

	CLASS_PROTODEF(cPlayer)

	cPlayer(const std::shared_ptr<cClientHandle> & a_Client);
	virtual ~cPlayer() override;

	/** Called when spectation stops, because the player crouched or when the entity we're spectating gets removed from the world. */
	void OnLoseSpectated();

	/** Resend the RenderDistanceCenter packet to the player */
	void ResendRenderDistanceCenter();

	void SendPlayerInventoryJoin();
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
	int GetXpLevel(void) const;

	/** Gets the experience bar percentage - XpP */
	float GetXpPercentage(void) const;

	/** Calculates the amount of XP needed for a given level
	Ref: https://minecraft.wiki/w/XP
	*/
	static int XpForLevel(int a_Level);

	/** Inverse of XpForLevel
	Ref: https://minecraft.wiki/w/XP
	values are as per this with pre-calculations
	*/
	static int CalcLevelFromXp(int a_CurrentXp);

	// tolua_end

	/** Gets the set of IDs for recipes this player has discovered. */
	const std::set<UInt32> & GetKnownRecipes() const;

	/** Starts charging the equipped bow */
	void StartChargingBow(void);

	/** Finishes charging the current bow. Returns the number of ticks for which the bow has been charged */
	int FinishChargingBow(void);

	/** Cancels the current bow charging */
	void CancelChargingBow(void);

	/** Gets If The Player Is Teleporting To Fix https://github.com/cuberite/cuberite/issues/5542 */
	bool GetIsTeleporting() { return m_IsTeleporting; }

	/** Returns true if the player is currently charging the bow */
	bool IsChargingBow(void) const { return m_IsChargingBow; }

	void SetTouchGround(bool a_bTouchGround);
	double GetEyeHeight(void) const;  // tolua_export
	Vector3d GetEyePosition(void) const;  // tolua_export
	inline cInventory &       GetInventory(void)       { return m_Inventory; }  // tolua_export
	inline const cInventory & GetInventory(void) const { return m_Inventory; }

	/** Gets the contents of the player's associated enderchest */
	cItemGrid & GetEnderChestContents(void) { return m_EnderChestContents; }

	inline const cItem & GetEquippedItem(void) const { return GetInventory().GetEquippedItem(); }  // tolua_export

	/** Returns whether the player is climbing (ladders, vines etc.) */
	bool IsClimbing(void) const;

	virtual void TeleportToCoords(double a_PosX, double a_PosY, double a_PosZ) override;

	/** Updates player's capabilities - flying, visibility, etc. from their gamemode. */
	void UpdateCapabilities();

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

	/** Spectates the target entity. If a_Target is nullptr or a pointer to self, end spectation. */
	void SpectateEntity(cEntity * a_Target);

	/** Returns the position where projectiles thrown by this player should start, player eye position + adjustment */
	Vector3d GetThrowStartPos(void) const;

	/** Returns the initial speed vector of a throw, with a 3D length of a_SpeedCoeff. */
	Vector3d GetThrowSpeed(double a_SpeedCoeff) const;

	/** Returns the current gamemode. Partly OBSOLETE, you should use IsGameModeXXX() functions wherever applicable */
	eGameMode GetGameMode(void) const { return m_GameMode; }

	/** Returns the current effective gamemode (inherited gamemode is resolved before returning) */
	eGameMode GetEffectiveGameMode(void) const;

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

	/** Returns true if the player is fireproof
	Stops players burning in creative or spectator modes.
	*/
	virtual bool IsFireproof() const override
	{
		return (m_IsFireproof || IsGameModeCreative() || IsGameModeSpectator());

	}

	/** Returns true if the player can be targeted by Mobs */
	bool CanMobsTarget(void) const;

	AString GetIP(void) const;  // tolua_export

	/** Return the associated statistic and achievement manager. */
	StatisticsManager & GetStatistics() { return m_Stats; }

	/** Returns the associated team, nullptr if none */
	cTeam * GetTeam(void) { return m_Team; }  // tolua_export

	/** Returns the associated team, nullptr if none */
	const cTeam * GetTeam(void) const { return m_Team; }

	/** Sets the player team, nullptr if none */
	void SetTeam(cTeam * a_Team);

	// tolua_end

	/** Forces the player to query the scoreboard for his team */
	cTeam * UpdateTeam(void);

	/** Awards the player an achievement.
	If all prerequisites are met, this method will award the achievement and will broadcast a chat message.
	If the achievement has been already awarded to the player, this method will just increment the stat counter. */
	void AwardAchievement(CustomStatistic a_Ach);

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

	cClientHandle * GetClientHandle(void) const { return m_ClientHandle.get(); }

	// tolua_end

	/** Get a copy of the PRNG for enchanting related generation, don't use this for other purposes.
	The PRNG's state is initialised with an internal seed, such that until PermuteEnchantmentSeed is called, this function returns the same PRNG. */
	MTRand GetEnchantmentRandomProvider();

	/** Permute the seed for enchanting related PRNGs, don't use this for other purposes. */
	void PermuteEnchantmentSeed();

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

	const AString & GetName(void) const;

	// tolua_end

	bool HasPermission(const AString & a_Permission) const;  // tolua_export

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

	/** Removes one item from the the current equipped item stack, and attempts to add the specified item stack
	back to the same slot. If it is not possible to place the item in the same slot, tries to place the specified
	item elsewhere in the inventory. If this is not possible, then any remaining items are tossed. If the currently
	equipped slot is empty, its contents are simply set to the given Item.
	*/
	void ReplaceOneEquippedItemTossRest(const cItem &);

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
	bool IsEating(void) const { return m_EatingFinishTick >= 0_tick; }

	/** Returns true if the player is currently flying */
	bool IsFlying(void) const { return m_IsFlying; }

	/** Returns true if a player is sleeping in a bed. */
	bool IsInBed(void) const;

	/** Returns true if the player's left hand is dominant. */
	bool IsLeftHanded() const;

	/** Returns true if the player has thrown out a floater */
	bool IsFishing(void) const { return m_IsFishing; }

	void SetIsFishing(bool a_IsFishing, UInt32 a_FloaterID = cEntity::INVALID_ID) { m_IsFishing = a_IsFishing; m_FloaterID = a_FloaterID; }

	UInt32 GetFloaterID(void) const { return m_FloaterID; }

	// tolua_end

	/** Returns true if a player is standing normally, that is, in a neutral pose. */
	bool IsStanding() const;

	/** Tosses a list of items. */
	void TossItems(const cItems & a_Items);

	/** Sets a player's in-bed state.
	We can't be sure plugins will keep this value updated, so no exporting.
	If value is false (not in bed), will update players of the fact that they have been ejected from the bed. */
	void SetIsInBed(bool a_IsInBed);

	/** Starts eating the currently equipped item. Resets the eating timer and sends the proper animation packet */
	void StartEating(void);

	/** Finishes eating the currently equipped item. Consumes the item, updates health and broadcasts the packets */
	void FinishEating(void);

	/** Aborts the current eating operation */
	void AbortEating(void);

	virtual void KilledBy(TakeDamageInfo & a_TDI) override;

	virtual void Killed(const cEntity & a_Victim, eDamageType a_DamageType) override;

	void Respawn(void);  // tolua_export

	void SetVisible( bool a_bVisible);  // tolua_export

	/** Saves all player data, such as inventory, to JSON. */
	void SaveToDisk(void);

	/** Loads the player data from the save file.
	Sets m_World to the world where the player will spawn, based on the stored world name or the default world by calling LoadFromFile(). */
	void LoadFromDisk();

	const AString & GetLoadedWorldName() const { return m_CurrentWorldName; }

	/** Opens the inventory of any tame horse the player is riding.
	If the player is not riding a horse or if the horse is untamed, does nothing. */
	void OpenHorseInventory();

	/** Damage the player's equipped item by a_Damage, possibly less if the
	equipped item is enchanted. */
	void UseEquippedItem(short a_Damage = 1);

	/** Damage the player's equipped item by the amount of damage such an item
	is damaged by when used for a_Action */
	void UseEquippedItem(cItemHandler::eDurabilityLostAction a_Action);

	/** Damage the item in a_SlotNumber by a_Damage, possibly less if the
	equipped item is enchanted. */
	void UseItem(int a_SlotNumber, short a_Damage = 1);

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

	/** Starts or stops crouching, if our current body stance permits, broadcasting the state change. */
	void SetCrouch(bool a_ShouldCrouch);

	/** Starts or stops elytra flight, if our current body stance permits, broadcasting the state change. */
	void SetElytraFlight(bool a_ShouldElytraFly);

	/** Starts or stops flying, broadcasting the state change. */
	void SetFlying(bool a_ShouldFly);

	/** Sets the dominant hand of the player. */
	void SetLeftHanded(bool a_IsLeftHanded);

	/** Starts or stops sprinting, if our current body stance permits, broadcasting the state change. */
	void SetSprint(bool a_ShouldSprint);

	/** If true the player can fly even when he's not in creative. */
	void SetCanFly(bool a_CanFly);

	/** Returns true if the player has a custom name. */
	bool HasCustomName(void) const { return !m_CustomName.empty(); }

	/** Returns the custom name of this player. If the player hasn't a custom name, it will return an empty string. */
	const AString & GetCustomName(void) const { return m_CustomName; }

	/** Sets the custom name of this player. If you want to disable the custom name, simply set an empty string.
	The custom name will be used in the tab-list, in the player nametag and in the tab-completion. */
	void SetCustomName(const AString & a_CustomName);

	/** Gets the player's potential respawn position (named LastBedPos for compatibility reasons). */
	Vector3i GetLastBedPos(void) const { return m_RespawnPosition; }

	/** Returns if the respawn point is unconditionally used. */
	bool IsRespawnPointForced(void) const { return m_IsRespawnPointForced; }

	/** Sets the player's bed position to the specified position.
	Sets the respawn world to the player's world and unforces the respawn point.
	The given position will be used subject to bed checks when respawning. */
	void SetBedPos(Vector3i a_Position);

	/** Sets the player's bed position to the specified position.
	The spawn point is unforced. The given position will be used subject to bed checks when respawning. */
	void SetBedPos(Vector3i a_Position, const cWorld & a_World);

	/** Sets the player's forced respawn position and world. */
	void SetRespawnPosition(Vector3i a_Position, const cWorld & a_World);

	// tolua_end

	// TODO lua export GetRespawnWorld
	cWorld * GetRespawnWorld();

	/** Update movement-related statistics. */
	void UpdateMovementStats(const Vector3d & a_DeltaPos, bool a_PreviousIsOnGround);

	/** Whether placing the given blocks would intersect any entitiy */
	bool DoesPlacingBlocksIntersectEntity(std::initializer_list<sSetBlock> a_Blocks) const;

	/** Returns the UUID that has been read from the client, or nil if not available. */
	const cUUID & GetUUID(void) const;  // Exported in ManualBindings.cpp

	// tolua_begin

	/** Returns wheter the player can fly or not. */
	virtual bool CanFly(void) const { return m_IsFlightCapable; }

	/** (Re)loads the rank and permissions from the cRankManager and sends a permission level update to the client. */
	void LoadRank(void);

	/** Sends the block in the specified range around the specified coord to the client
	as a block change packet.
	The blocks in range (a_BlockX - a_Range, a_BlockX + a_Range) are sent (NY-metric). */
	void SendBlocksAround(Vector3i a_BlockPos, int a_Range = 1);

	bool HasSkinPart(eSkinPart a_Part) const { return (m_SkinParts & a_Part) != 0; }
	int GetSkinParts(void) const { return m_SkinParts; }
	void SetSkinParts(int a_Parts);

	// tolua_end

	/** Attempts to place the block in the world with a call to PlaceBlocks. */
	bool PlaceBlock(Vector3i a_Pos, BlockState a_Block);

	/** Calls the block placement hooks and places the blocks in the world.
	First the "placing" hooks for all the blocks are called, then the blocks are placed, and finally the "placed" hooks are called.
	If the any of the "placing" hooks aborts, none of the blocks are placed and the function returns false.
	Returns true if all the blocks are placed. */
	bool PlaceBlocks(std::initializer_list<sSetBlock> a_Blocks);

	/** Notify nearby wolves that the player or one of the player's wolves took damage or did damage to an entity
	@param a_Opponent the opponent we're fighting.
	@param a_IsPlayerInvolved Should be true if the player took or did damage, and false if one of the player's wolves took or did damage.
	*/
	void NotifyNearbyWolves(cPawn * a_Opponent, bool a_IsPlayerInvolved);

	/** Returns the progress mined per tick for the block a_Block as a fraction
	(1 would be completely mined)
	Depends on hardness values so check those are correct.
	Source: https://minecraft.gamepedia.com/Breaking#Calculation */
	float GetMiningProgressPerTick(BlockState a_Block);

	/** Given tool, enchantments, status effects, and world position
	returns whether a_Block would be instantly mined.
	Depends on hardness values so check those are correct.
	Source: https://minecraft.gamepedia.com/Breaking#Instant_breaking */
	bool CanInstantlyMine(BlockState a_Block);

	/** Adds an Item to the list of known items.
	If the item is already known, does nothing. */
	void AddKnownItem(const cItem & a_Item);

	// cEntity overrides:
	virtual cItem GetEquippedWeapon(void) const override { return m_Inventory.GetEquippedItem(); }
	virtual cItem GetEquippedHelmet(void) const override { return m_Inventory.GetEquippedHelmet(); }
	virtual cItem GetEquippedChestplate(void) const override { return m_Inventory.GetEquippedChestplate(); }
	virtual cItem GetEquippedLeggings(void) const override { return m_Inventory.GetEquippedLeggings(); }
	virtual cItem GetEquippedBoots(void) const override { return m_Inventory.GetEquippedBoots(); }
	virtual cItem GetOffHandEquipedItem(void) const override { return m_Inventory.GetShieldSlot(); }
	virtual bool IsCrouched(void) const override;
	virtual bool IsOnGround(void) const override { return m_bTouchGround; }
	virtual bool IsSprinting(void) const override;

private:

	typedef std::vector<std::vector<AString> > AStringVectorVector;

	/** The current body stance the player has adopted. */
	std::variant<BodyStanceCrouching, BodyStanceSleeping, BodyStanceSprinting, BodyStanceStanding, BodyStanceGliding> m_BodyStance;

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

	// Food-related variables:
	/** Represents the food bar, one point equals half a "drumstick" */
	int m_FoodLevel;

	/** "Overcharge" for the m_FoodLevel; is depleted before m_FoodLevel */
	double m_FoodSaturationLevel;

	/** Count-up to the healing or damaging action, based on m_FoodLevel */
	int m_FoodTickTimer;

	/** A "buffer" which adds up hunger before it is substracted from m_FoodSaturationLevel or m_FoodLevel. Each action adds a little */
	double m_FoodExhaustionLevel;

	/** Stores the player's inventory, consisting of crafting grid, hotbar, and main slots */
	cInventory m_Inventory;

	/** An item grid that stores the player specific enderchest contents */
	cItemGrid m_EnderChestContents;

	cWindow * m_CurrentWindow;
	cWindow * m_InventoryWindow;

	/** The player's potential respawn position, initialised to world spawn by default.
	During player respawn from death, if m_IsRespawnPointForced is false and no bed exists here, it will be reset to world spawn. */
	Vector3i m_RespawnPosition;

	/** The name of the world which the player respawns in upon death.
	This is stored as a string to enable SaveToDisk to not touch cRoot, and thus can be safely called in the player's destructor. */
	std::string m_SpawnWorldName;

	/** The name of the world which the player currently resides in.
	Stored in addition to m_World to allow SaveToDisk to be safely called in the player's destructor. */
	std::string m_CurrentWorldName;

	/** The save path of the default world. */
	std::string m_DefaultWorldPath;

	eGameMode m_GameMode;

	/** The item being dragged by the cursor while in a UI window */
	cItem m_DraggingItem;

	std::shared_ptr<cClientHandle> m_ClientHandle;

	cSlotNums m_InventoryPaintSlots;

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

	bool m_IsChargingBow;
	bool m_IsFishing;

	/** If this is true the player can fly. Even if he is not in creative. */
	bool m_IsFlightCapable;

	bool m_IsFlying;

	/** If true, we are locking m_Position to m_FrozenPosition. */
	bool m_IsFrozen;

	/** Whether the player is left-handed, or right-handed. */
	bool m_IsLeftHanded;

	/** Was the player frozen manually by a plugin or automatically by the server? */
	bool m_IsManuallyFrozen;

	/** Whether we unconditionally respawn to m_RespawnPosition, or check if a bed is unobstructed and available first. */
	bool m_IsRespawnPointForced;

	/** Flag used by food handling system to determine whether a teleport has just happened.
	Will not apply food penalties if found to be true; will set to false after processing. */
	bool m_IsTeleporting;

	bool m_IsVisible;

	/** The world tick in which eating will be finished. -1 if not eating */
	cTickTimeLong m_EatingFinishTick;

	/** Player Xp level */
	int m_LifetimeTotalXp;
	int m_CurrentXp;
	unsigned int m_EnchantmentSeed;

	int m_BowCharge;

	UInt32 m_FloaterID;

	cTeam * m_Team;

	/** The entity that this player is spectating, nullptr if none. */
	cEntity * m_Spectating;

	StatisticsManager m_Stats;

	/** How long till the player's inventory will be saved
	Default save interval is #defined in PLAYER_INVENTORY_SAVE_INTERVAL */
	unsigned int m_TicksUntilNextSave;

	AString m_CustomName;

	/** Displayed skin part bit mask */
	int m_SkinParts;

	/** List on known recipes as Ids */
	std::set<UInt32> m_KnownRecipes;

	/** List of known items as Ids */
	std::set<cItem, cItem::sItemCompare> m_KnownItems;

	/** Called in each tick to handle food-related processing */
	void HandleFood(void);

	/** Called in each tick if the player is fishing to make sure the floater dissapears when the player doesn't have a fishing rod as equipped item. */
	void HandleFloater(void);

	/** Returns the filename for the player data based on the UUID given.
	This can be used both for online and offline UUIDs. */
	AString GetUUIDFileName(const cUUID & a_UUID);

	/** Pins the player to a_Location until Unfreeze() is called.
	If ManuallyFrozen is false, the player will unfreeze when the chunk is loaded. */
	void FreezeInternal(const Vector3d & a_Location, bool a_ManuallyFrozen);

	/** Returns how high the liquid is in percent. Used by IsInsideWater */
	float GetLiquidHeightPercent(BlockState a_Block);

	/** Checks if the player is inside of water */
	bool IsInsideWater();

	/** Returns the dig speed using the current tool on the block a_Block.
	Returns one if using hand.
	If the player is using a tool that is good to break the block the value is higher.
	If he has an enchanted tool with efficiency or he has a haste or mining fatique effect it gets multiplied by a specific factor depending on the strength of the effect or enchantment.
	In he is in water it gets divided by 5 except if his tool is enchanted with aqua affinity.
	If he is not on ground it also gets divided by 5.
	Source: https://minecraft.gamepedia.com/Breaking#Calculation */
	float GetDigSpeed(BlockState a_Block);

	/** Add the recipe Id to the known recipes.
	If the recipe is already known, does nothing. */
	void AddKnownRecipe(UInt32 RecipeId);

	void TickFreezeCode();

	/** (Re)loads the rank and permissions from the cRankManager.
	Loads the m_Rank, m_Permissions, m_MsgPrefix, m_MsgSuffix and m_MsgNameColorCode members. */
	void RefreshRank();

	// cEntity overrides:
	virtual void ApplyArmorDamage(int DamageBlocked) override;
	virtual void BroadcastMovementUpdate(const cClientHandle * a_Exclude = nullptr) override;
	virtual bool DoTakeDamage(TakeDamageInfo & TDI) override;
	virtual float GetEnchantmentBlastKnockbackReduction() override;
	virtual void HandlePhysics(std::chrono::milliseconds a_Dt, cChunk &) override { UNUSED(a_Dt); }
	virtual bool IsElytraFlying(void) const override;
	virtual bool IsInvisible() const override;
	virtual bool IsRclking(void) const override { return IsEating() || IsChargingBow(); }
	virtual void OnAddToWorld(cWorld & a_World) override;
	virtual void OnDetach() override;
	virtual void OnRemoveFromWorld(cWorld & a_World) override;
	virtual void SpawnOn(cClientHandle & a_Client) override;
	virtual void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;

} ;  // tolua_export
