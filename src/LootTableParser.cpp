
#include "LootTableParser.h"

namespace LootTable
{
	cLootTablePool ParsePool(const Json::Value & a_Value)
	{
		cLootTablePoolRolls PoolRolls;
		cLootTablePoolRolls PoolBonusRolls;
		cLootTablePoolEntryVector PoolEntries;
		cLootTableFunctionVector Functions;
		cLootTableConditionVector Conditions;
		for (auto & PoolElement : a_Value.getMemberNames())
		{
			if (NoCaseCompare(PoolElement, "rolls") == 0)
			{
				auto Rolls = a_Value[PoolElement];
				PoolRolls = LootTable::ParseRolls(Rolls);
			}
			else if (
				(NoCaseCompare(PoolElement, "bonus_rolls") == 0) ||
				(NoCaseCompare(PoolElement, "BonusRolls") == 0))
			{
				auto Rolls = a_Value[PoolElement];
				PoolBonusRolls = ParseRolls(Rolls);
			}
			else if (NoCaseCompare(PoolElement, "entries") == 0)
			{
				auto Entries = a_Value[PoolElement];
				for (unsigned int EntryIndex = 0; EntryIndex < Entries.size(); EntryIndex++)
				{
					PoolEntries.push_back(ParsePoolEntry(Entries[EntryIndex]));
				}
			}
			else if (NoCaseCompare(PoolElement, "functions") == 0)
			{
				auto FunctionsObject = a_Value[PoolElement];
				for (unsigned int FunctionIndex = 0; FunctionIndex < FunctionsObject.size(); FunctionIndex++)
				{
					Functions.push_back(ParseFunction(FunctionsObject[FunctionIndex]));
				}
			}
			else if (NoCaseCompare(PoolElement, "conditions") == 0)
			{
				auto ConditionsObject = a_Value[PoolElement];
				for (unsigned int ConditionId = 0; ConditionId < ConditionsObject.size(); ConditionId++)
				{
					Conditions.push_back(ParseCondition(ConditionsObject[ConditionId]));
				}
			}
		}
		return cLootTablePool(PoolRolls, PoolEntries, Conditions);
	}





	cLootTableFunction ParseFunction(const Json::Value & a_Value)
	{
		enum LootTable::eFunctionType Type;
		cLootTableConditionVector Conditions;
		Json::Value Parameter = a_Value;
		for (auto & FunctionInfo : a_Value.getMemberNames())
		{
			if (NoCaseCompare(FunctionInfo, "function") == 0)
			{
				Type = LootTable::eFunctionType(LootTable::NamespaceConverter(a_Value[FunctionInfo].asString()));
			}
			else if (NoCaseCompare(FunctionInfo, "conditions") == 0)
			{
				auto ConditionsObject = a_Value[FunctionInfo];
				for (unsigned int ConditionId = 0; ConditionId < ConditionsObject.size(); ConditionId++)
				{
					Conditions.push_back(ParseCondition(ConditionsObject[ConditionId]));
				}
				Parameter.removeMember(FunctionInfo);  // Removes the conditions so the json is a bit smaller
			}
		}
		return cLootTableFunction(Type, Parameter, Conditions);
	}





	cLootTableCondition ParseCondition(const Json::Value & a_Value)
	{
		enum LootTable::eConditionType Type;

		// Type has to be known beforehand
		if (a_Value.isMember("condition"))
		{
			Type = LootTable::eConditionType(LootTable::NamespaceConverter(a_Value["condition"].asString()));
		}
		else if (a_Value.isMember("Condition"))
		{
			Type = LootTable::eConditionType(LootTable::NamespaceConverter(a_Value["Condition"].asString()));
		}
		else
		{
			LOGWARNING("Loot table is missing condition type. Dropping condition!");
			return cLootTableCondition();
		}

		switch (Type)
		{
			case LootTable::eConditionType::Alternative:
			{
				Json::Value Terms;
				cLootTableConditionVector SubConditions;
				if (a_Value.isMember("terms"))
				{
					Terms = a_Value["terms"];
				}
				else if (a_Value.isMember("Terms"))
				{
					Terms = a_Value["Terms"];
				}
				else
				{
					LOGWARNING("Loot table condition \"Alternative\" is missing sub - conditions. Dropping condition!");
					return cLootTableCondition();
				}
				for (unsigned int i = 0; i < Terms.size(); i++)
				{
					SubConditions.push_back(ParseCondition(Terms[i]));
				}
				return cLootTableCondition(Type, SubConditions);
				break;
			}
			case LootTable::eConditionType::Inverted:
			{
				Json::Value Term;
				cLootTableConditionVector SubConditions;
				if (a_Value.isMember("term"))
				{
					Term = a_Value["term"];
					SubConditions.push_back(ParseCondition(Term));
					return cLootTableCondition(Type, SubConditions);
				}
				else if (a_Value.isMember("Term"))
				{
					Term = a_Value["Term"];
					SubConditions.push_back(ParseCondition(Term));
					return cLootTableCondition(Type, SubConditions);
				}
				else
				{
					LOGWARNING("Loot table condition \"Inverted\" is missing sub-condition. Dropping condition!");
					return cLootTableCondition();
				}
			}
			case LootTable::eConditionType::None:
			{
				return cLootTableCondition();
			}
			case LootTable::eConditionType::BlockStateProperty:
			case LootTable::eConditionType::DamageSourceProperties:
			case LootTable::eConditionType::EntityProperties:
			case LootTable::eConditionType::EntityScores:
			case LootTable::eConditionType::KilledByPlayer:
			case LootTable::eConditionType::LocationCheck:
			case LootTable::eConditionType::MatchTool:
			case LootTable::eConditionType::RandomChance:
			case LootTable::eConditionType::RandomChanceWithLooting:
			case LootTable::eConditionType::Reference:
			case LootTable::eConditionType::SurvivesExplosion:
			case LootTable::eConditionType::TableBonus:
			case LootTable::eConditionType::TimeCheck:
			case LootTable::eConditionType::WeatherCheck:
			{
				return cLootTableCondition(Type, a_Value);
			}
			default: return cLootTableCondition();
		}
	}





