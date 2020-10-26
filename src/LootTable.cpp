
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "LootTable.h"
#include "LootTableParser.h"

#include "BlockEntities/ChestEntity.h"
#include "Entities/Player.h"
#include "Registries/ItemTags.h"


////////////////////////////////////////////////////////////////////////////////
// cLootTable


cLootTable::cLootTable(const Json::Value & a_Description)
{
	for (const auto & RootId : a_Description.getMemberNames())
	{
		if (NoCaseCompare(RootId, "pools") == 0)
		{
			const Json::Value & Pools = a_Description[RootId];
			if (!Pools.isArray())
			{
				LOGWARNING("Loot table: Didn't get a array while parsing loot table pools, dropping loot table!");
				return;
			}
			for (unsigned int PoolId = 0; PoolId < Pools.size(); PoolId++)
			{
				m_LootTablePools.emplace_back(LootTable::ParsePool(Pools[PoolId]));
			}
		}
		else if (NoCaseCompare(RootId, "functions") == 0)
		{
			const auto & FunctionsObject = a_Description[RootId];
			if (!FunctionsObject.isArray())
			{
				LOGWARNING("Loot table: Didn't get an array while parsing loot table wide functions, dropping functions!");
				continue;
			}
			for (unsigned int FunctionIndex = 0; FunctionIndex < FunctionsObject.size(); FunctionIndex++)
			{
				if (FunctionsObject[FunctionIndex].empty() || FunctionsObject[FunctionIndex].isArray())
				{
					LOGWARNING("Loot table: Encountered a problem to while parsing loot table wide functions, dropping function!");
					continue;
				}
				m_Functions.emplace_back(LootTable::ParseFunction(FunctionsObject[FunctionIndex]));
			}
		}
	}
}





bool cLootTable::FillWithLoot(cItemGrid & a_ItemGrid, cWorld & a_World, const Vector3i a_Pos, const UInt32 a_PlayerID, int a_Seed = 0) const
{
	if (a_Seed == 0)
	{
		a_Seed = a_World.GetSeed();
	}
	auto Noise = cNoise(a_Seed);

	// The player is killed and killer here because he influences both in the functions or conditions
	auto DamageInfo = TakeDamageInfo();
	auto Items = GetItems(Noise, a_Pos, a_World, a_PlayerID, a_PlayerID, DamageInfo);

	// Places items in a_ItemGrid
	int I = 0;  // This value is used for some more randomness
	for (auto & Item : Items)
	{
		cItem Copy;
		while (Item.m_ItemCount > 0)
		{
			Copy = Item.CopyOne();
			a_ItemGrid.AddItem(Copy, true, abs((Noise.IntNoise3DInt(a_Pos) * Noise.IntNoise1DInt(I)) % a_ItemGrid.GetNumSlots()));
			Item.m_ItemCount--;
			I++;
		}
	}
	return true;
}





cItems cLootTable::GetItems(const cNoise & a_Noise, const Vector3i & a_Pos, cWorld & a_World, UInt32 a_KilledID, UInt32 a_KillerID, const TakeDamageInfo & a_DamageSource, int a_ExplosionSize) const
{
	auto Items = cItems();
	for (const auto & Pool : m_LootTablePools)
	{
		Items = GetItems(Pool, a_World, a_Noise, a_Pos, a_KilledID, a_KillerID, a_DamageSource, a_ExplosionSize);
	}
	for (auto & Item : Items)
	{
		for (const auto & Function : m_Functions)
		{
			ApplyFunction(Function, Item, a_World, a_Noise, a_Pos, a_KilledID, a_KillerID, a_DamageSource, a_ExplosionSize);
		}
	}
	return Items;
}





cItems cLootTable::GetItems(const LootTable::cLootTablePool & a_Pool, cWorld & a_World, const cNoise & a_Noise, const Vector3i & a_Pos, UInt32 a_KilledID, UInt32 a_KillerID, const TakeDamageInfo & a_DamageSource, int a_ExplosionSize)
{
	auto Items = cItems();
	if (!ConditionsApply(a_Pool.m_Conditions, a_World, a_Noise, a_Pos, a_KilledID, a_KillerID, a_DamageSource, a_ExplosionSize))
	{
		return Items;
	}

	int Luck = 0;
	/*  TODO: Luck 16.09.2020 - 12xx12
	auto Callback = [&] (cPlayer & a_Player)
	{
		Luck = a_Player.GetLuck();
	};
	a_World.DoWithPlayerByUUID(a_Player, Callback);
	*/

	// Determines the total weight manipulated by the quality
	int TotalWeight = a_Pool.m_TotalWeight + Luck * a_Pool.m_TotalQuality;
	int TotalRolls = a_Pool.m_Rolls(a_Noise, a_Pos) + FloorC(a_Pool.m_BonusRolls(a_Noise, a_Pos) * Luck);
	size_t EntryNum;

	for (int i = 0; i < TotalRolls; i++)
	{
		auto Rnd = a_Noise.IntNoise3DInt(a_Pos * 25 * a_Noise.IntNoise1DInt(i)) % TotalWeight;
		EntryNum = 0;
		do
		{
			Rnd -= a_Pool.m_Entries[EntryNum].m_Weight + a_Pool.m_Entries[EntryNum].m_Quality * Luck;
			EntryNum = (EntryNum + 1) % a_Pool.m_Entries.size();
		} while (Rnd > 0);
		const auto & Entry = a_Pool.m_Entries[EntryNum];
		auto NewItems = GetItems(Entry, a_World, a_Noise, a_Pos, a_KilledID, a_KillerID, a_DamageSource, a_ExplosionSize);
		Items.insert(Items.end(), NewItems.begin(), NewItems.end());
	}
	return Items;
}





