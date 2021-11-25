
#pragma once

class cItemHayBaleHandler : public cItemHandler
{
public:
	using Super = cItemHandler;

	cItemHayBaleHandler() : cItemHandler(Item::HayBale) {}
};