	cLootTablePoolEntry ParsePoolEntry(const Json::Value & a_Value)
	{
		cLootTableConditionVector Conditions;
		cLootTableFunctionVector Functions;
		enum LootTable::ePoolEntryType Type;

		cItem Item;
		AString Name;
		cLootTablePoolEntryVector Children;

		bool Expand = true;
		int Weight = 1;
		int Quality = 0;

		// The type has to be known beforehand
		if (a_Value.isMember("type"))
		{
			Type = LootTable::ePoolEntryType(LootTable::NamespaceConverter(a_Value["type"].asString()));
		}
		else if (a_Value.isMember("Type"))
		{
			Type = LootTable::ePoolEntryType(LootTable::NamespaceConverter(a_Value["Type"].asString()));
		}
		else
		{
			LOGWARNING("No loot table poll entry type provided - dropping entry");
			return cLootTablePoolEntry();
		}

		for (auto & EntryParameter : a_Value.getMemberNames())
		{
			if (NoCaseCompare(EntryParameter, "weight") == 0)
			{
				Weight = a_Value[EntryParameter].asInt();
			}
			else if (NoCaseCompare(EntryParameter, "name") == 0)
			{
				switch (Type)
				{
					case LootTable::ePoolEntryType::Item:
					{
						StringToItem(LootTable::NamespaceConverter(a_Value[EntryParameter].asString()), Item);
						break;
					}
					case LootTable::ePoolEntryType::Tag:
					case LootTable::ePoolEntryType::LootTable:
					case LootTable::ePoolEntryType::Dynamic:
					{
						Name = LootTable::NamespaceConverter(a_Value[EntryParameter].asString());
						break;
					}
					default: break;
				}
			}
			else if (NoCaseCompare(EntryParameter, "functions") == 0)
			{
				auto FunctionsObject = a_Value[EntryParameter];
				for (unsigned int FunctionIndex = 0; FunctionIndex < FunctionsObject.size(); FunctionIndex++)
				{
					Functions.push_back(ParseFunction(FunctionsObject[FunctionIndex]));
				}
			}
			else if (NoCaseCompare(EntryParameter, "conditions") == 0)
			{
				auto ConditionsObject = a_Value[EntryParameter];
				for (unsigned int ConditionId = 0; ConditionId < ConditionsObject.size(); ConditionId++)
				{
					Conditions.push_back(ParseCondition(ConditionsObject[ConditionId]));
				}
			}
			else if (NoCaseCompare(EntryParameter, "children") == 0)
			{
				switch (Type)
				{
					case LootTable::ePoolEntryType::Group:
					case LootTable::ePoolEntryType::Alternatives:
					case LootTable::ePoolEntryType::Sequence:
					{
						auto ChildrenObject = a_Value[EntryParameter];
						for (unsigned int ChildrenObjectId = 0; ChildrenObjectId < ChildrenObject.size(); ++ChildrenObjectId)
						{
							Children.push_back(ParsePoolEntry(ChildrenObject[ChildrenObjectId]));
						}
						break;
					}
					default: break;
				}
			}
			else if (NoCaseCompare(EntryParameter, "expand") == 0)
			{
				Expand = a_Value[EntryParameter].asBool();
			}
			else if (NoCaseCompare(EntryParameter, "quality") == 0)
			{
				Quality = a_Value[EntryParameter].asInt();
			}
		}
		switch (Type)
		{
			case LootTable::ePoolEntryType::Item:
			{
				return cLootTablePoolEntry(Conditions, Functions, Type, Item, Weight, Quality);
			}

			case LootTable::ePoolEntryType::Tag:
			case LootTable::ePoolEntryType::LootTable:
			case LootTable::ePoolEntryType::Dynamic:
			{
				return cLootTablePoolEntry(Conditions, Functions, Type, Name, Expand, Weight, Quality);
			}

			case LootTable::ePoolEntryType::Group:
			case LootTable::ePoolEntryType::Alternatives:
			case LootTable::ePoolEntryType::Sequence:
			{
				return cLootTablePoolEntry(Conditions, Functions, Type, Children, Weight, Quality);
			}

			case LootTable::ePoolEntryType::Empty: return cLootTablePoolEntry();
			default:                               return cLootTablePoolEntry();
		}
	}





	cLootTablePoolRolls ParseRolls(const Json::Value & a_Value)
	{
		if (a_Value.isInt())
		{
			return cLootTablePoolRolls(a_Value.asInt(), a_Value.asInt());
		}
		else
		{
			int Min = 0, Max = -1;
			for (auto & RollEntry: a_Value.getMemberNames())
			{
				if (NoCaseCompare(RollEntry, "min") == 0)
				{
					Min = a_Value[RollEntry].asInt();
				}
				else if (NoCaseCompare(RollEntry, "max") == 0)
				{
					Max = a_Value[RollEntry].asInt();
				}
			}
			if (Max == -1)
			{
				LOGWARNING("Missing maximum value in loot table pool - assuming steady roll");
				Max = Min;
			}
			return cLootTablePoolRolls(Min, Max);
		}
	}
}




