
#include "Globals.h"

#include "SocketSet.h"






cSocketSet::cSocketSet() :
	m_Selected(false)
{
}






cSocketSet::cSocketSet(const std::vector<cSocket> & a_Sockets) :
	m_Sockets(a_Sockets),
	m_Selected(false)
{
}





void cSocketSet::Add(cSocket a_Socket)
{
	if (a_Socket > Highest)
	{
		Highest = a_Socket;
	}
	m_Sockets.push_back(a_Socket);
}





bool cSocketSet::IsSet(cSocket & a_Socket)
{
	if (!m_Selected) 
	{
		return false;
	}
	
	return FD_ISSET(a_Socket.m_Socket, &m_SelectedSockets);
}





void cSocketSet::Reset()
{
	m_Selected = false;
}





cSocketSet::cEventIterator cSocketSet::Eventbegin()
{
	if (!m_Selected)
	{
		return Eventend();
	}
	return cSocketSet::cEventIterator(m_Sockets.begin(), m_Sockets.end(), m_SelectedSockets);
}





cSocketSet::cEventIterator cSocketSet::Eventend()
{
	return cSocketSet::cEventIterator(m_Sockets.end(), m_Sockets.end(), m_SelectedSockets);
}



bool cSocketSet::SelectRead(int timeout)
{
	FD_ZERO(&m_SelectedSockets);
	for(std::vector<cSocket>::iterator itr = m_Sockets.begin(), end = m_Sockets.end(); itr != end; itr++)
	{
		FD_SET(itr->m_Socket, &m_SelectedSockets);
	}
	timeval tv;
	tv.tv_sec = timeout;
	tv.tv_usec = 0;
	return (select(Highest.m_Socket + 1, &m_SelectedSockets, NULL, NULL, &tv) != -1);
}





bool cSocketSet::SelectReadWrite(cSocketSet & a_ReadSockets, cSocketSet & a_WriteSockets, int timeout)
{
	FD_ZERO(&a_ReadSockets.m_SelectedSockets);
	for(std::vector<cSocket>::iterator itr = a_ReadSockets.m_Sockets.begin(), end = a_ReadSockets.m_Sockets.end(); itr != end; itr++)
	{
		FD_SET(itr->m_Socket, &a_ReadSockets.m_SelectedSockets);
	}
	FD_ZERO(&a_WriteSockets.m_SelectedSockets);
	for(std::vector<cSocket>::iterator itr = a_WriteSockets.m_Sockets.begin(), end = a_WriteSockets.m_Sockets.end(); itr != end; itr++)
	{
		FD_SET(itr->m_Socket, &a_WriteSockets.m_SelectedSockets);
	}
	timeval tv;
	tv.tv_sec = timeout;
	tv.tv_usec = 0;
	int highest = std::max(a_ReadSockets.Highest.m_Socket, a_WriteSockets.Highest.m_Socket) + 1;
	return (select(highest, &a_ReadSockets.m_SelectedSockets, &a_WriteSockets.m_SelectedSockets, NULL, &tv) != -1);
}

