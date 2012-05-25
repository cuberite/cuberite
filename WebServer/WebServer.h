/*
   WebServer.h

   Copyright (C) 2003-2004 René Nyffenegger

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


class cEvents;
class Socket;
class SocketServer;
class webserver {
public:
	struct formdata
	{
		std::string name_;
		std::string filename_;
		std::string content_type_;
		std::string value_;
	};

	struct http_request {

		http_request()
			: s_( 0 )
			, authentication_given_(false)
			, content_length_( 0 )
		{}

		Socket*                            s_;
		std::string                        method_;
		std::string                        path_;
		std::map<std::string, std::string> params_;
		std::map<std::string, std::string> params_post_;

		std::string                        accept_;
		std::string                        accept_language_;
		std::string                        accept_encoding_;
		std::string                        user_agent_;
		int                                content_length_;
		std::string                        content_type_;
		std::vector< formdata >            multipart_formdata_;

		/* status_: used to transmit server's error status, such as
		o  202 OK
		o  404 Not Found
		and so on */
		std::string                        status_;

		/* auth_realm_: allows to set the basic realm for an authentication,
		no need to additionally set status_ if set */
		std::string                        auth_realm_;

		std::string                        answer_;

		/*   authentication_given_ is true when the user has entered a username and password.
		These can then be read from username_ and password_ */
		bool authentication_given_;
		std::string username_;
		std::string password_;
	};

	typedef   void (*request_func) (http_request*);
	webserver(unsigned int port_to_listen, request_func);
	~webserver();

	bool Begin();
	void Stop();

private:
	bool m_bStop;
	
	#ifdef _WIN32
	static unsigned __stdcall Request(void*);
	#else
	static void* Request(void*);
	#endif
	
	static request_func request_func_;

	cEvents * m_Events;
	SocketServer* m_Socket;
};
