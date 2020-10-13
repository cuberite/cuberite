
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Player.h"
#include "../Mobs/Wolf.h"
#include "../Mobs/Horse.h"
#include "../BoundingBox.h"
#include "../ChatColor.h"
#include "../Server.h"
#include "../UI/InventoryWindow.h"
#include "../UI/WindowOwner.h"
#include "../Bindings/PluginManager.h"
#include "../BlockEntities/BlockEntity.h"
#include "../BlockEntities/EnderChestEntity.h"
#include "../Root.h"
#include "../Chunk.h"
#include "../Items/ItemHandler.h"
#include "../FastRandom.h"
#include "../ClientHandle.h"

#include "../WorldStorage/StatSerializer.h"
#include "../CompositeChat.h"

#include "../Blocks/BlockHandler.h"
#include "../Blocks/BlockSlab.h"
#include "../Blocks/ChunkInterface.h"

#include "../IniFile.h"
#include "../JsonUtils.h"
#include "json/json.h"

#include "../CraftingRecipes.h"

// 6000 ticks or 5 minutes
#define PLAYER_INVENTORY_SAVE_INTERVAL 6000

// 1000 = once per second
#define PLAYER_LIST_TIME_MS std::chrono::milliseconds(1000)

namespace
{

/** Returns the old Offline UUID generated before becoming vanilla compliant. */
cUUID GetOldStyleOfflineUUID(const AString & a_PlayerName)
{
	// Use lowercase username
	auto BaseUUID = cUUID::GenerateVersion3(StrToLower(a_PlayerName)).ToRaw();
	// Clobber a full nibble around the variant bits
	BaseUUID[8] = (BaseUUID[8] & 0x0f) | 0x80;

	cUUID Ret;
	Ret.FromRaw(BaseUUID);
	return Ret;
}





/** Returns the folder for the player data based on the UUID given.
This can be used both for online and offline UUIDs. */
AString GetUUIDFolderName(const cUUID & a_Uuid)
{
	AString UUID = a_Uuid.ToShortString();

	AString res("players/");
	res.append(UUID, 0, 2);
	res.push_back('/');
	return res;
}

}  // namespace (anonymous)





const int cPlayer::MAX_HEALTH = 20;

const int cPlayer::MAX_FOOD_LEVEL = 20;

/** Number of ticks it takes to eat an item */
const int cPlayer::EATING_TICKS = 30;





cPlayer::cPlayer(const cClientHandlePtr & a_Client, const AString & a_PlayerName) :
	Super(etPlayer, 0.6, 1.8),
	m_bVisible(true),
	m_FoodLevel(MAX_FOOD_LEVEL),
	m_FoodSaturationLevel(5.0),
	m_FoodTickTimer(0),
	m_FoodExhaustionLevel(0.0),
	m_Stance(0.0),
	m_Inventory(*this),
	m_EnderChestContents(9, 3),
	m_CurrentWindow(nullptr),
	m_InventoryWindow(nullptr),
	m_GameMode(eGameMode_NotSet),
	m_IP(""),
	m_ClientHandle(a_Client),
	m_IsFrozen(false),
	m_NormalMaxSpeed(1.0),
	m_SprintingMaxSpeed(1.3),
	m_FlyingMaxSpeed(1.0),
	m_IsCrouched(false),
	m_IsSprinting(false),
	m_IsFlying(false),
	m_IsFishing(false),
	m_CanFly(false),
	m_EatingFinishTick(-1),
	m_LifetimeTotalXp(0),
	m_CurrentXp(0),
	m_bDirtyExperience(false),
	m_IsChargingBow(false),
	m_BowCharge(0),
	m_FloaterID(cEntity::INVALID_ID),
	m_Team(nullptr),
	m_bIsInBed(false),
	m_TicksUntilNextSave(PLAYER_INVENTORY_SAVE_INTERVAL),
	m_bIsTeleporting(false),
	m_UUID((a_Client != nullptr) ? a_Client->GetUUID() : cUUID{}),
	m_CustomName(""),
	m_SkinParts(0),
	m_MainHand(mhRight)
{
	ASSERT(a_PlayerName.length() <= 16);  // Otherwise this player could crash many clients...

	m_InventoryWindow = new cInventoryWindow(*this);
	m_CurrentWindow = m_InventoryWindow;
	m_InventoryWindow->OpenedByPlayer(*this);

	SetMaxHealth(MAX_HEALTH);
	m_Health = MAX_HEALTH;

	m_LastPlayerListTime = std::chrono::steady_clock::now();
	m_PlayerName = a_PlayerName;

	cWorld * World = nullptr;
	if (!LoadFromDisk(World))
	{
		m_Inventory.Clear();
		SetPosX(World->GetSpawnX());
		SetPosY(World->GetSpawnY());
		SetPosZ(World->GetSpawnZ());

		// This is a new player. Set the player spawn point to the spawn point of the default world
		SetBedPos(Vector3i(static_cast<int>(World->GetSpawnX()), static_cast<int>(World->GetSpawnY()), static_cast<int>(World->GetSpawnZ())), World);

		SetWorld(World);  // Use default world

		m_EnchantmentSeed = GetRandomProvider().RandInt<unsigned int>();  // Use a random number to seed the enchantment generator

		FLOGD("Player \"{0}\" is connecting for the first time, spawning at default world spawn {1:.2f}",
			a_PlayerName, GetPosition()
		);
	}

	m_LastGroundHeight = static_cast<float>(GetPosY());
	m_Stance = GetPosY() + 1.62;


	if (m_GameMode == gmNotSet)
	{
		if (World->IsGameModeCreative())
		{
			m_CanFly = true;
		}
		if (World->IsGameModeSpectator())  // Otherwise Player will fall out of the world on join
		{
			m_CanFly = true;
			m_IsFlying = true;
		}
	}

	if (m_GameMode == gmSpectator)  // If player is reconnecting to the server in spectator mode
	{
		m_CanFly = true;
		m_IsFlying = true;
		m_bVisible = false;
	}
}





bool cPlayer::Initialize(OwnedEntity a_Self, cWorld & a_World)
{
	UNUSED(a_World);
	ASSERT(GetWorld() != nullptr);
	ASSERT(GetParentChunk() == nullptr);
	GetWorld()->AddPlayer(std::unique_ptr<cPlayer>(static_cast<cPlayer *>(a_Self.release())));

	cPluginManager::Get()->CallHookSpawnedEntity(*GetWorld(), *this);

	if (m_KnownRecipes.empty())
	{
		m_ClientHandle->SendInitRecipes(0);
	}
	else
	{
		for (const auto KnownRecipe : m_KnownRecipes)
		{
			m_ClientHandle->SendInitRecipes(KnownRecipe);
		}
	}

	return true;
}





void cPlayer::AddKnownItem(const cItem & a_Item)
{
	if (a_Item.m_ItemType < 0)
	{
		return;
	}

	auto Response = m_KnownItems.insert(a_Item.CopyOne());
	if (!Response.second)
	{
		// The item was already known, bail out:
		return;
	}

	// Process the recipes that got unlocked by this newly-known item:
	auto Recipes = cRoot::Get()->GetCraftingRecipes()->FindNewRecipesForItem(a_Item, m_KnownItems);
	for (const auto & RecipeId : Recipes)
	{
		AddKnownRecipe(RecipeId);
	}
}





void cPlayer::AddKnownRecipe(UInt32 a_RecipeId)
{
	auto Response = m_KnownRecipes.insert(a_RecipeId);
	if (!Response.second)
	{
		// The recipe was already known, bail out:
		return;
	}
	m_ClientHandle->SendUnlockRecipe(a_RecipeId);
}





cPlayer::~cPlayer(void)
{
	if (!cRoot::Get()->GetPluginManager()->CallHookPlayerDestroyed(*this))
	{
		cRoot::Get()->BroadcastChatLeave(Printf("%s has left the game", GetName().c_str()));
		LOGINFO("Player %s has left the game", GetName().c_str());
	}

	LOGD("Deleting cPlayer \"%s\" at %p, ID %d", GetName().c_str(), static_cast<void *>(this), GetUniqueID());

	SaveToDisk();

	m_ClientHandle = nullptr;

	delete m_InventoryWindow;
	m_InventoryWindow = nullptr;

	LOGD("Player %p deleted", static_cast<void *>(this));
}





void cPlayer::Destroyed()
{
	CloseWindow(false);
	Super::Destroyed();
}





void cPlayer::SpawnOn(cClientHandle & a_Client)
{
	if (!m_bVisible || (m_ClientHandle.get() == (&a_Client)))
	{
		return;
	}

	LOGD("Spawing %s on %s", GetName().c_str(), a_Client.GetUsername().c_str());

	a_Client.SendPlayerSpawn(*this);
	a_Client.SendEntityHeadLook(*this);
	a_Client.SendEntityEquipment(*this, 0, m_Inventory.GetEquippedItem());
	a_Client.SendEntityEquipment(*this, 1, m_Inventory.GetEquippedBoots());
	a_Client.SendEntityEquipment(*this, 2, m_Inventory.GetEquippedLeggings());
	a_Client.SendEntityEquipment(*this, 3, m_Inventory.GetEquippedChestplate());
	a_Client.SendEntityEquipment(*this, 4, m_Inventory.GetEquippedHelmet());
}





void cPlayer::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	if (m_ClientHandle != nullptr)
	{
		if (m_ClientHandle->IsDestroyed())
		{
			// This should not happen, because destroying a client will remove it from the world, but just in case
			ASSERT(!"Player ticked whilst in the process of destruction!");
			m_ClientHandle = nullptr;
			return;
		}

		if (!m_ClientHandle->IsPlaying())
		{
			// We're not yet in the game, ignore everything
			return;
		}
	}
	else
	{
		ASSERT(!"Player ticked whilst in the process of destruction!");
	}


	m_Stats.AddValue(Statistic::PlayOneMinute);
	m_Stats.AddValue(Statistic::TimeSinceDeath);
	if (IsCrouched())
	{
		m_Stats.AddValue(Statistic::SneakTime);
	}

	// Handle the player detach, when the player is in spectator mode
	if (
		(IsGameModeSpectator()) &&
		(m_AttachedTo != nullptr) &&
		(
			(m_AttachedTo->IsDestroyed()) ||  // Watching entity destruction
			(m_AttachedTo->GetHealth() <= 0) ||  // Watching entity dead
			(IsCrouched())  // Or the player wants to be detached
		)
	)
	{
		Detach();
	}

	// Handle a frozen player
	TickFreezeCode();
	if (m_IsFrozen)
	{
		return;
	}
	ASSERT((GetParentChunk() != nullptr) && (GetParentChunk()->IsValid()));

	ASSERT(a_Chunk.IsValid());

	Super::Tick(a_Dt, a_Chunk);

	// Handle charging the bow:
	if (m_IsChargingBow)
	{
		m_BowCharge += 1;
	}

	// Handle updating experience
	if (m_bDirtyExperience)
	{
		SendExperience();
	}

	BroadcastMovementUpdate(m_ClientHandle.get());

	if (m_Health > 0)  // make sure player is alive
	{
		m_World->CollectPickupsByPlayer(*this);

		if ((m_EatingFinishTick >= 0) && (m_EatingFinishTick <= m_World->GetWorldAge()))
		{
			FinishEating();
		}

		HandleFood();
	}

	if (m_IsFishing)
	{
		HandleFloater();
	}

	// Update items (e.g. Maps)
	m_Inventory.UpdateItems();

	// Send Player List (Once per m_LastPlayerListTime/1000 ms)
	if (m_LastPlayerListTime + PLAYER_LIST_TIME_MS <= std::chrono::steady_clock::now())
	{
		m_World->BroadcastPlayerListUpdatePing(*this);
		m_LastPlayerListTime = std::chrono::steady_clock::now();
	}

	if (m_TicksUntilNextSave == 0)
	{
		SaveToDisk();
		m_TicksUntilNextSave = PLAYER_INVENTORY_SAVE_INTERVAL;
	}
	else
	{
		m_TicksUntilNextSave--;
	}
}





