
#pragma once

#include "Component.h"
#include "Vector3.h"

namespace Redstone
{
	class Torch : public Component
	{

	public:

		Torch(Vector3i location, BLOCKTYPE blockType, NIBBLETYPE meta) : 
			Component(location, TORCH), isBurntOut(false), heat(0)
		{
			LOGD("Torch const: %d %d %d", location.x, location.y, location.z);
			isOn = (blockType == E_BLOCK_REDSTONE_TORCH_ON);

			switch (meta)
			{
				case E_META_TORCH_FLOOR: attachedTo = Vector3i(Location.x, Location.y - 1, Location.z);
				case E_META_TORCH_EAST: attachedTo = Vector3i(Location.x + 1, Location.y, Location.z);
				case E_META_TORCH_WEST: attachedTo = Vector3i(Location.x - 1, Location.y, Location.z);
				case E_META_TORCH_NORTH: attachedTo = Vector3i(Location.x, Location.y, Location.z - 1);
				case E_META_TORCH_SOUTH: attachedTo = Vector3i(Location.x, Location.y, Location.z + 1);

			}
		}

		virtual int CanStrongPower(Vector3i location)
		{
			if (isOn && location == Up())
			{
				return 15;
			}
			return 0;
		}

		virtual int CanWeakPower(Vector3i location)
		{
			if (location == attachedTo)
			{
				return 0;
			}
			else if (isOn && IsAdjacent(location))
			{
				return 15;
			}
			return 0;
		}

		virtual cVector3iArray Update(ComponentFactory & factory, BLOCKTYPE & block, NIBBLETYPE & meta)
		{
			LOGD("Evaluating torch (%d %d %d)", Location.x, Location.y, Location.z);

			cVector3iArray result;
			ComponentPtr attachedBlock = factory.GetComponent(attachedTo);

			// this should never happen
			if (attachedBlock == nullptr)
			{
				return{};
			}

			bool state = (attachedBlock->CanWeakPower(Location) > 0);
			bool updateBlocks = false;
			if (isOn != state)
			{
				result.push_back(Location);
				if (state)
				{
					heat += 5;
					if (heat > 15)
					{
						isBurntOut = true;
					}
					else
					{
						block = E_BLOCK_REDSTONE_TORCH_ON;
						isOn = state;
						updateBlocks = true;
					}
				}
				else
				{
					block = E_BLOCK_REDSTONE_TORCH_OFF;
					isOn = state;
					updateBlocks = true;
				}
			}

			heat -= 1;
			if (heat > 0)
			{
				updateBlocks = true;
			}
			else
			{
				isBurntOut = false;
			}

			if (updateBlocks)
			{
				auto adjacents = GetAdjacent();
				result.insert(result.end(), adjacents.begin(), adjacents.end());
			}

			return result;
		}

		~Torch()
		{
			LOGD("Torch destoryed (%d %d %d)", Location.x, Location.y, Location.z);
		}

	private:
		bool isOn;
		bool isBurntOut;
		int heat;
		Vector3i attachedTo;
	};
}