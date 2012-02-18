/*
   Socket.cpp

   Copyright (C) 2002-2004 René Nyffenegger

   This source code is provided 'as-is', without any express or implied
   warranty. In no event will the author be held liable for any damages
   arising from the use of this software.

   Permission is granted to anyone to use this software for any purpose,
   including commercial applications, and to alter it and redistribute it
   freely, subject to the following restrictions:

   1. The origin of this source code must not be misrepresented; you must not
      claim that you wrote the original source code. If you use this source code
      in a product, an acknowledgment in the product documentation would be
      appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
      misrepresented as being the original source code.

   3. This notice may not be removed or altered from any source distribution.

   René Nyffenegger rene.nyffenegger@adp-gmbh.ch
*/

/*
	Note on point 2:
		THIS IS NOT THE ORIGINAL SOURCE1!!1!!!~!!~`1ONE!!`1
*/

#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Socket.h"
#include <iostream>

#ifndef _WIN32
	#include <cstring>
	#include <sys/time.h>
	#define SD_SEND 0x01
#else
	#define MSG_NOSIGNAL (0)
#endif

#ifdef __MAC_NA
	#define MSG_NOSIGNAL (0)
#endif

using namespace std;

int Socket::nofSockets_= 0;





void Socket::Start()
{
	#ifdef _WIN32
	if (!nofSockets_)
	{
		WSADATA info;
		if (WSAStartup(MAKEWORD(2,0), &info))
		{
			throw "Could not start WSA";
		}
	}
	#endif
	++nofSockets_;
}





void Socket::End()
{
#ifdef _WIN32
	WSACleanup();
#endif
}





Socket::Socket() : 
	s_(INVALID_SOCKET)
{
	Start();
	// UDP: use SOCK_DGRAM instead of SOCK_STREAM
	s_ = socket(AF_INET,SOCK_STREAM,0);

	if (!IsValid())
	{
		throw "INVALID_SOCKET";
	}

	refCounter_ = new int(1);
}





Socket::Socket(SOCKET s) : s_(s)
{
	Start();
	refCounter_ = new int(1);
};





Socket::~Socket()
{
	if (! --(*refCounter_))
	{
		Close();
		delete refCounter_;
	}

	--nofSockets_;
	if (!nofSockets_)
	{
		End();
	}
}





Socket::Socket(const Socket& o)
{
	refCounter_=o.refCounter_;
	(*refCounter_)++;
	s_         =o.s_;

	nofSockets_++;
}





Socket& Socket::operator=(Socket& o)
{
	(*o.refCounter_)++;

	refCounter_ = o.refCounter_;
	s_          = o.s_;

	nofSockets_++;

	return *this;
}





bool Socket::IsValid(void) const
{
	#ifdef _WIN32
	return (s_ != INVALID_SOCKET);
	#else
	return (s_ >= 0);
	#endif
}





void Socket::Close( bool a_WaitSend /* = false */ )
{
	if (IsValid())
	{
		if (a_WaitSend)
		{
			assert( shutdown(s_, SD_SEND ) == 0 );
			char c;
			while( recv(s_, &c, 1, 0 ) > 0 )
			{}
		}

		#ifndef _WIN32
		// Linux needs to shut the socket down first, otherwise the socket keeps blocking accept() and select() calls!
		// Ref.: http://forum.mc-server.org/showthread.php?tid=344
		if (shutdown(s_, SHUT_RDWR) != 0)
		{
			LOGWARN("Error on shutting down socket %d", s_);
		}
		#endif  // _WIN32
		
		closesocket(s_);

		s_ = INVALID_SOCKET;
	}
}





std::string Socket::ReceiveLine()
{
	std::string ret;
	while (1)
	{
		char r;

		if (recv(s_, &r, 1, 0) <= 0 )
		{
			return "";
		}
		ret += r;
		if (r == '\n')  return ret;
	}
}





std::string Socket::ReceiveBytes( unsigned int a_Length )
{
	std::string ret;
	while( ret.size() < a_Length )
	{
		char r;

		if (recv(s_, &r, 1, 0) <= 0 )
		{
			return "";
		}
		ret += r;
	}
	return ret;
}





void Socket::SendLine(std::string s)
{
	s += '\n';
	if( send(s_,s.c_str(),s.length(),MSG_NOSIGNAL) <= 0 )
	{
		//printf("SendLine Socket error!! \n" );
		Close();
	}
}





void Socket::SendBytes(const std::string& s)
{
	if( send(s_,s.c_str(),s.length(), MSG_NOSIGNAL) <= 0 )
	{
		//printf("SendBytes Socket error!! \n" );
		Close();
	}
}





SocketServer::SocketServer(int port, int connections, TypeSocket type)
{
	sockaddr_in sa;

	memset(&sa, 0, sizeof(sa));

	sa.sin_family = PF_INET;
	sa.sin_port = htons(port);
	s_ = socket(AF_INET, SOCK_STREAM, 0);

	if (!IsValid())
	{
		LOG("WebServer: INVALID_SOCKET");
	}

	if(type==NonBlockingSocket)
	{
		return;
	}

	#ifdef _WIN32
	char yes = 1;
	#else
	int yes = 1;
	#endif
	
	if (setsockopt(s_,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) == -1)
	{
		LOG("WebServer: setsockopt == -1");
		return;
	}

	/* bind the socket to the internet address */
	if (bind(s_, (sockaddr *)&sa, sizeof(sockaddr_in)) == SOCKET_ERROR)
	{
		closesocket(s_);
		LOG("WebServer: INVALID_SOCKET");
	}

	listen(s_, connections);
}





Socket* SocketServer::Accept()
{
	Socket * r = new Socket(accept(s_, 0, 0));
	if (!r->IsValid())
	{
		delete r;
		r = NULL;
	}

	return r;
}





SocketSelect::SocketSelect(Socket const * const s1, Socket const * const s2, TypeSocket type)
{
	FD_ZERO(&fds_);
	SOCKET Highest = s1->s_;
	FD_SET(const_cast<Socket*>(s1)->s_,&fds_);
	if(s2)
	{
		FD_SET(const_cast<Socket*>(s2)->s_,&fds_);
		if (s2->s_ > Highest)
		{
			Highest = s2->s_;
		}
	}
	if (select(Highest + 1, &fds_, NULL, NULL, NULL) == SOCKET_ERROR)
	{
		throw "Error in select";
	}
}





bool SocketSelect::Readable(Socket const* const s)
{
	return (FD_ISSET(s->s_,&fds_) != 0);
}




