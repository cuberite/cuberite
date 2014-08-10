
#include "LogDispacher.h"

namespace Logger
{

	class cFileListener
		: public cLoggerListener
	{
		public:
		
			cFileListener();
			cFileListener(AString a_Filename);
		
			virtual void Log(AString a_Message, eLogLevel a_LogLevel) override;
		private:
			cFile m_File;
	};

	cLoggerListener * MakeConsoleListener();
}
