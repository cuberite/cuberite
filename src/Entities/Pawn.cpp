
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Pawn.h"
#include "Player.h"
#include "../BlockInfo.h"
#include "../World.h"
#include "../Bindings/PluginManager.h"
#include "../BoundingBox.h"
#include "../Blocks/BlockHandler.h"
#include "../Blocks/BlockFarmland.h"
#include "../EffectID.h"
#include "../Mobs/Monster.h"
#include "../Protocol/Palettes/Upgrade.h"




cPawn::cPawn(eEntityType a_EntityType, float a_Width, float a_Height) :
	Super(a_EntityType, Vector3d(), a_Width, a_Height),
	m_EntityEffects(tEffectMap()),
	m_LastGroundHeight(0),
	m_bTouchGround(false)
{
	SetGravity(-32.0f);
	SetAirDrag(0.02f);
}





void cPawn::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	std::vector<cEntityEffect *> EffectsToTick;

	// Iterate through this entity's applied effects
	for (tEffectMap::iterator iter = m_EntityEffects.begin(); iter != m_EntityEffects.end();)
	{
		// Copies values to prevent pesky wrong accesses and erasures
		cEntityEffect::eType EffectType = iter->first;
		cEntityEffect * Effect = iter->second.get();

		// Iterates (must be called before any possible erasure)
		++iter;

		// Remove effect if duration has elapsed
		if (Effect->GetDuration() - Effect->GetTicks() <= 0)
		{
			RemoveEntityEffect(EffectType);
		}
		// Call OnTick later to make sure the iterator won't be invalid
		else
		{
			EffectsToTick.push_back(Effect);
		}

		// TODO: Check for discrepancies between client and server effect values
	}


	for (auto * Effect : EffectsToTick)
	{
		Effect->OnTick(*this);
	}

	// Spectators cannot push entities around
	if ((!IsPlayer()) || (!static_cast<cPlayer *>(this)->IsGameModeSpectator()))
	{
		m_World->ForEachEntityInBox(GetBoundingBox(), [=](cEntity & a_Entity)
			{
				if (a_Entity.GetUniqueID() == GetUniqueID())
				{
					return false;
				}

				// we only push other mobs, boats and minecarts
				if ((a_Entity.GetEntityType() != etMonster) && (a_Entity.GetEntityType() != etMinecart) && (a_Entity.GetEntityType() != etBoat))
				{
					return false;
				}

				// do not push a boat / minecart you're sitting in
				if (IsAttachedTo(&a_Entity))
				{
					return false;
				}

				Vector3d v3Delta = a_Entity.GetPosition() - GetPosition();
				v3Delta.y = 0.0;  // we only push sideways
				v3Delta *= 1.0 / (v3Delta.Length() + 0.01);  // we push harder if we're close
				// QUESTION: is there an additional multiplier for this? current shoving seems a bit weak

				a_Entity.AddSpeed(v3Delta);
				return false;
			}
		);
	}

	Super::Tick(a_Dt, a_Chunk);
	if (!IsTicking())
	{
		// The base class tick destroyed us
		return;
	}
	HandleFalling();

	// Handle item pickup
	if (m_Health > 0)
	{
		if (IsPlayer())
		{
			m_World->CollectPickupsByEntity(*this);
		}
		else if (IsMob())
		{
			cMonster & Mob = static_cast<cMonster &>(*this);
			if (Mob.CanPickUpLoot())
			{
				m_World->CollectPickupsByEntity(*this);
			}
		}
	}
}





void cPawn::KilledBy(TakeDamageInfo & a_TDI)
{
	ClearEntityEffects();

	// Is death eligible for totem reanimation?
	if (DeductTotem(a_TDI.DamageType))
	{
		m_World->BroadcastEntityAnimation(*this, EntityAnimation::PawnTotemActivates);

		AddEntityEffect(cEntityEffect::effAbsorption, 100, 1);
		AddEntityEffect(cEntityEffect::effRegeneration, 900, 1);
		AddEntityEffect(cEntityEffect::effFireResistance, 800, 0);

		m_Health = 1;
		return;
	}

	Super::KilledBy(a_TDI);
}





