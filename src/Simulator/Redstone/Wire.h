#pragma once

#include "Component.h"
#include "Vector3.h"

namespace Redstone
{
	class Wire : public Component
	{

	public:

		Wire(Vector3i location) :
			Component(location, WIRE), power(0), lastUpdateTick(-1)
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
			return 0;
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
			bool connectionsUpdated = false;
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

			if (lastUpdateTick != ticks)
			{
				cVector3iArray oldConnections;
				oldConnections.swap(Connections);
				lastUpdateTick = ticks;
				cp = std::max(cp, UpdateConnections(factory, down, up));

				// connections = 2 means the wire is pointed at a block
				// test to see if the connections change, that should queue an update to the surrounding blocks.
				connectionsUpdated = (oldConnections.size() != Connections.size()) || (oldConnections.size() == 2 &&
					std::equal(oldConnections.begin(), oldConnections.end(), Connections.begin()));
			}
			else
			{
				auto calculatePower = [&](Vector3i side) {
					auto comp = factory.GetComponent(side);
					if (comp != nullptr && comp->IsFullBlock)
					{
						cp = std::max(cp, comp->CanStrongPower(this));
					}
					else if (comp != nullptr)
					{
						cp = std::max(cp, comp->CanWeakPower(this));
					}
				};
				
				for (auto side : Connections)
				{
					calculatePower(side);
				}
				for (auto side : GetLaterals())
				{
					calculatePower(side);
				}
			}

			// cause an update if the power level changes, or if the connections change.
			if (cp != power || connectionsUpdated)
			{
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

		int UpdateConnections(ComponentFactory & factory, ComponentPtr down, ComponentPtr up)
		{
			int cp = 0;

			for (auto side : GetLaterals())
			{
				ComponentPtr comp = factory.GetComponent(side);
				if (comp == nullptr) // the side is air, check the block under
				{
					cp = std::max(cp, ConnectBlockDown(side, down, factory));
				}
				else if (comp->IsFullBlock)
				{
					cp = std::max(cp, comp->CanStrongPower(this));
					cp = std::max(cp, ConnectBlockUp(side, up, factory));
				}
				else
				{
					cp = std::max(cp, comp->CanWeakPower(this));
					Connections.push_back(side); 
				}
			}

			if (Connections.size() == 1)
			{
				auto side = Location + ((Connections.front() - Location) * (Vector3i{ -1, 0, -1 }));
				Connections.push_back(side);
			}
			else  if (Connections.size() == 0)
			{
				Connections = GetLaterals();
			}
			return cp;
		}

		int ConnectBlockDown(Vector3i side, ComponentPtr down, ComponentFactory & factory)
		{
			if (down != nullptr && down->IsFullBlock) // we are not on a slab
			{
				side.Move({ 0, -1, 0 });
				auto comp = factory.GetComponent(side);
				if (comp != nullptr && comp->Type == WIRE)
				{
					Connections.push_back(side);
					return comp->CanWeakPower(this);
				}
			}
		}

		int ConnectBlockUp(Vector3i side, ComponentPtr up, ComponentFactory & factory)
		{
			// to connect to a wire on the side, that is also up
			// there cannot be a solid block ontop of us
			if (up == nullptr || !up->IsFullBlock)
			{
				side.Move({ 0, 1, 0 });
				auto comp = factory.GetComponent(side);
				// we can only connect to wires this far away
				if (comp != nullptr && comp->Type == WIRE)
				{
					Connections.push_back(side);
					return comp->CanWeakPower(this);
				}
			}
			return 0;
		}

		int power;
		int lastUpdateTick;
		cVector3iArray Connections;
	};
}