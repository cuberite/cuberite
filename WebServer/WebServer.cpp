/*
   WebServer.cpp

   Copyright (C) 2003-2007 René Nyffenegger

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

   Thanks to Tom Lynn who pointed out an error in this source code.
*/

/*
	Note on point 2:
		THIS IS NOT THE ORIGINAL SOURCE1!!1!!!~!!~`1ONE!!`1
*/

#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include <ctime>
#ifdef _WIN32
	#include <process.h>
#endif
#include <iostream>
#include <sstream>


#include "WebServer.h"
#include "cEvents.h"
#include "Socket.h"
#include "UrlHelper.h"
#include "base64.h"

webserver::request_func webserver::request_func_=0;

#ifdef _WIN32
unsigned webserver::Request(void* ptr_s)
#else
void* webserver::Request(void* ptr_s)
#endif
{
	Socket* s = (reinterpret_cast<Socket*>(ptr_s));

	std::string line = s->ReceiveLine();
	if (line.empty()) 
	{
		s->Close();
		delete s;
		return 0;
	}

	http_request req;
	std::string path;
	std::map<std::string, std::string> params;
	size_t posStartPath = 0;

	if (line.find("GET") == 0) 
	{
		req.method_="GET";
		posStartPath = line.find_first_not_of(" ",3);
	}
	else if (line.find("POST") == 0) 
	{
		req.method_="POST";
		posStartPath = line.find_first_not_of(" ",4);
	}

	SplitGetReq(line.substr(posStartPath), path, params);

	req.status_ = "202 OK";
	req.s_      = s;
	req.path_   = path;
	req.params_ = params;

	static const std::string authorization   = "Authorization: Basic ";
	static const std::string accept          = "Accept: "             ;
	static const std::string accept_language = "Accept-Language: "    ;
	static const std::string accept_encoding = "Accept-Encoding: "    ;
	static const std::string user_agent      = "User-Agent: "         ;
	static const std::string content_length  = "Content-Length: "	  ;
	static const std::string content_type    = "Content-Type: "		  ;

	while(1)
	{
		line=s->ReceiveLine();
		if (line.empty()) break;

		unsigned int pos_cr_lf = line.find_first_of("\x0a\x0d");
		if (pos_cr_lf == 0) break;

		line = line.substr(0,pos_cr_lf);

		if (line.substr(0, authorization.size()) == authorization) 
		{
			req.authentication_given_ = true;
			std::string encoded = line.substr(authorization.size());
			std::string decoded = base64_decode(encoded);

			unsigned int pos_colon = decoded.find(":");

			req.username_ = decoded.substr(0, pos_colon);
			req.password_ = decoded.substr(pos_colon+1 );
		}
		else if (line.substr(0, accept.size()) == accept) 
		{
			req.accept_ = line.substr(accept.size());
		}
		else if (line.substr(0, accept_language.size()) == accept_language) 
		{
			req.accept_language_ = line.substr(accept_language.size());
		}
		else if (line.substr(0, accept_encoding.size()) == accept_encoding) 
		{
			req.accept_encoding_ = line.substr(accept_encoding.size());
		}
		else if (line.substr(0, user_agent.size()) == user_agent) 
		{
			req.user_agent_ = line.substr(user_agent.size());
		}
		else if (line.substr(0, content_length.size()) == content_length) 
		{
			req.content_length_ = atoi( line.substr(content_length.size()).c_str() );
		}
		else if (line.substr(0, content_type.size()) == content_type) 
		{
			req.content_type_ = line.substr(content_type.size());
		}
	}

	if( req.method_.compare("POST") == 0 )
	{
		if( req.content_length_ > 0 )
		{
			// The only content type we can parse at the moment, the default HTML post data
			if( req.content_type_.compare( "application/x-www-form-urlencoded" ) == 0 )
			{
				std::string Content = s->ReceiveBytes( req.content_length_ );
				Content.insert( 0, "/ ?" ); // Little hack, inserts dummy URL so that SplitGetReq() can work with this content

				std::string dummy;
				std::map<std::string, std::string> post_params;
				SplitGetReq(Content, dummy, post_params);

				req.params_post_ = post_params;
			}
		}
	}

	request_func_(&req);

	std::stringstream str_str;
	str_str << req.answer_.size();

	time_t ltime;
	time(&ltime);
	tm* gmt= gmtime(&ltime);

#ifdef _WIN32
	static std::string const serverName = "MCServerWebAdmin (Windows)";
#elif __APPLE__
	static std::string const serverName = "MCServerWebAdmin (MacOSX)";
#else
	static std::string const serverName = "MCServerWebAdmin (Linux)";
#endif


	char* asctime_remove_nl = std::asctime(gmt);
	asctime_remove_nl[24] = 0;

	s->SendBytes("HTTP/1.1 ");

	if (! req.auth_realm_.empty() ) 
	{
		s->SendLine("401 Unauthorized");
		s->SendBytes("WWW-Authenticate: Basic Realm=\"");
		s->SendBytes(req.auth_realm_);
		s->SendLine("\"");
	}
	else 
	{
		s->SendLine(req.status_);
	}
	s->SendLine(std::string("Date: ") + asctime_remove_nl + " GMT");
	s->SendLine(std::string("Server: ") +serverName);
	s->SendLine("Connection: close");
	s->SendLine("Content-Type: text/html; charset=ISO-8859-1");
	s->SendLine("Content-Length: " + str_str.str());
	s->SendLine("");
	s->SendLine(req.answer_);

	s->Close( true ); // true = wait for all data to be sent before closing
	delete s;


	return 0;
}

void webserver::Stop()
{
	m_bStop = true;
	m_Socket->Close();
	m_Events->Wait();
}

void webserver::Begin()
{
	m_bStop = false;
	while ( !m_bStop )
	{
		Socket* ptr_s=m_Socket->Accept();
		if( m_bStop )
		{
			if( ptr_s != 0 )
			{
				ptr_s->Close();
				delete ptr_s;
			}
			break;
		}

		// unused variable 'ret'
		//_beginthreadex(0,0,Request,(void*) ptr_s,0,&ret);
		// Thanks to Frank M. Hoffmann for fixing a HANDLE leak
		#ifdef _WIN32
		unsigned ret;
		HANDLE hHandle = reinterpret_cast<HANDLE>(_beginthreadex(0,0,Request,(void*) ptr_s,0,&ret));
		CloseHandle(hHandle);
		#else
		pthread_t* hHandle = new pthread_t;
		pthread_create( hHandle, NULL, Request, ptr_s);
		#endif
	}
	m_Events->Set();
}

webserver::webserver(unsigned int port_to_listen, request_func r) {
  m_Socket = new SocketServer(port_to_listen,1);

  request_func_ = r;
  m_Events = new cEvents();
}

webserver::~webserver()
{
	delete m_Socket;
	delete m_Events;
}
