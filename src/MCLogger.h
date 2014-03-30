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




class cLog;





class cMCLogger														// tolua_export
{																	// tolua_export
public:																// tolua_export
	/// Creates a logger with the default filename, "logs/LOG_<timestamp>.log"
	cMCLogger(void);
	
	/// Creates a logger with the specified filename inside "logs" folder
	cMCLogger(const AString & a_FileName);  // tolua_export
	
	~cMCLogger();													// tolua_export

	void Log(const char* a_Format, va_list a_ArgList)   FORMATSTRING(2, 0);
	void Info(const char* a_Format, va_list a_ArgList)  FORMATSTRING(2, 0);
	void Warn(const char* a_Format, va_list a_ArgList)  FORMATSTRING(2, 0);
	void Error(const char* a_Format, va_list a_ArgList) FORMATSTRING(2, 0);

	void LogSimple(const char* a_Text, int a_LogType = 0 );			// tolua_export

	static cMCLogger* GetInstance();
private:
	enum eColorScheme
	{
		csRegular,
		csInfo,
		csWarning,
		csError,
	} ;
	
	cCriticalSection m_CriticalSection;
	cLog * m_Log;
	static cMCLogger * s_MCLogger;


	/// Sets the specified color scheme in the terminal (TODO: if coloring available)
	void SetColor(eColorScheme a_Scheme);
	
	/// Resets the color back to whatever is the default in the terminal
	void ResetColor(void);
	
	/// Common initialization for all constructors, creates a logfile with the specified name and assigns s_MCLogger to this
	void InitLog(const AString & a_FileName);
};																	// tolua_export





extern void LOG(const char* a_Format, ...)      FORMATSTRING(1, 2);
extern void LOGINFO(const char* a_Format, ...)  FORMATSTRING(1, 2);
extern void LOGWARN(const char* a_Format, ...)  FORMATSTRING(1, 2);
extern void LOGERROR(const char* a_Format, ...) FORMATSTRING(1, 2);





// In debug builds, translate LOGD to LOG, otherwise leave it out altogether:
#ifdef _DEBUG
	#define LOGD LOG
#else
	#define LOGD(...)
#endif  // _DEBUG





#define LOGWARNING LOGWARN