void cPlayer::TickFreezeCode()
{
	if (m_IsFrozen)
	{
		if ((!m_IsManuallyFrozen) && (GetClientHandle()->IsPlayerChunkSent()))
		{
			// If the player was automatically frozen, unfreeze if the chunk the player is inside is loaded and sent
			Unfreeze();

			// Pull the player out of any solids that might have loaded on them.
			PREPARE_REL_AND_CHUNK(GetPosition(), *(GetParentChunk()));
			if (RelSuccess)
			{
				int NewY = Rel.y;
				if (NewY < 0)
				{
					NewY = 0;
				}
				while (NewY < cChunkDef::Height - 2)
				{
					// If we find a position with enough space for the player
					if (
						!cBlockInfo::IsSolid(Chunk->GetBlock(Rel.x, NewY, Rel.z)) &&
						!cBlockInfo::IsSolid(Chunk->GetBlock(Rel.x, NewY + 1, Rel.z))
					)
					{
						// If the found position is not the same as the original
						if (NewY != Rel.y)
						{
							SetPosition(GetPosition().x, NewY, GetPosition().z);
							GetClientHandle()->SendPlayerPosition();
						}
						break;
					}
					++NewY;
				}
			}
		}
		else if (GetWorld()->GetWorldAge() % 100 == 0)
		{
			// Despite the client side freeze, the player may be able to move a little by
			// Jumping or canceling flight. Re-freeze every now and then
			FreezeInternal(GetPosition(), m_IsManuallyFrozen);
		}
	}
	else
	{
		if (!GetClientHandle()->IsPlayerChunkSent() || (!GetParentChunk()->IsValid()))
		{
			FreezeInternal(GetPosition(), false);
		}
	}
}





int cPlayer::CalcLevelFromXp(int a_XpTotal)
{
	// level 0 to 15
	if (a_XpTotal <= XP_TO_LEVEL15)
	{
		return a_XpTotal / XP_PER_LEVEL_TO15;
	}

	// level 30+
	if (a_XpTotal > XP_TO_LEVEL30)
	{
		return static_cast<int>((151.5 + sqrt( 22952.25 - (14 * (2220 - a_XpTotal)))) / 7);
	}

	// level 16 to 30
	return static_cast<int>((29.5 + sqrt( 870.25 - (6 * ( 360 - a_XpTotal)))) / 3);
}





int cPlayer::XpForLevel(int a_Level)
{
	// level 0 to 15
	if (a_Level <= 15)
	{
		return a_Level * XP_PER_LEVEL_TO15;
	}

	// level 30+
	if (a_Level >= 31)
	{
		return static_cast<int>((3.5 * a_Level * a_Level) - (151.5 * a_Level) + 2220);
	}

	// level 16 to 30
	return static_cast<int>((1.5 * a_Level * a_Level) - (29.5 * a_Level) + 360);
}





int cPlayer::GetXpLevel() const
{
	return CalcLevelFromXp(m_CurrentXp);
}





float cPlayer::GetXpPercentage() const
{
	int currentLevel = CalcLevelFromXp(m_CurrentXp);
	int currentLevel_XpBase = XpForLevel(currentLevel);

	return static_cast<float>(m_CurrentXp - currentLevel_XpBase) /
		static_cast<float>(XpForLevel(1 + currentLevel) - currentLevel_XpBase);
}





bool cPlayer::SetCurrentExperience(int a_CurrentXp)
{
	if (!(a_CurrentXp >= 0) || (a_CurrentXp > (std::numeric_limits<int>::max() - m_LifetimeTotalXp)))
	{
		LOGWARNING("Tried to update experiece with an invalid Xp value: %d", a_CurrentXp);
		return false;  // oops, they gave us a dodgey number
	}

	m_CurrentXp = a_CurrentXp;

	// Set experience to be updated
	m_bDirtyExperience = true;

	return true;
}





int cPlayer::DeltaExperience(int a_Xp_delta)
{
	if (a_Xp_delta > (std::numeric_limits<int>().max() - m_CurrentXp))
	{
		// Value was bad, abort and report
		LOGWARNING("Attempt was made to increment Xp by %d, which overflowed the int datatype. Ignoring.", a_Xp_delta);
		return -1;  // Should we instead just return the current Xp?
	}

	m_CurrentXp += a_Xp_delta;

	// Make sure they didn't subtract too much
	m_CurrentXp = std::max(m_CurrentXp, 0);


	// Update total for score calculation
	if (a_Xp_delta > 0)
	{
		m_LifetimeTotalXp += a_Xp_delta;
	}

	LOGD("Player \"%s\" gained / lost %d experience, total is now: %d", GetName().c_str(), a_Xp_delta, m_CurrentXp);

	// Set experience to be updated
	m_bDirtyExperience = true;

	return m_CurrentXp;
}





void cPlayer::StartChargingBow(void)
{
	LOGD("Player \"%s\" started charging their bow", GetName().c_str());
	m_IsChargingBow = true;
	m_BowCharge = 0;
	m_World->BroadcastEntityMetadata(*this, m_ClientHandle.get());
}





int cPlayer::FinishChargingBow(void)
{
	LOGD("Player \"%s\" finished charging their bow at a charge of %d", GetName().c_str(), m_BowCharge);
	int res = m_BowCharge;
	m_IsChargingBow = false;
	m_BowCharge = 0;
	m_World->BroadcastEntityMetadata(*this, m_ClientHandle.get());

	return res;
}





void cPlayer::CancelChargingBow(void)
{
	LOGD("Player \"%s\" cancelled charging their bow at a charge of %d", GetName().c_str(), m_BowCharge);
	m_IsChargingBow = false;
	m_BowCharge = 0;
	m_World->BroadcastEntityMetadata(*this, m_ClientHandle.get());
}





void cPlayer::SetTouchGround(bool a_bTouchGround)
{
	if (IsGameModeSpectator())  // You can fly through the ground in Spectator
	{
		return;
	}

	UNUSED(a_bTouchGround);
	/* Unfortunately whatever the reason, there are still desyncs in on-ground status between the client and server. For example:
		1. Walking off a ledge (whatever height)
		2. Initial login
	Thus, it is too risky to compare their value against ours and kick them for hacking */
}





void cPlayer::Heal(int a_Health)
{
	Super::Heal(a_Health);
	SendHealth();
}





void cPlayer::SetFoodLevel(int a_FoodLevel)
{
	int FoodLevel = Clamp(a_FoodLevel, 0, MAX_FOOD_LEVEL);

	if (cRoot::Get()->GetPluginManager()->CallHookPlayerFoodLevelChange(*this, FoodLevel))
	{
		m_FoodSaturationLevel = 5.0;
		return;
	}

	m_FoodLevel = FoodLevel;
	SendHealth();
}





void cPlayer::SetFoodSaturationLevel(double a_FoodSaturationLevel)
{
	m_FoodSaturationLevel = Clamp(a_FoodSaturationLevel, 0.0, static_cast<double>(m_FoodLevel));
}





void cPlayer::SetFoodTickTimer(int a_FoodTickTimer)
{
	m_FoodTickTimer = a_FoodTickTimer;
}





void cPlayer::SetFoodExhaustionLevel(double a_FoodExhaustionLevel)
{
	m_FoodExhaustionLevel = Clamp(a_FoodExhaustionLevel, 0.0, 40.0);
}





bool cPlayer::Feed(int a_Food, double a_Saturation)
{
	if (IsSatiated())
	{
		return false;
	}

	SetFoodSaturationLevel(m_FoodSaturationLevel + a_Saturation);
	SetFoodLevel(m_FoodLevel + a_Food);
	return true;
}





void cPlayer::AddFoodExhaustion(double a_Exhaustion)
{
	if (!(IsGameModeCreative() || IsGameModeSpectator()))
	{
		m_FoodExhaustionLevel = std::min(m_FoodExhaustionLevel + a_Exhaustion, 40.0);
	}
}





void cPlayer::TossItems(const cItems & a_Items)
{
	if (IsGameModeSpectator())  // Players can't toss items in spectator
	{
		return;
	}

	m_Stats.AddValue(Statistic::Drop, static_cast<cStatManager::StatValue>(a_Items.Size()));

	const auto Speed = (GetLookVector() + Vector3d(0, 0.2, 0)) * 6;  // A dash of height and a dollop of speed
	const auto Position = GetEyePosition() - Vector3d(0, 0.2, 0);  // Correct for eye-height weirdness
	m_World->SpawnItemPickups(a_Items, Position, Speed, true);  // 'true' because created by player
}





void cPlayer::StartEating(void)
{
	// Set the timer:
	m_EatingFinishTick = m_World->GetWorldAge() + EATING_TICKS;

	// Send the packets:
	m_World->BroadcastEntityAnimation(*this, 3);
	m_World->BroadcastEntityMetadata(*this);
}





void cPlayer::FinishEating(void)
{
	// Reset the timer:
	m_EatingFinishTick = -1;

	// Send the packets:
	m_ClientHandle->SendEntityStatus(*this, esPlayerEatingAccepted);
	m_World->BroadcastEntityMetadata(*this);

	// consume the item:
	cItem Item(GetEquippedItem());
	Item.m_ItemCount = 1;
	cItemHandler * ItemHandler = cItemHandler::GetItemHandler(Item.m_ItemType);
	if (!ItemHandler->EatItem(this, &Item))
	{
		return;
	}
	ItemHandler->OnFoodEaten(m_World, this, &Item);
}





void cPlayer::AbortEating(void)
{
	m_EatingFinishTick = -1;
	m_World->BroadcastEntityMetadata(*this);
}





void cPlayer::SendHealth(void)
{
	if (m_ClientHandle != nullptr)
	{
		m_ClientHandle->SendHealth();
	}
}





void cPlayer::SendHotbarActiveSlot(void)
{
	if (m_ClientHandle != nullptr)
	{
		m_ClientHandle->SendHeldItemChange(m_Inventory.GetEquippedSlotNum());
	}
}





