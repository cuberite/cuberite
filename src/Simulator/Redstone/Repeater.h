
#pragma once

#include "Component.h"
#include "Vector3.h"

namespace Redstone
{
	class Repeater : public Component
	{

	public:
		Repeater(Vector3i location, BLOCKTYPE blockType, NIBBLETYPE meta) :
			Component(location, REPEATER), meta(meta), pushUpdate(false)
		{
			Vector3i arrow; // point me in the right direction
			switch (meta & 0x3)
			{
				case 0: // z -1
					arrow = { 0, 0, -1 };
				case 1: // x +1
					arrow = { 1, 0, 0 };
				case 2: // z +1
					arrow = { 0, 0, 1 };
				case 3: // x -1
					arrow = { -1, 0, 0 };
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
			if (isProvidingPower && component->Location == front)
			{
				return 15;
			}
			return 0;
		}

		virtual int CanWeakPower(Component * component)
		{
			return CanStrongPower(component);
		}

		virtual bool GetState(BLOCKTYPE & block, NIBBLETYPE & meta)
		{
			if (!pushUpdate)
			{
				return false;
			}
			pushUpdate = false;
			block = (isOn || isProvidingPower) ? 
				E_BLOCK_REDSTONE_REPEATER_ON : E_BLOCK_REDSTONE_REPEATER_OFF;
			meta = this->meta;
			return true;
		}

		virtual cVector3iArray Update(ComponentFactory & factory, int ticks)
		{

			// check if locked
			if (CheckIsPoweredByDiode(factory.GetComponent(right)) ||
				CheckIsPoweredByDiode(factory.GetComponent(left)))
			{
				// I dont' see the blocktype or meta updating if repeater is locked, so just exit
				targetTicks = ticks - 1;
				return{};
			}

			// check state
			bool state = (factory.GetComponent(back)->CanWeakPower(this) > 0);
			if (isProvidingPower != state || isOn != state)
			{
				// we are changing set intial delay
				if (isOn != state && targetTicks < ticks)
				{
					targetTicks = ticks + (delay * 2);
					return{ Location };
				}
				else if (targetTicks >= ticks)
				{
					isProvidingPower = state;
					isOn = state;
					pushUpdate = true;
					return GetAdjacent();
				}
				// only occurs if delay is > 1, turn the repeater 'on' but don't power forward block yet.
				else if (isOn != state)
				{
					isOn = state;
					pushUpdate = true;
					return{ Location };
				}
			}

			targetTicks = ticks - 1;

			return{};
		}

	private:

		bool CheckIsPoweredByDiode(ComponentPtr comp)
		{
			return comp != nullptr &&
				(comp->Type == REPEATER || comp->Type == COMPARATOR) &&
				(comp->CanStrongPower(this) > 0);
		}

		int targetTicks;
		NIBBLETYPE meta;
		Vector3i front;
		Vector3i back;
		Vector3i left;
		Vector3i right;
		int delay;
		bool isProvidingPower;
		bool isOn;
		bool pushUpdate;
	};

}