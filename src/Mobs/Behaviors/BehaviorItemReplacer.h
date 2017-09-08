#pragma once


class cBehaviorItemReplacer;

#include "Behavior.h"

/** When right clicked while holding a_OriginalItem, a mob having this behavior replaces the original item
with a_NewItem. This is used for milking cows.

Connections to other behaviors:
 - None.

Special connections:
 - None.

*/
class cBehaviorItemReplacer : public cBehavior
{

public:
	cBehaviorItemReplacer(short a_OriginalItem, short a_NewItem);
	void AttachToMonster(cMonster & a_Parent);
	void OnRightClicked(cPlayer & a_Player) override;
private:
	// Our parent
	cMonster * m_Parent;
	short m_OriginalItem; // Replace this item with NewItem
	short m_NewItem;
};