bool cPawn::IsFireproof(void) const
{
	return Super::IsFireproof() || HasEntityEffect(cEntityEffect::effFireResistance);
}





bool cPawn::IsInvisible() const
{
	return HasEntityEffect(cEntityEffect::effInvisibility);
}





void cPawn::HandleAir(void)
{
	if (IsHeadInWater() && HasEntityEffect(cEntityEffect::effWaterBreathing))
	{
		// Prevent the oxygen from decreasing
		return;
	}

	Super::HandleAir();
}





void cPawn::AddEntityEffect(cEntityEffect::eType a_EffectType, int a_Duration, short a_Intensity, double a_DistanceModifier)
{
	// Check if the plugins allow the addition:
	if (cPluginManager::Get()->CallHookEntityAddEffect(*this, a_EffectType, a_Duration, a_Intensity, a_DistanceModifier))
	{
		// A plugin disallows the addition, bail out.
		return;
	}

	// No need to add empty effects:
	if (a_EffectType == cEntityEffect::effNoEffect)
	{
		return;
	}
	a_Duration = static_cast<int>(a_Duration * a_DistanceModifier);

	// If we already have the effect, we have to deactivate it or else it will act cumulatively
	auto ExistingEffect = m_EntityEffects.find(a_EffectType);
	if (ExistingEffect != m_EntityEffects.end())
	{
		ExistingEffect->second->OnDeactivate(*this);
	}

	auto Res = m_EntityEffects.emplace(a_EffectType, cEntityEffect::CreateEntityEffect(a_EffectType, a_Duration, a_Intensity, a_DistanceModifier));
	m_World->BroadcastEntityEffect(*this, a_EffectType, a_Intensity, a_Duration);
	cEntityEffect * Effect = Res.first->second.get();
	Effect->OnActivate(*this);
}





void cPawn::RemoveEntityEffect(cEntityEffect::eType a_EffectType)
{
	m_World->BroadcastRemoveEntityEffect(*this, a_EffectType);
	auto itr = m_EntityEffects.find(a_EffectType);
	if (itr != m_EntityEffects.end())
	{
		// Erase from effect map before calling OnDeactivate to allow metadata broadcasts (e.g. for invisibility effect)
		auto Effect = std::move(itr->second);
		m_EntityEffects.erase(itr);
		Effect->OnDeactivate(*this);
	}
}





bool cPawn::HasEntityEffect(cEntityEffect::eType a_EffectType) const
{
	return m_EntityEffects.find(a_EffectType) != m_EntityEffects.end();
}





void cPawn::ClearEntityEffects()
{
	// Iterate through this entity's applied effects
	for (tEffectMap::iterator iter = m_EntityEffects.begin(); iter != m_EntityEffects.end();)
	{
		// Copy values to prevent pesky wrong erasures
		cEntityEffect::eType EffectType = iter->first;

		// Iterates (must be called before any possible erasure)
		++iter;

		// Remove effect
		RemoveEntityEffect(EffectType);
	}
}





void cPawn::NoLongerTargetingMe(cMonster * a_Monster)
{
	ASSERT(IsTicking());  // Our destroy override is supposed to clear all targets before we're destroyed.
	for (auto i = m_TargetingMe.begin(); i != m_TargetingMe.end(); ++i)
	{
		cMonster * Monster = *i;
		if (Monster == a_Monster)
		{
			ASSERT(Monster->GetTarget() != this);  // The monster is notifying us it is no longer targeting us, assert if that's a lie
			m_TargetingMe.erase(i);
			return;
		}
	}
	ASSERT(false);  // If this happens, something is wrong. Perhaps the monster never called TargetingMe() or called NoLongerTargetingMe() twice.
}





void cPawn::TargetingMe(cMonster * a_Monster)
{
	ASSERT(IsTicking());
	ASSERT(m_TargetingMe.size() < 10000);
	ASSERT(a_Monster->GetTarget() == this);
	m_TargetingMe.push_back(a_Monster);
}





