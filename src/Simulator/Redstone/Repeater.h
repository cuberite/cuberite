
#pragma once

#include "Component.h"
#include "Vector3.h"

namespace Redstone
{
	class Repeater : public Component
	{

	public:
		Repeater(Vector3i location, BLOCKTYPE blockType, NIBBLETYPE meta) :
			Component(location, REPEATER), meta(meta), pushUpdate(false), isOn(false), targetTicks(0), lastUpdate(-1)
		{
			Vector3i arrow; // point me in the right direction
			switch (meta & 0x3)
			{
				case 0: // z -1
					arrow = { 0, 0, -1 };
					break;
				case 1: // x +1
					arrow = { 1, 0, 0 };
					break;
				case 2: // z +1
					arrow = { 0, 0, 1 };
					break;
				case 3: // x -1
					arrow = { -1, 0, 0 };
					break;
			}

			front = location + arrow;
			arrow.TurnCW();
			right = location + arrow;
			arrow.TurnCW();
			back = location + arrow;
			arrow.TurnCW();
			left = location + arrow;

			delay = ((meta & 0xC) >> 0x2) + 1;
		}

		virtual int CanStrongPower(Component * component)
		{
			if (isOn && component->Location == front)
			{
				return 15;
			}
			return 0;
		}

		virtual int CanWeakPower(Component * component)
		{
			return CanStrongPower(component);
		}
		virtual void SetState(BLOCKTYPE & block, NIBBLETYPE & meta)
		{
			delay = ((meta & 0xC) >> 0x2) + 1;
			this->meta = meta;
		}
		virtual bool GetState(BLOCKTYPE & block, NIBBLETYPE & meta)
		{
			if (!pushUpdate)
			{
				return false;
			}
			pushUpdate = false;
			// i have no idea why this is backwards...
			block = (isOn) ? E_BLOCK_REDSTONE_REPEATER_ON : E_BLOCK_REDSTONE_REPEATER_OFF;

			meta = this->meta;
			return true;
		}

		virtual cVector3iArray Update(ComponentFactory & factory, int ticks)
		{
		/*	if (lastUpdate == ticks) return{};
			lastUpdate = ticks;*/

			// check if locked
			if (CheckIsPoweredByDiode(factory.GetComponent(right)) ||
				CheckIsPoweredByDiode(factory.GetComponent(left)))
			{
				// I dont' see the blocktype or meta updating if repeater is locked, so just exit
				targetTicks = ticks - 1;
				return{};
			}
			
			// check state
			
			ComponentPtr b = factory.GetComponent(back);
			bool state = (b != nullptr && (b->CanWeakPower(this) > 0));

			if (isOn != state)
			{
				// we are changing set intial delay
				if (targetTicks < ticks)
				{
					lastUpdate = ticks;
					targetTicks = ticks + (delay * 2);
					return{ Location };
				}
				else if (targetTicks == ticks)
				{
					isOn = state;
					pushUpdate = true;
					return GetAdjacent(true);
				}
				return{ Location };
			}

			if (targetTicks > ticks)
			{
				lastUpdate = ticks;
				targetTicks = ticks;
			}

			return{};
		}

	private:

		bool CheckIsPoweredByDiode(ComponentPtr comp)
		{
			return comp != nullptr &&
				(comp->Type == REPEATER || comp->Type == COMPARATOR) &&
				(comp->CanStrongPower(this) > 0);
		}
		int lastUpdate;
		int targetTicks;
		NIBBLETYPE meta;
		Vector3i front;
		Vector3i back;
		Vector3i left;
		Vector3i right;
		int delay;
		bool isOn;
		bool pushUpdate;
	};

}