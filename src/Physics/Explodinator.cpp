
#include "Globals.h"
#include "BlockInfo.h"
#include "Explodinator.h"
#include "Blocks/BlockHandler.h"
#include "Blocks/ChunkInterface.h"
#include "Chunk.h"
#include "ClientHandle.h"
#include "Entities/FallingBlock.h"
#include "LineBlockTracer.h"
#include "Simulator/SandSimulator.h"





namespace Explodinator
{
	const auto StepUnit = 0.3f;
	const auto KnockbackFactor = 25U;
	const auto StepAttenuation = 0.225f;
	const auto TraceCubeSideLength = 16U;
	const auto BoundingBoxStepUnit = 0.5;

	/** Converts an absolute floating-point Position into a Chunk-relative one. */
	static Vector3f AbsoluteToRelative(const Vector3f a_Position, const cChunkCoords a_ChunkPosition)
	{
		return { a_Position.x - a_ChunkPosition.m_ChunkX * cChunkDef::Width, a_Position.y, a_Position.z - a_ChunkPosition.m_ChunkZ * cChunkDef::Width };
	}

	/** Make a From Chunk-relative Position into a To Chunk-relative position. */
	static Vector3f RebaseRelativePosition(const cChunkCoords a_From, const cChunkCoords a_To, const Vector3f a_Position)
	{
		return
		{
			a_Position.x + (a_From.m_ChunkX - a_To.m_ChunkX) * cChunkDef::Width,
			a_Position.y,
			a_Position.z + (a_From.m_ChunkZ - a_To.m_ChunkZ) * cChunkDef::Width
		};
	}

	/** Calculates the approximate percentage of an Entity's bounding box that is exposed to an explosion centred at Position. */
	static float CalculateEntityExposure(cChunk & a_Chunk, const cEntity & a_Entity, const Vector3f a_Position, const float a_SquareRadius)
	{
		const Vector3d Position = a_Position;
		unsigned Unobstructed = 0, Total = 0;
		const auto Box = a_Entity.GetBoundingBox();

		for (double X = Box.GetMinX(); X < Box.GetMaxX(); X += BoundingBoxStepUnit)
		{
			for (double Y = Box.GetMinY(); Y < Box.GetMaxY(); Y += BoundingBoxStepUnit)
			{
				for (double Z = Box.GetMinZ(); Z < Box.GetMaxZ(); Z += BoundingBoxStepUnit)
				{
					const Vector3d Destination{X, Y, Z};
					if ((Destination - Position).SqrLength() > a_SquareRadius)
					{
						// Don't bother with points outside our designated area-of-effect
						// This is, surprisingly, a massive amount of work saved (~3m to detonate a sphere of 37k TNT before, ~1m after):
						continue;
					}

					if (cLineBlockTracer::LineOfSightTrace(*a_Chunk.GetWorld(), a_Position, Destination, cLineBlockTracer::eLineOfSight::losAir))
					{
						Unobstructed++;
					}
					Total++;
				}
			}
		}

		return static_cast<float>(Unobstructed) / Total;
	}

	/** Applies distance-based damage and knockback to all entities within the explosion's effect range. */
	static void DamageEntities(cChunk & a_Chunk, const Vector3f a_Position, const unsigned a_Power)
	{
		const auto Radius = a_Power * 2.f;
		const auto SquareRadius = Radius * Radius;

		a_Chunk.GetWorld()->ForEachEntityInBox({ a_Position, Radius * 2 }, [&a_Chunk, a_Position, a_Power, Radius, SquareRadius](cEntity & Entity)
		{
			// Percentage of rays unobstructed.
			const auto Exposure = CalculateEntityExposure(a_Chunk, Entity, a_Position, SquareRadius);
			const auto Direction = Entity.GetPosition() - a_Position;
			const auto Impact = (1 - (static_cast<float>(Direction.Length()) / Radius)) * Exposure;

			// Don't apply damage to other TNT entities and falling blocks, they should be invincible:
			if (!Entity.IsTNT() && !Entity.IsFallingBlock())
			{
				const auto Damage = (Impact * Impact + Impact) * 7 * a_Power + 1;
				Entity.TakeDamage(dtExplosion, nullptr, FloorC(Damage), 0);
			}

			// Impact reduced by armour, expensive call so only apply to Pawns:
			if (Entity.IsPawn())
			{
				const auto ReducedImpact = Impact - Impact * Entity.GetEnchantmentBlastKnockbackReduction();
				Entity.SetSpeed(Direction.NormalizeCopy() * KnockbackFactor * ReducedImpact);
			}
			else
			{
				Entity.SetSpeed(Direction.NormalizeCopy() * KnockbackFactor * Impact);
			}

			// Continue iteration:
			return false;
		});
	}