void cPlayer::SendExperience(void)
{
	if (m_ClientHandle != nullptr)
	{
		m_ClientHandle->SendExperience();
		m_bDirtyExperience = false;
	}
}





void cPlayer::ClearInventoryPaintSlots(void)
{
	// Clear the list of slots that are being inventory-painted. Used by cWindow only
	m_InventoryPaintSlots.clear();
}





void cPlayer::AddInventoryPaintSlot(int a_SlotNum)
{
	// Add a slot to the list for inventory painting. Used by cWindow only
	m_InventoryPaintSlots.push_back(a_SlotNum);
}





const cSlotNums & cPlayer::GetInventoryPaintSlots(void) const
{
	// Return the list of slots currently stored for inventory painting. Used by cWindow only
	return m_InventoryPaintSlots;
}





double cPlayer::GetMaxSpeed(void) const
{
	if (m_IsFlying)
	{
		return m_FlyingMaxSpeed;
	}
	else if (m_IsSprinting)
	{
		return m_SprintingMaxSpeed;
	}
	else
	{
		return m_NormalMaxSpeed;
	}
}





void cPlayer::SetNormalMaxSpeed(double a_Speed)
{
	m_NormalMaxSpeed = a_Speed;
	if (!m_IsSprinting && !m_IsFlying && !m_IsFrozen)
	{
		// If we are frozen, we do not send this yet. We send when unfreeze() is called
		m_ClientHandle->SendPlayerMaxSpeed();
	}
}





void cPlayer::SetSprintingMaxSpeed(double a_Speed)
{
	m_SprintingMaxSpeed = a_Speed;
	if (m_IsSprinting && !m_IsFlying && !m_IsFrozen)
	{
		// If we are frozen, we do not send this yet. We send when unfreeze() is called
		m_ClientHandle->SendPlayerMaxSpeed();
	}
}





void cPlayer::SetFlyingMaxSpeed(double a_Speed)
{
	m_FlyingMaxSpeed = a_Speed;

	// Update the flying speed, always:
	if (!m_IsFrozen)
	{
		// If we are frozen, we do not send this yet. We send when unfreeze() is called
		m_ClientHandle->SendPlayerAbilities();
	}
}





void cPlayer::SetCrouch(bool a_IsCrouched)
{
	// Set the crouch status, broadcast to all visible players
	if (a_IsCrouched == m_IsCrouched)
	{
		// No change
		return;
	}

	if (a_IsCrouched)
	{
		cRoot::Get()->GetPluginManager()->CallHookPlayerCrouched(*this);
	}

	m_IsCrouched = a_IsCrouched;
	m_World->BroadcastEntityMetadata(*this);
}





void cPlayer::SetSprint(bool a_IsSprinting)
{
	if (a_IsSprinting == m_IsSprinting)
	{
		// No change
		return;
	}

	m_IsSprinting = a_IsSprinting;
	m_ClientHandle->SendPlayerMaxSpeed();
}





void cPlayer::SetCanFly(bool a_CanFly)
{
	if (a_CanFly == m_CanFly)
	{
		return;
	}

	m_CanFly = a_CanFly;
	m_ClientHandle->SendPlayerAbilities();
}





void cPlayer::SetCustomName(const AString & a_CustomName)
{
	if (m_CustomName == a_CustomName)
	{
		return;
	}

	m_World->BroadcastPlayerListRemovePlayer(*this);

	m_CustomName = a_CustomName;
	if (m_CustomName.length() > 16)
	{
		m_CustomName = m_CustomName.substr(0, 16);
	}

	m_World->BroadcastPlayerListAddPlayer(*this);
	m_World->BroadcastSpawnEntity(*this, GetClientHandle());
}





void cPlayer::SetBedPos(const Vector3i & a_Pos)
{
	m_LastBedPos = a_Pos;
	m_SpawnWorld = m_World;
}





void cPlayer::SetBedPos(const Vector3i & a_Pos, cWorld * a_World)
{
	m_LastBedPos = a_Pos;
	ASSERT(a_World != nullptr);
	m_SpawnWorld = a_World;
}





cWorld * cPlayer::GetBedWorld()
{
	return m_SpawnWorld;
}





void cPlayer::SetFlying(bool a_IsFlying)
{
	if (a_IsFlying == m_IsFlying)
	{
		return;
	}

	m_IsFlying = a_IsFlying;
	if (!m_IsFrozen)
	{
		// If we are frozen, we do not send this yet. We send when unfreeze() is called
		m_ClientHandle->SendPlayerAbilities();
	}
}





void cPlayer::ApplyArmorDamage(int a_DamageBlocked)
{
	short ArmorDamage = static_cast<short>(std::max(a_DamageBlocked / 4, 1));

	for (int i = 0; i < 4; i++)
	{
		UseItem(cInventory::invArmorOffset + i, ArmorDamage);
	}
}





bool cPlayer::DoTakeDamage(TakeDamageInfo & a_TDI)
{
	if ((a_TDI.DamageType != dtInVoid) && (a_TDI.DamageType != dtPlugin))
	{
		if (IsGameModeCreative() || IsGameModeSpectator())
		{
			// No damage / health in creative or spectator mode if not void or plugin damage
			return false;
		}
	}

	if ((a_TDI.Attacker != nullptr) && (a_TDI.Attacker->IsPlayer()))
	{
		cPlayer * Attacker = static_cast<cPlayer *>(a_TDI.Attacker);

		if ((m_Team != nullptr) && (m_Team == Attacker->m_Team))
		{
			if (!m_Team->AllowsFriendlyFire())
			{
				// Friendly fire is disabled
				return false;
			}
		}
	}

	if (Super::DoTakeDamage(a_TDI))
	{
		// Any kind of damage adds food exhaustion
		AddFoodExhaustion(0.3f);
		SendHealth();

		// Tell the wolves
		if (a_TDI.Attacker != nullptr)
		{
			if (a_TDI.Attacker->IsPawn())
			{
				NotifyNearbyWolves(static_cast<cPawn*>(a_TDI.Attacker), true);
			}
		}
		m_Stats.AddValue(Statistic::DamageTaken, FloorC<cStatManager::StatValue>(a_TDI.FinalDamage * 10 + 0.5));
		return true;
	}
	return false;
}





void cPlayer::NotifyNearbyWolves(cPawn * a_Opponent, bool a_IsPlayerInvolved)
{
	ASSERT(a_Opponent != nullptr);

	m_World->ForEachEntityInBox(cBoundingBox(GetPosition(), 16), [&] (cEntity & a_Entity)
		{
			if (a_Entity.IsMob())
			{
				auto & Mob = static_cast<cMonster&>(a_Entity);
				if (Mob.GetMobType() == mtWolf)
				{
					auto & Wolf = static_cast<cWolf&>(Mob);
					Wolf.ReceiveNearbyFightInfo(GetUUID(), a_Opponent, a_IsPlayerInvolved);
				}
			}
			return false;
		}
	);
}





void cPlayer::KilledBy(TakeDamageInfo & a_TDI)
{
	Super::KilledBy(a_TDI);

	if (m_Health > 0)
	{
		return;  //  not dead yet =]
	}

	m_bVisible = false;  // So new clients don't see the player

	// Detach player from object / entity. If the player dies, the server still says
	// that the player is attached to the entity / object
	Detach();

	// Puke out all the items
	cItems Pickups;
	m_Inventory.CopyToItems(Pickups);
	m_Inventory.Clear();

	if (GetName() == "Notch")
	{
		Pickups.Add(cItem(E_ITEM_RED_APPLE));
	}
	m_Stats.AddValue(Statistic::Drop, static_cast<cStatManager::StatValue>(Pickups.Size()));

	m_World->SpawnItemPickups(Pickups, GetPosX(), GetPosY(), GetPosZ(), 10);
	SaveToDisk();  // Save it, yeah the world is a tough place !
	cPluginManager * PluginManager = cRoot::Get()->GetPluginManager();

	if ((a_TDI.Attacker == nullptr) && m_World->ShouldBroadcastDeathMessages())
	{
		const AString DamageText = [&]
			{
				switch (a_TDI.DamageType)
				{
					case dtRangedAttack:    return "was shot";
					case dtLightning:       return "was plasmified by lightining";
					case dtFalling:         return GetRandomProvider().RandBool() ? "fell to death" : "hit the ground too hard";
					case dtDrowning:        return "drowned";
					case dtSuffocating:     return GetRandomProvider().RandBool() ? "git merge'd into a block" : "fused with a block";
					case dtStarving:        return "forgot the importance of food";
					case dtCactusContact:   return "was impaled on a cactus";
					case dtLavaContact:     return "was melted by lava";
					case dtPoisoning:       return "died from septicaemia";
					case dtWithering:       return "is a husk of their former selves";
					case dtOnFire:          return "forgot to stop, drop, and roll";
					case dtFireContact:     return "burnt themselves to death";
					case dtInVoid:          return "somehow fell out of the world";
					case dtPotionOfHarming: return "was magicked to death";
					case dtEnderPearl:      return "misused an ender pearl";
					case dtAdmin:           return "was administrator'd";
					case dtExplosion:       return "blew up";
					case dtAttack:          return "was attacked by thin air";
					case dtEnvironment:     return "played too much dress up";  // This is not vanilla - added a own pun
				}
				UNREACHABLE("Unsupported damage type");
			}();
		AString DeathMessage = Printf("%s %s", GetName().c_str(), DamageText.c_str());
		PluginManager->CallHookKilled(*this, a_TDI, DeathMessage);
		if (DeathMessage != AString(""))
		{
			GetWorld()->BroadcastChatDeath(DeathMessage);
		}
	}
	else if (a_TDI.Attacker == nullptr)  // && !m_World->ShouldBroadcastDeathMessages() by fallthrough
	{
		// no-op
	}
	else if (a_TDI.Attacker->IsPlayer())
	{
		cPlayer * Killer = static_cast<cPlayer *>(a_TDI.Attacker);
		AString DeathMessage = Printf("%s was killed by %s", GetName().c_str(), Killer->GetName().c_str());
		PluginManager->CallHookKilled(*this, a_TDI, DeathMessage);
		if (DeathMessage != AString(""))
		{
			GetWorld()->BroadcastChatDeath(DeathMessage);
		}
	}
	else
	{
		AString KillerClass = a_TDI.Attacker->GetClass();
		KillerClass.erase(KillerClass.begin());  // Erase the 'c' of the class (e.g. "cWitch" -> "Witch")
		AString DeathMessage = Printf("%s was killed by a %s", GetName().c_str(), KillerClass.c_str());
		PluginManager->CallHookKilled(*this, a_TDI, DeathMessage);
		if (DeathMessage != AString(""))
		{
			GetWorld()->BroadcastChatDeath(DeathMessage);
		}
	}

	m_Stats.AddValue(Statistic::Deaths);
	m_Stats.SetValue(Statistic::TimeSinceDeath, 0);

	m_World->GetScoreBoard().AddPlayerScore(GetName(), cObjective::otDeathCount, 1);
}





