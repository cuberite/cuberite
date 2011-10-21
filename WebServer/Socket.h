/*
   Socket.h

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

#ifndef SOCKET_H
#define SOCKET_H

#include "../source/MCSocket.h"
// #ifdef _WIN32
// #include <winsock2.h>
// #endif

#include <string>

enum TypeSocket {BlockingSocket, NonBlockingSocket};

class Socket {
public:

  virtual ~Socket();
  Socket(const Socket&);
  Socket& operator=(Socket&);

  std::string ReceiveLine();
  std::string ReceiveBytes();

  void   Close();

  // The parameter of SendLine is not a const reference
  // because SendLine modifes the std::string passed.
  void   SendLine (std::string);

  // The parameter of SendBytes is a const reference
  // because SendBytes does not modify the std::string passed
  // (in contrast to SendLine).
  void   SendBytes(const std::string&);

protected:
  friend class SocketServer;
  friend class SocketSelect;

  Socket(SOCKET s);
  Socket();


  SOCKET s_;

  int* refCounter_;

private:
  static void Start();
  static void End();
  static int  nofSockets_;
};

class SocketClient : public Socket {
public:
  SocketClient(const std::string& host, int port);
};

class SocketServer : public Socket {
public:
  SocketServer(int port, int connections, TypeSocket type=BlockingSocket);

  Socket* Accept();
};

// http://msdn.microsoft.com/library/default.asp?url=/library/en-us/winsock/wsapiref_2tiq.asp
class SocketSelect {
  public:
    SocketSelect(Socket const * const s1, Socket const * const s2=NULL, TypeSocket type=BlockingSocket);

    bool Readable(Socket const * const s);

  private:
    fd_set fds_;
};

#endif
