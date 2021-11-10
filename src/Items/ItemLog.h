
#pragma once

class cItemLogHandler : public cItemHandler
{
public:
	using Super = cItemHandler;
	cItemLogHandler(Item a_Item) : Super(a_Item) {}
};