void cPlayer::Killed(cEntity * a_Victim)
{
	cScoreboard & ScoreBoard = m_World->GetScoreBoard();

	if (a_Victim->IsPlayer())
	{
		m_Stats.AddValue(Statistic::PlayerKills);

		ScoreBoard.AddPlayerScore(GetName(), cObjective::otPlayerKillCount, 1);
	}
	else if (a_Victim->IsMob())
	{
		if (static_cast<cMonster *>(a_Victim)->GetMobFamily() == cMonster::mfHostile)
		{
			AwardAchievement(Statistic::AchKillEnemy);
		}

		m_Stats.AddValue(Statistic::MobKills);
	}

	ScoreBoard.AddPlayerScore(GetName(), cObjective::otTotalKillCount, 1);
}





void cPlayer::Respawn(void)
{
	ASSERT(m_World != nullptr);

	m_Health = GetMaxHealth();
	SetInvulnerableTicks(20);

	// Reset food level:
	m_FoodLevel = MAX_FOOD_LEVEL;
	m_FoodSaturationLevel = 5.0;
	m_FoodExhaustionLevel = 0.0;

	// Reset Experience
	m_CurrentXp = 0;
	m_LifetimeTotalXp = 0;
	// ToDo: send score to client? How?

	m_ClientHandle->SendRespawn(m_SpawnWorld->GetDimension(), true);

	// Extinguish the fire:
	StopBurning();

	if (GetWorld() != m_SpawnWorld)
	{
		MoveToWorld(*m_SpawnWorld, GetLastBedPos(), false, false);
	}
	else
	{
		TeleportToCoords(GetLastBedPos().x, GetLastBedPos().y, GetLastBedPos().z);
	}

	SetVisible(true);
}





double cPlayer::GetEyeHeight(void) const
{
	return m_Stance;
}





Vector3d cPlayer::GetEyePosition(void) const
{
	return Vector3d( GetPosX(), m_Stance, GetPosZ());
}





bool cPlayer::IsGameModeCreative(void) const
{
	return (GetEffectiveGameMode() == gmCreative);
}





bool cPlayer::IsGameModeSurvival(void) const
{
	return (GetEffectiveGameMode() == gmSurvival);
}





bool cPlayer::IsGameModeAdventure(void) const
{
	return (GetEffectiveGameMode() == gmAdventure);
}





bool cPlayer::IsGameModeSpectator(void) const
{
	return (GetEffectiveGameMode() == gmSpectator);
}





bool cPlayer::CanMobsTarget(void) const
{
	return (IsGameModeSurvival() || IsGameModeAdventure()) && (m_Health > 0);
}





void cPlayer::SetTeam(cTeam * a_Team)
{
	if (m_Team == a_Team)
	{
		return;
	}

	if (m_Team)
	{
		m_Team->RemovePlayer(GetName());
	}

	m_Team = a_Team;

	if (m_Team)
	{
		m_Team->AddPlayer(GetName());
	}
}





cTeam * cPlayer::UpdateTeam(void)
{
	if (m_World == nullptr)
	{
		SetTeam(nullptr);
	}
	else
	{
		cScoreboard & Scoreboard = m_World->GetScoreBoard();

		SetTeam(Scoreboard.QueryPlayerTeam(GetName()));
	}

	return m_Team;
}





void cPlayer::OpenWindow(cWindow & a_Window)
{
	if (cRoot::Get()->GetPluginManager()->CallHookPlayerOpeningWindow(*this, a_Window))
	{
		return;
	}

	if (&a_Window != m_CurrentWindow)
	{
		CloseWindow(false);
	}

	a_Window.OpenedByPlayer(*this);
	m_CurrentWindow = &a_Window;
	a_Window.SendWholeWindow(*GetClientHandle());
}





void cPlayer::CloseWindow(bool a_CanRefuse)
{
	if (m_CurrentWindow == nullptr)
	{
		m_CurrentWindow = m_InventoryWindow;
		return;
	}

	if (m_CurrentWindow->ClosedByPlayer(*this, a_CanRefuse) || !a_CanRefuse)
	{
		// Close accepted, go back to inventory window (the default):
		m_CurrentWindow = m_InventoryWindow;
	}
	else
	{
		// Re-open the window
		m_CurrentWindow->OpenedByPlayer(*this);
		m_CurrentWindow->SendWholeWindow(*GetClientHandle());
	}
}





void cPlayer::CloseWindowIfID(char a_WindowID, bool a_CanRefuse)
{
	if ((m_CurrentWindow == nullptr) || (m_CurrentWindow->GetWindowID() != a_WindowID))
	{
		return;
	}
	CloseWindow();
}





void cPlayer::SendMessage(const AString & a_Message)
{
	m_ClientHandle->SendChat(a_Message, mtCustom);
}





void cPlayer::SendMessageInfo(const AString & a_Message)
{
	m_ClientHandle->SendChat(a_Message, mtInformation);
}





void cPlayer::SendMessageFailure(const AString & a_Message)
{
	m_ClientHandle->SendChat(a_Message, mtFailure);
}





void cPlayer::SendMessageSuccess(const AString & a_Message)
{
	m_ClientHandle->SendChat(a_Message, mtSuccess);
}





void cPlayer::SendMessageWarning(const AString & a_Message)
{
	m_ClientHandle->SendChat(a_Message, mtWarning);
}





void cPlayer::SendMessageFatal(const AString & a_Message)
{
	m_ClientHandle->SendChat(a_Message, mtFailure);
}





void cPlayer::SendMessagePrivateMsg(const AString & a_Message, const AString & a_Sender)
{
	m_ClientHandle->SendChat(a_Message, mtPrivateMessage, a_Sender);
}





void cPlayer::SendMessage(const cCompositeChat & a_Message)
{
	m_ClientHandle->SendChat(a_Message);
}





void cPlayer::SendMessageRaw(const AString & a_MessageRaw, eChatType a_Type)
{
	m_ClientHandle->SendChatRaw(a_MessageRaw, a_Type);
}





void cPlayer::SendSystemMessage(const AString & a_Message)
{
	m_ClientHandle->SendChatSystem(a_Message, mtCustom);
}





void cPlayer::SendAboveActionBarMessage(const AString & a_Message)
{
	m_ClientHandle->SendChatAboveActionBar(a_Message, mtCustom);
}





void cPlayer::SendSystemMessage(const cCompositeChat & a_Message)
{
	m_ClientHandle->SendChatSystem(a_Message);
}





void cPlayer::SendAboveActionBarMessage(const cCompositeChat & a_Message)
{
	m_ClientHandle->SendChatAboveActionBar(a_Message);
}





void cPlayer::SetGameMode(eGameMode a_GameMode)
{
	if ((a_GameMode < gmMin) || (a_GameMode >= gmMax))
	{
		LOGWARNING("%s: Setting invalid gamemode: %d", GetName().c_str(), a_GameMode);
		return;
	}

	if (m_GameMode == a_GameMode)
	{
		// Gamemode already set
		return;
	}

	// Detach, if the player is switching from or to the spectator mode
	if ((m_GameMode == gmSpectator) || (a_GameMode == gmSpectator))
	{
		Detach();
	}

	m_GameMode = a_GameMode;
	m_ClientHandle->SendGameMode(a_GameMode);

	SetCapabilities();

	m_World->BroadcastPlayerListUpdateGameMode(*this);
}





void cPlayer::SetCapabilities()
{
	// Fly ability
	if (IsGameModeCreative() || IsGameModeSpectator())
	{
		SetCanFly(true);
	}
	else
	{
		SetFlying(false);
		SetCanFly(false);
	}

	// Visible
	if (IsGameModeSpectator())
	{
		SetVisible(false);
	}
	else
	{
		SetVisible(true);
	}

	// Set for spectator
	if (IsGameModeSpectator())
	{
		// Clear the current dragging item of the player
		if (GetWindow() != nullptr)
		{
			m_DraggingItem.Empty();
			GetClientHandle()->SendInventorySlot(-1, -1, m_DraggingItem);
		}
	}
}





void cPlayer::SetIP(const AString & a_IP)
{
	m_IP = a_IP;
}





void cPlayer::AwardAchievement(const Statistic a_Ach)
{
	// Check if the prerequisites are met:
	if (!m_Stats.SatisfiesPrerequisite(a_Ach))
	{
		return;
	}

	// Increment the statistic and check if we already have it:
	if (m_Stats.AddValue(a_Ach) != 1)
	{
		return;
	}

	if (m_World->ShouldBroadcastAchievementMessages())
	{
		cCompositeChat Msg;
		Msg.SetMessageType(mtSuccess);
		// TODO: cCompositeChat should not use protocol-specific strings
		// Msg.AddShowAchievementPart(GetName(), nameNew);
		Msg.AddTextPart("Achivement get!");
		m_World->BroadcastChat(Msg);
	}

	// Achievement Get!
	m_ClientHandle->SendStatistics(m_Stats);
}





void cPlayer::TeleportToCoords(double a_PosX, double a_PosY, double a_PosZ)
{
	//  ask plugins to allow teleport to the new position.
	if (!cRoot::Get()->GetPluginManager()->CallHookEntityTeleport(*this, m_LastPosition, Vector3d(a_PosX, a_PosY, a_PosZ)))
	{
		SetPosition({a_PosX, a_PosY, a_PosZ});
		FreezeInternal(GetPosition(), false);
		m_bIsTeleporting = true;

		m_ClientHandle->SendPlayerMoveLook();
	}
}





void cPlayer::Freeze(const Vector3d & a_Location)
{
	FreezeInternal(a_Location, true);
}





bool cPlayer::IsFrozen()
{
	return m_IsFrozen;
}





void cPlayer::Unfreeze()
{
	GetClientHandle()->SendPlayerAbilities();
	GetClientHandle()->SendPlayerMaxSpeed();

	m_IsFrozen = false;
	BroadcastMovementUpdate(GetClientHandle());
	GetClientHandle()->SendPlayerPosition();
}





void cPlayer::SendRotation(double a_YawDegrees, double a_PitchDegrees)
{
	SetYaw(a_YawDegrees);
	SetPitch(a_PitchDegrees);
	m_ClientHandle->SendPlayerMoveLook();
}





void cPlayer::SpectateEntity(cEntity * a_Target)
{
	if ((a_Target == nullptr) || (static_cast<cEntity *>(this) == a_Target))
	{
		GetClientHandle()->SendCameraSetTo(*this);
		m_AttachedTo = nullptr;
		return;
	}

	m_AttachedTo = a_Target;
	GetClientHandle()->SendCameraSetTo(*m_AttachedTo);
}





Vector3d cPlayer::GetThrowStartPos(void) const
{
	Vector3d res = GetEyePosition();

	// Adjust the position to be just outside the player's bounding box:
	res.x += 0.16 * cos(GetPitch());
	res.y += -0.1;
	res.z += 0.16 * sin(GetPitch());

	return res;
}





