
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
	Wiki values are https://minecraft.gamepedia.com/Explosion#Blast_resistance as of 2021-02-06. */
	static float GetExplosionAbsorption(const BlockState a_Block)
	{
		switch (a_Block.Type())
		{
			case BlockType::Bedrock:
			case BlockType::CommandBlock:
			case BlockType::ChainCommandBlock:
			case BlockType::RepeatingCommandBlock:
			case BlockType::EndGateway:
			case BlockType::EndPortal:
			case BlockType::EndPortalFrame: return 1080000.09f;
			case BlockType::Anvil:
			case BlockType::ChippedAnvil:
			case BlockType::DamagedAnvil:
			case BlockType::EnchantingTable:
			case BlockType::Obsidian: return 360.09f;
			case BlockType::EnderChest: return 180.09f;
			case BlockType::Lava:
			case BlockType::Water: return 30.09f;
			case BlockType::DragonEgg:
			case BlockType::EndStone:
			case BlockType::EndStoneBricks: return 2.79f;
			case BlockType::Stone:
			case BlockType::CoalBlock:
			case BlockType::DiamondBlock:
			case BlockType::EmeraldBlock:
			case BlockType::GoldBlock:
			case BlockType::IronBlock:
			case BlockType::RedstoneBlock:
			case BlockType::Bricks:
			case BlockType::BrickStairs:
			case BlockType::Cobblestone:
			case BlockType::CobblestoneStairs:
			case BlockType::IronBars:
			case BlockType::Jukebox:
			case BlockType::MossyCobblestone:
			case BlockType::NetherBricks:
			case BlockType::NetherBrickFence:
			case BlockType::NetherBrickStairs:
			case BlockType::Prismarine:
			case BlockType::StoneBricks:
			case BlockType::StoneBrickStairs:
			case BlockType::CobblestoneWall: return 1.89f;
			case BlockType::IronDoor:
			case BlockType::IronTrapdoor:
			case BlockType::Spawner: return 1.59f;
			case BlockType::Hopper: return 1.53f;

			case BlockType::BlackTerracotta:
			case BlockType::BlueTerracotta:
			case BlockType::BrownTerracotta:
			case BlockType::CyanTerracotta:
			case BlockType::GrayTerracotta:
			case BlockType::GreenTerracotta:
			case BlockType::LightBlueTerracotta:
			case BlockType::LightGrayTerracotta:
			case BlockType::LimeTerracotta:
			case BlockType::MagentaTerracotta:
			case BlockType::OrangeTerracotta:
			case BlockType::PinkTerracotta:
			case BlockType::PurpleTerracotta:
			case BlockType::RedTerracotta:
			case BlockType::WhiteTerracotta:
			case BlockType::YellowTerracotta:
				return 1.35f;
			case BlockType::Cobweb: return 1.29f;
			case BlockType::Dispenser:
			case BlockType::Dropper:
			case BlockType::Furnace:
			case BlockType::Observer: return 1.14f;
			case BlockType::Beacon:
			case BlockType::CoalOre:
			case BlockType::Cocoa:
			case BlockType::DiamondOre:
			case BlockType::EmeraldOre:
			case BlockType::GoldOre:
			case BlockType::NetherGoldOre:
			case BlockType::IronOre:
			case BlockType::LapisBlock:
			case BlockType::LapisOre:
			case BlockType::NetherQuartzOre:
			case BlockType::OakPlanks:
			case BlockType::RedstoneOre:
			case BlockType::OakFence:
			case BlockType::OakFenceGate:
			case BlockType::OakDoor:
			case BlockType::OakSlab:
			case BlockType::OakStairs:
			case BlockType::OakTrapdoor: return 0.99f;
			case BlockType::Chest:
			case BlockType::CraftingTable:
			case BlockType::TrappedChest: return 0.84f;
			case BlockType::BoneBlock:
			case BlockType::Cauldron:
			case BlockType::OakLog: return 0.69f;  // nIcE

			case BlockType::BlackConcrete:
			case BlockType::BlueConcrete:
			case BlockType::BrownConcrete:
			case BlockType::CyanConcrete:
			case BlockType::GrayConcrete:
			case BlockType::GreenConcrete:
			case BlockType::LightBlueConcrete:
			case BlockType::LightGrayConcrete:
			case BlockType::LimeConcrete:
			case BlockType::MagentaConcrete:
			case BlockType::OrangeConcrete:
			case BlockType::PinkConcrete:
			case BlockType::PurpleConcrete:
			case BlockType::RedConcrete:
			case BlockType::WhiteConcrete:
			case BlockType::YellowConcrete:
				return 0.63f;
			case BlockType::Bookshelf: return 0.54f;

			case BlockType::BlackBanner:
			case BlockType::BlueBanner:
			case BlockType::BrownBanner:
			case BlockType::CyanBanner:
			case BlockType::GrayBanner:
			case BlockType::GreenBanner:
			case BlockType::LightBlueBanner:
			case BlockType::LightGrayBanner:
			case BlockType::LimeBanner:
			case BlockType::MagentaBanner:
			case BlockType::OrangeBanner:
			case BlockType::PinkBanner:
			case BlockType::PurpleBanner:
			case BlockType::RedBanner:
			case BlockType::WhiteBanner:
			case BlockType::YellowBanner:

			case BlockType::BlackWallBanner:
			case BlockType::BlueWallBanner:
			case BlockType::BrownWallBanner:
			case BlockType::CyanWallBanner:
			case BlockType::GrayWallBanner:
			case BlockType::GreenWallBanner:
			case BlockType::LightBlueWallBanner:
			case BlockType::LightGrayWallBanner:
			case BlockType::LimeWallBanner:
			case BlockType::MagentaWallBanner:
			case BlockType::OrangeWallBanner:
			case BlockType::PinkWallBanner:
			case BlockType::PurpleWallBanner:
			case BlockType::RedWallBanner:
			case BlockType::WhiteWallBanner:
			case BlockType::YellowWallBanner:
			case BlockType::JackOLantern:
			case BlockType::Melon:

			case BlockType::CreeperHead:
			case BlockType::CreeperWallHead:
			case BlockType::DragonHead:
			case BlockType::DragonWallHead:
			case BlockType::PlayerHead:
			case BlockType::PlayerWallHead:
			case BlockType::ZombieHead:
			case BlockType::ZombieWallHead:
			case BlockType::SkeletonSkull:
			case BlockType::SkeletonWallSkull:
			case BlockType::WitherSkeletonSkull:
			case BlockType::WitherSkeletonWallSkull:

			case BlockType::NetherWartBlock:
			case BlockType::Pumpkin:

			case BlockType::AcaciaSign:
			case BlockType::AcaciaWallSign:
			case BlockType::BirchSign:
			case BlockType::BirchWallSign:
			case BlockType::CrimsonSign:
			case BlockType::CrimsonWallSign:
			case BlockType::DarkOakSign:
			case BlockType::DarkOakWallSign:
			case BlockType::JungleSign:
			case BlockType::JungleWallSign:
			case BlockType::OakSign:
			case BlockType::OakWallSign:
			case BlockType::SpruceSign:
			case BlockType::SpruceWallSign:
			case BlockType::WarpedSign:
			case BlockType::WarpedWallSign:
				return 0.39f;
			case BlockType::QuartzBlock:
			case BlockType::QuartzStairs:
			case BlockType::RedSandstone:
			case BlockType::RedSandstoneStairs:
			case BlockType::Sandstone:
			case BlockType::SandstoneStairs:

			case BlockType::BlackWool:
			case BlockType::BlueWool:
			case BlockType::BrownWool:
			case BlockType::CyanWool:
			case BlockType::GrayWool:
			case BlockType::GreenWool:
			case BlockType::LightBlueWool:
			case BlockType::LightGrayWool:
			case BlockType::LimeWool:
			case BlockType::MagentaWool:
			case BlockType::OrangeWool:
			case BlockType::PinkWool:
			case BlockType::PurpleWool:
			case BlockType::RedWool:
			case BlockType::WhiteWool:
			case BlockType::YellowWool:
				return 0.33f;
			case BlockType::InfestedChiseledStoneBricks:
			case BlockType::InfestedCobblestone:
			case BlockType::InfestedCrackedStoneBricks:
			case BlockType::InfestedMossyStoneBricks:
			case BlockType::InfestedStone:
				return 0.315f;
			case BlockType::ActivatorRail:
			case BlockType::DetectorRail:
			case BlockType::PoweredRail:
			case BlockType::Rail: return 0.3f;
			case BlockType::DirtPath:
			case BlockType::Clay:
			case BlockType::Farmland:
			case BlockType::GrassBlock:
			case BlockType::Gravel:
			case BlockType::Sponge:
			case BlockType::WetSponge: return 0.27f;
			case BlockType::BrewingStand:

			case BlockType::AcaciaButton:
			case BlockType::BirchButton:
			case BlockType::CrimsonButton:
			case BlockType::DarkOakButton:
			case BlockType::JungleButton:
			case BlockType::OakButton:
			case BlockType::PolishedBlackstoneButton:
			case BlockType::SpruceButton:
			case BlockType::StoneButton:
			case BlockType::WarpedButton:

			case BlockType::Cake:

			case BlockType::BlackConcretePowder:
			case BlockType::BlueConcretePowder:
			case BlockType::BrownConcretePowder:
			case BlockType::CyanConcretePowder:
			case BlockType::GrayConcretePowder:
			case BlockType::GreenConcretePowder:
			case BlockType::LightBlueConcretePowder:
			case BlockType::LightGrayConcretePowder:
			case BlockType::LimeConcretePowder:
			case BlockType::MagentaConcretePowder:
			case BlockType::OrangeConcretePowder:
			case BlockType::PinkConcretePowder:
			case BlockType::PurpleConcretePowder:
			case BlockType::RedConcretePowder:
			case BlockType::WhiteConcretePowder:
			case BlockType::YellowConcretePowder:

			case BlockType::Dirt:
			case BlockType::FrostedIce:
			case BlockType::HayBlock:
			case BlockType::Ice: return 0.24f;
			default: ASSERT("BLOCK MIGHT NOT HAVE RIGHT EXPLOSION ABSORPTION"); return 0.09f;
		}
	}

	/** Calculates the approximate percentage of an Entity's bounding box that is exposed to an explosion centred at Position. */
	static float CalculateEntityExposure(const cChunk & a_Chunk, const cEntity & a_Entity, const Vector3f a_Position, const int a_SquareRadius)
	{
		class LineOfSightCallbacks final : public cLineBlockTracer::cCallbacks
		{
			virtual bool OnNextBlock(Vector3i a_BlockPos, BlockState a_Block, eBlockFace a_EntryFace) override
			{
				return a_Block.Type() != BlockType::Air;
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
	static void DamageEntities(const cChunk & a_Chunk, const Vector3f a_Position, const int a_Power)
	{
		const auto Radius = a_Power * 2;
		const auto SquareRadius = Radius * Radius;

		a_Chunk.GetWorld()->ForEachEntityInBox({ a_Position, Radius * 2.f }, [&a_Chunk, a_Position, a_Power, Radius, SquareRadius](cEntity & Entity)
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
	static bool BlockAlwaysDrops(const BlockState a_Block)
	{
		if (IsBlockShulkerBox(a_Block))
		{
			return true;
		}

		switch (a_Block.Type())
		{
			case BlockType::DragonEgg:
			case BlockType::Beacon:
			case BlockType::CreeperHead:
			case BlockType::CreeperWallHead:
			case BlockType::DragonHead:
			case BlockType::DragonWallHead:
			case BlockType::PlayerHead:
			case BlockType::PlayerWallHead:
			case BlockType::ZombieHead:
			case BlockType::ZombieWallHead:
			case BlockType::SkeletonSkull:
			case BlockType::SkeletonWallSkull:
			case BlockType::WitherSkeletonSkull:
			case BlockType::WitherSkeletonWallSkull:
				return true;
			default: return false;
		}
	}

	/** Sets the block at the given position, updating surroundings. */
	static void SetBlock(cWorld & a_World, cChunk & a_Chunk, const Vector3i a_AbsolutePosition, const Vector3i a_RelativePosition, const BlockState a_DestroyedBlock, const BlockState a_NewBlock, const cEntity * const a_ExplodingEntity)
	{
		// SetBlock wakes up all simulators for the area, so that water and lava flows and sand falls into the blasted holes
		// It also is responsible for calling cBlockHandler::OnNeighborChanged to pop off blocks that fail CanBeAt
		// An explicit call to cBlockHandler::OnBroken handles the destruction of multiblock structures
		// References at (FS #391, GH #4418):
		a_Chunk.SetBlock(a_RelativePosition, a_NewBlock);

		cChunkInterface Interface(a_World.GetChunkMap());
		cBlockHandler::For(a_DestroyedBlock.Type()).OnBroken(Interface, a_World, a_AbsolutePosition, a_DestroyedBlock, a_ExplodingEntity);
	}

	/** Work out what should happen when an explosion destroys the given block.
	Tasks include lighting TNT, dropping pickups, setting fire and flinging shrapnel according to Minecraft rules.
	OK, _mostly_ Minecraft rules. */
	static void DestroyBlock(cChunk & a_Chunk, const Vector3i a_Position, const int a_Power, const bool a_Fiery, const cEntity * const a_ExplodingEntity)
	{
		const auto DestroyedBlock = a_Chunk.GetBlock(a_Position);
		if (IsBlockAir(DestroyedBlock))
		{
			// There's nothing left for us here, but a barren and empty land
			// Let's go.
			return;
		}

		auto & World = *a_Chunk.GetWorld();
		auto & Random = GetRandomProvider();
		const auto Absolute = cChunkDef::RelativeToAbsolute(a_Position, a_Chunk.GetPos());

		if (DestroyedBlock == BlockType::Tnt)  // If the block is TNT we should set it off
		{
			// Random fuse between 10 to 30 game ticks.
			const int FuseTime = Random.RandInt(10, 30);

			// Activate the TNT, with initial velocity and no fuse sound:
			World.SpawnPrimedTNT(Vector3d(0.5, 0, 0.5) + Absolute, FuseTime, 1, false);
		}
		else if ((a_ExplodingEntity != nullptr) && (a_ExplodingEntity->IsTNT() || BlockAlwaysDrops(DestroyedBlock) || Random.RandBool(1.f / a_Power)))  // For TNT explosions, destroying a block that always drops, or if RandBool, drop pickups
		{
			a_Chunk.GetWorld()->SpawnItemPickups(cBlockHandler::For(DestroyedBlock.Type()).ConvertToPickups(DestroyedBlock), Absolute);
		}
		else if (a_Fiery && Random.RandBool(1 / 3.0))  // 33% chance of starting fires if it can start fires
		{
			const auto Below = a_Position.addedY(-1);
			if ((Below.y >= 0) && cBlockInfo::FullyOccupiesVoxel(a_Chunk.GetBlock(Below)))
			{
				// Start a fire:
				SetBlock(World, a_Chunk, Absolute, a_Position, DestroyedBlock, Block::Fire::Fire(), a_ExplodingEntity);
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
				auto FallingBlock = std::make_unique<cFallingBlock>(Vector3d(0.5, 0, 0.5) + Absolute, DestroyedBlock);
				// TODO: correct velocity FallingBlock->SetSpeedY(40);
				FallingBlock->Initialize(std::move(FallingBlock), World);
			}
		}

		SetBlock(World, a_Chunk, Absolute, a_Position, DestroyedBlock, Block::Air::Air(), a_ExplodingEntity);
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

	void Kaboom(cWorld & a_World, const Vector3f a_Position, const int a_Power, const bool a_Fiery, const cEntity * const a_ExplodingEntity)
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
