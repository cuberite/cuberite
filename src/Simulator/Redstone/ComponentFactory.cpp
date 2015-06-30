
#include "Globals.h"
#include "ComponentFactory.h"

// Block types
#include "Torch.h"
#include "SolidBlock.h"


namespace Redstone
{

	ComponentFactory::ComponentFactory(cWorld & a_World, cRedstoneSimulatorChunkData * a_data) :
		m_World(a_World), m_Data(a_data)
	{ }


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
			default:
				return nullptr;
		}
	}
}






























