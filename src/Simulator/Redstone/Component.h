
#pragma once

#include "Vector3.h"

namespace Redstone
{
	class ComponentFactory;
	enum RedstoneType;

	class Component
	{
	public:
		const Vector3i Location;
		const bool IsFullBlock;
		const RedstoneType Type;

		Component(Vector3i location, RedstoneType type, bool isFullBlock = false) :
			Location(location), IsFullBlock(isFullBlock), Type(type) {}

		// disable the copy constructor and assignment operator
		Component(const Component &) = delete;
		Component * operator = (const Component &) = delete;

		virtual cVector3iArray Update(ComponentFactory & factory, int tick) = 0;
		virtual void SetState(BLOCKTYPE & block, NIBBLETYPE & meta) {}
		virtual bool GetState(BLOCKTYPE & block, NIBBLETYPE & meta) = 0;
		virtual int CanStrongPower(Component * component) = 0;
		virtual int CanWeakPower(Component * component) = 0;

		~Component() {}

	protected:

		Vector3i Up()
		{
			return Vector3i(Location.x, Location.y + 1, Location.z);
		}

		Vector3i Down()
		{
			return Vector3i(Location.x, Location.y - 1, Location.z);
		}

		bool IsAdjacent(const Vector3i & location)
		{
			double len = (location - Location).SqrLength();
			return len == -1 || len == 1;
		}

		inline cVector3iArray GetAdjacent(bool includeSelf = false)
		{
			if (includeSelf)
			{
				return
				{
					{ Location.x, Location.y, Location.z },
					{ Location.x + 1, Location.y, Location.z },
					{ Location.x - 1, Location.y, Location.z },
					{ Location.x, Location.y + 1, Location.z },
					{ Location.x, Location.y - 1, Location.z },
					{ Location.x, Location.y, Location.z + 1 },
					{ Location.x, Location.y, Location.z - 1 },
				};
			}

			return 
			{ 
				{ Location.x+1, Location.y, Location.z },
				{ Location.x-1, Location.y, Location.z },
				{ Location.x, Location.y+1, Location.z },
				{ Location.x, Location.y-1, Location.z },
				{ Location.x, Location.y, Location.z+1 },
				{ Location.x, Location.y, Location.z-1 },
			};
		}

		inline cVector3iArray GetLaterals()
		{
			return
			{
				{ Location.x + 1, Location.y, Location.z },
				{ Location.x - 1, Location.y, Location.z },
				{ Location.x, Location.y, Location.z + 1 },
				{ Location.x, Location.y, Location.z - 1 },
			};
		}
	};

	typedef std::shared_ptr<Component> ComponentPtr;
	
	enum RedstoneType : int
	{
		UNKNOWN,
		SOLIDBLOCK,
		TORCH,
		WIRE,
		REPEATER,
		COMPARATOR,
	};
}