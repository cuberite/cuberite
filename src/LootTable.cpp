
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "LootTable.h"

#include "Entities/Player.h"
#include "FurnaceRecipe.h"
#include "LootTableParser.h"
#include "Registries/ItemTags.h"
#include "Root.h"

using namespace LootTable;

////////////////////////////////////////////////////////////////////////////////
// cLootTable


cLootTable::cLootTable(const Json::Value & a_Description)
{
	for (const auto & RootId : a_Description.getMemberNames())
	{
		if (NoCaseCompare(RootId, "pools") == 0)
		{
			Json::Value Pools = a_Description[RootId];
			for (unsigned int PoolId = 0; PoolId < Pools.size(); PoolId++)
			{
				m_LootTablePools.push_back(LootTable::ParsePool(Pools[PoolId]));
			}
		}
		else if (NoCaseCompare(RootId, "functions") == 0)
		{
			auto FunctionsObject = a_Description[RootId];
			for (unsigned int FunctionIndex = 0; FunctionIndex < FunctionsObject.size(); FunctionIndex++)
			{
				m_Functions.push_back(LootTable::ParseFunction(FunctionsObject[FunctionIndex]));
			}
		}
	}
}





bool cLootTable::FillWithLoot(cItemGrid & a_ItemGrid, cWorld & a_World, const Vector3i a_Pos, const UInt32 a_PlayerID) const
{
	auto Noise = cNoise(a_World.GetGenerator().GetSeed());

	// The player is killed and killer here because he influences both in the functions or conditions
	auto Items = GetItems(Noise, a_Pos, a_World, a_PlayerID, a_PlayerID);

	// Places items in a_ItemGrid
	int i = 0;  // This value is used for some more randomness
	for (auto & Item : Items)
	{
		cItem Copy;
		while (Item.m_ItemCount > 0)
		{
			Copy = Item.CopyOne();
			a_ItemGrid.AddItem(Copy, true, abs((Noise.IntNoise3DInt(a_Pos) * Noise.IntNoise1DInt(i)) % a_ItemGrid.GetNumSlots()));
			Item.m_ItemCount--;
			i++;
		}
	}
	return true;
}





cItems cLootTable::GetItems(const cNoise & a_Noise, const Vector3i & a_Pos, cWorld & a_World, UInt32 a_KilledID, UInt32 a_KillerID) const
{
	auto Items = cItems();
	for (const auto & Pool : m_LootTablePools)
	{
		auto NewItems = GetItems(Pool, a_World, a_Noise, a_Pos, a_KilledID, a_KillerID);
		Items.insert(Items.end(), NewItems.begin(), NewItems.end());
	}
	for (auto & Item : Items)
	{
		for (const auto & Function : m_Functions)
		{
			ApplyFunction(Function, Item, a_World, a_Noise, a_Pos, a_KilledID, a_KillerID);
		}
	}
	return Items;
}





cItems cLootTable::GetItems(const cLootTablePool & a_Pool, cWorld & a_World, const cNoise & a_Noise, const Vector3i & a_Pos, UInt32 a_KilledID, UInt32 a_KillerID)
{
	auto Items = cItems();
	if (!ConditionsApply(a_Pool.m_Conditions, a_World, a_Noise, a_Pos, a_KilledID, a_KillerID))
	{
		return Items;
	}

	int Luck = 0;
	/*  TODO: 07.09.2020 - Luck
	auto Callback = [&] (cPlayer & a_Player)
	{
		Luck = a_Player.GetLuck();
	};
	a_World.DoWithPlayerByUUID(a_Player, Callback);
	*/

	// Determines the total weight manipulated by the quality
	int TotalWeight = a_Pool.m_TotalWeight + Luck * a_Pool.m_TotalQuality;
	int TotalRolls = a_Pool.m_Rolls(a_Noise, a_Pos) + round(a_Pool.m_BonusRolls(a_Noise, a_Pos) * Luck);
	int EntryNum;

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
		auto NewItems = GetItems(Entry, a_World, a_Noise, a_Pos, a_KilledID, a_KillerID);
		Items.insert(Items.end(), NewItems.begin(), NewItems.end());
	}
	return Items;
}





