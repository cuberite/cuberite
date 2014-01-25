
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Player.h"
#include "../Server.h"
#include "../ClientHandle.h"
#include "../UI/Window.h"
#include "../UI/WindowOwner.h"
#include "../World.h"
#include "../Bindings/PluginManager.h"
#include "../BlockEntities/BlockEntity.h"
#include "../GroupManager.h"
#include "../Group.h"
#include "../ChatColor.h"
#include "../Item.h"
#include "../Tracer.h"
#include "../Root.h"
#include "../OSSupport/Timer.h"
#include "../MersenneTwister.h"
#include "../Chunk.h"
#include "../Items/ItemHandler.h"

#include "../Vector3d.h"
#include "../Vector3f.h"

#include "inifile/iniFile.h"
#include "json/json.h"






cPlayer::cPlayer(cClientHandle* a_Client, const AString & a_PlayerName)
	: super(etPlayer, 0.6, 1.8)
	, m_bVisible(true)
	, m_FoodLevel(MAX_FOOD_LEVEL)
	, m_FoodSaturationLevel(5)
	, m_FoodTickTimer(0)
	, m_FoodExhaustionLevel(0)
	, m_FoodPoisonedTicksRemaining(0)
	, m_LastJumpHeight(0)
	, m_LastGroundHeight(0)
	, m_bTouchGround(false)
	, m_Stance(0.0)
	, m_Inventory(*this)
	, m_CurrentWindow(NULL)
	, m_InventoryWindow(NULL)
	, m_TimeLastPickupCheck(0.f)
	, m_Color('-')
	, m_LastBlockActionTime(0)
	, m_LastBlockActionCnt(0)
	, m_GameMode(eGameMode_NotSet)
	, m_IP("")
	, m_ClientHandle(a_Client)
	, m_NormalMaxSpeed(0.1)
	, m_SprintingMaxSpeed(0.13)
	, m_IsCrouched(false)
	, m_IsSprinting(false)
	, m_IsFlying(false)
	, m_IsSwimming(false)
	, m_IsSubmerged(false)
	, m_IsFishing(false)
	, m_CanFly(false)
	, m_EatingFinishTick(-1)
	, m_LifetimeTotalXp(0)
	, m_CurrentXp(0)
	, m_bDirtyExperience(false)
	, m_IsChargingBow(false)
	, m_BowCharge(0)
	, m_FloaterID(-1)
	, m_Team(NULL)
{
	LOGD("Created a player object for \"%s\" @ \"%s\" at %p, ID %d", 
		a_PlayerName.c_str(), a_Client->GetIPString().c_str(),
		this, GetUniqueID()
	);
	
	m_InventoryWindow = new cInventoryWindow(*this);
	m_CurrentWindow = m_InventoryWindow;
	m_InventoryWindow->OpenedByPlayer(*this);

	SetMaxHealth(MAX_HEALTH);
	m_Health = MAX_HEALTH;
	
	cTimer t1;
	m_LastPlayerListTime = t1.GetNowTime();

	m_TimeLastTeleportPacket = 0;
	m_TimeLastPickupCheck = 0;
	
	m_PlayerName = a_PlayerName;
	m_bDirtyPosition = true; // So chunks are streamed to player at spawn

	if (!LoadFromDisk())
	{
		m_Inventory.Clear();
		SetPosX(cRoot::Get()->GetDefaultWorld()->GetSpawnX());
		SetPosY(cRoot::Get()->GetDefaultWorld()->GetSpawnY());
		SetPosZ(cRoot::Get()->GetDefaultWorld()->GetSpawnZ());
		
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
	LOGD("Deleting cPlayer \"%s\" at %p, ID %d", m_PlayerName.c_str(), this, GetUniqueID());
	
	// Notify the server that the player is being destroyed
	cRoot::Get()->GetServer()->PlayerDestroying(this);

	SaveToDisk();

	m_World->RemovePlayer( this );

	m_ClientHandle = NULL;
	
	delete m_InventoryWindow;
	
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
	a_Client.SendEntityEquipment(*this, 0, m_Inventory.GetEquippedItem() );
	a_Client.SendEntityEquipment(*this, 1, m_Inventory.GetEquippedBoots() );
	a_Client.SendEntityEquipment(*this, 2, m_Inventory.GetEquippedLeggings() );
	a_Client.SendEntityEquipment(*this, 3, m_Inventory.GetEquippedChestplate() );
	a_Client.SendEntityEquipment(*this, 4, m_Inventory.GetEquippedHelmet() );
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
	
	//handle updating experience
	if (m_bDirtyExperience)
	{
		SendExperience();
	}

	if (m_bDirtyPosition)
	{
		// Apply food exhaustion from movement:
		ApplyFoodExhaustionFromMovement();
		
		cRoot::Get()->GetPluginManager()->CallHookPlayerMoving(*this);
		BroadcastMovementUpdate(m_ClientHandle);
		m_ClientHandle->StreamChunks();
	}
	else
	{
		BroadcastMovementUpdate(m_ClientHandle);
	}

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

	// Send Player List (Once per m_LastPlayerListTime/1000 ms)
	cTimer t1;
	if (m_LastPlayerListTime + cPlayer::PLAYER_LIST_TIME_MS <= t1.GetNowTime())
	{
		m_World->SendPlayerList(this);
		m_LastPlayerListTime = t1.GetNowTime();
	}

	if (IsFlying())
	{
		m_LastGroundHeight = (float)GetPosY();
	}
}





short cPlayer::CalcLevelFromXp(short a_XpTotal)
{
	//level 0 to 15
	if(a_XpTotal <= XP_TO_LEVEL15)
	{
		return a_XpTotal / XP_PER_LEVEL_TO15;
	}

	//level 30+
	if(a_XpTotal > XP_TO_LEVEL30)
	{
		return (short) (151.5 + sqrt( 22952.25 - (14 * (2220 - a_XpTotal)))) / 7;
	}

	//level 16 to 30
	return (short) ( 29.5 + sqrt( 870.25 - (6 * ( 360 - a_XpTotal )))) / 3;
}





short cPlayer::XpForLevel(short a_Level)
{
	//level 0 to 15
	if(a_Level <= 15)
	{
		return a_Level * XP_PER_LEVEL_TO15;
	}

	//level 30+
	if(a_Level >= 31)
	{
		return (short) ( (3.5 * a_Level * a_Level) - (151.5 * a_Level) + 2220 );
	}

	//level 16 to 30
	return (short) ( (1.5 * a_Level * a_Level) - (29.5 * a_Level) + 360 );
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
	if(!(a_CurrentXp >= 0) || (a_CurrentXp > (SHRT_MAX - m_LifetimeTotalXp)))
	{
		LOGWARNING("Tried to update experiece with an invalid Xp value: %d", a_CurrentXp);
		return false; //oops, they gave us a dodgey number
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
		return -1; // Should we instead just return the current Xp?
	}

	m_CurrentXp += a_Xp_delta;

	// Make sure they didn't subtract too much
	if (m_CurrentXp < 0)
	{
		m_CurrentXp = 0;
	}

	// Update total for score calculation
	if (a_Xp_delta > 0)
	{
		m_LifetimeTotalXp += a_Xp_delta;
	}

	LOGD("Player \"%s\" gained/lost %d experience, total is now: %d", 
		m_PlayerName.c_str(), a_Xp_delta, m_CurrentXp);

	// Set experience to be updated
	m_bDirtyExperience = true;

	return m_CurrentXp;
}





void cPlayer::StartChargingBow(void)
{
	LOGD("Player \"%s\" started charging their bow", m_PlayerName.c_str());
	m_IsChargingBow = true;
	m_BowCharge = 0;
}





int cPlayer::FinishChargingBow(void)
{
	LOGD("Player \"%s\" finished charging their bow at a charge of %d", m_PlayerName.c_str(), m_BowCharge);
	int res = m_BowCharge;
	m_IsChargingBow = false;
	m_BowCharge = 0;
	return res;
}





void cPlayer::CancelChargingBow(void)
{
	LOGD("Player \"%s\" cancelled charging their bow at a charge of %d", m_PlayerName.c_str(), m_BowCharge);
	m_IsChargingBow = false;
	m_BowCharge = 0;
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
			BLOCKTYPE BlockType = World->GetBlock((int)floor(GetPosX()), (int)floor(GetPosY()), (int)floor(GetPosZ()));
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
		int Damage = (int)(Dist - 3.f);
		if (m_LastJumpHeight > m_LastGroundHeight) Damage++;
		m_LastJumpHeight = (float)GetPosY();

		if (Damage > 0)
		{
			if (!IsGameModeCreative())
			{
				TakeDamage(dtFalling, NULL, Damage, Damage, 0);
			}
			
			// Fall particles
			GetWorld()->BroadcastSoundParticleEffect(2006, (int)floor(GetPosX()), (int)GetPosY() - 1, (int)floor(GetPosZ()), Damage /* Used as particle effect speed modifier */);
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
	m_FoodLevel = std::max(0, std::min(a_FoodLevel, (int)MAX_FOOD_LEVEL));
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





void cPlayer::SetFoodPoisonedTicksRemaining(int a_FoodPoisonedTicksRemaining)
{
	m_FoodPoisonedTicksRemaining = a_FoodPoisonedTicksRemaining;
}





bool cPlayer::Feed(int a_Food, double a_Saturation)
{
	if (m_FoodLevel >= MAX_FOOD_LEVEL)
	{
		return false;
	}
	
	m_FoodLevel = std::min(a_Food + m_FoodLevel, (int)MAX_FOOD_LEVEL);
	m_FoodSaturationLevel = std::min(m_FoodSaturationLevel + a_Saturation, (double)m_FoodLevel);
	
	SendHealth();
	return true;
}





void cPlayer::FoodPoison(int a_NumTicks)
{
	bool HasBeenFoodPoisoned = (m_FoodPoisonedTicksRemaining > 0);
	m_FoodPoisonedTicksRemaining = std::max(m_FoodPoisonedTicksRemaining, a_NumTicks);
	if (!HasBeenFoodPoisoned)
	{
		m_World->BroadcastRemoveEntityEffect(*this, E_EFFECT_HUNGER);
		SendHealth();
	}
	else
	{
		m_World->BroadcastEntityEffect(*this, E_EFFECT_HUNGER, 0, 400); // Give the player the "Hunger" effect for 20 seconds.
	}
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
	m_ClientHandle->SendEntityStatus(*this, ENTITY_STATUS_EATING_ACCEPTED);
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

	//if the food is mushroom soup, return a bowl to the inventory
	if( Item.m_ItemType == E_ITEM_MUSHROOM_SOUP ) {
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
	return m_IsSprinting ? m_SprintingMaxSpeed : m_NormalMaxSpeed;
}





void cPlayer::SetNormalMaxSpeed(double a_Speed)
{
	m_NormalMaxSpeed = a_Speed;
	if (!m_IsSprinting)
	{
		m_ClientHandle->SendPlayerMaxSpeed();
	}
}





void cPlayer::SetSprintingMaxSpeed(double a_Speed)
{
	m_SprintingMaxSpeed = a_Speed;
	if (m_IsSprinting)
	{
		m_ClientHandle->SendPlayerMaxSpeed();
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





void cPlayer::DoTakeDamage(TakeDamageInfo & a_TDI)
{
	if (a_TDI.DamageType != dtInVoid)
	{
		if (IsGameModeCreative())
		{
			// No damage / health in creative mode
			return;
		}
	}

	if ((a_TDI.Attacker != NULL) && (a_TDI.Attacker->IsPlayer()))
	{
		cPlayer* Attacker = (cPlayer*) a_TDI.Attacker;

		if ((m_Team != NULL) && (m_Team == Attacker->m_Team))
		{
			if (!m_Team->AllowsFriendlyFire())
			{
				// Friendly fire is disabled
				return;
			}
		}
	}
	
	super::DoTakeDamage(a_TDI);
	
	// Any kind of damage adds food exhaustion
	AddFoodExhaustion(0.3f);
	
	SendHealth();
}





void cPlayer::KilledBy(cEntity * a_Killer)
{
	super::KilledBy(a_Killer);

	if (m_Health > 0)
	{
		return; //  not dead yet =]
	}

	m_bVisible = false; // So new clients don't see the player

	// Puke out all the items
	cItems Pickups;
	m_Inventory.CopyToItems(Pickups);
	m_Inventory.Clear();
	m_World->SpawnItemPickups(Pickups, GetPosX(), GetPosY(), GetPosZ(), 10);
	SaveToDisk();  // Save it, yeah the world is a tough place !

	if (a_Killer == NULL)
	{
		GetWorld()->BroadcastChat(Printf("%s[DEATH] %s%s was killed by environmental damage", cChatColor::Red.c_str(), cChatColor::White.c_str(), GetName().c_str()));
	}
	else if (a_Killer->IsPlayer())
	{
		GetWorld()->BroadcastChat(Printf("%s[DEATH] %s%s was killed by %s", cChatColor::Red.c_str(), cChatColor::White.c_str(), GetName().c_str(), ((cPlayer *)a_Killer)->GetName().c_str()));
	}
	else
	{
		AString KillerClass = a_Killer->GetClass();
		KillerClass.erase(KillerClass.begin()); // Erase the 'c' of the class (e.g. "cWitch" -> "Witch")

		GetWorld()->BroadcastChat(Printf("%s[DEATH] %s%s was killed by a %s", cChatColor::Red.c_str(), cChatColor::White.c_str(), GetName().c_str(), KillerClass.c_str()));
	}

	class cIncrementCounterCB
		: public cObjectiveCallback
	{
		AString m_Name;
	public:
		cIncrementCounterCB(const AString & a_Name) : m_Name(a_Name) {}

		virtual bool Item(cObjective * a_Objective) override
		{
			a_Objective->AddScore(m_Name, 1);
			return true;
		}
	} IncrementCounter (GetName());

	cScoreboard & Scoreboard = m_World->GetScoreBoard();

	// Update scoreboard objectives
	Scoreboard.ForEachObjectiveWith(cObjective::E_TYPE_DEATH_COUNT, IncrementCounter);
}





void cPlayer::Respawn(void)
{
	m_Health = GetMaxHealth();
	
	// Reset food level:
	m_FoodLevel = MAX_FOOD_LEVEL;
	m_FoodSaturationLevel = 5;

	// Reset Experience
	m_CurrentXp = 0;
	m_LifetimeTotalXp = 0;
	// ToDo: send score to client? How?

	m_ClientHandle->SendRespawn();
	
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
	return Vector3d( GetPosX(), m_Stance, GetPosZ() );
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





void cPlayer::SetLastBlockActionTime()
{
	if (m_World != NULL)
	{
		m_LastBlockActionTime = m_World->GetWorldAge() / 20.0f;
	}
}





void cPlayer::SetLastBlockActionCnt( int a_LastBlockActionCnt )
{
	m_LastBlockActionCnt = a_LastBlockActionCnt;
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





void cPlayer::LoginSetGameMode( eGameMode a_GameMode )
{
	m_GameMode = a_GameMode;
}





void cPlayer::SetIP(const AString & a_IP)
{
	m_IP = a_IP;
}





void cPlayer::SendMessage(const AString & a_Message)
{
	m_ClientHandle->SendChat(a_Message);
}





void cPlayer::TeleportToCoords(double a_PosX, double a_PosY, double a_PosZ)
{
	SetPosition( a_PosX, a_PosY, a_PosZ );
	m_LastGroundHeight = (float)a_PosY;

	m_World->BroadcastTeleportEntity(*this, GetClientHandle());
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





void cPlayer::ForceSetSpeed(Vector3d a_Direction)
{
	SetSpeed(a_Direction);
	m_ClientHandle->SendEntityVelocity(*this);
}





void cPlayer::MoveTo( const Vector3d & a_NewPos )
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
	
	SetPosition( a_NewPos );
	SetStance(a_NewPos.y + 1.62);
}





void cPlayer::SetVisible(bool a_bVisible)
{
	if (a_bVisible && !m_bVisible) // Make visible
	{
		m_bVisible = true;
		m_World->BroadcastSpawnEntity(*this);
	}
	if (!a_bVisible && m_bVisible)
	{
		m_bVisible = false;
		m_World->BroadcastDestroyEntity(*this, m_ClientHandle);	// Destroy on all clients
	}
}





void cPlayer::AddToGroup( const AString & a_GroupName )
{
	cGroup* Group = cRoot::Get()->GetGroupManager()->GetGroup( a_GroupName );
	m_Groups.push_back( Group );
	LOGD("Added %s to group %s", m_PlayerName.c_str(), a_GroupName.c_str() );
	ResolveGroups();
	ResolvePermissions();
}





void cPlayer::RemoveFromGroup( const AString & a_GroupName )
{
	bool bRemoved = false;
	for( GroupList::iterator itr = m_Groups.begin(); itr != m_Groups.end(); ++itr )
	{
		if( (*itr)->GetName().compare(a_GroupName ) == 0 )
		{
			m_Groups.erase( itr );
			bRemoved = true;
			break;
		}
	}

	if( bRemoved )
	{
		LOGD("Removed %s from group %s", m_PlayerName.c_str(), a_GroupName.c_str() );
		ResolveGroups();
		ResolvePermissions();
	}
	else
	{
		LOGWARN("Tried to remove %s from group %s but was not in that group", m_PlayerName.c_str(), a_GroupName.c_str() );
	}
}





bool cPlayer::CanUseCommand( const AString & a_Command )
{
	for( GroupList::iterator itr = m_Groups.begin(); itr != m_Groups.end(); ++itr )
	{
		if( (*itr)->HasCommand( a_Command ) ) return true;
	}
	return false;
}





bool cPlayer::HasPermission(const AString & a_Permission)
{
	if (a_Permission.empty())
	{
		// Empty permission request is always granted
		return true;
	}
	
	AStringVector Split = StringSplit( a_Permission, "." );
	PermissionMap Possibilities = m_ResolvedPermissions;
	// Now search the namespaces
	while( Possibilities.begin() != Possibilities.end() )
	{
		PermissionMap::iterator itr = Possibilities.begin();
		if( itr->second )
		{
			AStringVector OtherSplit = StringSplit( itr->first, "." );
			if( OtherSplit.size() <= Split.size() )
			{
				unsigned int i;
				for( i = 0; i < OtherSplit.size(); ++i )
				{
					if( OtherSplit[i].compare( Split[i] ) != 0 )
					{
						if( OtherSplit[i].compare("*") == 0 ) return true; // WildCard man!! WildCard!
						break;
					}
				}
				if( i == Split.size() ) return true;
			}
		}
		Possibilities.erase( itr );
	}

	// Nothing that matched :(
	return false;
}





bool cPlayer::IsInGroup( const AString & a_Group )
{
	for( GroupList::iterator itr = m_ResolvedGroups.begin(); itr != m_ResolvedGroups.end(); ++itr )
	{
		if( a_Group.compare( (*itr)->GetName().c_str() ) == 0 )
			return true;
	}
	return false;
}





void cPlayer::ResolvePermissions()
{
	m_ResolvedPermissions.clear();	// Start with an empty map yo~

	// Copy all player specific permissions into the resolved permissions map
	for( PermissionMap::iterator itr = m_Permissions.begin(); itr != m_Permissions.end(); ++itr )
	{
		m_ResolvedPermissions[ itr->first ] = itr->second;
	}

	for( GroupList::iterator GroupItr = m_ResolvedGroups.begin(); GroupItr != m_ResolvedGroups.end(); ++GroupItr )
	{
		const cGroup::PermissionMap & Permissions = (*GroupItr)->GetPermissions();
		for( cGroup::PermissionMap::const_iterator itr = Permissions.begin(); itr != Permissions.end(); ++itr )
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
	std::map< cGroup*, bool > AllGroups;	// Use a map, because it's faster than iterating through a list to find duplicates
	GroupList ToIterate;
	for( GroupList::iterator GroupItr = m_Groups.begin(); GroupItr != m_Groups.end(); ++GroupItr )
	{
		ToIterate.push_back( *GroupItr );
	}
	while( ToIterate.begin() != ToIterate.end() )
	{
		cGroup* CurrentGroup = *ToIterate.begin();
		if( AllGroups.find( CurrentGroup ) != AllGroups.end() )
		{
			LOGWARNING("ERROR: Player \"%s\" is in the group multiple times (\"%s\"). Please fix your settings in users.ini!",
				m_PlayerName.c_str(), CurrentGroup->GetName().c_str()
			);
		}
		else
		{
			AllGroups[ CurrentGroup ] = true;
			m_ResolvedGroups.push_back( CurrentGroup );	// Add group to resolved list
			const cGroup::GroupList & Inherits = CurrentGroup->GetInherits();
			for( cGroup::GroupList::const_iterator itr = Inherits.begin(); itr != Inherits.end(); ++itr )
			{
				if( AllGroups.find( *itr ) != AllGroups.end() )
				{
					LOGERROR("ERROR: Player %s is in the same group multiple times due to inheritance (%s). FIX IT!", m_PlayerName.c_str(), (*itr)->GetName().c_str() );
					continue;
				}
				ToIterate.push_back( *itr );
			}
		}
		ToIterate.erase( ToIterate.begin() );
	}
}





AString cPlayer::GetColor(void) const
{
	if ( m_Color != '-' )
	{
		return cChatColor::Color + m_Color;
	}

	if ( m_Groups.size() < 1 )
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
			NewAmount = GetInventory().GetEquippedItem().m_ItemCount; // Drop only what's there
		}

		GetInventory().GetHotbarGrid().ChangeSlotCount(GetInventory().GetEquippedSlotNum() /* Returns hotbar subslot, which HotbarGrid takes */, -a_Amount);

		DroppedItem.m_ItemCount = NewAmount;
		Drops.push_back(DroppedItem);
	}

	double vX = 0, vY = 0, vZ = 0;
	EulerToVector(-GetYaw(), GetPitch(), vZ, vX, vY);
	vY = -vY * 2 + 1.f;
	m_World->SpawnItemPickups(Drops, GetPosX(), GetEyeHeight(), GetPosZ(), vX * 3, vY * 3, vZ * 3, true); // 'true' because created by player
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

	double vX = 0, vY = 0, vZ = 0;
	EulerToVector(-GetYaw(), GetPitch(), vZ, vX, vY);
	vY = -vY * 2 + 1.f;
	m_World->SpawnItemPickups(Drops, GetPosX(), GetEyeHeight(), GetPosZ(), vX * 3, vY * 3, vZ * 3, true); // 'true' because created by player
}





void cPlayer::TossPickup(const cItem & a_Item)
{
	cItems Drops;
	Drops.push_back(a_Item);

	double vX = 0, vY = 0, vZ = 0;
	EulerToVector(-GetYaw(), GetPitch(), vZ, vX, vY);
	vY = -vY * 2 + 1.f;
	m_World->SpawnItemPickups(Drops, GetPosX(), GetEyeHeight(), GetPosZ(), vX * 3, vY * 3, vZ * 3, true); // 'true' because created by player
}





bool cPlayer::MoveToWorld(const char * a_WorldName)
{
	cWorld * World = cRoot::Get()->GetWorld(a_WorldName);
	if (World == NULL)
	{
		LOG("%s: Couldn't find world \"%s\".", __FUNCTION__, a_WorldName);
		return false;
	}
	
	eDimension OldDimension = m_World->GetDimension();
	
	// Remove all links to the old world
	m_World->RemovePlayer(this);
	m_ClientHandle->RemoveFromAllChunks();
	m_World->RemoveEntity(this);

	// If the dimension is different, we can send the respawn packet
	// http://wiki.vg/Protocol#0x09 says "don't send if dimension is the same" as of 2013_07_02
	m_ClientHandle->MoveToWorld(*World, (OldDimension != World->GetDimension()));

	// Add player to all the necessary parts of the new world
	SetWorld(World);
	World->AddEntity(this);
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
		std::string Groups = IniFile.GetValue(m_PlayerName, "Groups", "");
		if (!Groups.empty())
		{
			AStringVector Split = StringSplit( Groups, "," );
			for( unsigned int i = 0; i < Split.size(); i++ )
			{
				AddToGroup( Split[i].c_str() );
			}
		}
		else
		{
			AddToGroup("Default");
		}

		m_Color = IniFile.GetValue(m_PlayerName, "Color", "-")[0];
	}
	else
	{
		LOGWARN("Regenerating users.ini, player %s will be added to the \"Default\" group", m_PlayerName.c_str());
		IniFile.AddHeaderComment(" This is the file in which the group the player belongs to is stored");
		IniFile.AddHeaderComment(" The format is: [PlayerName] | Groups=GroupName");

		IniFile.SetValue(m_PlayerName, "Groups", "Default");
		IniFile.WriteFile("users.ini");
		AddToGroup("Default");
	}
	ResolvePermissions();
}




bool cPlayer::LoadFromDisk()
{
	LoadPermissionsFromDisk();

	// Log player permissions, cause it's what the cool kids do
	LOGINFO("Player %s has permissions:", m_PlayerName.c_str() );
	for( PermissionMap::iterator itr = m_ResolvedPermissions.begin(); itr != m_ResolvedPermissions.end(); ++itr )
	{
		if( itr->second ) LOG(" - %s", itr->first.c_str() );
	}

	AString SourceFile;
	Printf(SourceFile, "players/%s.json", m_PlayerName.c_str() );

	cFile f;
	if (!f.Open(SourceFile, cFile::fmRead))
	{
		// This is a new player whom we haven't seen yet, bail out, let them have the defaults
		return false;
	}

	AString buffer;
	if (f.ReadRestOfFile(buffer) != f.GetSize())
	{
		LOGWARNING("Cannot read player data from file \"%s\"", SourceFile.c_str()); 
		return false;
	}
	f.Close(); //cool kids play nice

	Json::Value root;
	Json::Reader reader;
	if (!reader.parse(buffer, root, false))
	{
		LOGWARNING("Cannot parse player data in file \"%s\", player will be reset", SourceFile.c_str());
	}

	Json::Value & JSON_PlayerPosition = root["position"];
	if (JSON_PlayerPosition.size() == 3)
	{
		SetPosX(JSON_PlayerPosition[(unsigned int)0].asDouble());
		SetPosY(JSON_PlayerPosition[(unsigned int)1].asDouble());
		SetPosZ(JSON_PlayerPosition[(unsigned int)2].asDouble());
		m_LastPosX = GetPosX();
		m_LastPosY = GetPosY();
		m_LastPosZ = GetPosZ();
		m_LastFoodPos = GetPosition();
	}

	Json::Value & JSON_PlayerRotation = root["rotation"];
	if (JSON_PlayerRotation.size() == 3)
	{
		SetYaw      ((float)JSON_PlayerRotation[(unsigned int)0].asDouble());
		SetPitch    ((float)JSON_PlayerRotation[(unsigned int)1].asDouble());
		SetRoll     ((float)JSON_PlayerRotation[(unsigned int)2].asDouble());
	}

	m_Health              = root.get("health", 0).asInt();
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

	m_LoadedWorldName = root.get("world", "world").asString();
	
	LOGD("Player \"%s\" was read from file, spawning at {%.2f, %.2f, %.2f} in world \"%s\"",
		m_PlayerName.c_str(), GetPosX(), GetPosY(), GetPosZ(), m_LoadedWorldName.c_str()
	);
	
	return true;
}





bool cPlayer::SaveToDisk()
{
	cFile::CreateFolder(FILE_IO_PREFIX + AString("players"));

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

	Json::Value root;
	root["position"]       = JSON_PlayerPosition;
	root["rotation"]       = JSON_PlayerRotation;
	root["inventory"]      = JSON_Inventory;
	root["health"]         = m_Health;
	root["xpTotal"]        = m_LifetimeTotalXp;
	root["xpCurrent"]      = m_CurrentXp;
	root["air"]            = m_AirLevel;
	root["food"]           = m_FoodLevel;
	root["foodSaturation"] = m_FoodSaturationLevel;
	root["foodTickTimer"]  = m_FoodTickTimer;
	root["foodExhaustion"] = m_FoodExhaustionLevel;
	root["world"]          = GetWorld()->GetName();
	root["isflying"]       = IsFlying();

	if (m_GameMode == GetWorld()->GetGameMode())
	{
		root["gamemode"] = (int) eGameMode_NotSet;
	}
	else
	{
		root["gamemode"] = (int) m_GameMode;
	}

	Json::StyledWriter writer;
	std::string JsonData = writer.write(root);

	AString SourceFile;
	Printf(SourceFile, "players/%s.json", m_PlayerName.c_str() );

	cFile f;
	if (!f.Open(SourceFile, cFile::fmWrite))
	{
		LOGERROR("ERROR WRITING PLAYER \"%s\" TO FILE \"%s\" - cannot open file", m_PlayerName.c_str(), SourceFile.c_str());
		return false;
	}
	if (f.Write(JsonData.c_str(), JsonData.size()) != (int)JsonData.size())
	{
		LOGERROR("ERROR WRITING PLAYER JSON TO FILE \"%s\"", SourceFile.c_str()); 
		return false;
	}
	return true;
}





cPlayer::StringList cPlayer::GetResolvedPermissions()
{
	StringList Permissions;

	const PermissionMap& ResolvedPermissions = m_ResolvedPermissions;
	for( PermissionMap::const_iterator itr = ResolvedPermissions.begin(); itr != ResolvedPermissions.end(); ++itr )
	{
		if( itr->second ) Permissions.push_back( itr->first );
	}

	return Permissions;
}





void cPlayer::UseEquippedItem(void)
{
	if (IsGameModeCreative()) // No damage in creative
	{
		return;
	}

	if (GetInventory().DamageEquippedItem())
	{
		m_World->BroadcastSoundEffect("random.break", (int)GetPosX() * 8, (int)GetPosY() * 8, (int)GetPosZ() * 8, 0.5f, (float)(0.75 + ((float)((GetUniqueID() * 23) % 32)) / 64));
	}
}





void cPlayer::HandleFood(void)
{
	// Ref.: http://www.minecraftwiki.net/wiki/Hunger
	
	// Remember the food level before processing, for later comparison
	int LastFoodLevel = m_FoodLevel;
	
	// Heal or damage, based on the food level, using the m_FoodTickTimer:
	if ((m_FoodLevel > 17) || (m_FoodLevel <= 0))
	{
		m_FoodTickTimer++;
		if (m_FoodTickTimer >= 80)
		{
			m_FoodTickTimer = 0;

			if (m_FoodLevel >= 17)
			{
				// Regenerate health from food, incur 3 pts of food exhaustion:
				Heal(1);
				m_FoodExhaustionLevel += 3;
			}
			else if (m_FoodLevel <= 0)
			{
				// Damage from starving
				TakeDamage(dtStarving, NULL, 1, 1, 0);
			}
		}
	}
	
	// Apply food poisoning food exhaustion:
	if (m_FoodPoisonedTicksRemaining > 0)
	{
		m_FoodPoisonedTicksRemaining--;
		m_FoodExhaustionLevel += 0.025;  // 0.5 per second = 0.025 per tick
	}
	else
	{
		m_World->BroadcastRemoveEntityEffect(*this, E_EFFECT_HUNGER); // Remove the "Hunger" effect.
	}

	// Apply food exhaustion that has accumulated:
	if (m_FoodExhaustionLevel >= 4)
	{
		m_FoodExhaustionLevel -= 4;

		if (m_FoodSaturationLevel >= 1)
		{
			m_FoodSaturationLevel -= 1;
		}
		else
		{
			m_FoodLevel = std::max(m_FoodLevel - 1, 0);
		}
	}
	
	if (m_FoodLevel != LastFoodLevel)
	{
		SendHealth();
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





void cPlayer::ApplyFoodExhaustionFromMovement()
{
	if (IsGameModeCreative())
	{
		return;
	}
	
	// Calculate the distance travelled, update the last pos:
	Vector3d Movement(GetPosition() - m_LastFoodPos);
	Movement.y = 0;  // Only take XZ movement into account
	m_LastFoodPos = GetPosition();
	
	// If riding anything, apply no food exhaustion
	if (m_AttachedTo != NULL)
	{
		return;
	}

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
	int PosX = (int)floor(GetPosX());
	int PosY = (int)floor(GetPosY());
	int PosZ = (int)floor(GetPosZ());

	// Search for a position within an area to teleport player after detachment
	// Position must be solid land, and occupied by a nonsolid block
	// If nothing found, player remains where they are
	for (int x = PosX - 2; x <= (PosX + 2); ++x)
	{
		for (int y = PosY; y <= (PosY + 3); ++y)
		{
			for (int z = PosZ - 2; z <= (PosZ + 2); ++z)
			{
				if (!g_BlockIsSolid[m_World->GetBlock(x, y, z)] && g_BlockIsSolid[m_World->GetBlock(x, y - 1, z)])
				{
					TeleportToCoords(x, y, z);
					return;
				}
			}
		}
	}
}




