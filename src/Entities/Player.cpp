
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Player.h"
#include "../ChatColor.h"
#include "../Server.h"
#include "../UI/Window.h"
#include "../UI/WindowOwner.h"
#include "../World.h"
#include "../Bindings/PluginManager.h"
#include "../BlockEntities/BlockEntity.h"
#include "../BlockEntities/EnderChestEntity.h"
#include "../GroupManager.h"
#include "../Group.h"
#include "../Root.h"
#include "../OSSupport/Timer.h"
#include "../Chunk.h"
#include "../Items/ItemHandler.h"
#include "../Vector3.h"

#include "../WorldStorage/StatSerializer.h"
#include "../CompositeChat.h"

#include "inifile/iniFile.h"
#include "json/json.h"

// 6000 ticks or 5 minutes
#define PLAYER_INVENTORY_SAVE_INTERVAL 6000

// 1000 = once per second
#define PLAYER_LIST_TIME_MS 1000






cPlayer::cPlayer(cClientHandle* a_Client, const AString & a_PlayerName) :
	super(etPlayer, 0.6, 1.8),
	m_bVisible(true),
	m_FoodLevel(MAX_FOOD_LEVEL),
	m_FoodSaturationLevel(5.0),
	m_FoodTickTimer(0),
	m_FoodExhaustionLevel(0.0),
	m_LastJumpHeight(0),
	m_LastGroundHeight(0),
	m_bTouchGround(false),
	m_Stance(0.0),
	m_Inventory(*this),
	m_EnderChestContents(9, 3),
	m_CurrentWindow(NULL),
	m_InventoryWindow(NULL),
	m_Color('-'),
	m_GameMode(eGameMode_NotSet),
	m_IP(""),
	m_ClientHandle(a_Client),
	m_NormalMaxSpeed(1.0),
	m_SprintingMaxSpeed(1.3),
	m_FlyingMaxSpeed(1.0),
	m_IsCrouched(false),
	m_IsSprinting(false),
	m_IsFlying(false),
	m_IsSwimming(false),
	m_IsSubmerged(false),
	m_IsFishing(false),
	m_CanFly(false),
	m_EatingFinishTick(-1),
	m_LifetimeTotalXp(0),
	m_CurrentXp(0),
	m_bDirtyExperience(false),
	m_IsChargingBow(false),
	m_BowCharge(0),
	m_FloaterID(-1),
	m_Team(NULL),
	m_TicksUntilNextSave(PLAYER_INVENTORY_SAVE_INTERVAL),
	m_bIsTeleporting(false),
	m_UUID((a_Client != NULL) ? a_Client->GetUUID() : "")
{
	m_InventoryWindow = new cInventoryWindow(*this);
	m_CurrentWindow = m_InventoryWindow;
	m_InventoryWindow->OpenedByPlayer(*this);

	SetMaxHealth(MAX_HEALTH);
	m_Health = MAX_HEALTH;
	
	cTimer t1;
	m_LastPlayerListTime = t1.GetNowTime();
	
	m_PlayerName = a_PlayerName;

	if (!LoadFromDisk())
	{
		m_Inventory.Clear();
		cWorld * DefaultWorld = cRoot::Get()->GetDefaultWorld();
		SetPosX(DefaultWorld->GetSpawnX());
		SetPosY(DefaultWorld->GetSpawnY());
		SetPosZ(DefaultWorld->GetSpawnZ());
		
		LOGD("Player \"%s\" is connecting for the first time, spawning at default world spawn {%.2f, %.2f, %.2f}",
			a_PlayerName.c_str(), GetPosX(), GetPosY(), GetPosZ()
		);
	}

	m_LastJumpHeight = (float)(GetPosY());
	m_LastGroundHeight = (float)(GetPosY());
	m_Stance = GetPosY() + 1.62;

	if (m_GameMode == gmNotSet)
	{
		cWorld * World = cRoot::Get()->GetWorld(GetLoadedWorldName());
		if (World == NULL)
		{
			World = cRoot::Get()->GetDefaultWorld();
		}
		if (World->IsGameModeCreative())
		{
			m_CanFly = true;
		}
	}
	
	cRoot::Get()->GetServer()->PlayerCreated(this);
}





cPlayer::~cPlayer(void)
{
	if (!cRoot::Get()->GetPluginManager()->CallHookPlayerDestroyed(*this))
	{
		cRoot::Get()->BroadcastChatLeave(Printf("%s has left the game", GetName().c_str()));
		LOGINFO("Player %s has left the game", GetName().c_str());
	}

	LOGD("Deleting cPlayer \"%s\" at %p, ID %d", GetName().c_str(), this, GetUniqueID());
	
	// Notify the server that the player is being destroyed
	cRoot::Get()->GetServer()->PlayerDestroying(this);

	SaveToDisk();

	m_World->RemovePlayer( this);

	m_ClientHandle = NULL;
	
	delete m_InventoryWindow;
	m_InventoryWindow = NULL;
	
	LOGD("Player %p deleted", this);
}





void cPlayer::Destroyed()
{
	CloseWindow(false);
	
	m_ClientHandle = NULL;
}





void cPlayer::SpawnOn(cClientHandle & a_Client)
{
	if (!m_bVisible || (m_ClientHandle == (&a_Client)))
	{
		return;
	}
	a_Client.SendPlayerSpawn(*this);
	a_Client.SendEntityHeadLook(*this);
	a_Client.SendEntityEquipment(*this, 0, m_Inventory.GetEquippedItem());
	a_Client.SendEntityEquipment(*this, 1, m_Inventory.GetEquippedBoots());
	a_Client.SendEntityEquipment(*this, 2, m_Inventory.GetEquippedLeggings());
	a_Client.SendEntityEquipment(*this, 3, m_Inventory.GetEquippedChestplate());
	a_Client.SendEntityEquipment(*this, 4, m_Inventory.GetEquippedHelmet());
}





