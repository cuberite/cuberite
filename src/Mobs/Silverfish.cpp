
#include "Globals.h"

#include "Silverfish.h"

#include "../World.h"
#include "../Chunk.h"
#include "../Blocks/BlockHandler.h"
#include "../Blocks/BlockInfested.h"





cSilverfish::cSilverfish() :
	Super("Silverfish", mtSilverfish, "entity.silverfish.hurt", "entity.silverfish.death", "entity.silverfish.ambient", 0.4f, 0.3f)
{
}





bool cSilverfish::DoTakeDamage(TakeDamageInfo &a_TDI)
{
	// Call on our brethren to attack!
	// TODO: stop this if /gamerule mobGriefing is set to false

	// If the entity didn't take any damage, bail:
	if (!Super::DoTakeDamage(a_TDI))
	{
		return false;
	}

	// Or, conversely took lethal damage, bail:
	if (m_Health <= 0)
	{
		return true;
	}

	if (a_TDI.Attacker == nullptr)
	{
		if ((a_TDI.DamageType != dtPoison) && (a_TDI.DamageType != dtPotionOfHarming))
		{
			// Bail if attacker doesn't exist and it wasn't a splash potion:
			return true;
		}
	}
	else if (!a_TDI.Attacker->IsPlayer())
	{
		// Bail if it wasn't a player attack:
		return true;
	}

	auto & Random = GetRandomProvider();

	// Tries to spawn another Silverfish, returning if the search should continue.
	auto CheckInfested = [this, &Random, Position = GetPosition().Floor()](const Vector3i Offset) mutable
	{
		const auto Block = Position + Offset;
		if (cBlockInfestedHandler::IsBlockInfested(m_World->GetBlock(Block)))
		{
			m_World->DigBlock(Block);
			return Random.RandBool();
		}
		return false;
	};

	// Search the faces of an increasingly large cube (so the positions closest get looked at first)
	// of min 3, max 10, for infested blocks and spawn additional reinforcements:
	for (int CubeSideLength = 3; CubeSideLength <= 10; CubeSideLength += 2)
	{
		const int HalfSide = CubeSideLength / 2;

		for (int OffsetX = -HalfSide; OffsetX <= HalfSide; OffsetX++)
		{
			for (int OffsetZ = -HalfSide; OffsetZ <= HalfSide; OffsetZ++)
			{
				if (CheckInfested({ OffsetX, +HalfSide, OffsetZ }) || CheckInfested({ OffsetX, -HalfSide, OffsetZ }))
				{
					return true;
				}
			}
		}

		for (int OffsetX = -HalfSide; OffsetX <= HalfSide; OffsetX++)
		{
			for (int OffsetY = -HalfSide + 1; OffsetY <= HalfSide - 1; OffsetY++)
			{
				if (CheckInfested({ OffsetX, OffsetY, +HalfSide }) || CheckInfested({ OffsetX, OffsetY, -HalfSide }))
				{
					return true;
				}
			}
		}

		for (int OffsetZ = -HalfSide + 1; OffsetZ <= HalfSide - 1; OffsetZ++)
		{
			for (int OffsetY = -HalfSide + 1; OffsetY <= HalfSide - 1; OffsetY++)
			{
				if (CheckInfested({ +HalfSide, OffsetY, OffsetZ }) || CheckInfested({ -HalfSide, OffsetY, OffsetZ }))
				{
					return true;
				}
			}
		}
	}

	return true;
}
