
#include "Globals.h"

#include "Silverfish.h"

#include "../World.h"
#include "../Chunk.h"
#include "../Blocks/BlockHandler.h"
#include "../Blocks/BlockInfested.h"

bool cSilverfish::DoTakeDamage(TakeDamageInfo &a_TDI)
{
	bool SuperResult = Super::DoTakeDamage(a_TDI);
	// Todo: stop this if /gamerule mobGriefing is set to false

	// If the entity didn't take andy damage
	if (!SuperResult)
	{
		return SuperResult;
	}

	// Entity does receive lethal damage or Attacker doesn't exist
	if ((m_Health < a_TDI.FinalDamage) ||
		((a_TDI.Attacker == nullptr) && (a_TDI.DamageType != dtPoison) && (a_TDI.DamageType != dtPotionOfHarming)))
	{
		return SuperResult;
	}

	// If attacker is player or splash potion
	bool ShouldSpawn = (
		(a_TDI.DamageType == dtPoison) || (a_TDI.DamageType == dtPotionOfHarming) ||
		a_TDI.Attacker->IsPlayer()
	);

	if (!ShouldSpawn)
	{
		return SuperResult;
	}
	auto Blocks = sSetBlockVector();
	for (int X = static_cast<int>(GetPosX() - 10); X <= static_cast<int>(GetPosX() + 10); X++)
	{
		for (int Y = static_cast<int>(GetPosY() - 5); Y <= static_cast<int>(GetPosY() + 5); Y++)
		{
			for (int Z = static_cast<int>(GetPosZ() - 10); Z <= static_cast<int>(GetPosZ() + 10); Z++)
			{
				Blocks.emplace_back(sSetBlock({X, Y, Z}, 0, 0));
			}
		}
	}
	m_World->GetBlocks(Blocks, true);
	for (const auto & BlockInfo : Blocks)
	{
		if (BlockInfo.m_BlockType == E_BLOCK_SILVERFISH_EGG)
		{
			m_World->DigBlock(BlockInfo.GetAbsolutePos(), nullptr);
		}
	}

	return SuperResult;
}
