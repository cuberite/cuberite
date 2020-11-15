
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Pawn.h"
#include "Player.h"
#include "../BlockInfo.h"
#include "../World.h"
#include "../Bindings/PluginManager.h"
#include "../BoundingBox.h"
#include "../Blocks/BlockHandler.h"
#include "../EffectID.h"
#include "../Mobs/Monster.h"




cPawn::cPawn(eEntityType a_EntityType, double a_Width, double a_Height) :
	Super(a_EntityType, Vector3d(), a_Width, a_Height),
	m_EntityEffects(tEffectMap()),
	m_LastGroundHeight(0),
	m_bTouchGround(false)
{
	SetGravity(-32.0f);
	SetAirDrag(0.02f);
}





cPawn::~cPawn()
{
	ASSERT(m_TargetingMe.size() == 0);
}





void cPawn::Destroyed()
{
	StopEveryoneFromTargetingMe();
	Super::Destroyed();
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
}





void cPawn::KilledBy(TakeDamageInfo & a_TDI)
{
	ClearEntityEffects();
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
	BLOCKTYPE BlockAtFoot = (cChunkDef::IsValidHeight(POSY_TOINT)) ? GetWorld()->GetBlock(POS_TOINT) : E_BLOCK_AIR;

	/* We initialize these with what the foot is really IN, because for sampling we will move down with the epsilon above */
	bool IsFootInWater = IsBlockWater(BlockAtFoot);
	bool IsFootInLiquid = IsFootInWater || IsBlockLava(BlockAtFoot) || (BlockAtFoot == E_BLOCK_COBWEB);  // okay so cobweb is not _technically_ a liquid...
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

			if (!cChunkDef::IsValidHeight(BlockTestPosition.y))
			{
				continue;
			}

			BLOCKTYPE BlockType = GetWorld()->GetBlock(BlockTestPosition);
			NIBBLETYPE BlockMeta = GetWorld()->GetBlockMeta(BlockTestPosition);

			/* we do the cross-shaped sampling to check for water / liquids, but only on our level because water blocks are never bigger than unit voxels */
			if (j == 0)
			{
				IsFootInWater |= IsBlockWater(BlockType);
				IsFootInLiquid |= IsFootInWater || IsBlockLava(BlockType) || (BlockType == E_BLOCK_COBWEB);  // okay so cobweb is not _technically_ a liquid...
				IsFootOnSlimeBlock |= (BlockType == E_BLOCK_SLIME_BLOCK);
			}

			/* If the block is solid, and the blockhandler confirms the block to be inside, we're officially on the ground. */
			if ((cBlockInfo::IsSolid(BlockType)) && (cBlockHandler::For(BlockType).IsInsideBlock(CrossTestPosition - BlockTestPosition, BlockMeta)))
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
		FallDamageAbsorbed |= Player->IsFlying() || Player->IsClimbing();

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
		auto Damage = static_cast<int>(m_LastGroundHeight - GetPosY() - 3.0);
		if ((Damage > 0) && !FallDamageAbsorbed)
		{
			TakeDamage(dtFalling, nullptr, Damage, static_cast<float>(Damage), 0);

			// Fall particles
			GetWorld()->BroadcastParticleEffect(
				"blockdust",
				GetPosition(),
				{ 0, 0, 0 },
				(Damage - 1.f) * ((0.3f - 0.1f) / (15.f - 1.f)) + 0.1f,  // Map damage (1 - 15) to particle speed (0.1 - 0.3)
				static_cast<int>((Damage - 1.f) * ((50.f - 20.f) / (15.f - 1.f)) + 20.f),  // Map damage (1 - 15) to particle quantity (20 - 50)
				{ { GetWorld()->GetBlock(POS_TOINT - Vector3i(0, 1, 0)), 0 } }
			);
		}

		m_bTouchGround = true;
		m_LastGroundHeight = GetPosY();
	}
	else
	{
		m_bTouchGround = false;
	}

	/* Note: it is currently possible to fall through lava and still die from fall damage
	because of the client skipping an update about the lava block. This can only be resolved by
	somehow integrating these above checks into the tracer in HandlePhysics. */
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