cItems cLootTable::GetItems(const cLootTablePoolEntry & a_Entry, cWorld & a_World, const cNoise & a_Noise, const Vector3i & a_Pos, UInt32 a_KilledID, UInt32 a_KillerID)
{
	auto Items = cItems();

	if (!ConditionsApply(a_Entry.m_Conditions, a_World, a_Noise, a_Pos, a_KilledID, a_KillerID))
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
				LOGWARNING("Unsupported Data type in loot table pool - dropping entry");
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
				LOGWARNING("Unsupported Data type in loot table pool - dropping entry");
				break;
			}

			cItems TagItems = ItemTag::ItemTags(ItemTag::eItemTags(Tag));

			if (a_Entry.m_Expand)
			{
				Items.Add(TagItems[a_Noise.IntNoise3DInt(a_Pos * TagItems.size()) % TagItems.size()]);
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
				LOGWARNING("Unsupported Data type in loot table pool - dropping entry");
				break;
			}

			auto NewItems = a_World.GetLootTableProvider()->GetLootTable(Loot)->GetItems(a_Noise, a_Pos, a_World, a_KilledID, a_KillerID);
			Items.insert(Items.end(), NewItems.begin(), NewItems.end());
			break;
		}
		case LootTable::ePoolEntryType::Group:  // Grants all listed entries
		{
			try
			{
				for (const auto & Child : std::get<cLootTablePoolEntries>(a_Entry.m_Content))
				{
					auto NewItems = GetItems(Child, a_World, a_Noise, a_Pos, a_KilledID, a_KillerID);
					Items.insert(Items.end(), NewItems.begin(), NewItems.end());
				}
			}
			catch (const std::bad_variant_access &)
			{
				LOGWARNING("Unsupported Data type in loot table pool - dropping entry");
			}
			break;
		}
		case LootTable::ePoolEntryType::Alternatives:  // Grants one of the listed entries
		{
			try
			{
				auto Children = std::get<cLootTablePoolEntries>(a_Entry.m_Content);
				auto ChildPos = a_Noise.IntNoise3DInt(a_Pos * Children.size()) % Children.size();
				auto NewItems = GetItems(Children[ChildPos], a_World, a_Noise, a_Pos, a_KilledID, a_KillerID);
				Items.insert(Items.end(), NewItems.begin(), NewItems.end());
			}
			catch (const std::bad_variant_access &)
			{
				LOGWARNING("Unsupported Data type in loot table pool - dropping entry");
			}
			break;
		}
		case LootTable::ePoolEntryType::Sequence:  // Selects entries from Children until one is not granted.
		{
			cLootTablePoolEntries Children;
			try
			{
				Children = std::get<cLootTablePoolEntries>(a_Entry.m_Content);
			}
			catch (const std::bad_variant_access &)
			{
				LOGWARNING("Unsupported Data type in loot table pool - dropping entry!");
				break;
			}
			cItems NewItems;
			int LockCont = 0;  // This is how many times the whole table is repeated to prevent infinite loops
			unsigned int ChildPos = 0;
			do
			{
				NewItems = GetItems(Children[ChildPos], a_World, a_Noise, a_Pos, a_KilledID, a_KillerID);
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
			// TODO: 07.09.2020 - 12xx12
			LOGWARNING("Loot table pool entry type \"Dynamic\" is not supported yet - dropping entry!");
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
			ApplyFunction(Function, Item, a_World, a_Noise, a_Pos, a_KilledID, a_KillerID);
		}
	}
	return Items;
}





bool cLootTable::ConditionsApply(const cLootTableConditions & a_Conditions, cWorld & a_World, const cNoise & a_Noise, const Vector3i a_Pos, UInt32 a_KilledID, UInt32 a_KillerID)
{
	bool Success = true;
	for (const auto & Condition : a_Conditions)
	{
		Success &= ConditionApplies(Condition, a_World, a_Noise, a_Pos, a_KilledID, a_KillerID);
	}
	return Success;
}





bool cLootTable::ConditionApplies(const cLootTableCondition & a_Condition, cWorld & a_World, const cNoise & a_Noise, const Vector3i a_Pos, UInt32 a_KilledID, UInt32 a_KillerID)
{
	return std::visit(VISITCONDITION, a_Condition.m_Parameter);
}





