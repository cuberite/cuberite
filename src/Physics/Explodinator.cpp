
#include "Globals.h"
#include "Chunk.h"
#include "ClientHandle.h"
#include "BlockArea.h"
#include "BlockInfo.h"
#include "Simulator/SandSimulator.h"
#include "World.h"





namespace Explodinator
{
	const auto StepUnit = 0.3f;
	const auto StepAttenuation = 0.225f;
	const auto TraceCubeSideLength = 16U;

	inline void DamageEntities(cWorld & World, const Vector3f Position, const unsigned Power)
	{

	}

	inline float GetAbsorption(const BLOCKTYPE Block)
	{
		switch (Block)
		{
			case E_BLOCK_AIR: return 0;
			case E_BLOCK_BEDROCK:
			case E_BLOCK_OBSIDIAN: return 3600000;
			default: return 1;
		}
	}

	inline void DestroyBlock(cWorld & World, cBlockArea & Area, MTRand & Random, const Vector3i Position, const unsigned Power, const bool Fiery)
	{
		const auto Destroyed = Area.GetBlockType(Position.x, Position.y, Position.z);
		if (Destroyed == E_BLOCK_AIR)
		{
			// There's nothing left for us here, but a barren and empty land
			// Let's go.
			return;
		}

		if (Destroyed == E_BLOCK_TNT)
		{
			// Random fuse between 10 to 30 game ticks.
			const int FuseTime = Random.RandInt(10, 30);

			// Activate the TNT, with initial velocity and no fuse sound:
			World.SpawnPrimedTNT(Position + Vector3d(0.5, 0, 0.5), FuseTime, 1, false);
		}
		else if (Random.RandBool(1.f / Power))
		{
			World.SpawnItemPickups(Area.PickupsFromBlock(Position), Position);
		}
		else if (Fiery && Random.RandBool(1.f / 3.f))  // 33% chance of starting fires if it can start fires
		{
			// Start a fire, depend on the fire simulator to extinguish invalid positions:
			Area.SetBlockTypeMeta(Position.x, Position.y, Position.z, E_BLOCK_FIRE, 0);
			return;
		}
		else if (const auto Shrapnel = World.GetTNTShrapnelLevel(); (Shrapnel > slNone) && Random.RandBool(0.20))  // 20% chance of flinging stuff around
		{
			// If the block is shrapnel-able, make a falling block entity out of it:
			if (
				((Shrapnel == slAll) && cBlockInfo::FullyOccupiesVoxel(Destroyed)) ||
				((Shrapnel == slGravityAffectedOnly) && cSandSimulator::IsAllowedBlock(Destroyed))
			)
			{
				World.SpawnFallingBlock(Position, Destroyed, Area.GetBlockMeta(Position.x, Position.y, Position.z));
			}
		}

		Area.SetBlockTypeMeta(Position.x, Position.y, Position.z, E_BLOCK_AIR, 0);
	}

	inline void DestructionTrace(cWorld & World, cBlockArea & Area, MTRand & Random, Vector3f Origin, const Vector3f Destination, const unsigned Power, const bool Fiery, float Intensity)
	{
		const auto Step = (Destination - Origin).NormalizeCopy() * StepUnit;

		while ((Destination - Origin).SqrLength() > 0)
		{
			Origin += Step;
			Intensity -= 0.225f;

			const auto Position = Origin.Floor();
			Intensity -= 0.3f * (0.3f + GetAbsorption(Area.GetBlockType(Position.x, Position.y, Position.z)));

			if (Intensity <= 0)
			{
				break;
			}

			DestroyBlock(World, Area, Random, Position, Power, Fiery);
		}
	}

	inline void DamageBlocks(cWorld & World, const Vector3f Position, const unsigned Power, const bool Fiery)
	{
		auto & Random = GetRandomProvider();
		const auto Intensity = Power * (0.7f + Random.RandReal(0.6f));
		const auto ExplosionRadius = CeilC((Intensity / StepAttenuation) * StepUnit);
		const auto Direction = Vector3i(ExplosionRadius, ExplosionRadius, ExplosionRadius);

		auto Maximum = Position.Ceil() + Direction;
		auto Minimum = Position.Floor() - Direction;

		Minimum.y = std::max(Minimum.y, 0);
		Maximum.y = std::min(Maximum.y, cChunkDef::Height - 1);

		cBlockArea Area;
		if (!Area.Read(World, Minimum, Maximum))
		{
			return;
		}

		// Oh boy... Better hope you have a hot cache, 'cos this little manoeuvre's gonna cost us 1352 raytraces in one tick...
		const auto HalfSide = TraceCubeSideLength / 2.f;

		for (float OffsetX = -HalfSide; OffsetX < HalfSide; OffsetX++)
		{
			for (float OffsetZ = -HalfSide; OffsetZ < HalfSide; OffsetZ++)
			{
				DestructionTrace(World, Area, Random, Position, Position + Vector3f(OffsetX, +ExplosionRadius, OffsetZ), Power, Fiery, Intensity);
				DestructionTrace(World, Area, Random, Position, Position + Vector3f(OffsetX, -ExplosionRadius, OffsetZ), Power, Fiery, Intensity);
			}
		}

		// TODO: not symmetric
		for (float Offset = -HalfSide; Offset < HalfSide - 1; Offset++)
		{
			for (float OffsetY = -HalfSide + 1; OffsetY < HalfSide - 1; OffsetY++)
			{
				DestructionTrace(World, Area, Random, Position, Position + Vector3f(+ExplosionRadius, OffsetY, Offset), Power, Fiery, Intensity);
				DestructionTrace(World, Area, Random, Position, Position + Vector3f(-ExplosionRadius, OffsetY, Offset), Power, Fiery, Intensity);
				DestructionTrace(World, Area, Random, Position, Position + Vector3f(Offset, OffsetY, +ExplosionRadius), Power, Fiery, Intensity);
				DestructionTrace(World, Area, Random, Position, Position + Vector3f(Offset, OffsetY, -ExplosionRadius), Power, Fiery, Intensity);
			}
		}

		Area.Write(World, Minimum);

		// Wake up all simulators for the area, so that water and lava flows and sand falls into the blasted holes (FS #391):
		World.GetSimulatorManager()->WakeUp({ Minimum, Maximum });
	}

	inline void LagTheClient(cWorld & World, const Vector3f Position, const unsigned Power)
	{
		World.DoWithChunkAt(Position.Floor(), [Position, Power](cChunk & Chunk)
		{
			for (const auto Client : Chunk.GetAllClients())
			{
				Client->SendExplosion(Position, Power);
			}

			return false;
		});
	}

	void Kaboom(cWorld & World, const Vector3f Position, const unsigned Power, const bool Fiery)
	{
		LagTheClient(World, Position, Power);
		DamageEntities(World, Position, Power);
		DamageBlocks(World, Position, Power, Fiery);
	}
}
