
#pragma once

#include "Mixins.h"





class cBlockGlazedTerracottaHandler final :
	public cClearMetaOnDrop<cYawRotator<cBlockHandler, 0x03, 0x00, 0x01, 0x02, 0x03>>
{
	using Super = cClearMetaOnDrop<cYawRotator<cBlockHandler, 0x03, 0x00, 0x01, 0x02, 0x03>>;

public:

	using Super::Super;
};
