//
// Created by MightyFilipns on 01 / 04 /25.
//
#pragma once
#include "Globals.h"
#include "CompositeChat.h"
#include <utility>
#include <variant>
#include "JsonDataCompLoader.h"


class cItem;

namespace DataComponents
{
	#define COMP_EQU_OP(Comp, Comparer) inline bool operator == (const Comp & lhs, const Comp & rhs) { return Comparer; }
	struct MaxDamageComponent
	{
		MaxDamageComponent() = default;
		MaxDamageComponent(UInt32 a_MaxDamage)
		{
			MaxDamage = a_MaxDamage;
		}
		UInt32 MaxDamage;
	};

	struct DamageComponent
	{
		DamageComponent() = default;
		DamageComponent(UInt32 a_Damage)
		{
			Damage = a_Damage;
		}
		UInt32 Damage;
	};

	struct MaxStackSizeComponent
	{
		MaxStackSizeComponent() = default;
		MaxStackSizeComponent(UInt32 a_maxStackSize)
		{
			maxStackSize = a_maxStackSize;
		}
		UInt32 maxStackSize;
	};

	struct UnbreakableComponent
	{
		UnbreakableComponent() = default;
		UnbreakableComponent(bool a_unbreakable)
		{
			unbreakable = a_unbreakable;
		}
		bool unbreakable;
	};

	struct CustomNameComponent
	{
		CustomNameComponent() {}

		CustomNameComponent(const AString & a_Text) { Name = a_Text; }

		cCompositeChat Name;
	};

	struct RepairCostComponent
	{
		RepairCostComponent() = default;
		RepairCostComponent(UInt32 a_RepairCost)
		{
			RepairCost = a_RepairCost;
		}
		UInt32 RepairCost;
	};

	COMP_EQU_OP(MaxDamageComponent, lhs.MaxDamage == rhs.MaxDamage);
	COMP_EQU_OP(DamageComponent, lhs.Damage == rhs.Damage);
	COMP_EQU_OP(MaxStackSizeComponent, lhs.maxStackSize == rhs.maxStackSize);
	COMP_EQU_OP(UnbreakableComponent, lhs.unbreakable == rhs.unbreakable);
	COMP_EQU_OP(CustomNameComponent, lhs.Name.ExtractText() == rhs.Name.ExtractText());  // TODO: proper comparison
	COMP_EQU_OP(RepairCostComponent, lhs.RepairCost == rhs.RepairCost);

	typedef std::variant<MaxStackSizeComponent, UnbreakableComponent, CustomNameComponent, DamageComponent, MaxDamageComponent, RepairCostComponent> DataComponent;

	//  Original version taken from here: https://gist.github.com/nnaumenko/1db96f7e187979a057ee7ad757dee4f2
	template <typename T, size_t I = 0>
	static constexpr size_t GetIndexOfDataComponent()
	{
		if constexpr (I >= std::variant_size_v<DataComponent>)
		{
			static_assert(false, "Given type is not part of the DataComponent variant");
			return 0;
		}
		else
		{
			if constexpr (std::is_same_v<std::variant_alternative_t<I, DataComponent>, T>)
			{
				return (I);
			}
			else
			{
				return (GetIndexOfDataComponent<T, I + 1>());
			}
		}
	}

	class DefaultComponentsMap
	{
	public:
		DefaultComponentsMap() : m_data() {}
		friend class cDataComponents;
		const std::map<size_t, DataComponent> & GetComponentsFor(Item a_item)
		{
			if (!m_data.contains(a_item))
			{
				LOGERROR(fmt::format("Default Components not found for item {}", NamespaceSerializer::From(a_item)));
				VERIFY(false);
			}
			return m_data.at(a_item);
		}

	private:

		void AddItem(Item a_Item, const std::map<size_t, DataComponent> & a_Comps)
		{
			if (m_data.contains(a_Item))
			{
				LOGWARNING(fmt::format("Item {} already exists in default components map, ignoring", NamespaceSerializer::From(a_Item)));
				return;
			}
			m_data[a_Item] = a_Comps;
		}

		void ClearAllItems()
		{
			m_data.clear();
		}
		std::map<Item, std::map<size_t, DataComponent>> m_data;
	};

	class DataComponentMap
	{
	public:
		DataComponentMap() = default;
		DataComponentMap(std::initializer_list<DataComponent> a_Comps)
		{
			for (const auto & a_comp : a_Comps)
			{
				AddComp(a_comp);
			}
		}

		DataComponentMap(const DataComponent & a_Comps)
		{
			AddComp(a_Comps);
		}

		// A replace function at the same time
		void AddComp(const DataComponent & a_comp)
		{
			m_data[a_comp.index()] = a_comp;
		}

		template <typename TypeToFind> const TypeToFind & GetComp() const
		{
			if (!HasComponent<TypeToFind>())
			{
				LOGWARN("Component not found on item, returning null");
				throw std::runtime_error("Component not found on item");
			}
			return std::get<TypeToFind>(m_data.at(GetIndexOfDataComponent<TypeToFind, 0>()));
		}

		template <typename TypeToFind> bool HasComponent() const
		{
			return m_data.contains(GetIndexOfDataComponent<TypeToFind, 0>());
		}

		template <typename TypeToFind> void RemoveComp()
		{
			m_data.erase(GetIndexOfDataComponent<TypeToFind, 0>());
		}

		const std::map<size_t, DataComponent> & GetMap() const { return m_data; }

		std::map<size_t, DataComponent> m_data;
	};

	typedef std::vector<DataComponent> DefaultItemComps;
}

