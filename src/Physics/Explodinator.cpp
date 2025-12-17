
#include "Globals.h"
#include "BlockInfo.h"
#include "Explodinator.h"
#include "Blocks/ChunkInterface.h"
#include "Chunk.h"
#include "ClientHandle.h"
#include "Entities/FallingBlock.h"
#include "LineBlockTracer.h"
#include "Simulator/SandSimulator.h"





namespace Explodinator
{
	static const auto StepUnit = 0.3f;
	static const auto KnockbackFactor = 25U;
	static const auto StepAttenuation = 0.225f;
	static const auto TraceCubeSideLength = 16U;
	static const auto BoundingBoxStepUnit = 0.5;

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

	/** Returns how much of an explosion Destruction Lazor's (tm) intensity the given block attenuates.
	Values are scaled as 0.3 * (0.3 + Wiki) since some compilers miss the constant folding optimisation.
	Wiki values are https://minecraft.wiki/w/Explosion#Blast_resistance as of 2021-02-06. */
	static float GetExplosionAbsorption(const BLOCKTYPE Block)
	{
		switch (Block)
		{
			case E_BLOCK_BEDROCK:
			case E_BLOCK_COMMAND_BLOCK:
			case E_BLOCK_END_GATEWAY:
			case E_BLOCK_END_PORTAL:
			case E_BLOCK_END_PORTAL_FRAME: return 1080000.09f;
			case E_BLOCK_ANVIL:
			case E_BLOCK_ENCHANTMENT_TABLE:
			case E_BLOCK_OBSIDIAN: return 360.09f;
			case E_BLOCK_ENDER_CHEST: return 180.09f;
			case E_BLOCK_LAVA:
			case E_BLOCK_STATIONARY_LAVA:
			case E_BLOCK_WATER:
			case E_BLOCK_STATIONARY_WATER: return 30.09f;
			case E_BLOCK_DRAGON_EGG:
			case E_BLOCK_END_STONE:
			case E_BLOCK_END_BRICKS: return 2.79f;
			case E_BLOCK_STONE:
			case E_BLOCK_BLOCK_OF_COAL:
			case E_BLOCK_DIAMOND_BLOCK:
			case E_BLOCK_EMERALD_BLOCK:
			case E_BLOCK_GOLD_BLOCK:
			case E_BLOCK_IRON_BLOCK:
			case E_BLOCK_BLOCK_OF_REDSTONE:
			case E_BLOCK_BRICK:
			case E_BLOCK_BRICK_STAIRS:
			case E_BLOCK_COBBLESTONE:
			case E_BLOCK_COBBLESTONE_STAIRS:
			case E_BLOCK_IRON_BARS:
			case E_BLOCK_JUKEBOX:
			case E_BLOCK_MOSSY_COBBLESTONE:
			case E_BLOCK_NETHER_BRICK:
			case E_BLOCK_NETHER_BRICK_FENCE:
			case E_BLOCK_NETHER_BRICK_STAIRS:
			case E_BLOCK_PRISMARINE_BLOCK:
			case E_BLOCK_STONE_BRICKS:
			case E_BLOCK_STONE_BRICK_STAIRS:
			case E_BLOCK_COBBLESTONE_WALL: return 1.89f;
			case E_BLOCK_IRON_DOOR:
			case E_BLOCK_IRON_TRAPDOOR:
			case E_BLOCK_MOB_SPAWNER: return 1.59f;
			case E_BLOCK_HOPPER: return 1.53f;
			case E_BLOCK_TERRACOTTA: return 1.35f;
			case E_BLOCK_COBWEB: return 1.29f;
			case E_BLOCK_DISPENSER:
			case E_BLOCK_DROPPER:
			case E_BLOCK_FURNACE:
			case E_BLOCK_OBSERVER: return 1.14f;
			case E_BLOCK_BEACON:
			case E_BLOCK_COAL_ORE:
			case E_BLOCK_COCOA_POD:
			case E_BLOCK_DIAMOND_ORE:
			case E_BLOCK_EMERALD_ORE:
			case E_BLOCK_GOLD_ORE:
			case E_BLOCK_IRON_ORE:
			case E_BLOCK_LAPIS_BLOCK:
			case E_BLOCK_LAPIS_ORE:
			case E_BLOCK_NETHER_QUARTZ_ORE:
			case E_BLOCK_PLANKS:
			case E_BLOCK_REDSTONE_ORE:
			case E_BLOCK_FENCE:
			case E_BLOCK_FENCE_GATE:
			case E_BLOCK_WOODEN_DOOR:
			case E_BLOCK_WOODEN_SLAB:
			case E_BLOCK_WOODEN_STAIRS:
			case E_BLOCK_TRAPDOOR: return 0.99f;
			case E_BLOCK_CHEST:
			case E_BLOCK_WORKBENCH:
			case E_BLOCK_TRAPPED_CHEST: return 0.84f;
			case E_BLOCK_BONE_BLOCK:
			case E_BLOCK_CAULDRON:
			case E_BLOCK_LOG: return 0.69f;  // nIcE
			case E_BLOCK_CONCRETE: return 0.63f;
			case E_BLOCK_BOOKCASE: return 0.54f;
			case E_BLOCK_STANDING_BANNER:
			case E_BLOCK_WALL_BANNER:
			case E_BLOCK_JACK_O_LANTERN:
			case E_BLOCK_MELON:
			case E_BLOCK_HEAD:
			case E_BLOCK_NETHER_WART_BLOCK:
			case E_BLOCK_PUMPKIN:
			case E_BLOCK_SIGN_POST:
			case E_BLOCK_WALLSIGN: return 0.39f;
			case E_BLOCK_QUARTZ_BLOCK:
			case E_BLOCK_QUARTZ_STAIRS:
			case E_BLOCK_RED_SANDSTONE:
			case E_BLOCK_RED_SANDSTONE_STAIRS:
			case E_BLOCK_SANDSTONE:
			case E_BLOCK_SANDSTONE_STAIRS:
			case E_BLOCK_WOOL: return 0.33f;
			case E_BLOCK_SILVERFISH_EGG: return 0.315f;
			case E_BLOCK_ACTIVATOR_RAIL:
			case E_BLOCK_DETECTOR_RAIL:
			case E_BLOCK_POWERED_RAIL:
			case E_BLOCK_RAIL: return 0.3f;
			case E_BLOCK_GRASS_PATH:
			case E_BLOCK_CLAY:
			case E_BLOCK_FARMLAND:
			case E_BLOCK_GRASS:
			case E_BLOCK_GRAVEL:
			case E_BLOCK_SPONGE: return 0.27f;
			case E_BLOCK_BREWING_STAND:
			case E_BLOCK_STONE_BUTTON:
			case E_BLOCK_WOODEN_BUTTON:
			case E_BLOCK_CAKE:
			case E_BLOCK_CONCRETE_POWDER:
			case E_BLOCK_DIRT:
			case E_BLOCK_FROSTED_ICE:
			case E_BLOCK_HAY_BALE:
			case E_BLOCK_ICE: return 0.24f;
			default: return 0.09f;
		}
	}

