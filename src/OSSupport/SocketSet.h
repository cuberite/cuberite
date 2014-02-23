
#pragma once


#include "Socket.h"


class cSocketSet
{
public:

	cSocketSet();
	cSocketSet(const std::vector<cSocket> & a_Sockets);
	
	void Add(cSocket a_Socket);
	
	bool IsSet(cSocket & a_Socket);
	
	void Reset();
	
	class cEventIterator
	{
	public:
		cEventIterator(std::vector<cSocket>::iterator a_itr,
			std::vector<cSocket>::iterator a_end,
			fd_set & a_Selected) :
			m_itr(a_itr),
			m_end(a_end),
			m_Selected(a_Selected)
		{
		}
		~cEventIterator() {}
		
		
		bool operator== (const cEventIterator & other)
		{
			return m_itr == other.m_itr;
		}

		bool operator!= (const cEventIterator & other)
		{
			return m_itr != other.m_itr;
		}


		cEventIterator & operator++()
		{
			while(m_itr != m_end && !FD_ISSET(m_itr->m_Socket, &m_Selected))
			{
				m_itr++;
			}
			return *this;
		}
		
		cEventIterator operator++(int)
		{
			cEventIterator tmp(*this);
			++(*this);
			return(tmp);
		}

		cSocket & operator* ()
		{
			return *m_itr;
		}

		cSocket * operator-> ()
		{
			return &*m_itr;
		}
	
	private:
		
		std::vector<cSocket>::iterator m_itr;
		std::vector<cSocket>::iterator m_end;
		
		fd_set & m_Selected;

	};
	
	cEventIterator Eventbegin();
	cEventIterator Eventend();
	
	bool SelectRead(int timeout);
	static bool SelectReadWrite(cSocketSet & a_ReadSockets, cSocketSet & a_WriteSockets, int timeout);

	
private:

	std::vector<cSocket> m_Sockets;
	cSocket	Highest;
	
	bool m_Selected;
	fd_set m_SelectedSockets;
};