cItems cLootTable::GetItems(const LootTable::cLootTablePoolEntry & a_Entry, cWorld & a_World, const cNoise & a_Noise, const Vector3i & a_Pos, UInt32 a_KilledID, UInt32 a_KillerID, const TakeDamageInfo & a_DamageSource, int a_ExplosionSize)
{
	auto Items = cItems();

	if (!ConditionsApply(a_Entry.m_Conditions, a_World, a_Noise, a_Pos, a_KilledID, a_KillerID, a_DamageSource, a_ExplosionSize))
	{
		return Items;
	}

	switch (a_Entry.m_Type)
	{
		case LootTable::ePoolEntryType::Item:
		{
			try
			{
				Items.push_back(cItem(std::get<cItem>(a_Entry.m_Content)));
			}
			catch (const std::bad_variant_access &)
			{
				LOGWARNING("Loot table: Unsupported Data type in loot table pool - dropping entry");
			}
			break;
		}
		case LootTable::ePoolEntryType::Tag:
		{
			AString Tag;
			try
			{
				Tag = std::get<AString>(a_Entry.m_Content);
			}
			catch (const std::bad_variant_access &)
			{
				LOGWARNING("Loot table: Unsupported Data type in loot table pool - dropping entry");
				break;
			}

			cItems TagItems = ItemTag::GetItems(ItemTag::eItemTags(Tag));

			if (a_Entry.m_Expand)
			{
				Items.Add(TagItems[static_cast<size_t>(a_Noise.IntNoise3DInt(a_Pos * static_cast<int>(TagItems.size())) % static_cast<int>(TagItems.size()))]);
			}
			else
			{
				Items.insert(Items.begin(), TagItems.begin(), TagItems.end());
			}
			break;
		}
		case LootTable::ePoolEntryType::LootTable:  // Grants items based on the specified loot table
		{
			AString Loot;
			try
			{
				Loot = std::get<AString>(a_Entry.m_Content);
			}
			catch (const std::bad_variant_access &)
			{
				LOGWARNING("Loot table: Unsupported Data type in loot table pool - dropping entry");
				break;
			}

			auto NewItems = a_World.GetLootTableProvider()->GetLootTable(Loot)->GetItems(a_Noise, a_Pos, a_World, a_KilledID, a_KillerID, a_DamageSource);
			Items.insert(Items.end(), NewItems.begin(), NewItems.end());
			break;
		}
		case LootTable::ePoolEntryType::Group:  // Grants all listed entries
		{
			try
			{
				for (const auto & Child : std::get<LootTable::cLootTablePoolEntries>(a_Entry.m_Content))
				{
					auto NewItems = GetItems(Child, a_World, a_Noise, a_Pos, a_KilledID, a_KillerID, a_DamageSource, a_ExplosionSize);
					Items.insert(Items.end(), NewItems.begin(), NewItems.end());
				}
			}
			catch (const std::bad_variant_access &)
			{
				LOGWARNING("Loot table: Unsupported Data type in loot table pool - dropping entry");
			}
			break;
		}
		case LootTable::ePoolEntryType::Alternatives:  // Grants one of the listed entries
		{
			try
			{
				auto Children = std::get<LootTable::cLootTablePoolEntries>(a_Entry.m_Content);
				auto ChildPos = a_Noise.IntNoise3DInt(a_Pos * static_cast<int>(Children.size())) % static_cast<int>(Children.size());
				auto NewItems = GetItems(Children[static_cast<size_t>(ChildPos)], a_World, a_Noise, a_Pos, a_KilledID, a_KillerID, a_DamageSource, a_ExplosionSize);
				Items.insert(Items.end(), NewItems.begin(), NewItems.end());
			}
			catch (const std::bad_variant_access &)
			{
				LOGWARNING("Loot table: Unsupported Data type in loot table pool - dropping entry");
			}
			break;
		}
		case LootTable::ePoolEntryType::Sequence:  // Selects entries from Children until one is not granted.
		{
			LootTable::cLootTablePoolEntries Children;
			try
			{
				Children = std::get<LootTable::cLootTablePoolEntries>(a_Entry.m_Content);
			}
			catch (const std::bad_variant_access &)
			{
				LOGWARNING("Loot table: Unsupported Data type in loot table pool - dropping entry!");
				break;
			}
			cItems NewItems;
			int LockCont = 0;  // This is how many times the whole table is repeated to prevent infinite loops
			unsigned int ChildPos = 0;
			do
			{
				NewItems = GetItems(Children[ChildPos], a_World, a_Noise, a_Pos, a_KilledID, a_KillerID, a_DamageSource, a_ExplosionSize);
				Items.insert(Items.end(), NewItems.begin(), NewItems.end());
				ChildPos = (ChildPos + 1) % Children.size();
				if (ChildPos == Children.size() - 1)
				{
					LockCont++;
				}
			} while ((NewItems.size() != 0) && (LockCont < 10));

			break;
		}
		case LootTable::ePoolEntryType::Dynamic:  // Generates loot based on the block broken: chest content...
		{
			if (!cBlockEntity::IsBlockEntityBlockType(a_World.GetBlock(a_Pos)))
			{
				break;
			}
			bool IsSelf;
			try
			{
				IsSelf = std::get<bool>(a_Entry.m_Content);
			}
			catch (const std::bad_variant_access &)
			{
				LOGWARNING("Loot table: Unsupported Data type in loot table pool - dropping entry!");
				break;
			}

			if (!IsSelf)
			{
				// Drops contents of chest or similar...
				auto Block = a_World.GetBlock(a_Pos);
				switch (Block)
				{
					case E_BLOCK_BEACON:
					case E_BLOCK_BREWING_STAND:
					case E_BLOCK_DISPENSER:
					case E_BLOCK_DROPPER:
					case E_BLOCK_ENCHANTMENT_TABLE:
					case E_BLOCK_FURNACE:
					case E_BLOCK_LIT_FURNACE:
					case E_BLOCK_HOPPER:
					case E_BLOCK_CHEST:
					case E_BLOCK_TRAPPED_CHEST:
					{
						a_World.DoWithBlockEntityAt(a_Pos.x, a_Pos.y, a_Pos.z, [&] (cBlockEntity & a_Entity)
						{
							Items = a_Entity.ConvertToPickups();
							return true;
						});
					}
					default: break;
				}
				break;
			}
			else
			{
				// Drops the block itself...
				auto Block = a_World.GetBlock(a_Pos);
				switch (Block)
				{
					case E_BLOCK_BEACON:
					case E_BLOCK_BREWING_STAND:
					case E_BLOCK_DISPENSER:
					case E_BLOCK_DROPPER:
					case E_BLOCK_ENCHANTMENT_TABLE:
					case E_BLOCK_FURNACE:
					case E_BLOCK_LIT_FURNACE:
					case E_BLOCK_HOPPER:
					case E_BLOCK_CHEST:
					case E_BLOCK_TRAPPED_CHEST:
					{
						Items.Add(cItem(Block));
					}
					default:
					{
						a_World.DoWithBlockEntityAt(a_Pos.x, a_Pos.y, a_Pos.z, [&] (cBlockEntity & a_Entity)
						{
							Items = a_Entity.ConvertToPickups();
							return true;
						});
					}
				}
			}
			break;
		}
		case LootTable::ePoolEntryType::Empty:
		{
			break;
		}
	}

	for (auto & Item : Items)
	{
		for (const auto & Function : a_Entry.m_Functions)
		{
			ApplyFunction(Function, Item, a_World, a_Noise, a_Pos, a_KilledID, a_KillerID, a_DamageSource, a_ExplosionSize);
		}
	}
	return Items;
}





