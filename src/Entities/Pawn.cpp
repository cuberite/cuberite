
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Pawn.h"
#include "Player.h"
#include "../World.h"
#include "../Bindings/PluginManager.h"
#include "BoundingBox.h"
#include "../Blocks/BlockHandler.h"
#include "EffectID.h"
#include "../Mobs/Monster.h"




cPawn::cPawn(eEntityType a_EntityType, double a_Width, double a_Height) :
	super(a_EntityType, 0, 0, 0, a_Width, a_Height),
	m_EntityEffects(tEffectMap()),
	m_LastGroundHeight(0),
	m_bTouchGround(false)
{
	SetGravity(-32.0f);
	SetAirDrag(0.02f);
}





void cPawn::Destroyed()
{
	super::Destroyed();
}





void cPawn::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	// Iterate through this entity's applied effects
	for (tEffectMap::iterator iter = m_EntityEffects.begin(); iter != m_EntityEffects.end();)
	{
		// Copies values to prevent pesky wrong accesses and erasures
		cEntityEffect::eType EffectType = iter->first;
		cEntityEffect * Effect = iter->second;

		Effect->OnTick(*this);

		// Iterates (must be called before any possible erasure)
		++iter;

		// Remove effect if duration has elapsed
		if (Effect->GetDuration() - Effect->GetTicks() <= 0)
		{
			RemoveEntityEffect(EffectType);
		}

		// TODO: Check for discrepancies between client and server effect values
	}

	class Pusher : public cEntityCallback
	{
	public:
		cEntity * m_Pusher;

		Pusher(cEntity * a_Pusher) :
			m_Pusher(a_Pusher)
		{
		}

		virtual bool Item(cEntity * a_Entity) override
		{
			if (a_Entity->GetUniqueID() == m_Pusher->GetUniqueID())
			{
				return false;
			}

			// we only push other mobs, boats and minecarts
			if ((a_Entity->GetEntityType() != etMonster) && (a_Entity->GetEntityType() != etMinecart) && (a_Entity->GetEntityType() != etBoat))
			{
				return false;
			}

			// do not push a boat / minecart you're sitting in
			if (m_Pusher->IsAttachedTo(a_Entity))
			{
				return false;
			}

			Vector3d v3Delta = a_Entity->GetPosition() - m_Pusher->GetPosition();
			v3Delta.y = 0.0;  // we only push sideways
			v3Delta *= 1.0 / (v3Delta.Length() + 0.01);  // we push harder if we're close
			// QUESTION: is there an additional multiplier for this? current shoving seems a bit weak

			a_Entity->AddSpeed(v3Delta);
			return false;
		}
	} Callback(this);

	m_World->ForEachEntityInBox(cBoundingBox(GetPosition(), GetWidth(), GetHeight()), Callback);

	super::Tick(a_Dt, a_Chunk);

	HandleFalling();
}





void cPawn::KilledBy(TakeDamageInfo & a_TDI)
{
	ClearEntityEffects();
	super::KilledBy(a_TDI);
}





bool cPawn::IsFireproof(void) const
{
	return super::IsFireproof() || HasEntityEffect(cEntityEffect::effFireResistance);
}





void cPawn::HandleAir(void)
{
	if (IsSubmerged() && HasEntityEffect(cEntityEffect::effWaterBreathing))
	{
		// Prevent the oxygen from decreasing
		return;
	}

	super::HandleAir();
}





void cPawn::AddEntityEffect(cEntityEffect::eType a_EffectType, int a_Duration, short a_Intensity, double a_DistanceModifier)
{
	// Check if the plugins allow the addition:
	if (cPluginManager::Get().CallHookEntityAddEffect(*this, a_EffectType, a_Duration, a_Intensity, a_DistanceModifier))
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

	m_EntityEffects[a_EffectType] = cEntityEffect::CreateEntityEffect(a_EffectType, a_Duration, a_Intensity, a_DistanceModifier);
	m_World->BroadcastEntityEffect(*this, a_EffectType, a_Intensity, static_cast<short>(a_Duration));
	m_EntityEffects[a_EffectType]->OnActivate(*this);
}