void cLootTable::ApplyCommonFunction(const cLootTableFunction & a_Function, cItem & a_Item, cWorld & a_World, const cNoise & a_Noise, const Vector3i & a_Pos, UInt32 a_KilledID, UInt32 a_KillerID)
{
	switch (a_Function.m_Type)
	{
		case LootTable::eFunctionType::ApplyBonus:
		{
			Json::Value EnchantmentObject;

			if (a_Function.m_Parameter.isMember("enchantment"))
			{
				EnchantmentObject = a_Function.m_Parameter["enchantment"];
			}
			else if (a_Function.m_Parameter.isMember("Enchantment"))
			{
				EnchantmentObject = a_Function.m_Parameter["Enchantment"];
			}

			auto Enchantment = cEnchantments::StringToEnchantmentID(NamespaceConverter(EnchantmentObject.asString()));
			int Level;

			auto Callback = [&] (cEntity & a_Entity)
			{
				if (a_Entity.GetEntityType() != cEntity::etPlayer)
				{
					return false;
				}

				auto & Player = static_cast<cPlayer &>(a_Entity);

				Level = Player.GetEquippedItem().m_Enchantments.GetLevel(Enchantment);
				return true;
			};

			a_World.DoWithEntityByID(a_KilledID, Callback);

			AString Formula;
			if (a_Function.m_Parameter.isMember("formula"))
			{
				Formula = NamespaceConverter(a_Function.m_Parameter["formula"].asString());
			}
			else if (a_Function.m_Parameter.isMember("Formula"))
			{
				Formula = NamespaceConverter(a_Function.m_Parameter["Formula"].asString());
			}

			Json::Value Parameters;

			if (a_Function.m_Parameter.isMember("parameters"))
			{
				Parameters = a_Function.m_Parameter["parameters"];
			}
			else if (a_Function.m_Parameter.isMember("Parameters"))
			{
				Parameters = a_Function.m_Parameter["Parameters"];
			}

			if (Formula == "BinomialWithBonusCount")
			{
				// Binomial Probability Distribution with n=level + extra, p=probability
				int Extra = 0;

				if (Parameters.isMember("extra"))
				{
					Extra = Parameters["extra"].asInt();
				}
				else if (Parameters.isMember("Extra"))
				{
					Extra = Parameters["Extra"].asInt();
				}

				float Probability = 0.0f;

				if (Parameters.isMember("probability"))
				{
					Probability = Parameters["probability"].asFloat();
				}
				else if (Parameters.isMember("Probability"))
				{
					Probability = Parameters["Probability"].asFloat();
				}

				std::default_random_engine Generator(a_Noise.GetSeed());
				std::binomial_distribution<int> Dist(Level + Extra, Probability);
				std::vector<int> Values;
				for (int i = 0; i < Level + Extra; i++)
				{
					Values[i] = Dist(Generator);
				}
				a_Item.m_ItemCount += Values[a_Noise.IntNoise3DInt(a_Pos * 15) % Values.size()];
			}
			else if (Formula == "UniformBonusCount")
			{
				// prob = (0, level * BonusMultiplier)
				int BonusMultiplier = 0;
				if (Parameters.isMember("bonusMultiplier"))
				{
					BonusMultiplier = Parameters["bonusMultiplier"].asInt();
				}
				else if (Parameters.isMember("BonusMultiplier"))
				{
					BonusMultiplier = Parameters["BonusMultiplier"].asInt();
				}
				a_Item.m_ItemCount += a_Noise.IntNoise3DInt(a_Pos) * a_Item.m_ItemType % (Level * BonusMultiplier);
			}
			else if (Formula == "OreDrops")
			{
				// Count *= (max(0; random(0..Level + 2) - 1) + 1)
				a_Item.m_ItemCount *= std::max(0, a_Noise.IntNoise1DInt(a_Noise.GetSeed() * a_Item.m_ItemType) % (Level + 2) - 1) + 1;
			}
			break;
		}
		case LootTable::eFunctionType::EnchantRandomly:
		{
			if (!cItem::IsEnchantable(a_Item.m_ItemType))
			{
				LOGWARNING("Item %s can not be enchanted in loot table", ItemToString(a_Item));
			}

			cWeightedEnchantments EnchantmentLimiter;
			Json::Value Enchantments;
			if (a_Function.m_Parameter.isMember("enchantments"))
			{
				Enchantments = a_Function.m_Parameter["enchantments"];
			}
			else if (a_Function.m_Parameter.isMember("Enchantments"))
			{
				Enchantments = a_Function.m_Parameter["Enchantments"];
			}
			if (Enchantments.isArray())
			{
				for (unsigned int i = 0; i < Enchantments.size(); i++)
				{
					EnchantmentLimiter.push_back({1, cEnchantments(NamespaceConverter(Enchantments[i].asString()))});
				}
			}
			else  // All are possible
			{
				cEnchantments::AddItemEnchantmentWeights(EnchantmentLimiter, E_ITEM_BOOK, 24 + a_Noise.IntNoise3DInt(a_Pos * 20) % 7);
			}
			a_Item.m_ItemType = E_ITEM_ENCHANTED_BOOK;
			a_Item.m_Enchantments.Add(cEnchantments::GetRandomEnchantmentFromVector(EnchantmentLimiter));
			break;
		}
		case LootTable::eFunctionType::EnchantWithLevels:
		{
			// TODO: 02.09.2020 - Add treasure when implemented - 12xx12
			bool Treasure = false;
			if (a_Function.m_Parameter.isMember("treasure"))
			{
				Treasure = a_Function.m_Parameter["treasure"].asBool();
				LOGWARNING("Treasure enchantments are not yet supported");
			}
			else if (a_Function.m_Parameter.isMember("Treasure"))
			{
				Treasure = a_Function.m_Parameter["Treasure"].asBool();
				LOGWARNING("Treasure enchantments are not yet supported");
			}
			UNUSED_VAR(Treasure);
			Json::Value LevelsObject;
			if (a_Function.m_Parameter.isMember("levels"))
			{
				LevelsObject = a_Function.m_Parameter["levels"];
			}
			else if (a_Function.m_Parameter.isMember("Levels"))
			{
				LevelsObject = a_Function.m_Parameter["Levels"];
			}
			else
			{
				LOGWARNING("No levels provided for enchantments in Loot table, dropping function");
				break;
			}
			int Levels = 0;
			if (LevelsObject.isInt())
			{
				Levels = LevelsObject.asInt();
			}
			else if (LevelsObject.isObject())
			{
				Levels = LootTable::MinMaxRand(Levels, a_Noise, a_Pos, a_Item.m_ItemType);
			}
			if (!Levels)
			{
				break;
			}
			a_Item.EnchantByXPLevels(Levels);
			break;
		}
		case LootTable::eFunctionType::ExplorationMap:
		{
			// TODO: 02.09.2020 - Add when implemented - 12xx12
			LOGWARNING("Exploration maps are not implemented, dropping function!");
			break;
		}
		case LootTable::eFunctionType::FurnaceSmelt:
		{
			auto Recipes = cRoot::Get()->GetFurnaceRecipe();
			auto NewItem = Recipes->GetRecipeFrom(a_Item)->Out;
			a_Item.m_ItemType = NewItem->m_ItemType;
			a_Item.m_ItemDamage = NewItem->m_ItemDamage;
			break;
		}
		case LootTable::eFunctionType::LimitCount:
		{
			Json::Value LimitObject;
			if (a_Function.m_Parameter.isMember("limit"))
			{
				LimitObject = a_Function.m_Parameter["limit"];
			}
			else if (a_Function.m_Parameter.isMember("Limit"))
			{
				LimitObject = a_Function.m_Parameter["Limit"];
			}
			else
			{
				LOGWARNING("Missing limit, dropping function!");
				break;
			}

			int Limit = -1;
			if (LimitObject.isInt())
			{
				Limit = LimitObject.isInt();
			}
			else if (LimitObject.isObject())
			{
				Limit = LootTable::MinMaxRand(LimitObject, a_Noise, a_Pos, a_Item.m_ItemType);
			}
			if (Limit < 0)
			{
				break;
			}
			if (a_Item.m_ItemCount > Limit)
			{
				a_Item.m_ItemCount = Limit;
			}
			break;
		}
		case LootTable::eFunctionType::LootingEnchant:
		{
			int Looting = 0;
			auto Callback = [&] (cEntity & a_Entity)
			{
				if (a_Entity.GetEntityType() != cEntity::etPlayer)
				{
					return false;
				}
				auto & Player = static_cast<cPlayer &>(a_Entity);
				Looting = Player.GetEquippedItem().m_Enchantments.GetLevel(cEnchantments::enchLooting);
				return true;
			};

			a_World.DoWithEntityByID(a_KillerID, Callback);

			Json::Value CountObject;
			if (a_Function.m_Parameter.isMember("count"))
			{
				CountObject = a_Function.m_Parameter["count"];
			}
			else if (a_Function.m_Parameter.isMember("Count"))
			{
				CountObject = a_Function.m_Parameter["Count"];
			}

			int Count = 1;
			if (CountObject.isInt())
			{
				Count = CountObject.asInt();
			}
			else if (CountObject.isObject())
			{
				Count = LootTable::MinMaxRand(CountObject, a_Noise, a_Pos, 5);
			}
			int Limit = 0;
			if (a_Function.m_Parameter.isMember("limit"))
			{
				Limit = a_Function.m_Parameter["limit"].asInt();
			}
			else if (a_Function.m_Parameter.isMember("Limit"))
			{
				Limit = a_Function.m_Parameter["Limit"].asInt();
			}

			a_Item.m_ItemCount += Looting * Count;

			if (Limit > 0)
			{
				if (a_Item.m_ItemCount > Limit)
				{
					a_Item.m_ItemCount = Limit;
				}
			}

			break;
		}
		case LootTable::eFunctionType::SetAttributes:
		{
			// TODO: 02.09.2020 - Add when implemented - 12xx12
			LOGWARNING("Attributes for items are not implemented, dropping function!");
			break;
		}
		case LootTable::eFunctionType::SetCount:
		{
			Json::Value CountObject;
			if (a_Function.m_Parameter.isMember("count"))
			{
				CountObject = a_Function.m_Parameter["count"];
			}
			else if (a_Function.m_Parameter.isMember("Count"))
			{
				CountObject = a_Function.m_Parameter["Count"];
			}

			if (CountObject.isInt())
			{
				a_Item.m_ItemCount = CountObject.asInt();
			}
			else if (CountObject.isObject())
			{
				AString Type;
				if (CountObject.isMember("type"))
				{
					Type = NamespaceConverter(CountObject["type"].asString());
				}
				else if (CountObject.isMember("Type"))
				{
					Type = NamespaceConverter(CountObject["Type"].asString());
				}

				if ((Type == "Uniform") || (Type == "uniform"))
				{
					a_Item.m_ItemCount = LootTable::MinMaxRand(CountObject, a_Noise, a_Pos, 7);
				}
				else if ((Type == "Binomial") || (Type == "binomial"))
				{
					int N = 0;
					float P = 0;
					if (CountObject.isMember("n"))
					{
						N = CountObject["n"].asInt();
					}
					else if (CountObject.isMember("N"))
					{
						N = CountObject["N"].asInt();
					}

					if (CountObject.isMember("p"))
					{
						P = CountObject["p"].asFloat();
					}
					else if (CountObject.isMember("P"))
					{
						P = CountObject["P"].asFloat();
					}
					std::default_random_engine Generator(a_Noise.GetSeed());
					std::binomial_distribution<int> Dist(N, P);
					std::vector<int> Values;
					for (int i = 0; i < N; i++)
					{
						Values[i] = Dist(Generator);
					}
					a_Item.m_ItemCount += Values[a_Noise.IntNoise1DInt(a_Noise.GetSeed() * a_Item.m_ItemType) % Values.size()];
				}
			}
			break;
		}
		case LootTable::eFunctionType::SetDamage:
		{
			Json::Value DamageObject;

			if (a_Function.m_Parameter.isMember("damage"))
			{
				DamageObject = a_Function.m_Parameter["damage"];
			}
			else if (a_Function.m_Parameter.isMember("Damage"))
			{
				DamageObject = a_Function.m_Parameter["Damage"];
			}
			float Damage = 0;
			if (DamageObject.isNumeric())
			{
				Damage = DamageObject.asFloat();
			}
			else if (DamageObject.isObject())
			{
				float Min = 0.0f, Max = 0.0f;
				if (DamageObject.isMember("min"))
				{
					Min = DamageObject["min"].asFloat();
				}
				else if (DamageObject.isMember("Min"))
				{
					Min = DamageObject["Min"].asFloat();
				}

				if (DamageObject.isMember("max"))
				{
					Max = DamageObject["max"].asFloat();
				}
				else if (DamageObject.isMember("Max"))
				{
					Max = DamageObject["Max"].asFloat();
				}

				if (Min > Max)
				{
					Max = Min;
				}
				Damage = std::fmod(a_Noise.IntNoise1D(a_Noise.GetSeed() * a_Item.m_ItemType), Max - Min) + Min;
			}
			a_Item.m_ItemDamage = floor(a_Item.m_ItemDamage * Damage);
			break;
		}
		case LootTable::eFunctionType::SetLootTable:
		{
			// TODO: 02.09.2020 - Add when implemented - 12xx12
			LOGWARNING("NBT for items is not yet supported, dropping function!");
			break;
		}
		case LootTable::eFunctionType::SetLore:
		{
			// TODO: 06.09.2020 - Add the json string parsing - 12xx12
			LOGWARNING("Lore for dropped items is not yet supported, dropping function!");
			break;
		}
		case LootTable::eFunctionType::SetName:
		{
			// TODO: 06.09.2020 - Add the json string parsing - 12xx12
			LOGWARNING("Naming for dropped items is not yet supported, dropping function!");
			break;
		}
		case LootTable::eFunctionType::SetNbt:
		{
			// TODO: 02.09.2020 - Add when implemented - 12xx12
			LOGWARNING("NBT for items is not yet supported, dropping function!");
			break;
		}
		case LootTable::eFunctionType::SetStewEffect:
		{
			// TODO: 02.09.2020 - Add when implemented - 12xx12
			LOGWARNING("Stews are not yet supported, dropping function!");
			break;
		}
		default:
		{
			LOGWARNING("Function type is not supported for this entity, dropping function!");
			break;
		}
	}
}




