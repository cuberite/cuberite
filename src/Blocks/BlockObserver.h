
#pragma once

#include "BlockHandler.h"
#include "Mixins.h"


class cBlockObserverHandler final :
	public cClearMetaOnDrop<cPitchYawRotator<cBlockHandler>>
{
	using Super = cClearMetaOnDrop<cPitchYawRotator<cBlockHandler>>;

public:

	using Super::Super;

	inline static Vector3i GetObservingFaceOffset(NIBBLETYPE a_Meta)
	{
		return -GetSignalOutputOffset(a_Meta);
	}

	inline static Vector3i GetSignalOutputOffset(NIBBLETYPE a_Meta)
	{
		switch (a_Meta & 0x7)
		{
			case 0x00: return { 0, 1, 0 };
			case 0x01: return { 0, -1, 0 };
			case 0x02: return { 0, 0, 1 };
			case 0x03: return { 0, 0, -1 };
			case 0x04: return { 1, 0, 0 };
			case 0x05: return { -1, 0, 0 };
			default:
			{
				LOGWARNING("%s: Unknown metadata: %d", __FUNCTION__, a_Meta);
				ASSERT(!"Unknown metadata while determining orientation of observer!");
				return { 0, 0, 0 };
			}
		}
	}
};
