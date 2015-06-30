
#pragma once

#include "Component.h"
#include "Vector3.h"

namespace Redstone
{
	class SolidBlock : public Component
	{

	public:

		SolidBlock(Vector3i location) :
			Component(location, SOLIDBLOCK, true), isStrongPowered(false), power(0)
		{
			LOGD("SolidBlock created {%d %d %d}", location.x, location.y, location.z);
		}

		virtual int CanStrongPower(Component * component)
		{
			return isStrongPowered ? power : 0;
		}

		virtual int CanWeakPower(Component * component)
		{
			return power;
		}

		virtual bool GetState(BLOCKTYPE & block, NIBBLETYPE & meta)
		{
			return false;
		}

		virtual cVector3iArray Update(ComponentFactory & factory, int ticks)
		{
			UNUSED(ticks);
			LOGD("Evaluating SolidBlock (%d %d %d)", Location.x, Location.y, Location.z);
			// calculated value for power
			int cp = 0;
			// calculated value for strongly powered
			bool cSp = false;

			for (auto side : GetAdjacent())
			{
				ComponentPtr comp = factory.GetComponent(side);
				if (comp == nullptr) continue;
				if (!comp->IsFullBlock)
				{
					int p = comp->CanStrongPower(this);
					if (p > 0)
					{
						cSp = true;
					}
					else
					{
						p = comp->CanWeakPower(this);
					}
					cp = std::max(cp, p);
				}
			}

			if (cp != power || cSp != isStrongPowered)
			{
				power = cp;
				isStrongPowered = cSp;
				return GetAdjacent();
			}
			return {};
		}

		~SolidBlock()
		{
			LOGD("SolidBlock destoryed (%d %d %d)", Location.x, Location.y, Location.z);
		}

	private:
		bool isStrongPowered;
		int power;
	};
}