	/** Returns true if block should always drop when exploded.
	Currently missing conduits from 1.13 */
	static bool BlockAlwaysDrops(const BLOCKTYPE a_Block)
	{
		// If it's a Shulker box
		if (IsBlockShulkerBox(a_Block))
		{
			return true;
		}

		switch (a_Block)
		{
			case E_BLOCK_DRAGON_EGG:
			case E_BLOCK_BEACON:
			case E_BLOCK_HEAD: return true;
		}

		return false;
	}

	/** Sets the block at the given position, updating surroundings. */
	static void SetBlock(cWorld & a_World, cChunk & a_Chunk, const Vector3i a_AbsolutePosition, const Vector3i a_RelativePosition, const BLOCKTYPE a_DestroyedBlock, const BLOCKTYPE a_NewBlock, const cEntity * const a_ExplodingEntity)
	{
		const auto DestroyedMeta = a_Chunk.GetMeta(a_RelativePosition);

		// SetBlock wakes up all simulators for the area, so that water and lava flows and sand falls into the blasted holes
		// It also is responsible for calling cBlockHandler::OnNeighborChanged to pop off blocks that fail CanBeAt
		// An explicit call to cBlockHandler::OnBroken handles the destruction of multiblock structures
		// References at (FS #391, GH #4418):
		a_Chunk.SetBlock(a_RelativePosition, a_NewBlock, 0);

		cChunkInterface Interface(a_World.GetChunkMap());
		cBlockHandler::For(a_DestroyedBlock).OnBroken(Interface, a_World, a_AbsolutePosition, a_DestroyedBlock, DestroyedMeta, a_ExplodingEntity);
	}