// Item in container
void cLootTable::ApplyFunction(const cLootTableFunction & a_Function, cItem & a_Item, cWorld & a_World, const cNoise & a_Noise, const Vector3i & a_Pos, UInt32 a_KilledID, UInt32 a_KillerID)
{
	if (!ConditionsApply(a_Function.m_Conditions, a_World, a_Noise, a_Pos, a_KilledID, a_KillerID))
	{
		return;
	}

	switch (a_Function.m_Type)
	{
		case LootTable::eFunctionType::FillPlayerHead:
		{
			// TODO: 02.09.2020 - Add actual player head when implemented
			if (a_Item.m_ItemType == E_ITEM_HEAD)
			{
				a_Item.m_ItemDamage = E_META_HEAD_PLAYER;
				LOGWARNING("Setting players head as a item is not supported, setting to default player Design!");
			}
			else
			{
				LOGWARNING("Wrong Item provided to assign head type!");
			}
			break;
		}
		case LootTable::eFunctionType::None:
		{
			break;
		}
		default:
		{
			ApplyCommonFunction(a_Function, a_Item, a_World, a_Noise, a_Pos, a_KilledID, a_KillerID);
			break;
		}
	}
}




// Block
void cLootTable::ApplyFunction(const cLootTableFunction & a_Function, cItem & a_Item, cWorld & a_World, const cBlockHandler & a_Block, const cNoise & a_Noise, const Vector3i & a_Pos, UInt32 a_KilledID, UInt32 a_KillerID)
{
	if (!ConditionsApply(a_Function.m_Conditions, a_World, a_Noise, a_Pos, a_KilledID, a_KillerID))
	{
		return;
	}

	switch (a_Function.m_Type)
	{
		case LootTable::eFunctionType::CopyNbt:
		{
			// TODO: 06.09.2020 - Add when implemented - 12xx12
			LOGWARNING("NBT for items is not yet supported, Dropping function!");
			break;
		}
		case LootTable::eFunctionType::CopyState:
		{
			// TODO: 02.09.2020 - Add when implemented - 12xx12
			LOGWARNING("States for items is not yet supported, dropping function!");
			break;
		}
		case LootTable::eFunctionType::ExplosionDecay:
		{
			// TODO: 06.09.2020: Add. You need access to the next explosion
			LOG("Explosion decay is not supported - dropping function!");
			break;
		}
		case LootTable::eFunctionType::FillPlayerHead:
		{
			// TODO: 02.09.2020 - Add actual player head when implemented
			if (a_Item.m_ItemType == E_ITEM_HEAD)
			{
				a_Item.m_ItemDamage = E_META_HEAD_PLAYER;
				LOGWARNING("Setting players head as a item is not supported, setting to default player Design!");
			}
			else
			{
				LOGWARNING("Wrong Item provided to assign head type!");
			}
			break;
		}
		case LootTable::eFunctionType::SetContents:
		{
			// TODO: 02.09.2020 - Add when implemented - 12xx12
			LOGWARNING("NBT for items is not yet supported, Dropping function!");
			break;
		}
		case LootTable::eFunctionType::None:
		{
			break;
		}
		default:
		{
			ApplyCommonFunction(a_Function, a_Item, a_World, a_Noise, a_Pos, a_KilledID, a_KillerID);
			break;
		}
	}
}