void cPawn::HandleFalling(void)
{
	/* Not pretty looking, and is more suited to wherever server-sided collision detection is implemented.
	The following condition sets on-ground-ness if
	The player isn't swimming or flying (client hardcoded conditions) and
	they're on a block (Y is exact) - ensure any they could be standing on (including on the edges) is solid or
	they're on a slab (Y significand is 0.5) - ditto with slab check
	they're on a snow layer (Y divisible by 0.125) - ditto with snow layer check
	*/

	static const auto HalfWidth = GetWidth() / 2;
	static const auto EPS = 0.0001;

	/* Since swimming is decided in a tick and is asynchronous to this, we have to check for dampeners ourselves.
	The behaviour as of 1.8.9 is the following:
	- Landing in water alleviates all fall damage
	- Passing through any liquid (water + lava) and cobwebs "slows" the player down,
	i.e. resets the fall distance to that block, but only after checking for fall damage
	(this means that plummeting into lava will still kill the player via fall damage, although cobwebs
	will slow players down enough to have multiple updates that keep them alive)
	- Slime blocks reverse falling velocity, unless it's a crouching player, in which case they act as standard blocks.
	They also reset the topmost point of the damage calculation with each bounce,
	so if the block is removed while the player is bouncing or crouches after a bounce, the last bounce's zenith is considered as fall damage.

	With this in mind, we first check the block at the player's feet, then the one below that (because fences),
	and decide which behaviour we want to go with.
	*/
	const auto BlockAtFoot = (cChunkDef::IsValidHeight(POS_TOINT)) ? GetWorld()->GetBlock(POS_TOINT) : Block::Air::Air();

	/* We initialize these with what the foot is really IN, because for sampling we will move down with the epsilon above */
	bool IsFootInWater = BlockAtFoot.Type() == BlockType::Water;
	bool IsFootOnSlimeBlock = false;

	/* The "cross" we sample around to account for the player width/girth */
	static const struct
	{
		int x, z;
	} CrossSampleCoords[] =
	{
		{ 0, 0 },
		{ 1, 0 },
		{ -1, 0 },
		{ 0, 1 },
		{ 0, -1 },
	};

	/* The blocks we're interested in relative to the player to account for larger than 1 blocks.
	This can be extended to do additional checks in case there are blocks that are represented as one block
	in memory but have a hitbox larger than 1 (like fences) */
	static const Vector3i BlockSampleOffsets[] =
	{
		{ 0, 0, 0 },  // TODO: something went wrong here (offset 0?)
		{ 0, -1, 0 },  // Potentially causes mis-detection (IsFootInWater) when player stands on block diagonal to water (i.e. on side of pool)
	};

	/* Here's the rough outline of how this mechanism works:
	We take the player's pointlike position (sole of feet), and expand it into a crosslike shape.
	If any of the five points hit a block, we consider the player to be "on" (or "in") the ground. */
	bool OnGround = false;
	for (size_t i = 0; i < ARRAYCOUNT(CrossSampleCoords); i++)
	{
		/* We calculate from the player's position, one of the cross-offsets above, and we move it down slightly so it's beyond inaccuracy.
		The added advantage of this method is that if the player is simply standing on the floor,
		the point will move into the next block, and the floor() will retrieve that instead of air. */
		Vector3d CrossTestPosition = GetPosition() + Vector3d(CrossSampleCoords[i].x * HalfWidth, -EPS, CrossSampleCoords[i].z * HalfWidth);

		/* We go through the blocks that we consider "relevant" */
		for (size_t j = 0; j < ARRAYCOUNT(BlockSampleOffsets); j++)
		{
			Vector3i BlockTestPosition = CrossTestPosition.Floor() + BlockSampleOffsets[j];

			if (!cChunkDef::IsValidHeight(BlockTestPosition))
			{
				continue;
			}

			auto TestBlock = GetWorld()->GetBlock(BlockTestPosition);

			/* we do the cross-shaped sampling to check for water / liquids, but only on our level because water blocks are never bigger than unit voxels */
			if (j == 0)
			{
				IsFootInWater |= TestBlock == BlockType::Water;
				IsFootOnSlimeBlock |= (TestBlock.Type() == BlockType::SlimeBlock);
			}

			/* If the block is solid, and the blockhandler confirms the block to be inside, we're officially on the ground. */
			if ((cBlockInfo::IsSolid(TestBlock)) && (cBlockHandler::For(TestBlock.Type()).IsInsideBlock(CrossTestPosition - BlockTestPosition, TestBlock)))
			{
				OnGround = true;
			}
		}
	}

	/* So here's the use of the rules above: */
	/* 1. Falling in water absorbs all fall damage */
	bool FallDamageAbsorbed = IsFootInWater;

	/* 2. Falling in liquid (lava, water, cobweb) or hitting a slime block resets the "fall zenith".
	Note: Even though the pawn bounces back with no damage after hitting the slime block,
	the "fall zenith" will continue to increase back up when flying upwards - which is good */
	bool ShouldBounceOnSlime = true;

	if (IsPlayer())
	{
		auto Player = static_cast<cPlayer *>(this);

		/* 3. If the player is flying or climbing, absorb fall damage */
		FallDamageAbsorbed |= Player->IsFlying() || Player->IsClimbing() || Player->GetIsTeleporting();

		/* 4. If the player is about to bounce on a slime block and is not crouching, absorb all fall damage  */
		ShouldBounceOnSlime = !Player->IsCrouched();
		FallDamageAbsorbed |= (IsFootOnSlimeBlock && ShouldBounceOnSlime);
	}
	else
	{
		/* 5. Bouncing on a slime block absorbs all fall damage  */
		FallDamageAbsorbed |= IsFootOnSlimeBlock;
	}

	/* If the player is not crouching or is not a player, shoot them back up.
	NOTE: this will only work in some cases; should be done in HandlePhysics() */
	if (IsFootOnSlimeBlock && ShouldBounceOnSlime)
	{
		// TODO: doesn't work too well, causes dissatisfactory experience for players on slime blocks - SetSpeedY(-GetSpeedY());
	}

	// TODO: put player speed into GetSpeedY, and use that.
	// If flying, climbing, swimming, or going up...
	if (FallDamageAbsorbed || ((GetPosition() - m_LastPosition).y > 0))
	{
		// ...update the ground height to have the highest position of the player (i.e. jumping up adds to the eventual fall damage)
		m_LastGroundHeight = GetPosY();
	}

	if (OnGround)
	{
		const auto FallHeight = m_LastGroundHeight - GetPosY();
		auto Damage = static_cast<int>(FallHeight - 3.0);

		const auto Below = POS_TOINT.addedY(-1);
		const auto BlockBelow = (cChunkDef::IsValidHeight(Below)) ? GetWorld()->GetBlock(Below) : Block::Air::Air();

		if ((Damage > 0) && !FallDamageAbsorbed)
		{
			if (IsElytraFlying())
			{
				Damage = static_cast<int>(static_cast<float>(Damage) * 0.33);
			}

			if (BlockBelow.Type() == BlockType::HayBlock)
			{
				Damage = std::clamp(static_cast<int>(static_cast<float>(Damage) * 0.2), 1, 20);
			}

			// Fall particles
			// TODO: falling on a partial (e.g. slab) block shouldn't broadcast particles of the block below
			// auto NumericBlock = PaletteUpgrade::ToBlock(GetWorld()->GetBlock(Below));

			GetWorld()->BroadcastParticleEffect(
				"blockdust",
				GetPosition(),
				{ 0, 0, 0 },
				(Damage - 1.f) * ((0.3f - 0.1f) / (15.f - 1.f)) + 0.1f,  // Map damage (1 - 15) to particle speed (0.1 - 0.3)
				static_cast<int>((Damage - 1.f) * ((50.f - 20.f) / (15.f - 1.f)) + 20.f),  // Map damage (1 - 15) to particle quantity (20 - 50)
				{ { 0, 0 } }
			);

			TakeDamage(dtFalling, nullptr, Damage, static_cast<float>(Damage), 0);
		}

		m_bTouchGround = true;
		m_LastGroundHeight = GetPosY();

		// Farmland trampling. Mobs smaller than 0.512 cubic blocks won't trample (Java Edition's behavior)
		// We only have width and height, so we have to calculate Width^2
		if (GetWorld()->IsFarmlandTramplingEnabled())
		{
			HandleFarmlandTrampling(FallHeight, BlockAtFoot, BlockBelow);
		}
	}
	else
	{
		m_bTouchGround = false;
	}

	/* Note: it is currently possible to fall through lava and still die from fall damage
	because of the client skipping an update about the lava block. This can only be resolved by
	somehow integrating these above checks into the tracer in HandlePhysics. */
}