Vector3d cPlayer::GetThrowSpeed(double a_SpeedCoeff) const
{
	Vector3d res = GetLookVector();
	res.Normalize();

	// TODO: Add a slight random change (+-0.0075 in each direction)

	return res * a_SpeedCoeff;
}





void cPlayer::ForceSetSpeed(const Vector3d & a_Speed)
{
	SetSpeed(a_Speed);
}





void cPlayer::DoSetSpeed(double a_SpeedX, double a_SpeedY, double a_SpeedZ)
{
	if (m_IsFrozen)
	{
		// Do not set speed to a frozen client
		return;
	}
	Super::DoSetSpeed(a_SpeedX, a_SpeedY, a_SpeedZ);
	// Send the speed to the client so he actualy moves
	m_ClientHandle->SendEntityVelocity(*this);
}





void cPlayer::SetVisible(bool a_bVisible)
{
	// Need to Check if the player or other players are in gamemode spectator, but will break compatibility
	if (a_bVisible && !m_bVisible)  // Make visible
	{
		m_bVisible = true;
		m_World->BroadcastSpawnEntity(*this);
	}
	if (!a_bVisible && m_bVisible)
	{
		m_bVisible = false;
		m_World->BroadcastDestroyEntity(*this, m_ClientHandle.get());  // Destroy on all clients
	}
}





MTRand cPlayer::GetEnchantmentRandomProvider()
{
	return m_EnchantmentSeed;
}





void cPlayer::PermuteEnchantmentSeed()
{
	// Get a new random integer and save that as the seed:
	m_EnchantmentSeed = GetRandomProvider().RandInt<unsigned int>();
}





bool cPlayer::HasPermission(const AString & a_Permission)
{
	if (a_Permission.empty())
	{
		// Empty permission request is always granted
		return true;
	}

	AStringVector Split = StringSplit(a_Permission, ".");

	// Iterate over all restrictions; if any matches, then return failure:
	for (auto & Restriction: m_SplitRestrictions)
	{
		if (PermissionMatches(Split, Restriction))
		{
			return false;
		}
	}  // for Restriction - m_SplitRestrictions[]

	// Iterate over all granted permissions; if any matches, then return success:
	for (auto & Permission: m_SplitPermissions)
	{
		if (PermissionMatches(Split, Permission))
		{
			return true;
		}
	}  // for Permission - m_SplitPermissions[]

	// No granted permission matches
	return false;
}





bool cPlayer::PermissionMatches(const AStringVector & a_Permission, const AStringVector & a_Template)
{
	// Check the sub-items if they are the same or there's a wildcard:
	size_t lenP = a_Permission.size();
	size_t lenT = a_Template.size();
	size_t minLen = std::min(lenP, lenT);
	for (size_t i = 0; i < minLen; i++)
	{
		if (a_Template[i] == "*")
		{
			// Has matched so far and now there's a wildcard in the template, so the permission matches:
			return true;
		}
		if (a_Permission[i] != a_Template[i])
		{
			// Found a mismatch
			return false;
		}
	}

	// So far all the sub-items have matched
	// If the sub-item count is the same, then the permission matches
	return (lenP == lenT);
}





AString cPlayer::GetColor(void) const
{
	if (m_MsgNameColorCode.empty() || (m_MsgNameColorCode == "-"))
	{
		// Color has not been assigned, return an empty string:
		return AString();
	}

	// Return the color, including the delimiter:
	return cChatColor::Delimiter + m_MsgNameColorCode;
}





AString cPlayer::GetPrefix(void) const
{
	return m_MsgPrefix;
}





AString cPlayer::GetSuffix(void) const
{
	return m_MsgSuffix;
}





AString cPlayer::GetPlayerListName(void) const
{
	const AString & Color = GetColor();

	if (HasCustomName())
	{
		return m_CustomName;
	}
	else if ((GetName().length() <= 14) && !Color.empty())
	{
		return Printf("%s%s", Color.c_str(), GetName().c_str());
	}
	else
	{
		return GetName();
	}
}





void cPlayer::SetDraggingItem(const cItem & a_Item)
{
	if (GetWindow() != nullptr)
	{
		m_DraggingItem = a_Item;
		GetClientHandle()->SendInventorySlot(-1, -1, m_DraggingItem);
	}
}





void cPlayer::TossEquippedItem(char a_Amount)
{
	cItems Drops;
	cItem DroppedItem(GetInventory().GetEquippedItem());
	if (!DroppedItem.IsEmpty())
	{
		char NewAmount = a_Amount;
		if (NewAmount > GetInventory().GetEquippedItem().m_ItemCount)
		{
			NewAmount = GetInventory().GetEquippedItem().m_ItemCount;  // Drop only what's there
		}

		GetInventory().GetHotbarGrid().ChangeSlotCount(GetInventory().GetEquippedSlotNum() /* Returns hotbar subslot, which HotbarGrid takes */, -a_Amount);

		DroppedItem.m_ItemCount = NewAmount;
		Drops.push_back(DroppedItem);
	}

	TossItems(Drops);
}





void cPlayer::ReplaceOneEquippedItemTossRest(const cItem & a_Item)
{
	auto PlacedCount = GetInventory().ReplaceOneEquippedItem(a_Item);
	char ItemCountToToss = a_Item.m_ItemCount - static_cast<char>(PlacedCount);

	if (ItemCountToToss == 0)
	{
		return;
	}

	cItem Pickup = a_Item;
	Pickup.m_ItemCount = ItemCountToToss;
	TossPickup(Pickup);
}





void cPlayer::TossHeldItem(char a_Amount)
{
	cItems Drops;
	cItem & Item = GetDraggingItem();
	if (!Item.IsEmpty())
	{
		char OriginalItemAmount = Item.m_ItemCount;
		Item.m_ItemCount = std::min(OriginalItemAmount, a_Amount);
		Drops.push_back(Item);

		if (OriginalItemAmount > a_Amount)
		{
			Item.m_ItemCount = OriginalItemAmount - a_Amount;
		}
		else
		{
			Item.Empty();
		}
	}

	TossItems(Drops);
}





void cPlayer::TossPickup(const cItem & a_Item)
{
	cItems Drops;
	Drops.push_back(a_Item);

	TossItems(Drops);
}





void cPlayer::DoMoveToWorld(const cEntity::sWorldChangeInfo & a_WorldChangeInfo)
{
	ASSERT(a_WorldChangeInfo.m_NewWorld != nullptr);

	// Reset portal cooldown
	if (a_WorldChangeInfo.m_SetPortalCooldown)
	{
		m_PortalCooldownData.m_TicksDelayed = 0;
		m_PortalCooldownData.m_ShouldPreventTeleportation = true;
	}

	if (m_World == a_WorldChangeInfo.m_NewWorld)
	{
		// Moving to same world, don't need to remove from world
		SetPosition(a_WorldChangeInfo.m_NewPosition);
		return;
	}

	LOGD("Warping player \"%s\" from world \"%s\" to \"%s\". Source chunk: (%d, %d) ",
		GetName(), GetWorld()->GetName(), a_WorldChangeInfo.m_NewWorld->GetName(),
		GetChunkX(), GetChunkZ()
	);

	// Stop all mobs from targeting this player
	StopEveryoneFromTargetingMe();

	// If player is attached to entity, detach, to prevent client side effects
	Detach();

	// Prevent further ticking in this world
	SetIsTicking(false);

	// Remove from the old world
	auto & OldWorld = *GetWorld();
	auto Self = OldWorld.RemovePlayer(*this);

	ResetPosition(a_WorldChangeInfo.m_NewPosition);
	FreezeInternal(a_WorldChangeInfo.m_NewPosition, false);
	SetWorld(a_WorldChangeInfo.m_NewWorld);  // Chunks may be streamed before cWorld::AddPlayer() sets the world to the new value

	// Set capabilities based on new world
	SetCapabilities();

	cClientHandle * ch = GetClientHandle();
	if (ch != nullptr)
	{
		// The clienthandle caches the coords of the chunk we're standing at. Invalidate this.
		ch->InvalidateCachedSentChunk();

		// Send the respawn packet:
		if (a_WorldChangeInfo.m_SendRespawn)
		{
			ch->SendRespawn(a_WorldChangeInfo.m_NewWorld->GetDimension());
		}

		// Update the view distance.
		ch->SetViewDistance(ch->GetRequestedViewDistance());

		// Send current weather of target world to player
		if (a_WorldChangeInfo.m_NewWorld->GetDimension() == dimOverworld)
		{
			ch->SendWeather(a_WorldChangeInfo.m_NewWorld->GetWeather());
		}
	}

	// New world will take over and announce client at its next tick
	a_WorldChangeInfo.m_NewWorld->AddPlayer(std::move(Self), &OldWorld);
}





bool cPlayer::LoadFromDisk(cWorldPtr & a_World)
{
	LoadRank();

	// Load from the UUID file:
	if (LoadFromFile(GetUUIDFileName(m_UUID), a_World))
	{
		return true;
	}

	// Check for old offline UUID filename, if it exists migrate to new filename
	cUUID OfflineUUID = cClientHandle::GenerateOfflineUUID(GetName());
	auto OldFilename = GetUUIDFileName(GetOldStyleOfflineUUID(GetName()));
	auto NewFilename = GetUUIDFileName(m_UUID);
	// Only move if there isn't already a new file
	if (!cFile::IsFile(NewFilename) && cFile::IsFile(OldFilename))
	{
		cFile::CreateFolderRecursive(GetUUIDFolderName(m_UUID));  // Ensure folder exists to move to
		if (
			cFile::Rename(OldFilename, NewFilename) &&
			(m_UUID == OfflineUUID) &&
			LoadFromFile(NewFilename, a_World)
		)
		{
			return true;
		}
	}

	// Load from the offline UUID file, if allowed:
	const char * OfflineUsage = " (unused)";
	if (cRoot::Get()->GetServer()->ShouldLoadOfflinePlayerData())
	{
		OfflineUsage = "";
		if (LoadFromFile(GetUUIDFileName(OfflineUUID), a_World))
		{
			return true;
		}
	}

	// Load from the old-style name-based file, if allowed:
	if (cRoot::Get()->GetServer()->ShouldLoadNamedPlayerData())
	{
		AString OldStyleFileName = Printf("players/%s.json", GetName().c_str());
		if (LoadFromFile(OldStyleFileName, a_World))
		{
			// Save in new format and remove the old file
			if (SaveToDisk())
			{
				cFile::Delete(OldStyleFileName);
			}
			return true;
		}
	}

	// None of the files loaded successfully
	LOG("Player data file not found for %s (%s, offline %s%s), will be reset to defaults.",
		GetName().c_str(), m_UUID.ToShortString().c_str(), OfflineUUID.ToShortString().c_str(), OfflineUsage
	);

	if (a_World == nullptr)
	{
		a_World = cRoot::Get()->GetDefaultWorld();
	}
	return false;
}





