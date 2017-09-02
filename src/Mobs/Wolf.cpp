
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Wolf.h"
#include "../World.h"
#include "../Entities/Player.h"
#include "../Items/ItemHandler.h"
#include "Broadcaster.h"





cWolf::cWolf(void) :
	super(mtWolf, "entity.wolf.hurt", "entity.wolf.death", 0.6, 0.8),
	m_IsSitting(false),
	m_IsTame(false),
	m_IsBegging(false),
	m_IsAngry(false),
	m_OwnerName(""),
	m_CollarColor(E_META_DYE_ORANGE),
	m_NotificationCooldown(0)
{
	m_RelativeWalkSpeed = 2;
	m_EMPersonality = PASSIVE;
	GetMonsterConfig("Wolf");
}




bool cWolf::DoTakeDamage(TakeDamageInfo & a_TDI)
{
	/*TODO bring from master and adapt*/
}





void cWolf::NotifyAlliesOfFight(cPawn * a_Opponent)
{
	/*TODO bring from master and adapt*/
}





void cWolf::ReceiveNearbyFightInfo(const cUUID & a_PlayerID, cPawn * a_Opponent, bool a_IsPlayerInvolved)
{
	/*TODO bring from master and adapt
	*/

}





void cWolf::OnRightClicked(cPlayer & a_Player)
{
	/*TODO bring from master and adapt
	*/
}





void cWolf::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	//mobTodo behaviors!

	/*
	TODO bring from master and adapt
	*/
}





void cWolf::TickFollowPlayer()
{
	/*
	TODO bring from master and adapt
	*/
}


