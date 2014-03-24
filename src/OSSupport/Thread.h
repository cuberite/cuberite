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

class cThread
{
public:
	typedef void (ThreadFunc)(void*);
	cThread( ThreadFunc a_ThreadFunction, void* a_Param, const char* a_ThreadName = 0 );
	~cThread();

	void Start( bool a_bWaitOnDelete = true );
	void WaitForThread();
private:
	ThreadFunc* m_ThreadFunction;

#ifdef _WIN32
	static unsigned long MyThread(void* a_Param );
#else
	static void *MyThread( void *lpParam );
#endif

	void* m_Param;
	cEvent* m_Event;
	cEvent* m_StopEvent;

	AString m_ThreadName;
};