	/** Calculates the approximate percentage of an Entity's bounding box that is exposed to an explosion centred at Position. */
	static float CalculateEntityExposure(const cChunk & a_Chunk, const cEntity & a_Entity, const Vector3f a_Position, const int a_SquareRadius)
	{
		class LineOfSightCallbacks final : public cLineBlockTracer::cCallbacks
		{
			virtual bool OnNextBlock(Vector3i a_BlockPos, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, eBlockFace a_EntryFace) override
			{
				return a_BlockType != E_BLOCK_AIR;
			}
		} Callback;

		const Vector3d Position = a_Position;
		unsigned Unobstructed = 0, Total = 0;
		const auto Box = a_Entity.GetBoundingBox();
		cLineBlockTracer Tracer(*a_Chunk.GetWorld(), Callback);

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

					if (Tracer.Trace(a_Position, Destination))
					{
						Unobstructed++;
					}
					Total++;
				}
			}
		}

		return (Total == 0) ? 0 : (static_cast<float>(Unobstructed) / Total);
	}

	/** Applies distance-based damage and knockback to all entities within the explosion's effect range. */
	static void DamageEntities(cEntity * a_ExplodingEntity, const cChunk & a_Chunk, const Vector3f a_Position, const int a_Power, const eExplosionSource a_ExplosionSource = esOther)
	{
		const auto Radius = a_Power * 2;
		const auto SquareRadius = Radius * Radius;

		a_Chunk.GetWorld()->ForEachEntityInBox({ a_Position, Radius * 2.f }, [a_ExplodingEntity, &a_Chunk, a_Position, a_Power, a_ExplosionSource, Radius, SquareRadius](cEntity & Entity)
		{
			// Percentage of rays unobstructed.
			const auto Exposure = CalculateEntityExposure(a_Chunk, Entity, a_Position, SquareRadius);
			const auto Direction = Entity.GetPosition() - a_Position;
			const auto Impact = (1 - (static_cast<float>(Direction.Length()) / Radius)) * Exposure;

			// Don't apply damage to other TNT entities and falling blocks, they should be invincible:
			if (!Entity.IsTNT() && !Entity.IsFallingBlock())
			{
				const auto Damage = (Impact * Impact + Impact) * 7 * a_Power + 1;
				Entity.TakeDamage(dtExplosion, a_ExplodingEntity, FloorC(Damage), 0, a_ExplosionSource);
			}

			// Impact reduced by armour, expensive call so only apply to Pawns:
			if (Entity.IsPawn())
			{
				const auto ReducedImpact = Impact - Impact * Entity.GetEnchantmentBlastKnockbackReduction();
				Entity.AddSpeed(Direction.NormalizeCopy() * KnockbackFactor * ReducedImpact);
			}
			else
			{
				Entity.AddSpeed(Direction.NormalizeCopy() * KnockbackFactor * Impact);
			}

			// Continue iteration:
			return false;
		});
	}

	/** Returns true if block should always drop when exploded.
	Currently missing conduits from 1.13 */
	static bool BlockAlwaysDrops(const BLOCKTYPE a_Block)
	{
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
	static void DestroyBlock(cChunk & a_Chunk, const Vector3i a_Position, const int a_Power, const bool a_Fiery, const cEntity * const a_ExplodingEntity)
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
		else if ((a_ExplodingEntity != nullptr) && (a_ExplodingEntity->IsTNT() || BlockAlwaysDrops(DestroyedBlock) || Random.RandBool(1.f / a_Power)))  // For TNT explosions, destroying a block that always drops, or if RandBool, drop pickups
		{
			const auto DestroyedMeta = a_Chunk.GetMeta(a_Position);
			a_Chunk.GetWorld()->SpawnItemPickups(cBlockHandler::For(DestroyedBlock).ConvertToPickups(DestroyedMeta), Absolute);
		}
		else if (a_Fiery && Random.RandBool(1 / 3.0))  // 33% chance of starting fires if it can start fires
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
	static void DestructionTrace(cChunk * a_Chunk, Vector3f a_Origin, const Vector3f a_Direction, const int a_Power, const bool a_Fiery, float a_Intensity, const cEntity * const a_ExplodingEntity)
	{
		// The current position the ray is at.
		auto Checkpoint = a_Origin;

		// The displacement that the ray in one iteration step should travel.
		const auto Step = a_Direction.NormalizeCopy() * StepUnit;

		// Loop until intensity runs out:
		while (a_Intensity > 0)
		{
			auto Position = Checkpoint.Floor();
			if (!cChunkDef::IsValidHeight(Position))
			{
				break;
			}

			const auto Neighbour = a_Chunk->GetRelNeighborChunkAdjustCoords(Position);
			if ((Neighbour == nullptr) || !Neighbour->IsValid())
			{
				break;
			}

			a_Intensity -= GetExplosionAbsorption(Neighbour->GetBlock(Position));
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
	static float RandomIntensity(MTRand & a_Random, const int a_Power)
	{
		return a_Power * (0.7f + a_Random.RandReal(0.6f));
	}

	/** Sends out Explosion Lazors (tm) originating from the given position that destroy blocks. */
	static void DamageBlocks(cChunk & a_Chunk, const Vector3f a_Position, const int a_Power, const bool a_Fiery, const cEntity * const a_ExplodingEntity)
	{
		// Oh boy... Better hope you have a hot cache, 'cos this little manoeuvre's gonna cost us 1352 raytraces in one tick...
		const int HalfSide = TraceCubeSideLength / 2;
		auto & Random = GetRandomProvider();

		// The following loops implement the tracing algorithm described in http://minecraft.wiki/w/Explosion

		// Trace rays from the explosion centre to all points in a square of area TraceCubeSideLength * TraceCubeSideLength
		// for the top and bottom sides:
		for (float OffsetX = -HalfSide; OffsetX < HalfSide; OffsetX++)
		{
			for (float OffsetZ = -HalfSide; OffsetZ < HalfSide; OffsetZ++)
			{
				DestructionTrace(&a_Chunk, a_Position, Vector3f(OffsetX, +HalfSide, OffsetZ), a_Power, a_Fiery, RandomIntensity(Random, a_Power), a_ExplodingEntity);
				DestructionTrace(&a_Chunk, a_Position, Vector3f(OffsetX, -HalfSide, OffsetZ), a_Power, a_Fiery, RandomIntensity(Random, a_Power), a_ExplodingEntity);
			}
		}

		// Left and right sides, avoid duplicates at top and bottom edges:
		for (float OffsetX = -HalfSide; OffsetX < HalfSide; OffsetX++)
		{
			for (float OffsetY = -HalfSide + 1; OffsetY < HalfSide - 1; OffsetY++)
			{
				DestructionTrace(&a_Chunk, a_Position, Vector3f(OffsetX, OffsetY, +HalfSide), a_Power, a_Fiery, RandomIntensity(Random, a_Power), a_ExplodingEntity);
				DestructionTrace(&a_Chunk, a_Position, Vector3f(OffsetX, OffsetY, -HalfSide), a_Power, a_Fiery, RandomIntensity(Random, a_Power), a_ExplodingEntity);
			}
		}

		// Front and back sides, avoid all edges:
		for (float OffsetZ = -HalfSide + 1; OffsetZ < HalfSide - 1; OffsetZ++)
		{
			for (float OffsetY = -HalfSide + 1; OffsetY < HalfSide - 1; OffsetY++)
			{
				DestructionTrace(&a_Chunk, a_Position, Vector3f(+HalfSide, OffsetY, OffsetZ), a_Power, a_Fiery, RandomIntensity(Random, a_Power), a_ExplodingEntity);
				DestructionTrace(&a_Chunk, a_Position, Vector3f(-HalfSide, OffsetY, OffsetZ), a_Power, a_Fiery, RandomIntensity(Random, a_Power), a_ExplodingEntity);
			}
		}
	}

	/** Sends an explosion packet to all clients in the given chunk. */
	static void LagTheClient(cChunk & a_Chunk, const Vector3f a_Position, const int a_Power)
	{
		for (const auto Client : a_Chunk.GetAllClients())
		{
			Client->SendExplosion(a_Position, static_cast<float>(a_Power));
		}
	}

	void Kaboom(cWorld & a_World, const Vector3f a_Position, const int a_Power, const bool a_Fiery, cEntity * a_ExplodingEntity, const eExplosionSource a_ExplosionSource)
	{
		a_World.DoWithChunkAt(a_Position.Floor(), [a_Position, a_Power, a_Fiery, a_ExplodingEntity, a_ExplosionSource](cChunk & a_Chunk)
		{
			LagTheClient(a_Chunk, a_Position, a_Power);
			DamageEntities(a_ExplodingEntity, a_Chunk, a_Position, a_Power, a_ExplosionSource);
			DamageBlocks(a_Chunk, AbsoluteToRelative(a_Position, a_Chunk.GetPos()), a_Power, a_Fiery, a_ExplodingEntity);

			return false;
		});
	}
}
