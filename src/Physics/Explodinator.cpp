
#include "Globals.h"
#include "BlockInfo.h"
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
	const auto BoundingBoxStepUnit = 0.5f;

	/** Returns how much of a Explosion Lazor's (tm) intensity the given block attenuates. */
	static float GetAbsorption(const BLOCKTYPE Block)
	{
		switch (Block)
		{
			case E_BLOCK_BEDROCK:
			case E_BLOCK_COMMAND_BLOCK:
			case E_BLOCK_END_GATEWAY:
			case E_BLOCK_END_PORTAL:
			case E_BLOCK_END_PORTAL_FRAME: return 3600000;
			case E_BLOCK_ANVIL:
			case E_BLOCK_ENCHANTMENT_TABLE:
			case E_BLOCK_OBSIDIAN: return 1200;
			case E_BLOCK_ENDER_CHEST: return 600;
			case E_BLOCK_LAVA:
			case E_BLOCK_STATIONARY_LAVA:
			case E_BLOCK_WATER:
			case E_BLOCK_STATIONARY_WATER: return 100;
			case E_BLOCK_DRAGON_EGG:
			case E_BLOCK_END_STONE:
			case E_BLOCK_END_BRICKS: return 9;
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
			case E_BLOCK_COBBLESTONE_WALL: return 6;
			case E_BLOCK_IRON_DOOR:
			case E_BLOCK_IRON_TRAPDOOR:
			case E_BLOCK_MOB_SPAWNER: return 5;
			case E_BLOCK_HOPPER: return 4.8f;
			case E_BLOCK_TERRACOTTA: return 4.2f;
			case E_BLOCK_COBWEB: return 4;
			case E_BLOCK_DISPENSER:
			case E_BLOCK_DROPPER:
			case E_BLOCK_FURNACE:
			case E_BLOCK_OBSERVER: return 3.5f;
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
			case E_BLOCK_TRAPDOOR: return 3;
			case E_BLOCK_CHEST:
			case E_BLOCK_WORKBENCH:
			case E_BLOCK_TRAPPED_CHEST: return 2.5f;
			case E_BLOCK_BONE_BLOCK:
			case E_BLOCK_CAULDRON:
			case E_BLOCK_LOG: return 2;
			case E_BLOCK_CONCRETE: return 1.8f;
			case E_BLOCK_BOOKCASE: return 1.5f;
			case E_BLOCK_STANDING_BANNER:
			case E_BLOCK_WALL_BANNER:
			case E_BLOCK_JACK_O_LANTERN:
			case E_BLOCK_MELON:
			case E_BLOCK_HEAD:
			case E_BLOCK_NETHER_WART_BLOCK:
			case E_BLOCK_PUMPKIN:
			case E_BLOCK_SIGN_POST:
			case E_BLOCK_WALLSIGN: return 1;
			case E_BLOCK_QUARTZ_BLOCK:
			case E_BLOCK_QUARTZ_STAIRS:
			case E_BLOCK_RED_SANDSTONE:
			case E_BLOCK_RED_SANDSTONE_STAIRS:
			case E_BLOCK_SANDSTONE:
			case E_BLOCK_SANDSTONE_STAIRS:
			case E_BLOCK_WOOL: return 0.8f;
			case E_BLOCK_SILVERFISH_EGG: return 0.75f;
			case E_BLOCK_ACTIVATOR_RAIL:
			case E_BLOCK_DETECTOR_RAIL:
			case E_BLOCK_POWERED_RAIL:
			case E_BLOCK_RAIL: return 0.7f;
			case E_BLOCK_GRASS_PATH:
			case E_BLOCK_CLAY:
			case E_BLOCK_FARMLAND:
			case E_BLOCK_GRASS:
			case E_BLOCK_GRAVEL:
			case E_BLOCK_SPONGE: return 0.6f;
			case E_BLOCK_BREWING_STAND:
			case E_BLOCK_STONE_BUTTON:
			case E_BLOCK_WOODEN_BUTTON:
			case E_BLOCK_CAKE:
			case E_BLOCK_CONCRETE_POWDER:
			case E_BLOCK_DIRT:
			case E_BLOCK_FROSTED_ICE:
			case E_BLOCK_HAY_BALE:
			case E_BLOCK_ICE: return 0.5f;
			default: return 0;
		}
	}

	/** Converts an absolute floating-point Position into a Chunk-relative one. */
	static Vector3f AbsoluteToRelative(const Vector3f Position, const cChunkCoords ChunkPosition)
	{
		return { Position.x - ChunkPosition.m_ChunkX * cChunkDef::Width, Position.y, Position.z - ChunkPosition.m_ChunkZ * cChunkDef::Width };
	}

	/** Make a From Chunk-relative Position into a To Chunk-relative position. */
	static Vector3f RebaseRelativePosition(const cChunkCoords From, const cChunkCoords To, const Vector3f Position)
	{
		return
		{
			Position.x + (From.m_ChunkX - To.m_ChunkX) * cChunkDef::Width,
			Position.y,
			Position.z + (From.m_ChunkZ - To.m_ChunkZ) * cChunkDef::Width
		};
	}

	/** Calculates the approximate percentage of an Entity's bounding box that is exposed to an explosion centred at Position. */
	static float CalculateEntityExposure(cChunk & Chunk, const cEntity & Entity, const Vector3f Position, const float SquareRadius)
	{
		unsigned Unobstructed = 0, Total = 0;
		const auto Box = Entity.GetBoundingBox();

		for (float X = Box.GetMinX(); X < Box.GetMaxX(); X += BoundingBoxStepUnit)
		{
			for (float Y = Box.GetMinY(); Y < Box.GetMaxY(); Y += BoundingBoxStepUnit)
			{
				for (float Z = Box.GetMinZ(); Z < Box.GetMaxZ(); Z += BoundingBoxStepUnit)
				{
					const auto Destination = Vector3f(X, Y, Z);
					if ((Destination - Position).SqrLength() > SquareRadius)
					{
						// Don't bother with points outside our designated area-of-effect
						// This is, surprisingly, a massive amount of work saved (~3m to detonate a sphere of 37k TNT before, ~1m after):
						continue;
					}

					if (cLineBlockTracer::LineOfSightTrace(*Chunk.GetWorld(), Position, Destination, cLineBlockTracer::eLineOfSight::losAir))
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
	static void DamageEntities(cChunk & Chunk, const Vector3f Position, const unsigned Power)
	{
		const auto Radius = Power * 2.f;
		const auto SquareRadius = Radius * Radius;

		Chunk.GetWorld()->ForEachEntityInBox({ Position, Radius * 2 }, [&Chunk, Position, Power, Radius, SquareRadius](cEntity & Entity)
		{
			// Percentage of rays unobstructed.
			const auto Exposure = CalculateEntityExposure(Chunk, Entity, Position, SquareRadius);
			const auto Direction = Entity.GetPosition() - Position;
			const auto Impact = (1 - (static_cast<float>(Direction.Length()) / Radius)) * Exposure;

			// Don't apply damage to other TNT entities and falling blocks, they should be invincible:
			if (!Entity.IsTNT() && !Entity.IsFallingBlock())
			{
				const auto Damage = (Impact * Impact + Impact) * 7 * Power + 1;
				Entity.TakeDamage(dtExplosion, nullptr, FloorC(Damage), 0);
			}

			// Impact reduced by armour:
			const auto ReducedImpact = Impact - Impact * Entity.GetEnchantmentBlastKnockbackReduction();  // TODO: call is very expensive, should only apply to Pawns
			Entity.SetSpeed(Direction.NormalizeCopy() * KnockbackFactor * ReducedImpact);

			// Continue iteration:
			return false;
		});
	}

	/** Sets the block at the given Position to air, updates surroundings, and spawns pickups, fire, shrapnel according to Minecraft rules.
	OK, _mostly_ Minecraft rules. */
	static void DestroyBlock(cChunk & Chunk, const Vector3i Position, const unsigned Power, const bool Fiery)
	{
		BLOCKTYPE DestroyedBlock;
		NIBBLETYPE DestroyedMeta;
		Chunk.GetBlockTypeMeta(Position, DestroyedBlock, DestroyedMeta);
		if (DestroyedBlock == E_BLOCK_AIR)
		{
			// There's nothing left for us here, but a barren and empty land
			// Let's go.
			return;
		}

		auto & World = *Chunk.GetWorld();
		auto & Random = GetRandomProvider();
		const auto Absolute = cChunkDef::RelativeToAbsolute(Position, Chunk.GetPos());
		if (DestroyedBlock == E_BLOCK_TNT)
		{
			// Random fuse between 10 to 30 game ticks.
			const int FuseTime = Random.RandInt(10, 30);

			// Activate the TNT, with initial velocity and no fuse sound:
			World.SpawnPrimedTNT(Vector3d(0.5, 0, 0.5) + Absolute, FuseTime, 1, false);
		}
		else if (Random.RandBool(1.f / Power))
		{
			Chunk.GetWorld()->SpawnItemPickups(
				cBlockInfo::GetHandler(DestroyedBlock)->ConvertToPickups(DestroyedMeta, Chunk.GetBlockEntityRel(Position)),
				Absolute
			);
		}
		else if (Fiery && Random.RandBool(1.f / 3.f))  // 33% chance of starting fires if it can start fires
		{
			const auto Below = Position.addedY(-1);
			if ((Below.y >= 0) && cBlockInfo::FullyOccupiesVoxel(Chunk.GetBlock(Below)))
			{
				// Start a fire:
				Chunk.SetBlock(Position, E_BLOCK_FIRE, 0);
				return;
			}
		}
		else if (const auto Shrapnel = World.GetTNTShrapnelLevel(); (Shrapnel > slNone) && Random.RandBool(0))  // 20% chance of flinging stuff around
		{
			// If the block is shrapnel-able, make a falling block entity out of it:
			if (
				((Shrapnel == slAll) && cBlockInfo::FullyOccupiesVoxel(DestroyedBlock)) ||
				((Shrapnel == slGravityAffectedOnly) && cSandSimulator::IsAllowedBlock(DestroyedBlock))
			)
			{
				auto FallingBlock = std::make_unique<cFallingBlock>(Vector3d(0.5, 0, 0.5) + Absolute, DestroyedBlock, DestroyedMeta);
				// TODO: correct velocity FallingBlock->SetSpeedY(40);
				FallingBlock->Initialize(std::move(FallingBlock), World);
			}
		}

		// SetBlock wakes up all simulators for the area, so that water and lava flows and sand falls into the blasted holes
		// It also is responsible for calling cBlockHandler::OnNeighborChanged to pop off blocks that fail CanBeAt
		// An explicit call to cBlockHandler::OnBroken handles the destruction of multiblock structures
		// References at (FS #391, GH #):
		Chunk.SetBlock(Position, E_BLOCK_AIR, 0);

		cChunkInterface Interface(World.GetChunkMap());
		cBlockInfo::GetHandler(DestroyedBlock)->OnBroken(Interface, World, Absolute, DestroyedBlock, 0);
	}

	/** Traces the path taken by one Explosion Lazor (tm) with given direction and intensity, that will destroy blocks until it is exhausted. */
	static void DestructionTrace(cChunk * Chunk, Vector3f Origin, const Vector3f Destination, const unsigned Power, const bool Fiery, float Intensity)
	{
		// The current position the ray is at.
		auto Checkpoint = Origin;

		// The total displacement the ray must travel.
		const auto TraceDisplacement = (Destination - Origin);

		// The displacement that they ray in one iteration step should travel.
		const auto Step = TraceDisplacement.NormalizeCopy() * StepUnit;

		// Loop until we've reached the prescribed destination:
		while (TraceDisplacement > (Checkpoint - Origin))
		{
			auto Position = Checkpoint.Floor();
			if (!cChunkDef::IsValidHeight(Position.y))
			{
				break;
			}

			const auto Neighbour = Chunk->GetRelNeighborChunkAdjustCoords(Position);
			if ((Neighbour == nullptr) || !Neighbour->IsValid())
			{
				break;
			}

			Intensity -= 0.3f * (0.3f + GetAbsorption(Neighbour->GetBlock(Position)));
			if (Intensity <= 0)
			{
				// The ray is exhausted:
				break;
			}

			DestroyBlock(*Neighbour, Position, Power, Fiery);

			// Adjust coordinates to be relative to the neighbour chunk:
			Checkpoint = RebaseRelativePosition(Chunk->GetPos(), Neighbour->GetPos(), Checkpoint);
			Origin = RebaseRelativePosition(Chunk->GetPos(), Neighbour->GetPos(), Origin);
			Chunk = Neighbour;

			// Increment the simulation, weaken the ray:
			Checkpoint += Step;
			Intensity -= StepAttenuation;
		}
	}

	/** Sends out Explosion Lazors (tm) originating from the given position that destroy blocks. */
	static void DamageBlocks(cChunk & Chunk, const Vector3f Position, const unsigned Power, const bool Fiery)
	{
		const auto Intensity = Power * (0.7f + GetRandomProvider().RandReal(0.6f));
		const auto ExplosionRadius = CeilC((Intensity / StepAttenuation) * StepUnit);

		// Oh boy... Better hope you have a hot cache, 'cos this little manoeuvre's gonna cost us 1352 raytraces in one tick...
		const int HalfSide = TraceCubeSideLength / 2;

		// The following loops implement the tracing algorithm described in http://minecraft.gamepedia.com/Explosion

		// Trace rays from the explosion centre to all points in a square of area TraceCubeSideLength * TraceCubeSideLength
		// in the YM and YP directions:
		for (int OffsetX = -HalfSide; OffsetX < HalfSide; OffsetX++)
		{
			for (int OffsetZ = -HalfSide; OffsetZ < HalfSide; OffsetZ++)
			{
				DestructionTrace(&Chunk, Position, Position + Vector3f(OffsetX, +ExplosionRadius, OffsetZ), Power, Fiery, Intensity);
				DestructionTrace(&Chunk, Position, Position + Vector3f(OffsetX, -ExplosionRadius, OffsetZ), Power, Fiery, Intensity);
			}
		}

		/*
		Trace rays from the centre to the sides of the explosion cube, being careful to avoid duplicates:

		Top view:
		______
		.       | (dot to make style checker happy)
		|       |
		|       |
		|  ______

		Side view:
		+        +
		|======  |
		|     |  |
		|======  |
		+        +

		*/
		for (int Offset = -HalfSide; Offset < HalfSide - 1; Offset++)
		{
			for (int OffsetY = -HalfSide + 1; OffsetY < HalfSide - 1; OffsetY++)
			{
				DestructionTrace(&Chunk, Position, Position + Vector3f(ExplosionRadius, OffsetY, Offset + 1), Power, Fiery, Intensity);
				DestructionTrace(&Chunk, Position, Position + Vector3f(-ExplosionRadius, OffsetY, Offset), Power, Fiery, Intensity);
				DestructionTrace(&Chunk, Position, Position + Vector3f(Offset, OffsetY, ExplosionRadius), Power, Fiery, Intensity);
				DestructionTrace(&Chunk, Position, Position + Vector3f(Offset + 1, OffsetY, -ExplosionRadius), Power, Fiery, Intensity);
			}
		}
	}

	/** Sends an explosion packet to all clients in the given chunk. */
	static void LagTheClient(cChunk & Chunk, const Vector3f Position, const unsigned Power)
	{
		for (const auto Client : Chunk.GetAllClients())
		{
			Client->SendExplosion(Position, Power);
		}
	}

	void Kaboom(cWorld & World, const Vector3f Position, const unsigned Power, const bool Fiery)
	{
		World.DoWithChunkAt(Position.Floor(), [Position, Power, Fiery](cChunk & Chunk)
		{
			LagTheClient(Chunk, Position, Power);
			DamageEntities(Chunk, Position, Power);
			DamageBlocks(Chunk, AbsoluteToRelative(Position, Chunk.GetPos()), Power, Fiery);

			return false;
		});
	}
}