	/** Work out what should happen when an explosion destroys the given block.
	Tasks include lighting TNT, dropping pickups, setting fire and flinging shrapnel according to Minecraft rules.
	OK, _mostly_ Minecraft rules. */
	static void DestroyBlock(cChunk & a_Chunk, const Vector3i a_Position, const unsigned a_Power, const bool a_Fiery, const cEntity * const a_ExplodingEntity)
	{
		const auto DestroyedBlock = a_Chunk.GetBlock(a_Position);
		if (DestroyedBlock == E_BLOCK_AIR)
		{
			// There's nothing left for us here, but a barren and empty land
			// Let's go.
			return;
		}

		auto & World = *a_Chunk.GetWorld();
		auto & Random = GetRandomProvider();
		const auto Absolute = cChunkDef::RelativeToAbsolute(a_Position, a_Chunk.GetPos());

		if (DestroyedBlock == E_BLOCK_TNT)  // If the block is TNT we should set it off
		{
			// Random fuse between 10 to 30 game ticks.
			const int FuseTime = Random.RandInt(10, 30);

			// Activate the TNT, with initial velocity and no fuse sound:
			World.SpawnPrimedTNT(Vector3d(0.5, 0, 0.5) + Absolute, FuseTime, 1, false);
		}
		else if (a_ExplodingEntity->IsTNT() || BlockAlwaysDrops(DestroyedBlock) || Random.RandBool(1.f / a_Power))  // For TNT explosions, destroying a block that always drops, or if RandBool, drop pickups
		{
			const auto DestroyedMeta = a_Chunk.GetMeta(a_Position);
			a_Chunk.GetWorld()->SpawnItemPickups(cBlockHandler::For(DestroyedBlock).ConvertToPickups(DestroyedMeta), Absolute);
		}
		else if (a_Fiery && Random.RandBool(1.f / 3.f))  // 33% chance of starting fires if it can start fires
		{
			const auto Below = a_Position.addedY(-1);
			if ((Below.y >= 0) && cBlockInfo::FullyOccupiesVoxel(a_Chunk.GetBlock(Below)))
			{
				// Start a fire:
				SetBlock(World, a_Chunk, Absolute, a_Position, DestroyedBlock, E_BLOCK_FIRE, a_ExplodingEntity);
				return;
			}
		}
		else if (const auto Shrapnel = World.GetTNTShrapnelLevel(); (Shrapnel > slNone) && Random.RandBool(0))  // Currently 0% chance of flinging stuff around
		{
			// If the block is shrapnel-able, make a falling block entity out of it:
			if (
				((Shrapnel == slAll) && cBlockInfo::FullyOccupiesVoxel(DestroyedBlock)) ||
				((Shrapnel == slGravityAffectedOnly) && cSandSimulator::IsAllowedBlock(DestroyedBlock))
			)
			{
				const auto DestroyedMeta = a_Chunk.GetMeta(a_Position);
				auto FallingBlock = std::make_unique<cFallingBlock>(Vector3d(0.5, 0, 0.5) + Absolute, DestroyedBlock, DestroyedMeta);
				// TODO: correct velocity FallingBlock->SetSpeedY(40);
				FallingBlock->Initialize(std::move(FallingBlock), World);
			}
		}

		SetBlock(World, a_Chunk, Absolute, a_Position, DestroyedBlock, E_BLOCK_AIR, a_ExplodingEntity);
	}

	/** Traces the path taken by one Explosion Lazor (tm) with given direction and intensity, that will destroy blocks until it is exhausted. */
	static void DestructionTrace(cChunk * a_Chunk, Vector3f a_Origin, const Vector3f a_Direction, const unsigned a_Power, const bool a_Fiery, float a_Intensity, const cEntity * const a_ExplodingEntity)
	{
		// The current position the ray is at.
		auto Checkpoint = a_Origin;

		// The displacement that the ray in one iteration step should travel.
		const auto Step = a_Direction.NormalizeCopy() * StepUnit;

		// Loop until intensity runs out:
		while (a_Intensity > 0)
		{
			auto Position = Checkpoint.Floor();
			if (!cChunkDef::IsValidHeight(Position.y))
			{
				break;
			}

			const auto Neighbour = a_Chunk->GetRelNeighborChunkAdjustCoords(Position);
			if ((Neighbour == nullptr) || !Neighbour->IsValid())
			{
				break;
			}

			a_Intensity -= 0.3f * (0.3f + cBlockInfo::GetExplosionAbsorption(Neighbour->GetBlock(Position)));
			if (a_Intensity <= 0)
			{
				// The ray is exhausted:
				break;
			}

			DestroyBlock(*Neighbour, Position, a_Power, a_Fiery, a_ExplodingEntity);

			// Adjust coordinates to be relative to the neighbour chunk:
			Checkpoint = RebaseRelativePosition(a_Chunk->GetPos(), Neighbour->GetPos(), Checkpoint);
			a_Origin = RebaseRelativePosition(a_Chunk->GetPos(), Neighbour->GetPos(), a_Origin);
			a_Chunk = Neighbour;

			// Increment the simulation, weaken the ray:
			Checkpoint += Step;
			a_Intensity -= StepAttenuation;
		}
	}

	/** Returns a random intensity for an Explosion Lazor (tm) as a function of the explosion's power. */
	static float RandomIntensity(MTRand & a_Random, const unsigned a_Power)
	{
		return a_Power * (0.7f + a_Random.RandReal(0.6f));
	}

