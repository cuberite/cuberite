
#include "Globals.h"
#include "ComponentFactory.h"

// Block types
#include "Torch.h"
#include "SolidBlock.h"
#include "Wire.h"
#include "Repeater.h"


namespace Redstone
{

	ComponentFactory::ComponentFactory(cWorld & a_World, cRedstoneSimulatorChunkData * a_data) :
		m_World(a_World), m_Data(a_data)
	{ }

	void ComponentFactory::PlaySound(std::string name, Vector3i location, float pitch)
	{
		m_World.GetBroadcastManager().BroadcastSoundEffect(name, 
			static_cast<double>(location.x),
			static_cast<double>(location.y),
			static_cast<double>(location.z),
			0.5f, pitch);
	}


	ComponentPtr ComponentFactory::GetComponent(Vector3i location)
	{
		// first attempt to grab the component from chunk's cache
		// then create the component based on block type

		ComponentPtr component(m_Data->GetComponent(location));

		BLOCKTYPE blockType;
		NIBBLETYPE meta;
		RedstoneType type = GetType(location, blockType, meta);

		if (component == nullptr || component->Type != type)
		{
			component = ComponentPtr(CreateComponent(type, location, blockType, meta));
			if (component == nullptr)
			{
				m_Data->RemoveComponent(location);
				return nullptr;
			}
			m_Data->SetComponent(component);
		}

		return component;
	}

	RedstoneType ComponentFactory::GetType(Vector3i location, BLOCKTYPE & blockType, NIBBLETYPE & meta)
	{
		blockType = m_World.GetBlock(location);
		meta = m_World.GetBlockMeta(location);

		// filter solid blocks
		if (IsSolidBlock(blockType))
		{
			return SOLIDBLOCK;
		}

		switch (blockType)
		{
			// first redstone specific components
			case E_BLOCK_REDSTONE_TORCH_OFF:
			case E_BLOCK_REDSTONE_TORCH_ON:
				return TORCH;
			case E_BLOCK_REDSTONE_WIRE:
				return WIRE;
			case E_BLOCK_REDSTONE_REPEATER_ON:
			case E_BLOCK_REDSTONE_REPEATER_OFF:
				return REPEATER;
				// everything else is not understood by redstone simulator
			default:
				return UNKNOWN;
		}
	}

	ComponentPtr ComponentFactory::CreateComponent(RedstoneType type, Vector3i location, BLOCKTYPE blockType, NIBBLETYPE meta)
	{
		switch (type)
		{
			case TORCH:
				return ComponentPtr(std::make_shared<Torch>(location, blockType, meta));
			case SOLIDBLOCK:
				return ComponentPtr(std::make_shared<SolidBlock>(location));
			case WIRE:
				return ComponentPtr(std::make_shared<Wire>(location));
			case REPEATER:
				return ComponentPtr(std::make_shared<Repeater>(location, blockType, meta));
			default:
				return nullptr;
		}
	}
}






























