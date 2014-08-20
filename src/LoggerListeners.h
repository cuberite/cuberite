
#include "Logger.h"





class cFileListener
	: public cLogger::cListener
{
public:

	cFileListener();
	cFileListener(AString a_Filename);

	virtual void Log(AString a_Message, cLogger::eLogLevel a_LogLevel) override;
	
private:

	cFile m_File;
};





cLogger::cListener * MakeConsoleListener();