void cPlayer::Tick(float a_Dt, cChunk & a_Chunk)
{
	if (m_ClientHandle != NULL)
	{
		if (m_ClientHandle->IsDestroyed())
		{
			// This should not happen, because destroying a client will remove it from the world, but just in case
			m_ClientHandle = NULL;
			return;
		}
		
		if (!m_ClientHandle->IsPlaying())
		{
			// We're not yet in the game, ignore everything
			return;
		}
	}

	m_Stats.AddValue(statMinutesPlayed, 1);
	
	if (!a_Chunk.IsValid())
	{
		// This may happen if the cPlayer is created before the chunks have the chance of being loaded / generated (#83)
		return;
	}
	
	super::Tick(a_Dt, a_Chunk);
	
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

	if (!GetPosition().EqualsEps(m_LastPos, 0.01))  // Non negligible change in position from last tick?
	{
		// Apply food exhaustion from movement:
		ApplyFoodExhaustionFromMovement();
		
		cRoot::Get()->GetPluginManager()->CallHookPlayerMoving(*this);
		m_ClientHandle->StreamChunks();
	}

	BroadcastMovementUpdate(m_ClientHandle);

	if (m_Health > 0)  // make sure player is alive
	{
		m_World->CollectPickupsByPlayer(this);

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
	cTimer t1;
	if (m_LastPlayerListTime + PLAYER_LIST_TIME_MS <= t1.GetNowTime())
	{
		m_World->SendPlayerList(this);
		m_LastPlayerListTime = t1.GetNowTime();
	}

	if (IsFlying())
	{
		m_LastGroundHeight = (float)GetPosY();
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





short cPlayer::CalcLevelFromXp(short a_XpTotal)
{
	// level 0 to 15
	if (a_XpTotal <= XP_TO_LEVEL15)
	{
		return a_XpTotal / XP_PER_LEVEL_TO15;
	}

	// level 30+
	if (a_XpTotal > XP_TO_LEVEL30)
	{
		return (short) (151.5 + sqrt( 22952.25 - (14 * (2220 - a_XpTotal)))) / 7;
	}

	// level 16 to 30
	return (short) ( 29.5 + sqrt( 870.25 - (6 * ( 360 - a_XpTotal)))) / 3;
}





short cPlayer::XpForLevel(short a_Level)
{
	// level 0 to 15
	if (a_Level <= 15)
	{
		return a_Level * XP_PER_LEVEL_TO15;
	}

	// level 30+
	if (a_Level >= 31)
	{
		return (short) ( (3.5 * a_Level * a_Level) - (151.5 * a_Level) + 2220);
	}

	// level 16 to 30
	return (short) ( (1.5 * a_Level * a_Level) - (29.5 * a_Level) + 360);
}





short cPlayer::GetXpLevel()
{
	return CalcLevelFromXp(m_CurrentXp);
}





float cPlayer::GetXpPercentage()
{
	short int currentLevel = CalcLevelFromXp(m_CurrentXp);
	short int currentLevel_XpBase = XpForLevel(currentLevel);

	return (float)(m_CurrentXp - currentLevel_XpBase) /
		(float)(XpForLevel(1+currentLevel) - currentLevel_XpBase);
}





bool cPlayer::SetCurrentExperience(short int a_CurrentXp)
{
	if (!(a_CurrentXp >= 0) || (a_CurrentXp > (SHRT_MAX - m_LifetimeTotalXp)))
	{
		LOGWARNING("Tried to update experiece with an invalid Xp value: %d", a_CurrentXp);
		return false;  // oops, they gave us a dodgey number
	}

	m_CurrentXp = a_CurrentXp;

	// Set experience to be updated
	m_bDirtyExperience = true;

	return true;
}





short cPlayer::DeltaExperience(short a_Xp_delta)
{
	if (a_Xp_delta > (SHRT_MAX - m_CurrentXp))
	{
		// Value was bad, abort and report
		LOGWARNING("Attempt was made to increment Xp by %d, which overflowed the short datatype. Ignoring.",
			a_Xp_delta);
		return -1;  // Should we instead just return the current Xp?
	}

	m_CurrentXp += a_Xp_delta;

	// Make sure they didn't subtract too much
	m_CurrentXp = std::max<short int>(m_CurrentXp, 0);

	// Update total for score calculation
	if (a_Xp_delta > 0)
	{
		m_LifetimeTotalXp += a_Xp_delta;
	}

	LOGD("Player \"%s\" gained/lost %d experience, total is now: %d",
		GetName().c_str(), a_Xp_delta, m_CurrentXp);

	// Set experience to be updated
	m_bDirtyExperience = true;

	return m_CurrentXp;
}





void cPlayer::StartChargingBow(void)
{
	LOGD("Player \"%s\" started charging their bow", GetName().c_str());
	m_IsChargingBow = true;
	m_BowCharge = 0;
	m_World->BroadcastEntityMetadata(*this, m_ClientHandle);
}





int cPlayer::FinishChargingBow(void)
{
	LOGD("Player \"%s\" finished charging their bow at a charge of %d", GetName().c_str(), m_BowCharge);
	int res = m_BowCharge;
	m_IsChargingBow = false;
	m_BowCharge = 0;
	m_World->BroadcastEntityMetadata(*this, m_ClientHandle);

	return res;
}





void cPlayer::CancelChargingBow(void)
{
	LOGD("Player \"%s\" cancelled charging their bow at a charge of %d", GetName().c_str(), m_BowCharge);
	m_IsChargingBow = false;
	m_BowCharge = 0;
	m_World->BroadcastEntityMetadata(*this, m_ClientHandle);
}





void cPlayer::SetTouchGround(bool a_bTouchGround)
{
	m_bTouchGround = a_bTouchGround;

	if (!m_bTouchGround)
	{
		if (GetPosY() > m_LastJumpHeight)
		{
			m_LastJumpHeight = (float)GetPosY();
		}
		cWorld * World = GetWorld();
		if ((GetPosY() >= 0) && (GetPosY() < cChunkDef::Height))
		{
			BLOCKTYPE BlockType = World->GetBlock(POSX_TOINT, POSY_TOINT, POSZ_TOINT);
			if (BlockType != E_BLOCK_AIR)
			{
				m_bTouchGround = true;
			}
			if (
				(BlockType == E_BLOCK_WATER) ||
				(BlockType == E_BLOCK_STATIONARY_WATER) ||
				(BlockType == E_BLOCK_LADDER) ||
				(BlockType == E_BLOCK_VINES)
			)
			{
				m_LastGroundHeight = (float)GetPosY();
			}
		}
	}
	else
	{
		float Dist = (float)(m_LastGroundHeight - floor(GetPosY()));

		if (Dist >= 2.0)  // At least two blocks - TODO: Use m_LastJumpHeight instead of m_LastGroundHeight above
		{
			// Increment statistic
			m_Stats.AddValue(statDistFallen, (StatValue)floor(Dist * 100 + 0.5));
		}

		int Damage = (int)(Dist - 3.f);
		if (m_LastJumpHeight > m_LastGroundHeight)
		{
			Damage++;
		}
		m_LastJumpHeight = (float)GetPosY();

		if (Damage > 0)
		{
			// cPlayer makes sure damage isn't applied in creative, no need to check here
			TakeDamage(dtFalling, NULL, Damage, Damage, 0);
			
			// Fall particles
			GetWorld()->BroadcastSoundParticleEffect(2006, POSX_TOINT, (int)GetPosY() - 1, POSZ_TOINT, Damage /* Used as particle effect speed modifier */);
		}

		m_LastGroundHeight = (float)GetPosY();
	}
}





void cPlayer::Heal(int a_Health)
{
	super::Heal(a_Health);
	SendHealth();
}





void cPlayer::SetFoodLevel(int a_FoodLevel)
{
	int FoodLevel = std::max(0, std::min(a_FoodLevel, (int)MAX_FOOD_LEVEL));

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
	m_FoodSaturationLevel = std::max(0.0, std::min(a_FoodSaturationLevel, (double)m_FoodLevel));
}





void cPlayer::SetFoodTickTimer(int a_FoodTickTimer)
{
	m_FoodTickTimer = a_FoodTickTimer;
}





void cPlayer::SetFoodExhaustionLevel(double a_FoodExhaustionLevel)
{
	m_FoodExhaustionLevel = std::max(0.0, std::min(a_FoodExhaustionLevel, 4.0));
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





void cPlayer::FoodPoison(int a_NumTicks)
{
	AddEntityEffect(cEntityEffect::effHunger, a_NumTicks, 0, 1);
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
	m_World->BroadcastEntityAnimation(*this, 0);
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

	GetInventory().RemoveOneEquippedItem();

	// if the food is mushroom soup, return a bowl to the inventory
	if (Item.m_ItemType == E_ITEM_MUSHROOM_SOUP)
	{
		cItem emptyBowl(E_ITEM_BOWL, 1, 0, "");
		GetInventory().AddItem(emptyBowl, true, true);
	}
}





void cPlayer::AbortEating(void)
{
	m_EatingFinishTick = -1;
	m_World->BroadcastEntityAnimation(*this, 0);
	m_World->BroadcastEntityMetadata(*this);
}





void cPlayer::SendHealth(void)
{
	if (m_ClientHandle != NULL)
	{
		m_ClientHandle->SendHealth();
	}
}





void cPlayer::SendExperience(void)
{
	if (m_ClientHandle != NULL)
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
	else
	{
		if (m_IsSprinting)
		{
			return m_SprintingMaxSpeed;
		}
		else
		{
			return m_NormalMaxSpeed;
		}
	}
}





void cPlayer::SetNormalMaxSpeed(double a_Speed)
{
	m_NormalMaxSpeed = a_Speed;
	if (!m_IsSprinting && !m_IsFlying)
	{
		m_ClientHandle->SendPlayerMaxSpeed();
	}
}





void cPlayer::SetSprintingMaxSpeed(double a_Speed)
{
	m_SprintingMaxSpeed = a_Speed;
	if (m_IsSprinting && !m_IsFlying)
	{
		m_ClientHandle->SendPlayerMaxSpeed();
	}
}





void cPlayer::SetFlyingMaxSpeed(double a_Speed)
{
	m_FlyingMaxSpeed = a_Speed;
	
	// Update the flying speed, always:
	m_ClientHandle->SendPlayerAbilities();
}





void cPlayer::SetCrouch(bool a_IsCrouched)
{
	// Set the crouch status, broadcast to all visible players
	
	if (a_IsCrouched == m_IsCrouched)
	{
		// No change
		return;
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





void cPlayer::SetFlying(bool a_IsFlying)
{
	if (a_IsFlying == m_IsFlying)
	{
		return;
	}

	m_IsFlying = a_IsFlying;
	m_ClientHandle->SendPlayerAbilities();
}





bool cPlayer::DoTakeDamage(TakeDamageInfo & a_TDI)
{
	if ((a_TDI.DamageType != dtInVoid) && (a_TDI.DamageType != dtPlugin))
	{
		if (IsGameModeCreative())
		{
			// No damage / health in creative mode if not void or plugin damage
			return false;
		}
	}

	if ((a_TDI.Attacker != NULL) && (a_TDI.Attacker->IsPlayer()))
	{
		cPlayer * Attacker = (cPlayer *)a_TDI.Attacker;

		if ((m_Team != NULL) && (m_Team == Attacker->m_Team))
		{
			if (!m_Team->AllowsFriendlyFire())
			{
				// Friendly fire is disabled
				return false;
			}
		}
	}
	
	if (super::DoTakeDamage(a_TDI))
	{
		// Any kind of damage adds food exhaustion
		AddFoodExhaustion(0.3f);
		SendHealth();

		m_Stats.AddValue(statDamageTaken, (StatValue)floor(a_TDI.FinalDamage * 10 + 0.5));
		return true;
	}
	return false;
}





void cPlayer::KilledBy(TakeDamageInfo & a_TDI)
{
	super::KilledBy(a_TDI);

	if (m_Health > 0)
	{
		return;  //  not dead yet =]
	}

	m_bVisible = false;  // So new clients don't see the player

	// Puke out all the items
	cItems Pickups;
	m_Inventory.CopyToItems(Pickups);
	m_Inventory.Clear();

	if (GetName() == "Notch")
	{
		Pickups.Add(cItem(E_ITEM_RED_APPLE));
	}

	m_Stats.AddValue(statItemsDropped, Pickups.Size());

	m_World->SpawnItemPickups(Pickups, GetPosX(), GetPosY(), GetPosZ(), 10);
	SaveToDisk();  // Save it, yeah the world is a tough place !

	if (a_TDI.Attacker == NULL)
	{
		AString DamageText;
		switch (a_TDI.DamageType)
		{
			case dtRangedAttack: DamageText = "was shot"; break;
			case dtLightning: DamageText = "was plasmified by lightining"; break;
			case dtFalling: DamageText = (GetWorld()->GetTickRandomNumber(10) % 2 == 0) ? "fell to death" : "hit the ground too hard"; break;
			case dtDrowning: DamageText = "drowned"; break;
			case dtSuffocating: DamageText = (GetWorld()->GetTickRandomNumber(10) % 2 == 0) ? "git merge'd into a block" : "fused with a block"; break;
			case dtStarving: DamageText = "forgot the importance of food"; break;
			case dtCactusContact: DamageText = "was impaled on a cactus"; break;
			case dtLavaContact: DamageText = "was melted by lava"; break;
			case dtPoisoning: DamageText = "died from septicaemia"; break;
			case dtWithering: DamageText = "is a husk of their former selves"; break;
			case dtOnFire: DamageText = "forgot to stop, drop, and roll"; break;
			case dtFireContact: DamageText = "burnt themselves to death"; break;
			case dtInVoid: DamageText = "somehow fell out of the world"; break;
			case dtPotionOfHarming: DamageText = "was magicked to death"; break;
			case dtEnderPearl: DamageText = "misused an ender pearl"; break;
			case dtAdmin: DamageText = "was administrator'd"; break;
			case dtExplosion: DamageText = "blew up"; break;
			default: DamageText = "died, somehow; we've no idea how though"; break;
		}
		GetWorld()->BroadcastChatDeath(Printf("%s %s", GetName().c_str(), DamageText.c_str()));
	}
	else if (a_TDI.Attacker->IsPlayer())
	{
		cPlayer * Killer = (cPlayer *)a_TDI.Attacker;

		GetWorld()->BroadcastChatDeath(Printf("%s was killed by %s", GetName().c_str(), Killer->GetName().c_str()));
	}
	else
	{
		AString KillerClass = a_TDI.Attacker->GetClass();
		KillerClass.erase(KillerClass.begin());  // Erase the 'c' of the class (e.g. "cWitch" -> "Witch")

		GetWorld()->BroadcastChatDeath(Printf("%s was killed by a %s", GetName().c_str(), KillerClass.c_str()));
	}

	m_Stats.AddValue(statDeaths);

	m_World->GetScoreBoard().AddPlayerScore(GetName(), cObjective::otDeathCount, 1);
}





void cPlayer::Killed(cEntity * a_Victim)
{
	cScoreboard & ScoreBoard = m_World->GetScoreBoard();

	if (a_Victim->IsPlayer())
	{
		m_Stats.AddValue(statPlayerKills);

		ScoreBoard.AddPlayerScore(GetName(), cObjective::otPlayerKillCount, 1);
	}
	else if (a_Victim->IsMob())
	{
		if (((cMonster *)a_Victim)->GetMobFamily() == cMonster::mfHostile)
		{
			AwardAchievement(achKillMonster);
		}

		m_Stats.AddValue(statMobKills);
	}

	ScoreBoard.AddPlayerScore(GetName(), cObjective::otTotalKillCount, 1);
}





void cPlayer::Respawn(void)
{
	ASSERT(m_World != NULL);

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

	m_ClientHandle->SendRespawn(*m_World, true);
	
	// Extinguish the fire:
	StopBurning();

	TeleportToCoords(GetWorld()->GetSpawnX(), GetWorld()->GetSpawnY(), GetWorld()->GetSpawnZ());

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
	return (m_GameMode == gmCreative) ||  // Either the player is explicitly in Creative
		((m_GameMode == gmNotSet) &&  m_World->IsGameModeCreative());  // or they inherit from the world and the world is Creative
}





bool cPlayer::IsGameModeSurvival(void) const
{
	return (m_GameMode == gmSurvival) ||  // Either the player is explicitly in Survival
		((m_GameMode == gmNotSet) &&  m_World->IsGameModeSurvival());  // or they inherit from the world and the world is Survival
}





bool cPlayer::IsGameModeAdventure(void) const
{
	return (m_GameMode == gmAdventure) ||  // Either the player is explicitly in Adventure
		((m_GameMode == gmNotSet) &&  m_World->IsGameModeAdventure());  // or they inherit from the world and the world is Adventure
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
	if (m_World == NULL)
	{
		SetTeam(NULL);
	}
	else
	{
		cScoreboard & Scoreboard = m_World->GetScoreBoard();

		SetTeam(Scoreboard.QueryPlayerTeam(GetName()));
	}

	return m_Team;
}





void cPlayer::OpenWindow(cWindow * a_Window)
{
	if (a_Window != m_CurrentWindow)
	{
		CloseWindow(false);
	}
	a_Window->OpenedByPlayer(*this);
	m_CurrentWindow = a_Window;
	a_Window->SendWholeWindow(*GetClientHandle());
}





void cPlayer::CloseWindow(bool a_CanRefuse)
{
	if (m_CurrentWindow == NULL)
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
	if ((m_CurrentWindow == NULL) || (m_CurrentWindow->GetWindowID() != a_WindowID))
	{
		return;
	}
	CloseWindow();
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
	
	m_GameMode = a_GameMode;
	m_ClientHandle->SendGameMode(a_GameMode);

	if (!IsGameModeCreative())
	{
		SetFlying(false);
		SetCanFly(false);
	}
}





void cPlayer::LoginSetGameMode( eGameMode a_GameMode)
{
	m_GameMode = a_GameMode;
}





void cPlayer::SetIP(const AString & a_IP)
{
	m_IP = a_IP;
}





unsigned int cPlayer::AwardAchievement(const eStatistic a_Ach)
{
	eStatistic Prerequisite = cStatInfo::GetPrerequisite(a_Ach);

	// Check if the prerequisites are met
	if (Prerequisite != statInvalid)
	{
		if (m_Stats.GetValue(Prerequisite) == 0)
		{
			return 0;
		}
	}

	StatValue Old = m_Stats.GetValue(a_Ach);

	if (Old > 0)
	{
		return m_Stats.AddValue(a_Ach);
	}
	else
	{
		// First time, announce it
		cCompositeChat Msg;
		Msg.SetMessageType(mtSuccess);
		Msg.AddShowAchievementPart(GetName(), cStatInfo::GetName(a_Ach));
		m_World->BroadcastChat(Msg);

		// Increment the statistic
		StatValue New = m_Stats.AddValue(a_Ach);

		// Achievement Get!
		m_ClientHandle->SendStatistics(m_Stats);

		return New;
	}
}





void cPlayer::TeleportToCoords(double a_PosX, double a_PosY, double a_PosZ)
{
	SetPosition(a_PosX, a_PosY, a_PosZ);
	m_LastGroundHeight = (float)a_PosY;
	m_LastJumpHeight = (float)a_PosY;
	m_bIsTeleporting = true;

	m_World->BroadcastTeleportEntity(*this, GetClientHandle());
	m_ClientHandle->SendPlayerMoveLook();
}





void cPlayer::SendRotation(double a_YawDegrees, double a_PitchDegrees)
{
	SetYaw(a_YawDegrees);
	SetPitch(a_PitchDegrees);
	m_ClientHandle->SendPlayerMoveLook();
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
	super::DoSetSpeed(a_SpeedX, a_SpeedY, a_SpeedZ);

	// Send the speed to the client so he actualy moves
	m_ClientHandle->SendEntityVelocity(*this);
}





void cPlayer::MoveTo( const Vector3d & a_NewPos)
{
	if ((a_NewPos.y < -990) && (GetPosY() > -100))
	{
		// When attached to an entity, the client sends position packets with weird coords:
		// Y = -999 and X, Z = attempting to create speed, usually up to 0.03
		// We cannot test m_AttachedTo, because when deattaching, the server thinks the client is already deattached while
		// the client may still send more of these nonsensical packets.
		if (m_AttachedTo != NULL)
		{
			Vector3d AddSpeed(a_NewPos);
			AddSpeed.y = 0;
			m_AttachedTo->AddSpeed(AddSpeed);
		}
		return;
	}
	
	// TODO: should do some checks to see if player is not moving through terrain
	// TODO: Official server refuses position packets too far away from each other, kicking "hacked" clients; we should, too

	Vector3d DeltaPos = a_NewPos - GetPosition();
	UpdateMovementStats(DeltaPos);
	
	SetPosition( a_NewPos);
	SetStance(a_NewPos.y + 1.62);
}





void cPlayer::SetVisible(bool a_bVisible)
{
	if (a_bVisible && !m_bVisible)  // Make visible
	{
		m_bVisible = true;
		m_World->BroadcastSpawnEntity(*this);
	}
	if (!a_bVisible && m_bVisible)
	{
		m_bVisible = false;
		m_World->BroadcastDestroyEntity(*this, m_ClientHandle);  // Destroy on all clients
	}
}





void cPlayer::AddToGroup( const AString & a_GroupName)
{
	cGroup* Group = cRoot::Get()->GetGroupManager()->GetGroup( a_GroupName);
	m_Groups.push_back( Group);
	LOGD("Added %s to group %s", GetName().c_str(), a_GroupName.c_str());
	ResolveGroups();
	ResolvePermissions();
}





void cPlayer::RemoveFromGroup( const AString & a_GroupName)
{
	bool bRemoved = false;
	for (GroupList::iterator itr = m_Groups.begin(); itr != m_Groups.end(); ++itr)
	{
		if ((*itr)->GetName().compare(a_GroupName) == 0)
		{
			m_Groups.erase( itr);
			bRemoved = true;
			break;
		}
	}

	if (bRemoved)
	{
		LOGD("Removed %s from group %s", GetName().c_str(), a_GroupName.c_str());
		ResolveGroups();
		ResolvePermissions();
	}
	else
	{
		LOGWARN("Tried to remove %s from group %s but was not in that group", GetName().c_str(), a_GroupName.c_str());
	}
}





bool cPlayer::HasPermission(const AString & a_Permission)
{
	if (a_Permission.empty())
	{
		// Empty permission request is always granted
		return true;
	}
	
	AStringVector Split = StringSplit( a_Permission, ".");
	PermissionMap Possibilities = m_ResolvedPermissions;
	// Now search the namespaces
	while (Possibilities.begin() != Possibilities.end())
	{
		PermissionMap::iterator itr = Possibilities.begin();
		if (itr->second)
		{
			AStringVector OtherSplit = StringSplit( itr->first, ".");
			if (OtherSplit.size() <= Split.size())
			{
				unsigned int i;
				for (i = 0; i < OtherSplit.size(); ++i)
				{
					if (OtherSplit[i].compare( Split[i]) != 0)
					{
						if (OtherSplit[i].compare("*") == 0) return true;  // WildCard man!! WildCard!
						break;
					}
				}
				if (i == Split.size()) return true;
			}
		}
		Possibilities.erase( itr);
	}

	// Nothing that matched :(
	return false;
}





bool cPlayer::IsInGroup( const AString & a_Group)
{
	for (GroupList::iterator itr = m_ResolvedGroups.begin(); itr != m_ResolvedGroups.end(); ++itr)
	{
		if (a_Group.compare( (*itr)->GetName().c_str()) == 0)
			return true;
	}
	return false;
}





void cPlayer::ResolvePermissions()
{
	m_ResolvedPermissions.clear();  // Start with an empty map

	// Copy all player specific permissions into the resolved permissions map
	for (PermissionMap::iterator itr = m_Permissions.begin(); itr != m_Permissions.end(); ++itr)
	{
		m_ResolvedPermissions[ itr->first ] = itr->second;
	}

	for (GroupList::iterator GroupItr = m_ResolvedGroups.begin(); GroupItr != m_ResolvedGroups.end(); ++GroupItr)
	{
		const cGroup::PermissionMap & Permissions = (*GroupItr)->GetPermissions();
		for (cGroup::PermissionMap::const_iterator itr = Permissions.begin(); itr != Permissions.end(); ++itr)
		{
			m_ResolvedPermissions[ itr->first ] = itr->second;
		}
	}
}





void cPlayer::ResolveGroups()
{
	// Clear resolved groups first
	m_ResolvedGroups.clear();

	// Get a complete resolved list of all groups the player is in
	std::map< cGroup*, bool > AllGroups;  // Use a map, because it's faster than iterating through a list to find duplicates
	GroupList ToIterate;
	for (GroupList::iterator GroupItr = m_Groups.begin(); GroupItr != m_Groups.end(); ++GroupItr)
	{
		ToIterate.push_back( *GroupItr);
	}
	while (ToIterate.begin() != ToIterate.end())
	{
		cGroup* CurrentGroup = *ToIterate.begin();
		if (AllGroups.find( CurrentGroup) != AllGroups.end())
		{
			LOGWARNING("ERROR: Player \"%s\" is in the group multiple times (\"%s\"). Please fix your settings in users.ini!",
				GetName().c_str(), CurrentGroup->GetName().c_str()
			);
		}
		else
		{
			AllGroups[ CurrentGroup ] = true;
			m_ResolvedGroups.push_back( CurrentGroup);  // Add group to resolved list
			const cGroup::GroupList & Inherits = CurrentGroup->GetInherits();
			for (cGroup::GroupList::const_iterator itr = Inherits.begin(); itr != Inherits.end(); ++itr)
			{
				if (AllGroups.find( *itr) != AllGroups.end())
				{
					LOGERROR("ERROR: Player %s is in the same group multiple times due to inheritance (%s). FIX IT!", GetName().c_str(), (*itr)->GetName().c_str());
					continue;
				}
				ToIterate.push_back( *itr);
			}
		}
		ToIterate.erase( ToIterate.begin());
	}
}





AString cPlayer::GetColor(void) const
{
	if (m_Color != '-')
	{
		return cChatColor::Delimiter + m_Color;
	}

	if (m_Groups.size() < 1)
	{
		return cChatColor::White;
	}

	return (*m_Groups.begin())->GetColor();
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





void cPlayer::TossItems(const cItems & a_Items)
{
	m_Stats.AddValue(statItemsDropped, a_Items.Size());

	double vX = 0, vY = 0, vZ = 0;
	EulerToVector(-GetYaw(), GetPitch(), vZ, vX, vY);
	vY = -vY * 2 + 1.f;
	m_World->SpawnItemPickups(a_Items, GetPosX(), GetEyeHeight(), GetPosZ(), vX * 3, vY * 3, vZ * 3, true);  // 'true' because created by player
}





bool cPlayer::MoveToWorld(const char * a_WorldName)
{
	cWorld * World = cRoot::Get()->GetWorld(a_WorldName);
	if (World == NULL)
	{
		LOG("%s: Couldn't find world \"%s\".", __FUNCTION__, a_WorldName);
		return false;
	}
	
	// Send the respawn packet:
	if (m_ClientHandle != NULL)
	{
		m_ClientHandle->SendRespawn(*World);
	}

	// Remove all links to the old world
	m_World->RemovePlayer(this);

	// If the dimension is different, we can send the respawn packet
	// http://wiki.vg/Protocol#0x09 says "don't send if dimension is the same" as of 2013_07_02

	// Queue adding player to the new world, including all the necessary adjustments to the object
	World->AddPlayer(this);

	return true;
}





void cPlayer::LoadPermissionsFromDisk()
{
	m_Groups.clear();
	m_Permissions.clear();

	cIniFile IniFile;
	if (IniFile.ReadFile("users.ini"))
	{
		AString Groups = IniFile.GetValueSet(GetName(), "Groups", "Default");
		AStringVector Split = StringSplitAndTrim(Groups, ",");

		for (AStringVector::const_iterator itr = Split.begin(), end = Split.end(); itr != end; ++itr)
		{
			if (!cRoot::Get()->GetGroupManager()->ExistsGroup(*itr))
			{
				LOGWARNING("The group %s for player %s was not found!", itr->c_str(), GetName().c_str());
			}
			AddToGroup(*itr);
		}

		AString Color = IniFile.GetValue(GetName(), "Color", "-");
		if (!Color.empty())
		{
			m_Color = Color[0];
		}
	}
	else
	{
		cGroupManager::GenerateDefaultUsersIni(IniFile);
		IniFile.AddValue("Groups", GetName(), "Default");
		AddToGroup("Default");
	}
	IniFile.WriteFile("users.ini");
	ResolvePermissions();
}





bool cPlayer::LoadFromDisk(void)
{
	LoadPermissionsFromDisk();

	// Load from the UUID file:
	if (LoadFromFile(GetUUIDFileName(m_UUID)))
	{
		return true;
	}
	
	// Load from the offline UUID file, if allowed:
	AString OfflineUUID = cClientHandle::GenerateOfflineUUID(GetName());
	if (cRoot::Get()->GetServer()->ShouldLoadOfflinePlayerData())
	{
		if (LoadFromFile(GetUUIDFileName(OfflineUUID)))
		{
			return true;
		}
	}
	
	// Load from the old-style name-based file, if allowed:
	if (cRoot::Get()->GetServer()->ShouldLoadNamedPlayerData())
	{
		AString OldStyleFileName = Printf("players/%s.json", GetName().c_str());
		if (LoadFromFile(OldStyleFileName))
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
	LOG("Player data file not found for %s (%s, offline %s), will be reset to defaults.",
		GetName().c_str(), m_UUID.c_str(), OfflineUUID.c_str()
	);
	return false;
}





bool cPlayer::LoadFromFile(const AString & a_FileName)
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
	Json::Reader reader;
	if (!reader.parse(buffer, root, false))
	{
		LOGWARNING("Cannot parse player data in file \"%s\"", a_FileName.c_str());
		return false;
	}

	// Load the player data:
	Json::Value & JSON_PlayerPosition = root["position"];
	if (JSON_PlayerPosition.size() == 3)
	{
		SetPosX(JSON_PlayerPosition[(unsigned)0].asDouble());
		SetPosY(JSON_PlayerPosition[(unsigned)1].asDouble());
		SetPosZ(JSON_PlayerPosition[(unsigned)2].asDouble());
		m_LastPos = GetPosition();
	}

	Json::Value & JSON_PlayerRotation = root["rotation"];
	if (JSON_PlayerRotation.size() == 3)
	{
		SetYaw      ((float)JSON_PlayerRotation[(unsigned)0].asDouble());
		SetPitch    ((float)JSON_PlayerRotation[(unsigned)1].asDouble());
		SetRoll     ((float)JSON_PlayerRotation[(unsigned)2].asDouble());
	}

	m_Health              = root.get("health",         0).asInt();
	m_AirLevel            = root.get("air",            MAX_AIR_LEVEL).asInt();
	m_FoodLevel           = root.get("food",           MAX_FOOD_LEVEL).asInt();
	m_FoodSaturationLevel = root.get("foodSaturation", MAX_FOOD_LEVEL).asDouble();
	m_FoodTickTimer       = root.get("foodTickTimer",  0).asInt();
	m_FoodExhaustionLevel = root.get("foodExhaustion", 0).asDouble();
	m_LifetimeTotalXp     = (short) root.get("xpTotal", 0).asInt();
	m_CurrentXp           = (short) root.get("xpCurrent", 0).asInt();
	m_IsFlying            = root.get("isflying", 0).asBool();

	m_GameMode = (eGameMode) root.get("gamemode", eGameMode_NotSet).asInt();

	if (m_GameMode == eGameMode_Creative)
	{
		m_CanFly = true;
	}
	
	m_Inventory.LoadFromJson(root["inventory"]);
	cEnderChestEntity::LoadFromJson(root["enderchestinventory"], m_EnderChestContents);

	m_LoadedWorldName = root.get("world", "world").asString();

	// Load the player stats.
	// We use the default world name (like bukkit) because stats are shared between dimensions/worlds.
	cStatSerializer StatSerializer(cRoot::Get()->GetDefaultWorld()->GetName(), GetName(), &m_Stats);
	StatSerializer.Load();
	
	LOGD("Player %s was read from file \"%s\", spawning at {%.2f, %.2f, %.2f} in world \"%s\"",
		GetName().c_str(), a_FileName.c_str(), GetPosX(), GetPosY(), GetPosZ(), m_LoadedWorldName.c_str()
	);
	
	return true;
}





bool cPlayer::SaveToDisk()
{
	cFile::CreateFolder(FILE_IO_PREFIX + AString("players"));
	cFile::CreateFolder(FILE_IO_PREFIX + AString("players/") + m_UUID.substr(0, 2));

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

	Json::Value root;
	root["position"]            = JSON_PlayerPosition;
	root["rotation"]            = JSON_PlayerRotation;
	root["inventory"]           = JSON_Inventory;
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
	if (m_World != NULL)
	{
		root["world"] = m_World->GetName();
		if (m_GameMode == m_World->GetGameMode())
		{
			root["gamemode"] = (int) eGameMode_NotSet;
		}
		else
		{
			root["gamemode"] = (int) m_GameMode;
		}
	}
	else
	{
		// This happens if the player is saved to new format after loading from the old format
		root["world"]    = m_LoadedWorldName;
		root["gamemode"] = (int) eGameMode_NotSet;
	}

	Json::StyledWriter writer;
	std::string JsonData = writer.write(root);

	AString SourceFile = GetUUIDFileName(m_UUID);

	cFile f;
	if (!f.Open(SourceFile, cFile::fmWrite))
	{
		LOGWARNING("Error writing player \"%s\" to file \"%s\" - cannot open file. Player will lose their progress.",
			GetName().c_str(), SourceFile.c_str()
		);
		return false;
	}
	if (f.Write(JsonData.c_str(), JsonData.size()) != (int)JsonData.size())
	{
		LOGWARNING("Error writing player \"%s\" to file \"%s\" - cannot save data. Player will lose their progress. ",
			GetName().c_str(), SourceFile.c_str()
		);
		return false;
	}

	// Save the player stats.
	// We use the default world name (like bukkit) because stats are shared between dimensions/worlds.
	cStatSerializer StatSerializer(cRoot::Get()->GetDefaultWorld()->GetName(), GetName(), &m_Stats);
	if (!StatSerializer.Save())
	{
		LOGWARNING("Could not save stats for player %s", GetName().c_str());
		return false;
	}

	return true;
}





cPlayer::StringList cPlayer::GetResolvedPermissions()
{
	StringList Permissions;

	const PermissionMap& ResolvedPermissions = m_ResolvedPermissions;
	for (PermissionMap::const_iterator itr = ResolvedPermissions.begin(); itr != ResolvedPermissions.end(); ++itr)
	{
		if (itr->second)
		{
			Permissions.push_back( itr->first);
		}
	}

	return Permissions;
}





void cPlayer::UseEquippedItem(void)
{
	if (IsGameModeCreative())  // No damage in creative
	{
		return;
	}

	if (GetInventory().DamageEquippedItem())
	{
		m_World->BroadcastSoundEffect("random.break", GetPosX(), GetPosY(), GetPosZ(), 0.5f, (float)(0.75 + ((float)((GetUniqueID() * 23) % 32)) / 64));
	}
}




void cPlayer::TickBurning(cChunk & a_Chunk)
{
	// Don't burn in creative and stop burning in creative if necessary
	if (!IsGameModeCreative())
	{
		super::TickBurning(a_Chunk);
	}
	else if (IsOnFire())
	{
		m_TicksLeftBurning = 0;
		OnFinishedBurning();
	}
}





void cPlayer::HandleFood(void)
{
	// Ref.: http://www.minecraftwiki.net/wiki/Hunger

	if (IsGameModeCreative())
	{
		// Hunger is disabled for Creative
		return;
	}

	// Heal or damage, based on the food level, using the m_FoodTickTimer:
	if ((m_FoodLevel > 17) || (m_FoodLevel <= 0))
	{
		m_FoodTickTimer++;
		if (m_FoodTickTimer >= 80)
		{
			m_FoodTickTimer = 0;

			if ((m_FoodLevel > 17) && (GetHealth() < GetMaxHealth()))
			{
				// Regenerate health from food, incur 3 pts of food exhaustion:
				Heal(1);
				m_FoodExhaustionLevel += 3.0;
			}
			else if ((m_FoodLevel <= 0) && (m_Health > 1))
			{
				// Damage from starving
				TakeDamage(dtStarving, NULL, 1, 1, 0);
			}
		}
	}

	// Apply food exhaustion that has accumulated:
	if (m_FoodExhaustionLevel >= 4.0)
	{
		m_FoodExhaustionLevel -= 4.0;

		if (m_FoodSaturationLevel >= 1.0)
		{
			m_FoodSaturationLevel -= 1.0;
		}
		else
		{
			SetFoodLevel(m_FoodLevel - 1);
		}
	}
}





void cPlayer::HandleFloater()
{
	if (GetEquippedItem().m_ItemType == E_ITEM_FISHING_ROD)
	{
		return;
	}
	class cFloaterCallback :
		public cEntityCallback
	{
	public:
		virtual bool Item(cEntity * a_Entity) override
		{
			a_Entity->Destroy(true);
			return true;
		}
	} Callback;
	m_World->DoWithEntityByID(m_FloaterID, Callback);
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





void cPlayer::UpdateMovementStats(const Vector3d & a_DeltaPos)
{
	StatValue Value = (StatValue)floor(a_DeltaPos.Length() * 100 + 0.5);

	if (m_AttachedTo == NULL)
	{
		if (IsClimbing())
		{
			if (a_DeltaPos.y > 0.0)  // Going up
			{
				m_Stats.AddValue(statDistClimbed, (StatValue)floor(a_DeltaPos.y * 100 + 0.5));
			}
		}
		else if (IsSubmerged())
		{
			m_Stats.AddValue(statDistDove, Value);
		}
		else if (IsSwimming())
		{
			m_Stats.AddValue(statDistSwum, Value);
		}
		else if (IsOnGround())
		{
			m_Stats.AddValue(statDistWalked, Value);
		}
		else
		{
			if (Value >= 25)  // Ignore small/slow movement
			{
				m_Stats.AddValue(statDistFlown, Value);
			}
		}
	}
	else
	{
		switch (m_AttachedTo->GetEntityType())
		{
			case cEntity::etMinecart: m_Stats.AddValue(statDistMinecart, Value); break;
			case cEntity::etBoat:     m_Stats.AddValue(statDistBoat,     Value); break;
			case cEntity::etMonster:
			{
				cMonster * Monster = (cMonster *)m_AttachedTo;
				switch (Monster->GetMobType())
				{
					case cMonster::mtPig:   m_Stats.AddValue(statDistPig,   Value); break;
					case cMonster::mtHorse: m_Stats.AddValue(statDistHorse, Value); break;
					default: break;
				}
				break;
			}
			default: break;
		}
	}
}





void cPlayer::ApplyFoodExhaustionFromMovement()
{
	if (IsGameModeCreative())
	{
		return;
	}

	// If we have just teleported, apply no exhaustion
	if (m_bIsTeleporting)
	{
		m_bIsTeleporting = false;
		return;
	}

	// If riding anything, apply no food exhaustion
	if (m_AttachedTo != NULL)
	{
		return;
	}

	// Process exhaustion every two ticks as that is how frequently m_LastPos is updated
	// Otherwise, we apply exhaustion for a 'movement' every tick, one of which is an already processed value
	if (GetWorld()->GetWorldAge() % 2 != 0)
	{
		return;
	}
	
	// Calculate the distance travelled, update the last pos:
	Vector3d Movement(GetPosition() - m_LastPos);
	Movement.y = 0;  // Only take XZ movement into account

	// Apply the exhaustion based on distance travelled:
	double BaseExhaustion = Movement.Length();
	if (IsSprinting())
	{
		// 0.1 pt per meter sprinted
		BaseExhaustion = BaseExhaustion * 0.1;
	}
	else if (IsSwimming())
	{
		// 0.015 pt per meter swum
		BaseExhaustion = BaseExhaustion * 0.015;
	}
	else
	{
		// 0.01 pt per meter walked / sneaked
		BaseExhaustion = BaseExhaustion * 0.01;
	}
	m_FoodExhaustionLevel += BaseExhaustion;
}





void cPlayer::Detach()
{
	super::Detach();
	int PosX = POSX_TOINT;
	int PosY = POSY_TOINT;
	int PosZ = POSZ_TOINT;

	// Search for a position within an area to teleport player after detachment
	// Position must be solid land, and occupied by a nonsolid block
	// If nothing found, player remains where they are
	for (int x = PosX - 2; x <= (PosX + 2); ++x)
	{
		for (int y = PosY; y <= (PosY + 3); ++y)
		{
			for (int z = PosZ - 2; z <= (PosZ + 2); ++z)
			{
				if (!cBlockInfo::IsSolid(m_World->GetBlock(x, y, z)) && cBlockInfo::IsSolid(m_World->GetBlock(x, y - 1, z)))
				{
					TeleportToCoords(x, y, z);
					return;
				}
			}
		}
	}
}





AString cPlayer::GetUUIDFileName(const AString & a_UUID)
{
	ASSERT(a_UUID.size() == 36);
	
	AString res("players/");
	res.append(a_UUID, 0, 2);
	res.push_back('/');
	res.append(a_UUID, 2, AString::npos);
	res.append(".json");
	return res;
}