bool cPlayer::LoadFromFile(const AString & a_FileName, cWorldPtr & a_World)
{
	// Load the data from the file:
	cFile f;
	if (!f.Open(a_FileName, cFile::fmRead))
	{
		// This is a new player whom we haven't seen yet, bail out, let them have the defaults
		return false;
	}
	AString buffer;
	if (f.ReadRestOfFile(buffer) != f.GetSize())
	{
		LOGWARNING("Cannot read player data from file \"%s\"", a_FileName.c_str());
		return false;
	}
	f.Close();

	// Parse the JSON format:
	Json::Value root;
	AString ParseError;
	if (!JsonUtils::ParseString(buffer, root, &ParseError))
	{
		FLOGWARNING(
			"Cannot parse player data in file \"{0}\":\n  {1}",
			a_FileName, ParseError
		);
		return false;
	}

	// Load the player data:
	Json::Value & JSON_PlayerPosition = root["position"];
	if (JSON_PlayerPosition.size() == 3)
	{
		SetPosX(JSON_PlayerPosition[0].asDouble());
		SetPosY(JSON_PlayerPosition[1].asDouble());
		SetPosZ(JSON_PlayerPosition[2].asDouble());
		m_LastPosition = GetPosition();
	}

	Json::Value & JSON_PlayerRotation = root["rotation"];
	if (JSON_PlayerRotation.size() == 3)
	{
		SetYaw      (static_cast<float>(JSON_PlayerRotation[0].asDouble()));
		SetPitch    (static_cast<float>(JSON_PlayerRotation[1].asDouble()));
		SetRoll     (static_cast<float>(JSON_PlayerRotation[2].asDouble()));
	}

	m_Health              = root.get("health",         0).asFloat();
	m_AirLevel            = root.get("air",            MAX_AIR_LEVEL).asInt();
	m_FoodLevel           = root.get("food",           MAX_FOOD_LEVEL).asInt();
	m_FoodSaturationLevel = root.get("foodSaturation", MAX_FOOD_LEVEL).asDouble();
	m_FoodTickTimer       = root.get("foodTickTimer",  0).asInt();
	m_FoodExhaustionLevel = root.get("foodExhaustion", 0).asDouble();
	m_LifetimeTotalXp     = root.get("xpTotal",        0).asInt();
	m_CurrentXp           = root.get("xpCurrent",      0).asInt();
	m_IsFlying            = root.get("isflying",       0).asBool();
	m_EnchantmentSeed     = root.get("enchantmentSeed", GetRandomProvider().RandInt<unsigned int>()).asUInt();

	Json::Value & JSON_KnownItems = root["knownItems"];
	for (UInt32 i = 0; i < JSON_KnownItems.size(); i++)
	{
		cItem Item;
		Item.FromJson(JSON_KnownItems[i]);
		m_KnownItems.insert(Item);
	}

	const auto & RecipeNameMap = cRoot::Get()->GetCraftingRecipes()->GetRecipeNameMap();

	Json::Value & JSON_KnownRecipes = root["knownRecipes"];
	for (UInt32 i = 0; i < JSON_KnownRecipes.size(); i++)
	{
		auto RecipeId = RecipeNameMap.find(JSON_KnownRecipes[i].asString());
		if (RecipeId != RecipeNameMap.end())
		{
			m_KnownRecipes.insert(RecipeId->second);
		}
	}

	m_GameMode = static_cast<eGameMode>(root.get("gamemode", eGameMode_NotSet).asInt());

	if (m_GameMode == eGameMode_Creative)
	{
		m_CanFly = true;
	}

	m_Inventory.LoadFromJson(root["inventory"]);

	int equippedSlotNum = root.get("equippedItemSlot", 0).asInt();
	m_Inventory.SetEquippedSlotNum(equippedSlotNum);

	cEnderChestEntity::LoadFromJson(root["enderchestinventory"], m_EnderChestContents);

	m_LoadedWorldName = root.get("world", "world").asString();
	a_World = cRoot::Get()->GetWorld(GetLoadedWorldName());
	if (a_World == nullptr)
	{
		a_World = cRoot::Get()->GetDefaultWorld();
	}


	m_LastBedPos.x = root.get("SpawnX", a_World->GetSpawnX()).asInt();
	m_LastBedPos.y = root.get("SpawnY", a_World->GetSpawnY()).asInt();
	m_LastBedPos.z = root.get("SpawnZ", a_World->GetSpawnZ()).asInt();
	AString SpawnWorldName =  root.get("SpawnWorld", cRoot::Get()->GetDefaultWorld()->GetName()).asString();
	m_SpawnWorld = cRoot::Get()->GetWorld(SpawnWorldName);
	if (m_SpawnWorld == nullptr)
	{
		m_SpawnWorld = cRoot::Get()->GetDefaultWorld();
	}

	try
	{
		// Load the player stats.
		// We use the default world name (like bukkit) because stats are shared between dimensions / worlds.
		StatSerializer::Load(m_Stats, cRoot::Get()->GetDefaultWorld()->GetDataPath(), GetUUID().ToLongString());
	}
	catch (...)
	{
		LOGWARNING("Failed loading player statistics");
	}

	FLOGD("Player {0} was read from file \"{1}\", spawning at {2:.2f} in world \"{3}\"",
		GetName(), a_FileName, GetPosition(), a_World->GetName()
	);

	return true;
}





void cPlayer::OpenHorseInventory()
{
	if (
		(m_AttachedTo == nullptr) ||
		!m_AttachedTo->IsMob()
	)
	{
		return;
	}

	auto & Mob = static_cast<cMonster &>(*m_AttachedTo);

	if (Mob.GetMobType() != mtHorse)
	{
		return;
	}

	auto & Horse = static_cast<cHorse &>(Mob);
	// The client sends requests for untame horses as well but shouldn't actually open
	if (Horse.IsTame())
	{
		Horse.PlayerOpenWindow(*this);
	}
}





bool cPlayer::SaveToDisk()
{
	cFile::CreateFolderRecursive(GetUUIDFolderName(m_UUID));

	// create the JSON data
	Json::Value JSON_PlayerPosition;
	JSON_PlayerPosition.append(Json::Value(GetPosX()));
	JSON_PlayerPosition.append(Json::Value(GetPosY()));
	JSON_PlayerPosition.append(Json::Value(GetPosZ()));

	Json::Value JSON_PlayerRotation;
	JSON_PlayerRotation.append(Json::Value(GetYaw()));
	JSON_PlayerRotation.append(Json::Value(GetPitch()));
	JSON_PlayerRotation.append(Json::Value(GetRoll()));

	Json::Value JSON_Inventory;
	m_Inventory.SaveToJson(JSON_Inventory);

	Json::Value JSON_EnderChestInventory;
	cEnderChestEntity::SaveToJson(JSON_EnderChestInventory, m_EnderChestContents);

	Json::Value JSON_KnownItems;
	for (const auto & KnownItem : m_KnownItems)
	{
		Json::Value JSON_Item;
		KnownItem.GetJson(JSON_Item);
		JSON_KnownItems.append(JSON_Item);
	}

	Json::Value JSON_KnownRecipes;
	for (auto KnownRecipe : m_KnownRecipes)
	{
		auto Recipe = cRoot::Get()->GetCraftingRecipes()->GetRecipeById(KnownRecipe);
		JSON_KnownRecipes.append(Recipe->m_RecipeName);
	}

	Json::Value root;
	root["position"]            = JSON_PlayerPosition;
	root["rotation"]            = JSON_PlayerRotation;
	root["inventory"]           = JSON_Inventory;
	root["knownItems"]          = JSON_KnownItems;
	root["knownRecipes"]        = JSON_KnownRecipes;
	root["equippedItemSlot"]    = m_Inventory.GetEquippedSlotNum();
	root["enderchestinventory"] = JSON_EnderChestInventory;
	root["health"]              = m_Health;
	root["xpTotal"]             = m_LifetimeTotalXp;
	root["xpCurrent"]           = m_CurrentXp;
	root["air"]                 = m_AirLevel;
	root["food"]                = m_FoodLevel;
	root["foodSaturation"]      = m_FoodSaturationLevel;
	root["foodTickTimer"]       = m_FoodTickTimer;
	root["foodExhaustion"]      = m_FoodExhaustionLevel;
	root["isflying"]            = IsFlying();
	root["lastknownname"]       = GetName();
	root["SpawnX"]              = GetLastBedPos().x;
	root["SpawnY"]              = GetLastBedPos().y;
	root["SpawnZ"]              = GetLastBedPos().z;
	root["SpawnWorld"]          = m_SpawnWorld->GetName();
	root["enchantmentSeed"]     = m_EnchantmentSeed;

	if (m_World != nullptr)
	{
		root["world"] = m_World->GetName();
		if (m_GameMode == m_World->GetGameMode())
		{
			root["gamemode"] = static_cast<int>(eGameMode_NotSet);
		}
		else
		{
			root["gamemode"] = static_cast<int>(m_GameMode);
		}
	}
	else
	{
		// This happens if the player is saved to new format after loading from the old format
		root["world"]    = m_LoadedWorldName;
		root["gamemode"] = static_cast<int>(eGameMode_NotSet);
	}

	auto JsonData = JsonUtils::WriteStyledString(root);
	AString SourceFile = GetUUIDFileName(m_UUID);

	cFile f;
	if (!f.Open(SourceFile, cFile::fmWrite))
	{
		LOGWARNING("Error writing player \"%s\" to file \"%s\" - cannot open file. Player will lose their progress.",
			GetName().c_str(), SourceFile.c_str()
		);
		return false;
	}
	if (f.Write(JsonData.c_str(), JsonData.size()) != static_cast<int>(JsonData.size()))
	{
		LOGWARNING("Error writing player \"%s\" to file \"%s\" - cannot save data. Player will lose their progress. ",
			GetName().c_str(), SourceFile.c_str()
		);
		return false;
	}

	try
	{
		// Save the player stats.
		// We use the default world name (like bukkit) because stats are shared between dimensions / worlds.
		StatSerializer::Save(m_Stats, cRoot::Get()->GetDefaultWorld()->GetDataPath(), GetUUID().ToLongString());
	}
	catch (...)
	{
		LOGWARNING("Could not save stats for player %s", GetName().c_str());
		return false;
	}

	return true;
}





void cPlayer::UseEquippedItem(short a_Damage)
{
	// No durability loss in creative or spectator modes:
	if (IsGameModeCreative() || IsGameModeSpectator())
	{
		return;
	}

	UseItem(cInventory::invHotbarOffset + m_Inventory.GetEquippedSlotNum(), a_Damage);
}





void cPlayer::UseEquippedItem(cItemHandler::eDurabilityLostAction a_Action)
{
	// Get item being used:
	cItem Item = GetEquippedItem();

	// Get base damage for action type:
	short Dmg = cItemHandler::GetItemHandler(Item)->GetDurabilityLossByAction(a_Action);

	UseEquippedItem(Dmg);
}





