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





webserver::request_func webserver::request_func_ = NULL;





static std::string EatLine( std::string& a_String )
{
	std::string RetVal = "";
	unsigned int StringSize = a_String.size();
	const char* c = a_String.c_str();
	
	for( unsigned int i = 0; i < StringSize; ++i, ++c)
	{
		if( *c == '\n' )
		{
			RetVal += *c;
// 			++i; ++c;
// 			if( i < StringSize )
// 			{
// 				if( *c == '\r' )
// 				{
// 					RetVal += *c;
// 				}
// 			}
			break;
		}
		RetVal += *c;
	}
	a_String = a_String.substr( RetVal.size() );
	return RetVal;
}





// Turns 
// "blabla my string with \"quotes\"!" 
// into 
// blabla my string with "quotes"!
static std::string GetQuotedString( const std::string& a_String )
{
	std::string RetVal;

	bool bGotFirstQuote = false;
	bool bIgnoreNext = false;
	unsigned int StrLen = a_String.size();
	for( unsigned int i = 0; i < StrLen; ++i )
	{
		if( bIgnoreNext == false )
		{
			if( a_String[i] == '\"' )
			{
				if( bGotFirstQuote == false )
				{
					bGotFirstQuote = true;
				}
				else
				{
					break;
				}
				continue;
			}
			else if( a_String[i] == '\\' ) // Escape character
			{
				bIgnoreNext = true;
				continue;
			}
		}
		RetVal.push_back( a_String[i] );
		bIgnoreNext = false;
	}

	return RetVal;
}





void ParseMultipartFormData( webserver::http_request& req, Socket* s)
{
	static const std::string multipart_form_data = "multipart/form-data";
	if(req.content_type_.substr(0, multipart_form_data.size()) == multipart_form_data)  // Difficult data... :(
	{
		AStringVector ContentTypeData = StringSplit( req.content_type_, "; " ); 

		std::string boundary;
		// Find boundary
		for( unsigned int i = 0; i < ContentTypeData.size(); ++i )
		{
			static const std::string boundary_ = "boundary=";
			if( ContentTypeData[i].substr(0, boundary_.size()) == boundary_ ) // Found boundary
			{
				boundary = ContentTypeData[i].substr( boundary_.size() );
			}
		}

		//LOGINFO("Boundary: %s", boundary.c_str() );
		std::string boundary_start = "--" + boundary;
		std::string boundary_end = boundary_start + "--";

		std::string Content = s->ReceiveBytes( req.content_length_ );

		//printf("Total content: \n%s\n", Content.c_str() );

		// Should start with boundary!
		std::string line = EatLine( Content );
		if( line.substr(0, boundary_start.size() ) != boundary_start )
		{
			// Something was wrong! :(
			Content.clear();
		}

		while( !Content.empty() )
		{
			webserver::formdata FormData;

			static const std::string content_disposition = "Content-Disposition: ";
			static const std::string content_type		 = "Content-Type: ";

			std::string f_disposition;

			while( 1 )
			{
				std::string line = EatLine( Content );
				if( line.empty() )
					break;

				unsigned int pos_cr_lf = line.find_first_of("\x0a\x0d");
				if (pos_cr_lf == 0) break;	// Empty line, indicates end of mime thingy

				if( line.substr(0, content_disposition.size() ) == content_disposition )
				{
					f_disposition = line.substr(content_disposition.size());
					LOGINFO("Disposition: %s", f_disposition.c_str() );
				}
				else if( line.substr(0, content_type.size() ) == content_type )
				{
					FormData.content_type_ = line.substr(content_type.size());
				}

				//LOGINFO("Got line: '%s'", line.c_str() );
			}

			// Check if we got the proper headers
			if( !f_disposition.empty() )
			{
				static const std::string disp_name = "name=";
				static const std::string disp_filename = "filename=";

				// Parse the disposition
				AStringVector DispositionData = StringSplit( f_disposition, "; " );
				for( unsigned int i = 0; i < DispositionData.size(); ++i )
				{
					if( DispositionData[i].substr(0, disp_name.size()) == disp_name )
					{
						FormData.name_ = GetQuotedString( DispositionData[i].substr(disp_name.size()) );
					}
					else if( DispositionData[i].substr(0, disp_filename.size()) == disp_filename )
					{
						FormData.filename_ = GetQuotedString( DispositionData[i].substr(disp_filename.size()) );
					}
				}

				std::string ContentValue;
				// Parse until boundary_end is found
				while( 1 )
				{
					std::string line = EatLine( Content );
					if( line.empty() )
						break;

					if( line.substr(0, boundary_end.size() ) == boundary_end )
					{
						break;
					}
					else if( line.substr(0, boundary_start.size() ) == boundary_start )
					{
						break;
					}
					ContentValue.append( line.c_str(), line.size() );
				}


				FormData.value_ = ContentValue;
			}

			req.multipart_formdata_.push_back( FormData );
		}
	}
}





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

	static const char authorization[]   = "Authorization: Basic ";
	static const char accept[]          = "Accept: ";
	static const char accept_language[] = "Accept-Language: ";
	static const char accept_encoding[] = "Accept-Encoding: ";
	static const char user_agent[]      = "User-Agent: ";
	static const char content_length[]  = "Content-Length: ";
	static const char content_type[]    = "Content-Type: ";

	while(1)
	{
		line=s->ReceiveLine();
		if (line.empty())
		{
			break;
		}

		unsigned int pos_cr_lf = line.find_first_of("\x0a\x0d");
		if (pos_cr_lf == 0) break;

		line = line.substr(0,pos_cr_lf);

		if (line.compare(0, sizeof(authorization) - 1, authorization) == 0) 
		{
			req.authentication_given_ = true;
			std::string encoded = line.substr(sizeof(authorization) - 1);
			std::string decoded = base64_decode(encoded);

			unsigned int pos_colon = decoded.find(":");

			req.username_ = decoded.substr(0, pos_colon);
			req.password_ = decoded.substr(pos_colon + 1);
		}
		else if (line.compare(0, sizeof(accept) - 1, accept) == 0)
		{
			req.accept_ = line.substr(sizeof(accept) - 1);
		}
		else if (line.compare(0, sizeof(accept_language) - 1, accept_language) == 0)
		{
			req.accept_language_ = line.substr(sizeof(accept_language) - 1);
		}
		else if (line.compare(0, sizeof(accept_encoding) - 1, accept_encoding) == 0)
		{
			req.accept_encoding_ = line.substr(sizeof(accept_encoding) - 1);
		}
		else if (line.compare(0, sizeof(user_agent) - 1, user_agent) == 0)
		{
			req.user_agent_ = line.substr(sizeof(user_agent) - 1);
		}
		else if (line.compare(0, sizeof(content_length) - 1, content_length) == 0)
		{
			req.content_length_ = atoi(line.substr(sizeof(content_length) - 1).c_str() );
		}
		else if (line.compare(0, sizeof(content_type) - 1, content_type) == 0)
		{
			req.content_type_ = line.substr(sizeof(content_type) - 1);
		}
	}

	if( (req.method_.compare("POST") == 0) && (req.content_length_ > 0) )
	{
		const char FormUrlEncoded[] = "application/x-www-form-urlencoded";
		// The only content type we can parse at the moment, the default HTML post data
		if( req.content_type_.substr(0, strlen(FormUrlEncoded)).compare( FormUrlEncoded ) == 0 )
		{
			std::string Content = s->ReceiveBytes( req.content_length_ );
			Content.insert( 0, "/ ?" ); // Little hack, inserts dummy URL so that SplitGetReq() can work with this content

			std::string dummy;
			std::map<std::string, std::string> post_params;
			SplitGetReq(Content, dummy, post_params);

			req.params_post_ = post_params;
		}
		else 
		{
			ParseMultipartFormData( req, s );
		}
	}

	request_func_(&req);

	std::stringstream str_str;
	str_str << req.answer_.size();

	time_t ltime;
	time(&ltime);
	tm* gmt= gmtime(&ltime);