bool cLootTable::ConditionsApply(const LootTable::cLootTableConditions & a_Conditions, cWorld & a_World, const cNoise & a_Noise, const Vector3i a_Pos, UInt32 a_KilledID, UInt32 a_KillerID, const TakeDamageInfo & a_DamageSource, int a_ExplosionSize)
{
	bool Success = true;
	for (const auto & Condition : a_Conditions)
	{
		Success &= ConditionApplies(Condition, a_World, a_Noise, a_Pos, a_KilledID, a_KillerID, a_DamageSource, a_ExplosionSize);
	}
	return Success;
}





bool cLootTable::ConditionApplies(const LootTable::cLootTableCondition & a_Condition, cWorld & a_World, const cNoise & a_Noise, const Vector3i a_Pos, UInt32 a_KilledID, UInt32 a_KillerID, const TakeDamageInfo & a_DamageSource, int a_ExplosionSize)
{
	return std::visit(VISITCONDITION, a_Condition.m_Parameter);
}





void cLootTable::ApplyFunction(const LootTable::cLootTableFunction & a_Function, cItem & a_Item, cWorld & a_World, const cNoise & a_Noise, const Vector3i & a_Pos, UInt32 a_KilledID, UInt32 a_KillerID, const TakeDamageInfo & a_DamageSource, int a_ExplosionSize)
{
	if (!ConditionsApply(a_Function.m_Conditions, a_World, a_Noise, a_Pos, a_KilledID, a_KillerID, a_DamageSource, a_ExplosionSize))
	{
		return;
	}

	std::visit(VISITFUNCTION, a_Function.m_Function);
}