void cPlayer::UseItem(int a_SlotNumber, short a_Damage)
{
	const cItem & Item = m_Inventory.GetSlot(a_SlotNumber);
	if (Item.IsEmpty())
	{
		return;
	}

	// Ref: https://minecraft.gamepedia.com/Enchanting#Unbreaking
	unsigned int UnbreakingLevel = Item.m_Enchantments.GetLevel(cEnchantments::enchUnbreaking);
	double chance = ItemCategory::IsArmor(Item.m_ItemType)
		? (0.6 + (0.4 / (UnbreakingLevel + 1))) : (1.0 / (UnbreakingLevel + 1));

	// When durability is reduced by multiple points
	// Unbreaking is applied for each point of reduction.
	std::binomial_distribution<short> Dist(a_Damage, chance);
	short ReducedDamage = Dist(GetRandomProvider().Engine());
	if (m_Inventory.DamageItem(a_SlotNumber, ReducedDamage))
	{
		m_World->BroadcastSoundEffect("entity.item.break", GetPosition(), 0.5f, static_cast<float>(0.75 + (static_cast<float>((GetUniqueID() * 23) % 32)) / 64));
	}
}





void cPlayer::HandleFood(void)
{
	// Ref.: https://minecraft.gamepedia.com/Hunger

	if (IsGameModeCreative() || IsGameModeSpectator())
	{
		// Hunger is disabled for Creative and Spectator
		return;
	}

	// Apply food exhaustion that has accumulated:
	if (m_FoodExhaustionLevel > 4.0)
	{
		m_FoodExhaustionLevel -= 4.0;

		if (m_FoodSaturationLevel > 0.0)
		{
			m_FoodSaturationLevel = std::max(m_FoodSaturationLevel - 1.0, 0.0);
		}
		else
		{
			SetFoodLevel(m_FoodLevel - 1);
		}
	}

	// Heal or damage, based on the food level, using the m_FoodTickTimer:
	if ((m_FoodLevel >= 18) || (m_FoodLevel <= 0))
	{
		m_FoodTickTimer++;
		if (m_FoodTickTimer >= 80)
		{
			m_FoodTickTimer = 0;

			if ((m_FoodLevel >= 18) && (GetHealth() < GetMaxHealth()))
			{
				// Regenerate health from food, incur 3 pts of food exhaustion:
				Heal(1);
				AddFoodExhaustion(3.0);
			}
			else if ((m_FoodLevel <= 0) && (m_Health > 1))
			{
				// Damage from starving
				TakeDamage(dtStarving, nullptr, 1, 1, 0);
			}
		}
	}
	else
	{
		m_FoodTickTimer = 0;
	}
}





void cPlayer::HandleFloater()
{
	if (GetEquippedItem().m_ItemType == E_ITEM_FISHING_ROD)
	{
		return;
	}
	m_World->DoWithEntityByID(m_FloaterID, [](cEntity & a_Entity)
		{
			a_Entity.Destroy();
			return true;
		}
	);
	SetIsFishing(false);
}





bool cPlayer::IsClimbing(void) const
{
	int PosX = POSX_TOINT;
	int PosY = POSY_TOINT;
	int PosZ = POSZ_TOINT;

	if ((PosY < 0) || (PosY >= cChunkDef::Height))
	{
		return false;
	}

	BLOCKTYPE Block = m_World->GetBlock(PosX, PosY, PosZ);
	switch (Block)
	{
		case E_BLOCK_LADDER:
		case E_BLOCK_VINES:
		{
			return true;
		}
		default: return false;
	}
}





void cPlayer::UpdateMovementStats(const Vector3d & a_DeltaPos, bool a_PreviousIsOnGround)
{
	if (m_bIsTeleporting)
	{
		m_bIsTeleporting = false;
		return;
	}

	const auto Value = FloorC<cStatManager::StatValue>(a_DeltaPos.Length() * 100 + 0.5);
	if (m_AttachedTo == nullptr)
	{
		if (IsFlying())
		{
			m_Stats.AddValue(Statistic::FlyOneCm, Value);
			// May be flying and doing any of the following:
		}

		if (IsClimbing())
		{
			if (a_DeltaPos.y > 0.0)  // Going up
			{
				m_Stats.AddValue(Statistic::ClimbOneCm, FloorC<cStatManager::StatValue>(a_DeltaPos.y * 100 + 0.5));
			}
		}
		else if (IsInWater())
		{
			if (m_IsHeadInWater)
			{
				m_Stats.AddValue(Statistic::WalkUnderWaterOneCm, Value);
			}
			else
			{
				m_Stats.AddValue(Statistic::WalkOnWaterOneCm, Value);
			}
			AddFoodExhaustion(0.00015 * static_cast<double>(Value));
		}
		else if (IsOnGround())
		{
			if (IsCrouched())
			{
				m_Stats.AddValue(Statistic::CrouchOneCm, Value);
				AddFoodExhaustion(0.0001 * static_cast<double>(Value));
			}
			if (IsSprinting())
			{
				m_Stats.AddValue(Statistic::SprintOneCm, Value);
				AddFoodExhaustion(0.001 * static_cast<double>(Value));
			}
			else
			{
				m_Stats.AddValue(Statistic::WalkOneCm, Value);
				AddFoodExhaustion(0.0001 * static_cast<double>(Value));
			}
		}
		else
		{
			// If a jump just started, process food exhaustion:
			if ((a_DeltaPos.y > 0.0) && a_PreviousIsOnGround)
			{
				m_Stats.AddValue(Statistic::Jump, 1);
				AddFoodExhaustion((IsSprinting() ? 0.008 : 0.002) * static_cast<double>(Value));
			}
			else if (a_DeltaPos.y < 0.0)
			{
				// Increment statistic
				m_Stats.AddValue(Statistic::FallOneCm, static_cast<cStatManager::StatValue>(std::abs(a_DeltaPos.y) * 100 + 0.5));
			}
			// TODO: good opportunity to detect illegal flight (check for falling tho)
		}
	}
	else
	{
		switch (m_AttachedTo->GetEntityType())
		{
			case cEntity::etMinecart: m_Stats.AddValue(Statistic::MinecartOneCm, Value); break;
			case cEntity::etBoat:     m_Stats.AddValue(Statistic::BoatOneCm,     Value); break;
			case cEntity::etMonster:
			{
				cMonster * Monster = static_cast<cMonster *>(m_AttachedTo);
				switch (Monster->GetMobType())
				{
					case mtPig:   m_Stats.AddValue(Statistic::PigOneCm,   Value); break;
					case mtHorse: m_Stats.AddValue(Statistic::HorseOneCm, Value); break;
					default: break;
				}
				break;
			}
			default: break;
		}
	}
}





void cPlayer::LoadRank(void)
{
	// Load the values from cRankManager:
	cRankManager * RankMgr = cRoot::Get()->GetRankManager();
	m_Rank = RankMgr->GetPlayerRankName(m_UUID);
	if (m_Rank.empty())
	{
		m_Rank = RankMgr->GetDefaultRank();
	}
	else
	{
		// Update the name:
		RankMgr->UpdatePlayerName(m_UUID, m_PlayerName);
	}
	m_Permissions = RankMgr->GetPlayerPermissions(m_UUID);
	m_Restrictions = RankMgr->GetPlayerRestrictions(m_UUID);
	RankMgr->GetRankVisuals(m_Rank, m_MsgPrefix, m_MsgSuffix, m_MsgNameColorCode);

	// Break up the individual permissions on each dot, into m_SplitPermissions:
	m_SplitPermissions.clear();
	m_SplitPermissions.reserve(m_Permissions.size());
	for (auto & Permission: m_Permissions)
	{
		m_SplitPermissions.push_back(StringSplit(Permission, "."));
	}  // for Permission - m_Permissions[]

	// Break up the individual restrictions on each dot, into m_SplitRestrictions:
	m_SplitRestrictions.clear();
	m_SplitRestrictions.reserve(m_Restrictions.size());
	for (auto & Restriction: m_Restrictions)
	{
		m_SplitRestrictions.push_back(StringSplit(Restriction, "."));
	}  // for itr - m_Restrictions[]
}





bool cPlayer::PlaceBlock(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	sSetBlockVector blk{{a_BlockX, a_BlockY, a_BlockZ, a_BlockType, a_BlockMeta}};
	return PlaceBlocks(blk);
}





void cPlayer::SendBlocksAround(int a_BlockX, int a_BlockY, int a_BlockZ, int a_Range)
{
	// Collect the coords of all the blocks to send:
	sSetBlockVector blks;
	for (int y = a_BlockY - a_Range + 1; y < a_BlockY + a_Range; y++)
	{
		for (int z = a_BlockZ - a_Range + 1; z < a_BlockZ + a_Range; z++)
		{
			for (int x = a_BlockX - a_Range + 1; x < a_BlockX + a_Range; x++)
			{
				blks.emplace_back(x, y, z, E_BLOCK_AIR, 0);  // Use fake blocktype, it will get set later on.
			}
		}
	}  // for y

	// Get the values of all the blocks:
	if (!m_World->GetBlocks(blks, false))
	{
		LOGD("%s: Cannot query all blocks, not sending an update", __FUNCTION__);
		return;
	}

	// Divide the block changes by their respective chunks:
	std::unordered_map<cChunkCoords, sSetBlockVector, cChunkCoordsHash> Changes;
	for (const auto & blk: blks)
	{
		Changes[cChunkCoords(blk.m_ChunkX, blk.m_ChunkZ)].push_back(blk);
	}  // for blk - blks[]
	blks.clear();

	// Send the blocks for each affected chunk:
	for (auto itr = Changes.cbegin(), end = Changes.cend(); itr != end; ++itr)
	{
		m_ClientHandle->SendBlockChanges(itr->first.m_ChunkX, itr->first.m_ChunkZ, itr->second);
	}
}





bool cPlayer::DoesPlacingBlocksIntersectEntity(const sSetBlockVector & a_Blocks)
{
	// Compute the bounding box for each block to be placed
	std::vector<cBoundingBox> PlacementBoxes;
	cBoundingBox PlacingBounds(0, 0, 0, 0, 0, 0);
	bool HasInitializedBounds = false;
	for (auto blk: a_Blocks)
	{
		int x = blk.GetX();
		int y = blk.GetY();
		int z = blk.GetZ();
		cBoundingBox BlockBox = cBlockHandler::For(blk.m_BlockType).GetPlacementCollisionBox(
			m_World->GetBlock(x - 1, y, z),
			m_World->GetBlock(x + 1, y, z),
			(y == 0) ? E_BLOCK_AIR : m_World->GetBlock(x, y - 1, z),
			(y == cChunkDef::Height - 1) ? E_BLOCK_AIR : m_World->GetBlock(x, y + 1, z),
			m_World->GetBlock(x, y, z - 1),
			m_World->GetBlock(x, y, z + 1)
		);
		BlockBox.Move(x, y, z);

		PlacementBoxes.push_back(BlockBox);

		if (HasInitializedBounds)
		{
			PlacingBounds = PlacingBounds.Union(BlockBox);
		}
		else
		{
			PlacingBounds = BlockBox;
			HasInitializedBounds = true;
		}
	}

	cWorld * World = GetWorld();

	// Check to see if any entity intersects any block being placed
	return !World->ForEachEntityInBox(PlacingBounds, [&](cEntity & a_Entity)
		{
			// The distance inside the block the entity can still be.
			const double EPSILON = 0.0005;

			if (!a_Entity.DoesPreventBlockPlacement())
			{
				return false;
			}
			auto EntBox = a_Entity.GetBoundingBox();
			for (auto BlockBox : PlacementBoxes)
			{
				// Put in a little bit of wiggle room
				BlockBox.Expand(-EPSILON, -EPSILON, -EPSILON);
				if (EntBox.DoesIntersect(BlockBox))
				{
					return true;
				}
			}
			return false;
		}
	);
}