#ifdef _WIN32
	static const char serverName[] = "MCServerWebAdmin (Windows)";
#elif __APPLE__
	static const char serverName[] = "MCServerWebAdmin (MacOSX)";
#else
	static const char serverName[] = "MCServerWebAdmin (Linux)";
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
	s->SendLine(std::string("Server: ") + serverName);
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





bool webserver::Begin()
{
	if (!m_Socket->IsValid())
	{
		LOGINFO("WebAdmin: The server socket is invalid. Terminating WebAdmin.");
		return false;
	}
	m_bStop = false;
	while ( !m_bStop )
	{
		Socket * ptr_s = m_Socket->Accept();
		if (m_bStop)
		{
			if (ptr_s != 0)
			{
				ptr_s->Close();
				delete ptr_s;
			}
			break;
		}
		if (ptr_s == NULL)
		{
			LOGINFO("WebAdmin: Accepted socket is NULL. Terminating WebAdmin to avoid busywait.");
			return false;
		}

		#ifdef _WIN32
		unsigned ret;
		HANDLE hHandle = reinterpret_cast<HANDLE>(_beginthreadex(NULL, 0, Request, (void *)ptr_s, 0, &ret));
		CloseHandle(hHandle);
		#else
		// Mattes: TODO: this handle probably leaks!
		pthread_t * hHandle = new pthread_t;
		pthread_create( hHandle, NULL, Request, ptr_s);
		#endif
	}
	m_Events->Set();
	return true;
}





webserver::webserver(unsigned int port_to_listen, request_func r)
{
  m_Socket = new SocketServer(port_to_listen, 1);

  request_func_ = r;
  m_Events = new cEvents();
}





webserver::~webserver()
{
	delete m_Socket;
	delete m_Events;
}




