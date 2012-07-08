#pragma once
#include <sqrat.h>

class SquirrelObject
{
public:
	SquirrelObject(Sqrat::Object a_Obj)
	{
		m_SquirrelObject = a_Obj;
	}

	Sqrat::Function GetFunction(const char *methodName)
	{
		if(m_SquirrelObject.IsNull())
			return Sqrat::Function();

		Sqrat::Function method(m_SquirrelObject, methodName);
		return method;
	}

protected:
	Sqrat::Object m_SquirrelObject;

};