bool cPlayer::PlaceBlocks(const sSetBlockVector & a_Blocks)
{
	if (DoesPlacingBlocksIntersectEntity(a_Blocks))
	{
		// Abort - re-send all the current blocks in the a_Blocks' coords to the client:
		for (auto blk2: a_Blocks)
		{
			m_World->SendBlockTo(blk2.GetX(), blk2.GetY(), blk2.GetZ(), *this);
		}
		return false;
	}

	// Call the "placing" hooks; if any fail, abort:
	cPluginManager * pm = cPluginManager::Get();
	for (auto blk: a_Blocks)
	{
		if (pm->CallHookPlayerPlacingBlock(*this, blk))
		{
			// Abort - re-send all the current blocks in the a_Blocks' coords to the client:
			for (auto blk2: a_Blocks)
			{
				m_World->SendBlockTo(blk2.GetX(), blk2.GetY(), blk2.GetZ(), *this);
			}
			return false;
		}
	}  // for blk - a_Blocks[]

	cChunkInterface ChunkInterface(m_World->GetChunkMap());
	for (auto blk: a_Blocks)
	{
		// Set the blocks:
		m_World->PlaceBlock(blk.GetAbsolutePos(), blk.m_BlockType, blk.m_BlockMeta);

		// Notify the blockhandlers:
		cBlockHandler::For(blk.m_BlockType).OnPlacedByPlayer(ChunkInterface, *m_World, *this, blk);

		// Call the "placed" hooks:
		pm->CallHookPlayerPlacedBlock(*this, blk);
	}

	return true;
}





void cPlayer::SetSkinParts(int a_Parts)
{
	m_SkinParts = a_Parts & spMask;
	m_World->BroadcastEntityMetadata(*this, m_ClientHandle.get());
}





void cPlayer::SetMainHand(eMainHand a_Hand)
{
	m_MainHand = a_Hand;
	m_World->BroadcastEntityMetadata(*this, m_ClientHandle.get());
}





void cPlayer::AttachTo(cEntity * a_AttachTo)
{
	// Different attach, if this is a spectator
	if (IsGameModeSpectator())
	{
		SpectateEntity(a_AttachTo);
		return;
	}

	Super::AttachTo(a_AttachTo);
}





void cPlayer::Detach()
{
	if (m_AttachedTo == nullptr)
	{
		// The player is not attached to anything. Bail out.
		return;
	}

	// Different detach, if this is a spectator
	if (IsGameModeSpectator())
	{
		GetClientHandle()->SendCameraSetTo(*this);
		TeleportToEntity(*m_AttachedTo);
		m_AttachedTo = nullptr;
		return;
	}

	Super::Detach();
	int PosX = POSX_TOINT;
	int PosY = POSY_TOINT;
	int PosZ = POSZ_TOINT;

	// Search for a position within an area to teleport player after detachment
	// Position must be solid land with two air blocks above.
	// If nothing found, player remains where they are
	for (int x = PosX - 1; x <= (PosX + 1); ++x)
	{
		for (int y = PosY; y <= (PosY + 3); ++y)
		{
			for (int z = PosZ - 1; z <= (PosZ + 1); ++z)
			{
				if (
					(m_World->GetBlock(x, y, z) == E_BLOCK_AIR) &&
					(m_World->GetBlock(x, y + 1, z) == E_BLOCK_AIR) &&
					cBlockInfo::IsSolid(m_World->GetBlock(x, y - 1, z))
				)
				{
					TeleportToCoords(x + 0.5, y, z + 0.5);
					return;
				}
			}
		}
	}
}





void cPlayer::RemoveClientHandle(void)
{
	ASSERT(m_ClientHandle != nullptr);
	m_ClientHandle.reset();
}





AString cPlayer::GetUUIDFileName(const cUUID & a_UUID)
{
	AString UUID = a_UUID.ToLongString();

	AString res("players/");
	res.append(UUID, 0, 2);
	res.push_back('/');
	res.append(UUID, 2, AString::npos);
	res.append(".json");
	return res;
}





void cPlayer::FreezeInternal(const Vector3d & a_Location, bool a_ManuallyFrozen)
{
	SetSpeed(0, 0, 0);
	SetPosition(a_Location);
	m_IsFrozen = true;
	m_IsManuallyFrozen = a_ManuallyFrozen;

	double NormalMaxSpeed = GetNormalMaxSpeed();
	double SprintMaxSpeed = GetSprintingMaxSpeed();
	double FlyingMaxpeed = GetFlyingMaxSpeed();
	bool IsFlying = m_IsFlying;

	// Set the client-side speed to 0
	m_NormalMaxSpeed = 0;
	m_SprintingMaxSpeed = 0;
	m_FlyingMaxSpeed = 0;
	m_IsFlying = true;

	// Send the client its fake speed and max speed of 0
	GetClientHandle()->SendPlayerMoveLook();
	GetClientHandle()->SendPlayerAbilities();
	GetClientHandle()->SendPlayerMaxSpeed();
	GetClientHandle()->SendEntityVelocity(*this);

	// Keep the server side speed variables as they were in the first place
	m_NormalMaxSpeed = NormalMaxSpeed;
	m_SprintingMaxSpeed = SprintMaxSpeed;
	m_FlyingMaxSpeed = FlyingMaxpeed;
	m_IsFlying = IsFlying;
}





float cPlayer::GetLiquidHeightPercent(NIBBLETYPE a_Meta)
{
	if (a_Meta >= 8)
	{
		a_Meta = 0;
	}
	return static_cast<float>(a_Meta + 1) / 9.0f;
}





bool cPlayer::IsInsideWater()
{
	BLOCKTYPE Block = m_World->GetBlock(FloorC(GetPosX()), FloorC(m_Stance), FloorC(GetPosZ()));
	if ((Block != E_BLOCK_WATER) && (Block != E_BLOCK_STATIONARY_WATER))
	{
		return false;
	}
	NIBBLETYPE Meta = GetWorld()->GetBlockMeta(FloorC(GetPosX()), FloorC(m_Stance), FloorC(GetPosZ()));
	float f = GetLiquidHeightPercent(Meta) - 0.11111111f;
	float f1 = static_cast<float>(m_Stance + 1) - f;
	bool flag = (m_Stance < f1);
	return flag;
}





float cPlayer::GetDigSpeed(BLOCKTYPE a_Block)
{
	// Based on: https://minecraft.gamepedia.com/Breaking#Speed

	// Get the base speed multiplier of the equipped tool for the mined block
	float MiningSpeed = GetEquippedItem().GetHandler()->GetBlockBreakingStrength(a_Block);

	// If we can harvest the block then we can apply material and enchantment bonuses
	if (GetEquippedItem().GetHandler()->CanHarvestBlock(a_Block))
	{
		if (MiningSpeed > 1.0f)  // If the base multiplier for this block is greater than 1, now we can check enchantments
		{
			unsigned int EfficiencyModifier = GetEquippedItem().m_Enchantments.GetLevel(cEnchantments::eEnchantment::enchEfficiency);
			if (EfficiencyModifier > 0)  // If an efficiency enchantment is present, apply formula as on wiki
			{
				MiningSpeed += (EfficiencyModifier * EfficiencyModifier) + 1;
			}
		}
	}
	else  // If we can't harvest the block then no bonuses:
	{
		MiningSpeed = 1;
	}

	// Haste increases speed by 20% per level
	auto Haste = GetEntityEffect(cEntityEffect::effHaste);
	if (Haste != nullptr)
	{
		int intensity = Haste->GetIntensity() + 1;
		MiningSpeed *= 1.0f + (intensity * 0.2f);
	}

	// Mining fatigue decreases speed a lot
	auto MiningFatigue = GetEntityEffect(cEntityEffect::effMiningFatigue);
	if (MiningFatigue != nullptr)
	{
		int intensity = MiningFatigue->GetIntensity();
		switch (intensity)
		{
			case 0:  MiningSpeed *= 0.3f;     break;
			case 1:  MiningSpeed *= 0.09f;    break;
			case 2:  MiningSpeed *= 0.0027f;  break;
			default: MiningSpeed *= 0.00081f; break;

		}
	}

	// 5x speed loss for being in water
	if (IsInsideWater() && !(GetEquippedItem().m_Enchantments.GetLevel(cEnchantments::eEnchantment::enchAquaAffinity) > 0))
	{
		MiningSpeed /= 5.0f;
	}

	// 5x speed loss for not touching ground
	if (!IsOnGround())
	{
		MiningSpeed /= 5.0f;
	}

	return MiningSpeed;
}





float cPlayer::GetMiningProgressPerTick(BLOCKTYPE a_Block)
{
	// Based on https://minecraft.gamepedia.com/Breaking#Calculation
	// If we know it's instantly breakable then quit here:
	if (cBlockInfo::IsOneHitDig(a_Block))
	{
		return 1;
	}
	float BlockHardness = cBlockInfo::GetHardness(a_Block);
	ASSERT(BlockHardness > 0);  // Can't divide by 0 or less, IsOneHitDig should have returned true
	if (GetEquippedItem().GetHandler()->CanHarvestBlock(a_Block))
	{
		BlockHardness *= 1.5f;
	}
	else
	{
		BlockHardness *= 5.0f;
	}
	float DigSpeed = GetDigSpeed(a_Block);
	// LOGD("Time to mine block = %f", BlockHardness/DigSpeed);
	// Number of ticks to mine = (20 * BlockHardness)/DigSpeed;
	// Therefore take inverse to get fraction mined per tick:
	return DigSpeed / (20.0f * BlockHardness);
}





bool cPlayer::CanInstantlyMine(BLOCKTYPE a_Block)
{
	// Based on: https://minecraft.gamepedia.com/Breaking#Calculation
	// Check it has non-zero hardness
	if (cBlockInfo::IsOneHitDig(a_Block))
	{
		return true;
	}
	// If the dig speed is greater than 30 times the hardness, then the wiki says we can instantly mine
	return GetDigSpeed(a_Block) > 30 * cBlockInfo::GetHardness(a_Block);
}





float cPlayer::GetExplosionExposureRate(Vector3d a_ExplosionPosition, float a_ExlosionPower)
{
	if (
		IsGameModeSpectator() ||
		(IsGameModeCreative() && !IsOnGround())
	)
	{
		return 0;  // No impact from explosion
	}

	return Super::GetExplosionExposureRate(a_ExplosionPosition, a_ExlosionPower) / 30.0f;
}