void cPawn::HandleFarmlandTrampling(const double a_FallHeight, const BlockState a_BlockAtFoot, const BlockState a_BlockBelow)
{
	// No trampling if FallHeight <= 0.6875
	if (a_FallHeight <= 0.6875)
	{
		return;
	}
	// No trampling for mobs smaller than 0.512 cubic blocks
	if (GetWidth() * GetWidth() * GetHeight() < 0.512)
	{
		return;
	}

	auto AbsPos = POS_TOINT;

	// Check if the foot is "inside" a farmland - for 1.10.1 and newer clients
	// If it isn't, check if the block below is a farmland - for mobs and older clients
	if (a_BlockAtFoot != E_BLOCK_FARMLAND)
	{
		// These are probably the only blocks which:
		// - can be placed on a farmland and shouldn't destroy it
		// - will stop the player from falling down further
		// - are less than 1 block high
		if ((a_BlockAtFoot == E_BLOCK_HEAD) || (a_BlockAtFoot == E_BLOCK_FLOWER_POT))
		{
			return;
		}

		// Finally, check whether the block below is farmland
		if (a_BlockBelow != E_BLOCK_FARMLAND)
		{
			return;
		}

		// If we haven't returned, decrease the height
		AbsPos.y -= 1;
	}

	bool ShouldTrample = true;
	auto & Random = GetRandomProvider();

	// For FallHeight <= 1.5625 we need to get a random bool
	if (a_FallHeight <= 1.0625)
	{
		ShouldTrample = Random.RandBool(0.25);
	}
	else if (a_FallHeight <= 1.5625)
	{
		ShouldTrample = Random.RandBool(0.66);
	}
	// For FallHeight > 1.5625 we always trample - ShouldTrample remains true

	if (ShouldTrample)
	{
		GetWorld()->DoWithChunkAt(AbsPos, [&](cChunk & Chunk)
		{
			cBlockFarmlandHandler::TurnToDirt(Chunk, AbsPos);
			return true;
		});
	}
}





