
#pragma once





#ifdef USE_SQUIRREL





#include <sqrat.h>





class SquirrelObject
{
public:
	SquirrelObject(Sqrat::Object a_Obj)
	{
		m_SquirrelObject = a_Obj;
	}

	Sqrat::Function GetFunction(const char *a_MethodName)
	{
		if(m_SquirrelObject.IsNull())
			return Sqrat::Function();

		Sqrat::Function method(m_SquirrelObject, a_MethodName);
		return method;
	}

	bool HasFunction(const char *a_MethodName)
	{
		return !this->GetFunction(a_MethodName).IsNull();
	}

protected:
	Sqrat::Object m_SquirrelObject;

};





#endif  // USE_SQUIRREL