// Block Entity
void cLootTable::ApplyFunction(const cLootTableFunction & a_Function, cItem & a_Item, cWorld & a_World, const cBlockEntity & a_BlockEntity, const cNoise & a_Noise, const Vector3i & a_Pos, UInt32 a_KilledID, UInt32 a_KillerID)
{
	if (!ConditionsApply(a_Function.m_Conditions, a_World, a_Noise, a_Pos, a_KilledID, a_KillerID))
	{
		return;
	}

	switch (a_Function.m_Type)
	{
		case LootTable::eFunctionType::None:
		{
			break;
		}
		case LootTable::eFunctionType::FillPlayerHead:
		{
			// TODO:
			break;
		}
		default:
		{
			ApplyCommonFunction(a_Function, a_Item, a_World, a_Noise, a_Pos, a_KilledID, a_KillerID);
			break;
		}
	}
}



// Killed entity
void cLootTable::ApplyFunction(const cLootTableFunction & a_Function, cItem & a_Item, cWorld & a_World, const Vector3i & a_Pos, UInt32 a_KilledID, const cNoise & a_Noise, UInt32 a_KillerID)
{
	if (!ConditionsApply(a_Function.m_Conditions, a_World, a_Noise, a_Pos, a_KilledID, a_KillerID))
	{
		return;
	}

	switch (a_Function.m_Type)
	{
		case LootTable::eFunctionType::CopyNbt:
		{
			LOGWARNING("NBT for items is not yet supported, Dropping function!");
			break;
		}
		case LootTable::eFunctionType::FillPlayerHead:
		{
			// TODO: 02.09.2020 - Add actual player head when implemented
			if (a_Item.m_ItemType == E_ITEM_HEAD)
			{
				// Gets entity type and assigns the matching Head type to the item.
				auto CallBack = [&] (cEntity & a_Entity)
				{
					if (a_Entity.GetEntityType() != cEntity::etMob)
					{
						return false;
					}
					auto & Mob = static_cast<cMonster &>(a_Entity);
					if (Mob.GetMobType() == mtCreeper)
					{
						a_Item.m_ItemDamage = E_META_HEAD_CREEPER;
					}
					else if (Mob.GetMobType() == mtZombie)
					{
						a_Item.m_ItemDamage = E_META_HEAD_ZOMBIE;
					}
					else if (Mob.GetMobType() == mtSkeleton)
					{
						a_Item.m_ItemDamage = E_META_HEAD_SKELETON;
					}
					else if (Mob.GetMobType() == mtWitherSkeleton)
					{
						a_Item.m_ItemDamage = E_META_HEAD_WITHER;
					}
					else
					{
						a_Item.m_ItemDamage = E_META_HEAD_PLAYER;
					}
					return true;
				};
				a_World.DoWithEntityByID(a_KilledID, CallBack);
			}
			else
			{
				LOGWARNING("Wrong Item provided to assign head type!");
			}
			break;
		}
		case LootTable::eFunctionType::None:
		{
			break;
		}
		default:
		{
			ApplyCommonFunction(a_Function, a_Item, a_World, a_Noise, a_Pos, a_KilledID, a_KillerID);
			break;
		}
	}
}