	/** Sends out Explosion Lazors (tm) originating from the given position that destroy blocks. */
	static void DamageBlocks(cChunk & a_Chunk, const Vector3f a_Position, const unsigned a_Power, const bool a_Fiery, const cEntity * const a_ExplodingEntity)
	{
		// Oh boy... Better hope you have a hot cache, 'cos this little manoeuvre's gonna cost us 1352 raytraces in one tick...
		const int HalfSide = TraceCubeSideLength / 2;
		auto & Random = GetRandomProvider();

		// The following loops implement the tracing algorithm described in http://minecraft.gamepedia.com/Explosion

		// Trace rays from the explosion centre to all points in a square of area TraceCubeSideLength * TraceCubeSideLength
		// for the top and bottom sides:
		for (int OffsetX = -HalfSide; OffsetX < HalfSide; OffsetX++)
		{
			for (int OffsetZ = -HalfSide; OffsetZ < HalfSide; OffsetZ++)
			{
				DestructionTrace(&a_Chunk, a_Position, Vector3f(OffsetX, +HalfSide, OffsetZ), a_Power, a_Fiery, RandomIntensity(Random, a_Power), a_ExplodingEntity);
				DestructionTrace(&a_Chunk, a_Position, Vector3f(OffsetX, -HalfSide, OffsetZ), a_Power, a_Fiery, RandomIntensity(Random, a_Power), a_ExplodingEntity);
			}
		}

		// Left and right sides, avoid duplicates at top and bottom edges:
		for (int OffsetX = -HalfSide; OffsetX < HalfSide; OffsetX++)
		{
			for (int OffsetY = -HalfSide + 1; OffsetY < HalfSide - 1; OffsetY++)
			{
				DestructionTrace(&a_Chunk, a_Position, Vector3f(OffsetX, OffsetY, +HalfSide), a_Power, a_Fiery, RandomIntensity(Random, a_Power), a_ExplodingEntity);
				DestructionTrace(&a_Chunk, a_Position, Vector3f(OffsetX, OffsetY, -HalfSide), a_Power, a_Fiery, RandomIntensity(Random, a_Power), a_ExplodingEntity);
			}
		}

		// Front and back sides, avoid all edges:
		for (int OffsetZ = -HalfSide + 1; OffsetZ < HalfSide - 1; OffsetZ++)
		{
			for (int OffsetY = -HalfSide + 1; OffsetY < HalfSide - 1; OffsetY++)
			{
				DestructionTrace(&a_Chunk, a_Position, Vector3f(+HalfSide, OffsetY, OffsetZ), a_Power, a_Fiery, RandomIntensity(Random, a_Power), a_ExplodingEntity);
				DestructionTrace(&a_Chunk, a_Position, Vector3f(-HalfSide, OffsetY, OffsetZ), a_Power, a_Fiery, RandomIntensity(Random, a_Power), a_ExplodingEntity);
			}
		}
	}

	/** Sends an explosion packet to all clients in the given chunk. */
	static void LagTheClient(cChunk & a_Chunk, const Vector3f a_Position, const unsigned a_Power)
	{
		for (const auto Client : a_Chunk.GetAllClients())
		{
			Client->SendExplosion(a_Position, a_Power);
		}
	}

	void Kaboom(cWorld & a_World, const Vector3f a_Position, const unsigned a_Power, const bool a_Fiery, const cEntity * const a_ExplodingEntity)
	{
		a_World.DoWithChunkAt(a_Position.Floor(), [a_Position, a_Power, a_Fiery, a_ExplodingEntity](cChunk & a_Chunk)
		{
			LagTheClient(a_Chunk, a_Position, a_Power);
			DamageEntities(a_Chunk, a_Position, a_Power);
			DamageBlocks(a_Chunk, AbsoluteToRelative(a_Position, a_Chunk.GetPos()), a_Power, a_Fiery, a_ExplodingEntity);

			return false;
		});
	}
}