void cPawn::OnRemoveFromWorld(cWorld & a_World)
{
	StopEveryoneFromTargetingMe();
	Super::OnRemoveFromWorld(a_World);
}





void cPawn::StopEveryoneFromTargetingMe()
{
	std::vector<cMonster*>::iterator i = m_TargetingMe.begin();
	while (i != m_TargetingMe.end())
	{
		cMonster * Monster = *i;
		ASSERT(Monster->GetTarget() == this);
		Monster->UnsafeUnsetTarget();
		i = m_TargetingMe.erase(i);
	}
	ASSERT(m_TargetingMe.size() == 0);
}





std::map<cEntityEffect::eType, cEntityEffect *> cPawn::GetEntityEffects() const
{
	std::map<cEntityEffect::eType, cEntityEffect *> Effects;
	for (auto & Effect : m_EntityEffects)
	{
		Effects.insert({ Effect.first, Effect.second.get() });
	}
	return Effects;
}





cEntityEffect * cPawn::GetEntityEffect(cEntityEffect::eType a_EffectType) const
{
	auto itr = m_EntityEffects.find(a_EffectType);
	return (itr != m_EntityEffects.end()) ? itr->second.get() : nullptr;
}





void cPawn::ResetPosition(Vector3d a_NewPosition)
{
	Super::ResetPosition(a_NewPosition);
	m_LastGroundHeight = GetPosY();
}





