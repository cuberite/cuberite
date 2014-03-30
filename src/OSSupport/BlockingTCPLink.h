/**
 * Copyright 2014 MCServer Contributors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#pragma once

#include "Socket.h"





class cBlockingTCPLink															// tolua_export
{																				// tolua_export
public:																			// tolua_export
	cBlockingTCPLink(void);															// tolua_export
	~cBlockingTCPLink();														// tolua_export

	bool Connect( const char* a_Address, unsigned int a_Port );					// tolua_export
	int Send( char* a_Data, unsigned int a_Size, int a_Flags = 0 );				// tolua_export
	int SendMessage( const char* a_Message, int a_Flags = 0 );					// tolua_export
	void CloseSocket();															// tolua_export
	void ReceiveData(AString & oData);													// tolua_export
protected:

	cSocket m_Socket;
};	// tolua_export