void cPawn::RemoveEntityEffect(cEntityEffect::eType a_EffectType)
{
	m_World->BroadcastRemoveEntityEffect(*this, a_EffectType);
	m_EntityEffects[a_EffectType]->OnDeactivate(*this);
	delete m_EntityEffects[a_EffectType];
	m_EntityEffects.erase(a_EffectType);
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
	static const struct
	{
		int x, y, z;
	} BlockSampleOffsets[] =
	{
		{ 0, 0, 0 },
		{ 0, -1, 0 },
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
			Vector3i BlockTestPosition = CrossTestPosition.Floor() + Vector3i(BlockSampleOffsets[j].x, BlockSampleOffsets[j].y, BlockSampleOffsets[j].z);

			if (!cChunkDef::IsValidHeight(BlockTestPosition.y))
			{
				continue;
			}

			BLOCKTYPE Block = GetWorld()->GetBlock(BlockTestPosition);
			NIBBLETYPE BlockMeta = GetWorld()->GetBlockMeta(BlockTestPosition);

			/* we do the cross-shaped sampling to check for water / liquids, but only on our level because water blocks are never bigger than unit voxels */
			if (j == 0)
			{
				IsFootInWater |= IsBlockWater(Block);
				IsFootInLiquid |= IsFootInWater || IsBlockLava(Block) || (Block == E_BLOCK_COBWEB);  // okay so cobweb is not _technically_ a liquid...
				IsFootOnSlimeBlock |= (Block == E_BLOCK_SLIME_BLOCK);
			}

			/* If the block is solid, and the blockhandler confirms the block to be inside, we're officially on the ground. */
			if ((cBlockInfo::IsSolid(Block)) && (cBlockInfo::GetHandler(Block)->IsInsideBlock(CrossTestPosition - BlockTestPosition, Block, BlockMeta)))
			{
				OnGround = true;
			}
		}
	}

	// Update the ground height to have the highest position of the player (i.e. jumping up adds to the eventual fall damage)
	if (!OnGround)
	{
		m_LastGroundHeight = std::max(m_LastGroundHeight, GetPosY());
	}

	/* So here's the use of the rules above: */
	/* 1. Falling in water absorbs all fall damage */
	bool FallDamageAbsorbed = IsFootInWater;

	/* 2. Falling in liquid (lava, water, cobweb) or hitting a slime block resets the "fall zenith".
	Note: Even though the pawn bounces back with no damage after hitting the slime block,
	the "fall zenith" will continue to increase back up when flying upwards - which is good */
	bool FallDistanceReset = IsFootOnSlimeBlock || IsFootInLiquid;
	bool IsFlying = false;
	bool ShouldBounceOnSlime = true;

	if (GetEntityType() == eEntityType::etPlayer)
	{
		cPlayer * Player = static_cast<cPlayer *>(this);
		IsFlying = Player->IsFlying();

		/* 3. If the player is flying or climbing, absorb fall damage */
		FallDamageAbsorbed |= IsFlying || Player->IsClimbing();

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
		SetSpeedY(-GetSpeedY());
	}

	if (!IsFlying && OnGround)
	{
		auto Damage = static_cast<int>(m_LastGroundHeight - GetPosY() - 3.0);
		if ((Damage > 0) && !FallDamageAbsorbed)
		{
			TakeDamage(dtFalling, nullptr, Damage, Damage, 0);

			// Fall particles
			int ParticleSize = static_cast<int>((std::min(15, Damage) - 1.f) * ((50.f - 20.f) / (15.f - 1.f)) + 20.f);
			GetWorld()->BroadcastSoundParticleEffect(EffectID::PARTICLE_FALL_PARTICLES, POSX_TOINT, POSY_TOINT - 1, POSZ_TOINT, ParticleSize);
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
	if (FallDistanceReset)
	{
		m_LastGroundHeight = GetPosY();
	}
}