bool cPawn::DeductTotem(const eDamageType a_DamageType)
{
	if ((a_DamageType == dtAdmin) || (a_DamageType == dtInVoid))
	{
		// Beyond saving:
		return false;
	}

	if (!IsPlayer())
	{
		// TODO: implement when mobs will be able to pick up items based on CanPickUpLoot attribute:
		return false;
	}

	// If the player is holding a totem of undying in their off-hand or
	// main-hand slot and receives otherwise fatal damage, the totem saves the player from death.

	auto & inv = static_cast<cPlayer *>(this)->GetInventory();
	if (inv.GetEquippedItem().m_ItemType == Item::TotemOfUndying)
	{
		inv.SetEquippedItem({});
		return true;
	}
	if (inv.GetShieldSlot().m_ItemType == Item::TotemOfUndying)
	{
		inv.SetShieldSlot({});
		return true;
	}
	return false;
}





bool cPawn::FindTeleportDestination(cWorld & a_World, const int a_HeightRequired, const unsigned int a_NumTries, Vector3d & a_Destination, const Vector3i a_MinBoxCorner, const Vector3i a_MaxBoxCorner)
{
	/*
	Algorithm:
	Choose random destination.
	Seek downwards, regardless of distance until the block is made of movement-blocking material: https://minecraft.wiki/w/Materials
	Succeeds if no liquid or solid blocks prevents from standing at destination.
	*/
	auto & Random = GetRandomProvider();

	for (unsigned int i = 0; i < a_NumTries; i++)
	{
		const int DestX = Random.RandInt(a_MinBoxCorner.x, a_MaxBoxCorner.x);
		int DestY = Random.RandInt(a_MinBoxCorner.y, a_MaxBoxCorner.y);
		const int DestZ = Random.RandInt(a_MinBoxCorner.z, a_MaxBoxCorner.z);

		// Seek downwards from initial destination until we find a solid block or go into the void
		auto DestBlock = a_World.GetBlock({DestX, DestY, DestZ});
		while ((DestY >= 0) && !cBlockInfo::IsSolid(DestBlock))
		{
			DestBlock = a_World.GetBlock({DestX, DestY, DestZ});
			DestY--;
		}

		// Couldn't find a solid block so move to next attempt
		if (DestY < 0)
		{
			continue;
		}

		// Succeed if blocks above destination are empty
		bool Success = true;
		for (int j = 1; j <= a_HeightRequired; j++)
		{
			auto TestBlock = a_World.GetBlock({DestX, DestY + j, DestZ});
			if (cBlockInfo::IsSolid(TestBlock) || IsBlockLiquid(TestBlock))
			{
				Success = false;
				break;
			}
		}

		if (!Success)
		{
			continue;
		}

		// Offsets for entity to be centred and standing on solid block
		a_Destination = Vector3d(DestX + 0.5, DestY + 1, DestZ + 0.5);
		return true;
	}
	return false;
}





bool cPawn::FindTeleportDestination(cWorld & a_World, const int a_HeightRequired, const unsigned int a_NumTries, Vector3d & a_Destination, const cBoundingBox a_BoundingBox)
{
	return FindTeleportDestination(a_World, a_HeightRequired, a_NumTries, a_Destination, a_BoundingBox.GetMin(), a_BoundingBox.GetMax());
}





bool cPawn::FindTeleportDestination(cWorld & a_World, const int a_HeightRequired, const unsigned int a_NumTries, Vector3d & a_Destination, Vector3i a_Centre, const int a_HalfCubeWidth)
{
	Vector3i MinCorner(a_Centre.x - a_HalfCubeWidth, a_Centre.y - a_HalfCubeWidth, a_Centre.z - a_HalfCubeWidth);
	Vector3i MaxCorner(a_Centre.x + a_HalfCubeWidth, a_Centre.y + a_HalfCubeWidth, a_Centre.z + a_HalfCubeWidth);
	return FindTeleportDestination(a_World, a_HeightRequired, a_NumTries, a_Destination, MinCorner, MaxCorner);
}
