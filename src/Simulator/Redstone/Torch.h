
#pragma once

#include "Component.h"
#include "Vector3.h"

namespace Redstone
{
	class Torch : public Component
	{

	public:

		Torch(Vector3i location, BLOCKTYPE blockType, NIBBLETYPE meta) :
			Component(location, TORCH), m_Meta(meta), ticks(-1), pushUpdate(false), isBurntOut(false), heat(0), lastHeatTick(0)
		{
			LOGD("Torch created: %d %d %d", location.x, location.y, location.z);
			isOn = (blockType == E_BLOCK_REDSTONE_TORCH_ON);

			switch (meta)
			{
				case E_META_TORCH_FLOOR:
					attachedTo = Vector3i(Location.x, Location.y - 1, Location.z);
					break;
				case E_META_TORCH_EAST:
					attachedTo = Vector3i(Location.x - 1, Location.y, Location.z);
					break;
				case E_META_TORCH_WEST:
					attachedTo = Vector3i(Location.x + 1, Location.y, Location.z);
					break;
				case E_META_TORCH_NORTH:
					attachedTo = Vector3i(Location.x, Location.y, Location.z + 1);
					break;
				case E_META_TORCH_SOUTH:
					attachedTo = Vector3i(Location.x, Location.y, Location.z - 1);
					break;
			}
		}


		virtual int CanStrongPower(Component * component)
		{
			if (isOn && component->IsFullBlock && component->Location == Up())
			{
				return 15;
			}
			return 0;
		}

		virtual int CanWeakPower(Component * component)
		{
			if (!isOn || component->Location == attachedTo || component->Type == SOLIDBLOCK)
			{
				return 0;
			}
			else if (IsAdjacent(component->Location))
			{
				return 15;
			}
			return 0;
		}

		virtual bool GetState(BLOCKTYPE & block, NIBBLETYPE & meta)
		{
			if (!pushUpdate)
			{
				return false;
			}
			pushUpdate = false;
			block = isOn ? E_BLOCK_REDSTONE_TORCH_ON : E_BLOCK_REDSTONE_TORCH_OFF;
			meta = m_Meta;
			return true;
		}

		virtual cVector3iArray Update(ComponentFactory & factory, int ticks)
		{
			LOGD("Evaluating torch (%d %d %d)", Location.x, Location.y, Location.z);

			if (lastHeatTick != ticks)
			{
				lastHeatTick = ticks;
				if (isBurntOut)
				{
					if (heat == 0)
					{
						isBurntOut = false;
						return{};
					}
					else
					{
						heat = std::max(heat - 2, 0);
						return{ Location };
					}
				}
				heat = std::max(heat - 1, 0);
			}


			ComponentPtr attachedBlock = factory.GetComponent(attachedTo);

			// this should never happen
			if (attachedBlock == nullptr)
			{
				return{};
			}

			bool state = !(attachedBlock->CanWeakPower(this) > 0);

			if (isOn != state)
			{
				if (ticks > this->ticks)
				{
					this->ticks = ticks + 2;
					return{ Location };
				}
				else if (heat > 22 && state)
				{
					isBurntOut = true;
					// ok this is aweful, how else can I get sounds out?
					factory.PlaySound("random.fizz", Location, 3);
					return{ Location };
				}
				else
				{
					this->ticks = ticks - 1;
					isOn = state;
					pushUpdate = true;
					if (isOn)
					{
						heat += 5;
					}
					return GetAdjacent(true);
				}
			}



			if (this->ticks < 0)
			{
				this->ticks = ticks;
				return GetAdjacent(true);
			}


			return{};
		}

		~Torch()
		{
			LOGD("Torch destoryed (%d %d %d)", Location.x, Location.y, Location.z);
		}

	private:
		NIBBLETYPE m_Meta;
		bool isOn;
		int ticks;
		int heat;
		int lastHeatTick;
		bool isBurntOut;
		bool pushUpdate;
		Vector3i attachedTo;
	};
}