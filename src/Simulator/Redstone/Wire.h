#pragma once

#include "Component.h"
#include "Vector3.h"

namespace Redstone
{
	class Wire : public Component
	{

	public:

		Wire(Vector3i location) :
			Component(location, WIRE), power(0), ticks(-1)
		{
			LOGD("Wire created {%d %d %d}", location.x, location.y, location.z);
		}

		virtual int CanStrongPower(Component * component)
		{
			return 0;
		}

		virtual int CanWeakPower(Component * component)
		{
			if (component->Location == Down())
			{
				return power;
			}
			if (std::find(Connections.begin(), Connections.end(), component->Location) != Connections.end())
			{
				if (component->Type == WIRE)
				{
					return std::max(power - 1, 0);
				}
				return power;
			}
		}

		virtual bool GetState(BLOCKTYPE & block, NIBBLETYPE & meta)
		{
			block = E_BLOCK_REDSTONE_WIRE;
			meta = static_cast<NIBBLETYPE>(power);
			return true;
		}

		virtual cVector3iArray Update(ComponentFactory & factory, int ticks)
		{
			// calculated power
			int cp = 0;
			LOGD("Evaluating Wire (%d %d %d)", Location.x, Location.y, Location.z);

			ComponentPtr down = factory.GetComponent(Down());
			ComponentPtr up = factory.GetComponent(Up());
			if (down != nullptr && down->IsFullBlock)
			{
				cp = std::max(cp, down->CanStrongPower(this));
			}
			if (up != nullptr)
			{
				if (up->IsFullBlock)
				{
					cp = std::max(cp, up->CanStrongPower(this));
				}
				else if (up->Type == TORCH) // may not need the torch qualifyer here
				{
					cp = std::max(cp, up->CanWeakPower(this));
				}
			}

			// re calculate connections
			// TODO: possible perf gains if we dont have to calculate this on every update
			Connections.clear();
			
			for (auto side : GetLaterals())
			{
				ComponentPtr comp = factory.GetComponent(side);
				if (comp == nullptr) // the side is air, check the block under
				{
					if (down != nullptr && down->IsFullBlock) // we are not on a slab
					{
						side.Move({ 0, -1, 0 });
						comp = factory.GetComponent(side);
						if (comp != nullptr && comp->Type == WIRE)
						{
							Connections.push_back(side);
							cp = std::max(cp, comp->CanWeakPower(this));
						}
					}
				}
				else if (comp->IsFullBlock)
				{
					cp = std::max(cp, comp->CanStrongPower(this));
				}

				if (comp == nullptr || comp->IsFullBlock) // the side is block, check the block on top
				{
					// TODO: we can power this directly, if our only other connection is on the opposite side...
					if (up == nullptr || !up->IsFullBlock) // we can power blocks ontop of this side block, if the block above our head is not solid
					{
						side.Move({ 0, 1, 0 });
						comp = factory.GetComponent(side);
						if (comp != nullptr && comp->Type == WIRE)
						{
							Connections.push_back(side);
							cp = std::max(cp, comp->CanWeakPower(this));
						}
					}
				}
				else // the side is an actual component, connect to it
				{
					Connections.push_back(side);
					cp = std::max(cp, comp->CanWeakPower(this));
				}
			}

			if (cp != power || this->ticks < 0)
			{
				this->ticks = ticks;
				power = cp;
				auto sides = GetAdjacent(true);
				sides.insert(sides.end(), Connections.begin(), Connections.end());
				return sides;
			}

			return{};
		}

		~Wire()
		{
			LOGD("Wire destoryed (%d %d %d)", Location.x, Location.y, Location.z);
		}

	private:
		int power;
		int ticks;
		cVector3iArray Connections;
	};
}