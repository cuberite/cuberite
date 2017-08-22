#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "BehaviorItemFollower.h"
#include "../Monster.h"
#include "../../World.h"
#include "../../Entities/Player.h"


cBehaviorItemFollower::cBehaviorItemFollower(cMonster * a_Parent) :
    m_ParentInterface(a_ParentInterface)
{
    m_Parent = a_Parent;
    ASSERT(m_Parent != nullptr);
}





bool cBehaviorItemFollower::ActiveTick()
{
    cWorld * World = m_Parent->GetWorld();
    cItems FollowedItems;
    m_Parent->GetFollowedItems(FollowedItems);
    if (FollowedItems.Size() > 0)
    {
        cPlayer * a_Closest_Player = m_Parent->GetNearestPlayer();
        if (a_Closest_Player != nullptr)
        {
            cItem EquippedItem = a_Closest_Player->GetEquippedItem();
            if (FollowedItems.ContainsType(EquippedItem))
            {
                Vector3d PlayerPos = a_Closest_Player->GetPosition();
                m_Parent->MoveToPosition(PlayerPos);
                return true;  // We took control of the monster, prevent other Behaviors from doing so
            }
        }
    }
    return false;
}
